# wolfSSH v1.6.0 (September 8, 2026)

## Notes

- DH group exchange now enforces a 2048-bit floor and raises
  `WOLFSSH_DEFAULT_GEXDH_MIN` to 2048, so a client will not complete GEX
  with a 1024-bit-only server. (PR 1056)
- Server group selection follows RFC 4419, so an OpenSSH client now gets the
  4096-bit group 16 rather than group 14, at 5-8x the modexp cost. Cap it
  with `WOLFSSH_DEFAULT_GEXDH_MAX`. (PR 1056)
- `WS_CallbackFwd` must now return the allocated port for a port-0
  `WOLFSSH_FWD_REMOTE_SETUP`; returning `WS_FWD_SUCCESS` is rejected.
  (PR 1059)
- Applications must now drain stderr. Ignoring `WS_EXTDATA` exhausts the
  channel window and deadlocks it. (PR 1054)
- `wolfSSH_stream_read()` now fails on extended data for any channel but the
  first; use `wolfSSH_ChannelIdReadExt()`. (PR 1054)
- `wolfSSH_extended_data_read()` now returns `WS_BAD_ARGUMENT` where it
  returned 0, and `WS_INVALID_EXTDATA` is no longer produced. (PR 1054)
- The server now caps failed authentication attempts at 6 per connection. A
  value at or below 0 means the default, not unlimited. (PR 1117)
- The `SetAlgoList*()` setters now validate input and return
  `WS_INVALID_ALGO_ID` instead of always `WS_SUCCESS`. (PR 1117)
- Key and KeyAccepted accept NULL; Kex, Cipher and Mac reject it. (PR 1117)
- A "none" cipher or MAC now requires `--enable-none-cipher`. (PR 1117)
- RSA user authentication keys must now be at least 2048 bits
  (`WOLFSSH_RSA_MIN_KEY_BITS`); shorter keys must be regenerated. (PR 1101)
- The `wolfssh` client no longer accepts `-N`. It was parsed but never read,
  so it is now a usage error rather than silently ignored. (PR 1162)
- wolfSSHd sets no SFTP confinement path; its sessions are bounded by the OS
  uid drop or Windows impersonation token. (PR 1167)

## New Features

- Added ML-DSA-44, ML-DSA-65 and ML-DSA-87 user authentication, with X.509
  and composite signature variants. (PR 1048, 1109)
- Added OpenSSH certificate user authentication to wolfSSHd, behind
  `--enable-ossh-certs`. (PR 1060)
- Added TPM resident host keys and X.509 host certificates. (PR 1033, 1081)
- Added client-side remote port forwarding, and portfwd `-r`. (PR 1066)
- Added `wolfSSH_ReadCert_file()` and `wolfSSH_CTX_AddRootCert_file()`,
  which detect PEM or DER from the content. (PR 1140, 1150)
- Added SFTP session confinement with `wolfSSH_SFTP_SetConfinePath()`,
  separate from where a session starts. (PR 1000, 1167)
- Added per-channel stderr buffering with window flow control and
  `wolfSSH_Channel{,Id}{Read,Send}Ext()`. (PR 1054)
- Added independent cipher and MAC negotiation for each direction. (PR 952)
- Added a packet-count rekey trigger and `wolfSSH_SetMsgHighwater()`.
  (PR 963)
- Added wolfSSHd's `PubkeyAuthentication` directive. (PR 1011)
- Added `StrictModes` to wolfSSHd. (PR 1042)
- Added `prohibit-password` and `forced-commands-only` to wolfSSHd's
  `PermitRootLogin`. (PR 1111)
- Added `%u`, `%h` and `%%` expansion to `AuthorizedKeysFile`. (PR 1064)
- Added `PermitEmptyPasswords` support to wolfSSHd. (PR 986)
- Added `make sbom` targets producing CycloneDX and SPDX output. (PR 1050)
- Ported the Zephyr test sample to 4.4.0, still building on 3.4.0. (PR 1065)
- Added `wolfssh-options`, a build option probe for test scripts. (PR 1180)
- Added an `-E` log file option to the `wolfssh` client, and built and
  tested the client app in CI. (PR 1168)
- Added CI for X.509 interop, code coverage, threaded SFTP and SCP tests on
  Windows, the API tests under TPM, and a heapmath build.
  (PR 989, 1158, 1058, 1165, 1038)
- Added key exchange and user authentication tests for corrupted signatures,
  ed25519 server keys, certificate auth and the pre-auth message gate.
  (PR 1051, 924, 968, 992, 1068, 923, 929, 925)
