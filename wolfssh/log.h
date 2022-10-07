/* log.h
 *
 * Copyright (C) 2014-2022 wolfSSL Inc.
 *
 * This file is part of wolfSSH.
 *
 * wolfSSH is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * wolfSSH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with wolfSSH.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 * The log module contains the interface to the logging function. When
 * debugging is enabled and turned on, the logger will output to STDOUT.
 * A custom logging callback may be installed.
 */


#ifndef _WOLFSSH_LOG_H_
#define _WOLFSSH_LOG_H_

#include <wolfssh/settings.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef NO_TIMESTAMP
    /* The NO_TIMESTAMP tag is deprecated. Convert to new name. */
    #define WOLFSSH_NO_TIMESTAMP
#endif


enum wolfSSH_LogLevel {
    WS_LOG_ALL = 0,
    WS_LOG_TRACE,
    WS_LOG_DEBUG,
    WS_LOG_INFO,
    WS_LOG_WARN,
    WS_LOG_ERROR,
    WS_LOG_FATAL,
    WS_LOG_OFF
};

enum wolfSSH_OldLogLevel {
    WS_LOG_CERTMAN = 9,
    WS_LOG_AGENT = 8,
    WS_LOG_SCP   = 7,
    WS_LOG_SFTP  = 6,
    WS_LOG_USER  = 5,
    WS_LOG_ERROR = 4,
    WS_LOG_WARN  = 3,
    WS_LOG_INFO  = 2,
    WS_LOG_DEBUG = 1,
    WS_LOG_DEFAULT = WS_LOG_DEBUG
};


enum wolfSSH_LogDomain {
    WS_LOG_DOMAIN_GENERAL = 0,
    WS_LOG_DOMAIN_INIT,
    WS_LOG_DOMAIN_SETUP,
    WS_LOG_DOMAIN_CONNECT,
    WS_LOG_DOMAIN_ACCEPT,
    WS_LOG_DOMAIN_CBIO,
    WS_LOG_DOMAIN_KEX,
    WS_LOG_DOMAIN_USER_AUTH,
    WS_LOG_DOMAIN_SFTP,
    WS_LOG_DOMAIN_SCP,
    WS_LOG_DOMAIN_KEYGEN,
    WS_LOG_DOMAIN_TERM,
    WS_LOG_DOMAIN_AGENT,
};


typedef void (*wolfSSH_LoggingCb)(enum wolfSSH_LogLevel,
                                  const char *const logMsg);
WOLFSSH_API void wolfSSH_SetLoggingCb(wolfSSH_LoggingCb logF);
WOLFSSH_API int wolfSSH_LogEnabled(void);


#ifdef __GNUC__
    #define FMTCHECK __attribute__((format(printf,2,3)))
    #define FMTCHECKEX __attribute__((format(printf,3,4)))
#else
    #define FMTCHECK
    #define FMTCHECKEX
#endif /* __GNUC__ */


WOLFSSH_API void wolfSSH_Log(enum wolfSSH_LogLevel,
        const char *const, ...) FMTCHECK;
WOLFSSH_API void wolfSSH_Log_ex(enum wolfSSH_LogLevel,
        enum wolfSSH_LogDomain, *const, ...) FMTCHECKEX;

#ifndef WOLFSSH_NO_LOGGING
    #define WLOG(...) do { \
                      if (wolfSSH_LogEnabled()) \
                          wolfSSH_Log(__VA_ARGS__); \
                  } while (0)
    #define WLOGEX(...) do { \
                      if (wolfSSH_LogEnabled()) \
                          wolfSSH_Log_ex(__VA_ARGS__); \
                  } while (0)
#else
    #define WLOG(...) do { ; } while (0)
    #define WLOGEX(...) do { ; } while (0)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _WOLFSSH_LOG_H_ */

