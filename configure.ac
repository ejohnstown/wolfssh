# wolfssh
# Copyright (C) 2014-2023 wolfSSL Inc.
# All right reserved.

AC_COPYRIGHT([Copyright (C) 2014-2023 wolfSSL Inc.])
AC_INIT([wolfssh],[1.4.14],[support@wolfssl.com],[wolfssh],[https://www.wolfssl.com])
AC_PREREQ([2.63])
AC_CONFIG_AUX_DIR([build-aux])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.11 -Wall -Werror -Wno-portability foreign tar-ustar subdir-objects no-define color-tests])

AC_ARG_PROGRAM
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h:config.h.in wolfssh/user_settings.h])

WOLFSSH_LIBRARY_VERSION=15:1:7
#                       | | |
#                +------+ | +---+
#                |        |     |
#               current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                |        |        or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed
AC_SUBST([WOLFSSH_LIBRARY_VERSION])

LT_PREREQ([2.2])
LT_INIT([disable-static win32-dll])

# silent
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

gl_VISIBILITY
WOLFSSH_CFLAGS="$CFLAG_VISIBILITY"

# Checks for type sizes.
AC_CHECK_SIZEOF([long long])
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([off_t])

# Check headers/libs
AC_CHECK_HEADERS([sys/select.h sys/time.h pty.h util.h termios.h])
AC_CHECK_LIB([network],[socket])
AC_CHECK_LIB([util],[forkpty])

# liboqs
AC_ARG_WITH([liboqs],
    [AS_HELP_STRING([--with-liboqs=PATH],[Path to liboqs install (default /usr/local) EXPERIMENTAL!])],
    [ENABLED_LIBOQS=$withval],[ENABLED_LIBOQS='no'])

AS_IF([test $ENABLED_LIBOQS != 'no'],[
    AS_IF([test $ENABLED_LIBOQS = 'yes'],[OQSPATH='/usr/local'],[OQSPATH="$ENABLED_LIBOQS"])
    SAVE_LDFLAGS="$LDFLAGS"
    SAVE_LIBS="$LIBS"
    LDFLAGS="-L$OQSPATH/lib $LDFLAGS"
    AC_SEARCH_LIBS([OQS_init],[oqs],
        [ENABLED_LIBOQS='yes'; AC_DEFINE([WOLFSSH_HAVE_LIBOQS],[1],[Set to 1 for using liboqs])],
        [AC_MSG_ERROR([liboqs isn't found. If it's already installed, specify its path using --with-liboqs=/dir])]
    )
    LDFLAGS="$SAVE_LDFLAGS"
    LIBS="$SAVE_LIBS"
    WOLFSSH_LIBS="-loqs $WOLFSSH_LIBS"
    WOLFSSH_LDFLAGS="-L$OQSPATH/lib $WOLFSSH_LDFLAGS"
    WOLFSSH_CPPFLAGS="-I$OQSPATH/include $WOLFSSH_CPPFLAGS"
])

#wolfssl
AC_MSG_CHECKING([for wolfSSL])
if test "x$prefix" = "xNONE"
then
    wcpath=$ac_default_prefix
else
    wcpath=$prefix
fi
AC_MSG_NOTICE([prefix ${prefix}])
AC_ARG_WITH([wolfssl],
  [AS_HELP_STRING([--with-wolfssl=PATH], [PATH to wolfssl install (default /usr/local)])],
  [
    if test "$withval" != "no" ; then
      if test -d "${withval}/lib" && test -d "${withval}/include"; then
        wcpath=${withval}

        LDFLAGS="-L${wcpath}/lib $LDFLAGS"
        CPPFLAGS="-I${wcpath}/include $CPPFLAGS"
      else
        AC_MSG_ERROR([wolfSSL path error (${withval}): missing lib and include])
      fi
    fi
  ]
)

