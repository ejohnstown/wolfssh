#!/bin/bash

# A child that exits with error output still in its pipe must have all of it
# relayed before the session ends.
#
# The shell loop ends the session once the child is gone and its stdout has
# reported EOF. Nothing there speaks for stderr, so whatever the child left in
# that pipe goes out only through the single read the drain below the loop
# makes -- one bufferful, however much is waiting. A pipe holds far more than
# that, and a child that writes its output in one burst and exits leaves
# exactly this state, so the peer sees a truncated error stream and no error.
#
# Which side wins is a race, so one transfer proves nothing and this repeats.
# A short stream is never correct, so a failure here is always real.
#
# The payload is counted in whole lines rather than bytes: the client prints
# "Error reading stdin" to its own stderr when its stdin reaches EOF, which
# lands in the same capture as the relayed stream.

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "expecting host and port as arguments"
    echo "./sshd_stderr_drain_test.sh 127.0.0.1 22222"
    exit 1
fi

PWD=`pwd`

if [ ! -z "$3" ]; then
    USER="$3"
else
    USER=`whoami`
fi
TEST_HOST="$1"
TEST_PORT="$2"

# Enough lines to outlast one bufferful several times over, and enough passes
# to make the race show.
TEST_LINE="wolfsshd-stderr-drain"
TEST_LINES=9000
TEST_ITERS=40
TEST_TIMEOUT=60

# The regression this covers ends the session early rather than hanging, but
# the runner invokes this test synchronously and has no deadline of its own.
# Degraded rather than skipped where "timeout" is missing, matching
# run_all_sshd_tests.sh.
TIMEOUT=""
if command -v timeout >/dev/null 2>&1; then
    TIMEOUT="timeout $TEST_TIMEOUT"
fi

source ./start_sshd.sh

# Everything this test writes goes in a directory of its own, so a second run
# sharing the host cannot overwrite the payload or remove it mid-transfer.
TEST_TMP=`mktemp -d 2>/dev/null` || TEST_TMP=`mktemp -d -t stderrdrain`
if [ -z "$TEST_TMP" ] || [ ! -d "$TEST_TMP" ]; then
    echo "Failed to create a temp dir"
    exit 1
fi
TEST_CONFIG="$TEST_TMP/sshd_config_test_stderr_drain"
TEST_FILE="$TEST_TMP/stderr-drain-test.txt"
TEST_RESULT_FILE="$TEST_TMP/stderr-drain-test-result.txt"

# Installed before the daemon starts so a failure in between is covered too.
# stop_wolfsshd is idempotent, so the explicit call at the end still stands.
trap 'rm -rf "$TEST_TMP"; stop_wolfsshd' EXIT

cat <<CONF > "$TEST_CONFIG"
Port $TEST_PORT
Protocol 2
LoginGraceTime 600
PermitRootLogin yes
PasswordAuthentication yes
PermitEmptyPasswords no
UsePrivilegeSeparation no
UseDNS no
HostKey $PWD/../../../keys/server-key.pem
AuthorizedKeysFile $PWD/authorized_keys_test
CONF

start_wolfsshd "$TEST_CONFIG"
if [ -z "$PID" ]; then
    echo "Failed to start wolfsshd"
    exit 1
fi
cd ../../..

TEST_CLIENT="./examples/client/client"
PRIVATE_KEY="./keys/hansel-key-ecc.der"
PUBLIC_KEY="./keys/hansel-key-ecc.pub"

yes "$TEST_LINE" | head -n $TEST_LINES > "$TEST_FILE"

RESULT=0
for i in `seq 1 $TEST_ITERS`; do
    # "cat" fills the pipe and exits, so the child is gone with its output
    # still queued. Nothing reads the peer slowly here: the loop has to relay
    # the rest on its own account.
    $TIMEOUT $TEST_CLIENT -q -c "cat $TEST_FILE 1>&2" \
        -u $USER -i $PRIVATE_KEY -j $PUBLIC_KEY -h $TEST_HOST -p $TEST_PORT \
        < /dev/null > /dev/null 2> "$TEST_RESULT_FILE"
    CLIENT_RESULT=$?

    if [ "$CLIENT_RESULT" = 124 ]; then
        echo "pass $i of $TEST_ITERS never finished"
        echo "the client was still running after $TEST_TIMEOUT seconds"
        RESULT=1
        break
    fi

    GOT=`grep -c "^$TEST_LINE\$" "$TEST_RESULT_FILE"`
    if [ "$GOT" != "$TEST_LINES" ]; then
        echo "pass $i of $TEST_ITERS truncated the child's stderr"
        echo "expected $TEST_LINES lines, got $GOT, short by"\
            "$((TEST_LINES-GOT)); the client exited $CLIENT_RESULT"
        RESULT=1
        break
    fi
done

rm -rf "$TEST_TMP"
cd apps/wolfsshd/test
stop_wolfsshd

exit $RESULT