- Added channel and SFTP limit tests, including forged handles and the AEAD
  IV increment. (PR 1057, 943, 875, 1112, 1205)
- Added tests that secrets are zeroized on free and in DH KEX. (PR 980)
- Added wolfSSHd authentication tests for the privilege drop and authorized
  keys rejection. (PR 994, 1107, 914, 1100)
- Added tests for the protocol state machine's rejections. (PR 990, 935)

## Improvements

- Validated peer DH and ECDH public keys before key agreement, rejecting
  degenerate and off-curve values. (PR 1049, 1077)
- Rejected packets with less than the minimum padding, and refused
  password-change auth requests. (PR 1049)
- Reworked DH group exchange to honour the client's size window and enforce
  a 2048-bit floor. See Notes. (PR 1056)
- Bounded KEXINIT name-list parsing, closing a pre-auth CPU DoS. (PR 1062)
- Rejected inbound packets that are not cipher-block aligned. (PR 1189)
- Capped failed authentication attempts, validated the algorithm list
  setters, and zeroized transport buffers before free. See Notes. (PR 1117)
- Reworked `wolfSSH_RsaVerify()` to compare blocks in constant time rather
  than parse the recovered padding. (PR 1203)
- Validated the ECC curve name in user auth, and documented the user auth
  and public key check callback contracts. (PR 1141)
- Sanitized control bytes in `wolfSSH_Log()`, closing log injection.
  (PR 1031)
- Hardened SCP and SFTP against symlinks, masked special bits from peer
  modes, and added a recursive receive depth guard.
  (PR 1015, 1034, 1032, 999, 1037, 991)
- Bounded peer-declared SFTP request, NAME response and window sizes before
  allocating. (PR 1025, 1036, 995)
- Capped open SFTP handles per session. Thanks to @loganaden. (PR 1135)
- Tracked SFTP handles per session and validated them on use. (PR 875, 997)
- Zeroized secret buffers before free across the transport, agent, SFTP and
  terminal code. (PR 1099, 1053, 947, 1129, 1108, 1106)
- Hardened wolfSSHd's PID file, chroot and group drop. (PR 1074, 1088, 1085)
- Equalized the cost of a rejected wolfSSHd password, closing a user
  enumeration timing oracle. (PR 1116)
- Enforced shadow password and account aging in wolfSSHd. (PR 1184)
- Bound wolfSSHd certificate auth to the user and UPN realm. (PR 1019, 1079)
- Required an end-entity leaf and CA intermediates. (PR 1075, 1021)
- Rejected unsanitized names before writing `known_hosts`. (PR 1045)
- Replaced `atoi()` on peer-supplied fields with bounded parsers. (PR 1095)
- Gave wolfSSHd's `LoginGraceTime` a default. (PR 950)
- Added `WOLFSSH_NO_HOSTKEY_PERMS` so QNX owns host key permissions.
  (PR 1185)
- Shrank the `WOLFSSH` struct by about 4KB per connection. (PR 1104)
- Centralized the AES cipher lifecycle so a context is never keyed or freed
  uninitialized. (PR 1043)
- Advertised `ext-info-s` from the server. (PR 998)
- Made the client skip non-SSH banner lines before the version. (PR 959)
- Cleared all 378 clang-tidy findings. (PR 1002)
- Reworked the SFTP parsers onto the bounds-checked `Get*` helpers. (PR 961)
- Sanity checked the server's SFTP version, and reported a bad PEM as
  `WS_PARSE_E`. (PR 1133, 1151)
- Checked that a decoded private host key yields a public key. (PR 1121)
- Refactored the echoserver's forwarding, agent and shell paths. (PR 962)
- Added `CONTRIBUTING.md`. (PR 1160)
- Converted permission constants to octal, made the sources 7-bit ASCII
  clean, and restored C89 compliance. (PR 1007, 987, 948, 1087, 1175)
- Required wolfSSL built with `--enable-wolfssh` for `--enable-certs`, and
  tidied two build inputs. (PR 938, 960, 957)
- Removed `wolfSSH_CTX_SetFwdEnable()` and `wolfSSH_SetFwdEnable()`, which
  were declared but never defined. (PR 1210)
- Warned when a channel open arrives with no `channelOpenCb`. (PR 954, 939)
- Improved MQX filesystem compatibility, and exported two Windows directory
  wrappers. (PR 941, 958)
- Terminated the `ES_ERROR()` messages with a newline. (PR 1208)
- Reworked the wolfSSHd test harness, which passed while silently skipping
  the last eleven tests. (PR 1174, 1180, 1024)