AC_CHECK_LIB([wolfssl],[wolfCrypt_Init],,[AC_MSG_ERROR([libwolfssl is required for ${PACKAGE}. It can be obtained from https://www.wolfssl.com/download.html/ .])])
AC_CHECK_FUNCS([gethostbyname getaddrinfo gettimeofday inet_ntoa memset socket wc_ecc_set_rng])
AC_CHECK_DECLS([[pread],[pwrite]],,[unistd.h])

# DEBUG
AX_DEBUG
AS_IF([test "$ax_enable_debug" = 'yes'],
      [AC_DEFINE([DEBUG_WOLFSSH],[1],[Define to '1' for a debug build])
       WOLFSSH_CFLAGS="-g -O0 $WOLFSSH_CFLAGS"])

AX_PTHREAD([
    AC_DEFINE([HAVE_PTHREAD], [1], [Define if you have POSIX threads libraries and header files.])
    # If AX_PTHREAD is adding -Qunused-arguments, need to prepend with
    # -Xcompiler libtool will use it. Newer versions of clang don't need
    # the -Q flag when using pthreads.
    AS_CASE([$PTHREAD_CFLAGS],[-Qunused-arguments*],[PTHREAD_CFLAGS="-Xcompiler $PTHREAD_CFLAGS"])
    WOLFSSH_CFLAGS="$WOLFSSH_CFLAGS $PTHREAD_CFLAGS"])

AX_AM_JOBSERVER([yes])

# Inline Build
AC_ARG_ENABLE([inline],
    [AS_HELP_STRING([--disable-inline],[Disable inline functions (default: enabled)])],
    [ENABLED_INLINE="$enableval"],[ENABLED_INLINE='yes'])

# Examples Build
AC_ARG_ENABLE([examples],
    [AS_HELP_STRING([--disable-examples],[Disable examples (default: enabled)])],
    [ENABLED_EXAMPLES=$enableval],[ENABLED_EXAMPLES=yes])

# Key Generation
AC_ARG_ENABLE([keygen],
    [AS_HELP_STRING([--enable-keygen],[Enable key generation (default: disabled)])],
    [ENABLED_KEYGEN=$enableval],[ENABLED_KEYGEN=no])

# SCP
AC_ARG_ENABLE([scp],
    [AS_HELP_STRING([--enable-scp],[Enable scp support (default: disabled)])],
    [ENABLED_SCP=$enableval],[ENABLED_SCP=no])

# SFTP
AC_ARG_ENABLE([sftp],
    [AS_HELP_STRING([--enable-sftp],[Enable SFTP support (default: disabled)])],
    [ENABLED_SFTP=$enableval],[ENABLED_SFTP=no])

# SSHD
AC_ARG_ENABLE([sshd],
    [AS_HELP_STRING([--enable-sshd],[Enable SSHD support (default: disabled)])],
    [ENABLED_SSHD=$enableval],[ENABLED_SSHD=no])

# TCP/IP Forwarding
AC_ARG_ENABLE([fwd],
    [AS_HELP_STRING([--enable-fwd],[Enable TCP/IP Forwarding support (default: disabled)])],
    [ENABLED_FWD=$enableval],[ENABLED_FWD=no])

# pseudo-terminal
AC_ARG_ENABLE([term],
    [AS_HELP_STRING([--disable-term],[Disable pseudo-terminal support (default: enabled)])],
    [ENABLED_PTERM=$enableval],[ENABLED_PTERM=yes])

# shell support
AC_ARG_ENABLE([shell],
    [AS_HELP_STRING([--enable-shell],[Enable echoserver shell support (default: disabled)])],
    [ENABLED_SHELL=$enableval],[ENABLED_SHELL=no])

# ssh-agent
AC_ARG_ENABLE([agent],
    [AS_HELP_STRING([--enable-agent],[Enable ssh-agent support (default: disabled)])],
    [ENABLED_AGENT=$enableval],[ENABLED_AGENT=no])

# X.509 certs
AC_ARG_ENABLE([certs],
    [AS_HELP_STRING([--enable-certs],[Enable X.509 cert support (default: disabled)])],
    [ENABLED_CERTS=$enableval],[ENABLED_CERTS=no])

# smallstack
AC_ARG_ENABLE([smallstack],
    [AS_HELP_STRING([--enable-smallstack],[Enable small stack (default: disabled)])],
    [ENABLED_SMALLSTACK=$enableval],[ENABLED_SMALLSTACK=no])

# use PAM lib
AC_ARG_WITH([pam],
    [AS_HELP_STRING([--with-pam=PATH],[PATH to directory with the PAM library])],
    [PAM_LIB=$withval],
    [PAM_LIB=""])

# Enable All
AC_ARG_ENABLE([all],
    [AS_HELP_STRING([--enable-all],[Enable all wolfSSH features (default: disabled)])],
    [ENABLED_ALL=$enableval],[ENABLED_ALL=no])

# Distro build
AC_ARG_ENABLE([distro],
    [AS_HELP_STRING([--enable-distro],[Enable wolfSSH distro build (default: disabled)])],
    [ENABLED_DISTRO="$enableval"],[ENABLED_DISTRO='no'])

# Update the ENABLED flags for options that affect other options.
AS_IF([test "$ENABLED_DISTRO" = 'yes'],[ENABLED_ALL='yes'; enable_shared='yes'; enable_static='yes'])
AS_IF([test "$ENABLED_ALL" = 'yes'],[ENABLED_KEYGEN='yes'; ENABLED_SCP='yes'; ENABLED_SFTP='yes'; ENABLED_FWD='yes'; ENABLED_SHELL='yes'; ENABLED_AGENT='yes'; ENABLED_SSHD='yes'; ENABLED_CERTS='yes'])
AS_IF([test "$ENABLED_SSHD" = 'yes'],[ENABLED_SHELL='yes'])

# Set the defined flags for the code.
AS_IF([test "$ENABLED_INLINE" = 'no'],[AC_DEFINE([NO_INLINE],[1],[Define to turn off inline functions])])
AS_IF([test "$ENABLED_KEYGEN" = 'yes'],[AC_DEFINE(WOLFSSH_KEYGEN,[1],[Define to enable the keygen functions])])
AS_IF([test "$ENABLED_SCP" = 'yes'],[AC_DEFINE(WOLFSSH_SCP,[1],[Define to enable SCP])])
AS_IF([test "$ENABLED_SFTP" = 'yes'],[AC_DEFINE(WOLFSSH_SFTP,[1],[Define to enable SFTP])])
AS_IF([test "$ENABLED_FWD" = 'yes'],[AC_DEFINE(WOLFSSH_FWD,[1],[Define to enable port forwarding])])
AS_IF([test "$ENABLED_PTERM" = 'yes'],[AC_DEFINE(WOLFSSH_PTERM,[1],[Define to enable pseudoterminal])])
AS_IF([test "$ENABLED_SHELL" = 'yes'],[AC_DEFINE(WOLFSSH_SHELL,[1],[Define to enable shell support])])
AS_IF([test "$ENABLED_AGENT" = 'yes'],[AC_DEFINE(WOLFSSH_AGENT,[1],[Define to enable SSH-AGENT])])
AS_IF([test "$ENABLED_CERTS" = 'yes'],[AC_DEFINE(WOLFSSH_CERTS,[1],[Define to enable certificate support])])
AS_IF([test "$ENABLED_SMALLSTACK" = 'yes'],[AC_DEFINE(WOLFSSH_SMALLSTACK,[1],[Define to build for small stack])])
AS_IF([test "$ENABLED_SSHD" = 'yes'],[AC_DEFINE(WOLFSSH_SSHD,[1],[Define to enable sshd server])])

if test "$ENABLED_SSHD" = 'yes'; then
    if test -n "$PAM_LIB"
    then
        AC_MSG_CHECKING([for directory $PAM_LIB])
        if ! test -d "$PAM_LIB"
        then
            AC_MSG_ERROR([PAM lib dir $PAM_LIB not found.])
        fi
        AC_MSG_RESULT([yes])
        AM_LDFLAGS="-L$PAM_LIB $AM_LDFLAGS"

        LIBS="$LIBS -lpam"
        AM_CPPFLAGS="$AM_CPPFLAGS -DWOLFSSH_USE_PAM"
        #TODO check on link to lib
        #AC_CHECK_LIB([pam], [pam],
        #    [AM_CPPFLAGS="$AM_CPPFLAGS -DWOLFSSH_USE_PAM"; LIBS="$LIBS -lpam"],
        #    [AC_MSG_ERROR(libpam not found)])
    else
        case $host in
            *qnx*)
                # QNX uses liblogin for crypt operation
                AC_CHECK_LIB([login], [login],
                    [AM_CPPFLAGS="$AM_CPPFLAGS -DWOLFSSH_HAVE_LIBLOGIN";
                        LIBS="$LIBS -llogin"],
                    [AC_MSG_ERROR(liblogin not found)])
                ;;
            *darwin*)
                AM_CPPFLAGS="$AM_CPPFLAGS -DWOLFSSH_HAVE_LIBCRYPT"
                ;;
            *)
                AC_CHECK_LIB([crypt], [crypt],
                    [AM_CPPFLAGS="$AM_CPPFLAGS -DWOLFSSH_HAVE_LIBCRYPT";
                        LIBS="$LIBS -lcrypt"],
                    [AC_MSG_ERROR(libcrypt not found)])
        esac
    fi
fi


# Set the automake conditionals.
AM_CONDITIONAL([BUILD_EXAMPLE_SERVERS],[test "x$ENABLED_EXAMPLES" = "xyes"])
AM_CONDITIONAL([BUILD_EXAMPLE_CLIENTS],[test "x$ENABLED_EXAMPLES" = "xyes"])
AM_CONDITIONAL([BUILD_INLINE],[test "x$ENABLED_INLINE" = "xyes"])
AM_CONDITIONAL([BUILD_KEYGEN],[test "x$ENABLED_KEYGEN" = "xyes"])
AM_CONDITIONAL([BUILD_SCP],[test "x$ENABLED_SCP" = "xyes"])
AM_CONDITIONAL([BUILD_SFTP],[test "x$ENABLED_SFTP" = "xyes"])
AM_CONDITIONAL([BUILD_FWD],[test "x$ENABLED_FWD" = "xyes"])
AM_CONDITIONAL([BUILD_TERM],[test "x$ENABLED_TERM" = "xyes"])
AM_CONDITIONAL([BUILD_SHELL],[test "x$ENABLED_SHELL" = "xyes"])
AM_CONDITIONAL([BUILD_AGENT],[test "x$ENABLED_AGENT" = "xyes"])
AM_CONDITIONAL([BUILD_SSHD],[test "x$ENABLED_SSHD" = "xyes"])
AM_CONDITIONAL([BUILD_CERTS],[test "x$ENABLED_CERTS" = "xyes"])

AX_HARDEN_CC_COMPILER_FLAGS

CREATE_HEX_VERSION
AC_SUBST([WOLFSSH_CFLAGS])
AC_SUBST([WOLFSSH_CPPFLAGS])
AC_SUBST([WOLFSSH_LDFLAGS])
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

# FINAL
AC_CONFIG_FILES([Makefile wolfssh/version.h])

AX_CREATE_GENERIC_CONFIG

AC_OUTPUT

# force make clean
AS_ECHO(["---"])
AS_ECHO(["Running make clean..."])
make clean >/dev/null 2>&1

# output config summary
AS_ECHO(["---"])
AS_ECHO(["Configuration summary for $PACKAGE_NAME version $VERSION"])
AS_ECHO
AS_ECHO(["   * Installation prefix:       $prefix"])
AS_ECHO(["   * System type:               $host_vendor-$host_os"])
AS_ECHO(["   * Host CPU:                  $host_cpu"])
AS_ECHO(["   * C Compiler:                $CC"])
AS_ECHO(["   * C Flags:                   $CFLAGS"])
AS_ECHO(["   * CPP Flags:                 $CPPFLAGS"])
AS_ECHO(["   * Linker Flags:              $LDFLAGS"])
AS_ECHO
AS_ECHO(["   Features"])
AS_ECHO(["   * Inline Code:               $ENABLED_INLINE"])
AS_ECHO(["   * Small stack:               $ENABLED_SMALLSTACK"])
AS_ECHO(["   * keygen:                    $ENABLED_KEYGEN"])
AS_ECHO(["   * psuedo-terminal:           $ENABLED_PTERM"])
AS_ECHO(["   * echoserver shell support:  $ENABLED_SHELL"])
AS_ECHO(["   * scp:                       $ENABLED_SCP"])
AS_ECHO(["   * sftp:                      $ENABLED_SFTP"])
AS_ECHO(["   * sshd:                      $ENABLED_SSHD"])
AS_ECHO(["   * agent:                     $ENABLED_AGENT"])
AS_ECHO(["   * TCP/IP Forwarding:         $ENABLED_FWD"])
AS_ECHO(["   * X.509 Certs:               $ENABLED_CERTS"])
AS_ECHO(["   * Examples:                  $ENABLED_EXAMPLES"])
AS_ECHO(["   * liboqs Integration:        $ENABLED_LIBOQS"])