- Hardened `sftp.test`'s ready-file wait, dropped the external test.
  (PR 1206)
- Fixed an `api.test` SFTP race under `make -j check` and two wolfSSHd test
  dependencies on the host. (PR 1142, 1115, 1040)
- Let the tests fall back when RSA or ECDSA is disabled. (PR 951, 1187)
- Updated CI actions, wolfSSL versions and timeouts.
  (PR 970, 984, 1046, 1114)

## Fixes

- Fixed an auth bypass under `WOLFSSH_ALLOW_USERAUTH_NONE`. (PR 940)
- Fixed four fail-open wolfSSHd `Match` defects. (PR 1003, 1027, 1039, 1026)
- Fixed wolfSSHd `Match` composition, which applied only the first matching
  block and inherited from the preceding one. (PR 1153, 1186)
- Fixed `PermitRootLogin` gating on the name root, not UID 0. (PR 1073)
- Fixed wolfSSHd to fail closed when a privilege drop fails. (PR 1067)
- Fixed a stack over-read in the Windows pseudo-console resize. (PR 1005)
- Fixed a `pty-req` mode size wrap when stdin is not a terminal. (PR 1130)
- Fixed a heap over-read parsing an `SSH_FXP_HANDLE` reply, and a one-byte
  overflow in `LoadTpmSshKey()`. (PR 1083, 1164)
- Fixed out-of-bounds reads in `wolfSSH_DoOSC()` and
  `wolfSSH_DoControlSeq()`. (PR 1035, 1076)
- Fixed a stack out-of-bounds write in the example client, and a missing RFC
  6187 name length check. (PR 1004, 1052)
- Fixed six SFTP states dropping unsent bytes on a partial send. (PR 1008)
- Fixed SFTP and SCP transfers failing on a mid-flight rekey.
  (PR 1001, 1018)
- Fixed the client SFTP VERSION and DATA length reads, which could not
  recover from a short read. (PR 1138, 1181)
- Fixed `wolfSSH_SFTP_Put()` reporting success on a rejected write.
  (PR 1182)
- Fixed a resumed SFTP put truncating the destination. (PR 1191)
- Fixed `wPread()` and `wPwrite()` dropping the high offset word past 4 GiB.
  (PR 1166, 1172)
- Fixed `WFSEEK()` return checks on Nucleus and Harmony. (PR 1145)
- Fixed the SFTP `readdir` double free, and three FATFS attribute defects.
  (PR 973, 978, 974, 977)
- Fixed data loss in the wolfSSHd shell relay on `EINTR`, stderr backlog and
  `WS_WANT_WRITE`. (PR 996)
- Fixed wolfSSHd dropping channel data while the window was full. (PR 1212)
- Fixed wolfSSHd spinning a core on an idle SFTP session. (PR 1207)
- Fixed the `wolfssh` client discarding the remote command. (PR 1162)
- Fixed `ssh://hostname` destinations with no explicit port. (PR 1006)
- Fixed the agent failing every RSA identity above 2048 bits. (PR 1179)
- Fixed `SignHashRsa()` storing a signed error in a `word32`. (PR 1131)
- Fixed Ed25519 agent authentication sending no signature. (PR 1196)
- Fixed `DoAsn1Key()` treating ECDSA and Ed25519 public keys as RSA.
  (PR 1137)
- Fixed public key auth with Ed25519 or ML-DSA but no RSA or ECDSA.
  (PR 1183)
- Fixed name-list parsing to match OpenSSH on empty elements. (PR 1132)
- Fixed the KEXINIT language list skips and a folded trailing comma.
  (PR 1055)
- Fixed `first_kex_packet_follows` handling on both sides.
  (PR 927, 956, 1056)
- Fixed the client accepting an unencrypted `CHANNEL_OPEN` pre-KEX.
  (PR 1147)
- Fixed service messages being accepted while keying. (PR 1200)
- Fixed a bad service request leaving userauth state in place.
  (PR 1201, 953)
- Fixed the server not sending `USERAUTH_FAILURE` on a reject. (PR 1202)
- Fixed three keyboard-interactive defects: a response-count mismatch, the
  attempt cap and unvalidated prompts. (PR 1070, 1127, 1199)
- Fixed three message parsers consuming the wrong fields. (PR 937, 949, 942)
- Fixed the userauth username not being bound to the first request.
  (PR 1063)
- Fixed `wolfSSH_shutdown()` looking up the channel by the wrong ID, and
  made a disconnect end the session. (PR 1190)
- Fixed the drivers and `DoPacket()` running past a disconnect. (PR 1211)
- Fixed KEX failures aborting with no `SSH_MSG_DISCONNECT`. (PR 1091, 1171)
- Fixed seven channel and connection input-handling defects. (PR 1177)
- Fixed `wolfSSH_ChannelRead()` returning the window-adjust result rather
  than the byte count. (PR 1192)
- Fixed four channel message defects, including a missing size check.
  (PR 982)
- Fixed the SCP server sending a duplicate file header. (PR 1128)
- Fixed a recursive SCP source aborting on a separator-less path, and an
  exact-fit `ScpBuffer` rejected. (PR 1020, 1161)
- Fixed an SCP base path leak and privileges left raised. (PR 1125, 1126)
- Fixed loading a PKCS#8 PEM host key, and PEM private keys without
  `WOLFSSH_CERTS`. (PR 1118, 1119)
- Fixed root CA bundles loading only their first certificate, and a replaced
  host certificate being appended instead. (PR 1149, 1152)
- Fixed `wolfSSHD_ConfigSetAuthKeysFile()` not marking it set. (PR 1044)
- Fixed `LoginGraceTime` never being armed on Windows. (PR 1028)
- Fixed three wolfSSHd config defects around `Include` and trailing
  newlines. (PR 1029, 1023, 1156)
- Fixed `doAutopilot()`'s retry loop never iterating. (PR 993)
- Fixed the examples loading no public key under `WOLFSSH_NO_RSA`. (PR 1170)
- Fixed the portfwd example printing the SSH password. (PR 1198)
- Fixed three echoserver defects around agent sockets and rekeys. (PR 1209)
- Fixed nine issues from a security audit. (PR 1143)
- Fixed twelve memory-safety and error-handling findings. (PR 1136)
- Fixed twenty-four agent, SFTP, terminal and FPKI findings. (PR 1103)
- Fixed ten integer underflow and bounds defects. (PR 1096)
- Fixed six reported issues, including an unsigned underflow in
  `wstrncat()`. Thanks to Asif Nadaf for the report. (PR 1084)
- Hardened `DoOpenSshKey()` parsing, and fixed `wc_InitDecodedCert()` given
  the wrong argument. Thanks to Asif Nadaf. (PR 1078, 1080)
- Fixed the remaining fuzz findings: negative mpints and RSA signature blob
  parsing. (PR 1022, issue #1013)
- Fixed memory leaks in wolfSSHd and the echoserver, and a public key lookup
  that stopped at the first mismatch. (PR 1071, 1038)
- Fixed a password overflow check and a large stack key struct. (PR 1122)
- Fixed missing `WMALLOC` checks in the SFTP client. (PR 1124)
- Fixed Windows resource cleanup in three paths. (PR 981, 1163)
- Fixed SFTP short writes reporting success. (PR 998)
- Fixed an SFTP resume name filling the whole name field. (PR 1105)
- Fixed `ssh->fs` used out of scope, and a Zephyr build break.
  (PR 1082, 1144)
- Fixed `GetOpenSshPublicKey()` ignoring a failed key-type parse. (PR 1193)
- Fixed the `--disable-server` build, and a `Base16_Decode` collision under
  `--enable-tpm`. (PR 1155, 1169)
- Fixed a declined host key accepted, an Ed25519 verify overwriting its
  failure, and an SFTP read underflow. (PR 969)
- Static analysis fixes in the disconnect handlers, the client config and
  two wolfSSHd paths. (PR 965, 988)
- Static analysis fixes in `DoProtoId()`, `DoNewKeys()`, `DoKexInit()` and
  the client buffers. (PR 983)
- Static analysis fixes in the GEX state and the key readers. (PR 976)
- Static analysis fixes for an uninitialized variable, a `word16` truncation
  and a double free. (PR 945, 946, 944, 948)
- Static analysis fixes in `VerifyMac()`, `IdentifyAsn1Key()` and three SFTP
  handlers. (PR 967, 971, 972, 961)
- Static analysis fixes in the user auth and SFTP paths. (PR 966, 979)
- Coverity fixes for unchecked returns, an uninitialized scalar, a TOCTOU
  and dead code. (PR 964, 1041, 1110, 1154, 1134)
- Coverity fixes in wolfscp, wolfsftp, the client app and wolfSSHd.
  (PR 1139)
- Fixed the cppcheck `identicalInnerCondition` and `uninitvar` bugs.
  (PR 1204)
- Added a missing `wc_Sha256Free()` in `agent.c`. (PR 930)
