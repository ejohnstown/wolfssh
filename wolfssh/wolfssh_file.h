/* wolfssh_file.h
 *
 * Copyright (C) 2014-2024 wolfSSL Inc.
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


#ifdef WOLFSSL_USER_SETTINGS
    #include <wolfssl/wolfcrypt/settings.h>
#else
    #include <wolfssl/options.h>
#endif
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssh/settings.h>

#ifndef WOLFSSH_WOLFSSH_FILE_H
#define WOLFSSH_WOLFSSH_FILE_H

#if 1
    #include <stdio.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/types.h>

    /* Allows wolfSSH to track a file without caring
     * about how it is handled. wolfSSH will treat this
     * as opaque. */
    typedef struct WOLFSSH_FILE {
        FILE* file;
    } WOLFSSH_FILE;

    /* Allows wolfSSH to track a directory without caring
     * about how it is handled. wolfSSH will treat this
     * as opaque. */
    typedef struct WOLFSSH_DIR {
        DIR* dir;
    } WOLFSSH_DIR;

    /* How wolfSSH knows file stats. */
    typedef struct WOLFSSH_FILE_STAT {
        struct stat stat;
    } WOLFSSH_FILE_STAT;

    /* How wolfSSH knows file modes. */
    typedef struct WOLFSSH_FILE_MODE {
        mode_t mode;
    } WOLFSSH_FILE_MODE;

    /* How wolfSSH knows offsets. */
    typedef struct WOLFSSH_OFFSET {
        word32 offset[2];
    } WOLFSSH_OFFSET;

    /* How wolfSSH knows sizes. */
    typedef struct WOLFSSH_SIZE {
        word32 size;
    } WOLFSSH_SIZE;

    #define WOLFSSH_SEEK_SET SEEK_SET
    #define WOLFSSH_SEEK_CUR SEEK_CUR
    #define WOLFSSH_SEEK_END SEEK_END
#elif defined(WINDOWS)
    typedef struct WOLFSSH_FILE WOLFSSH_FILE;
    typedef struct WOLFSSH_FILE_STAT WOLFSSH_FILE_STAT;
    typedef struct WOLFSSH_FILE_MODE WOLFSSH_FILE_MODE;
    typedef struct WOLFSSH_DIR WOLFSSH_DIR;
    typedef struct WOLFSSH_OFFSET WOLFSSH_OFFSET;
    typedef struct WOLFSSH_SIZE WOLFSSH_SIZE;
#else
    typedef struct WOLFSSH_FILE WOLFSSH_FILE;
    typedef struct WOLFSSH_FILE_STAT WOLFSSH_FILE_STAT;
    typedef struct WOLFSSH_FILE_MODE WOLFSSH_FILE_MODE;
    typedef struct WOLFSSH_DIR WOLFSSH_DIR;
    typedef struct WOLFSSH_OFFSET WOLFSSH_OFFSET;
    typedef struct WOLFSSH_SIZE WOLFSSH_SIZE;
#endif


/*
1. NO_FILESYSTEM
2. WOLFSSH_NUCLEUS
3. FREESCALE_MQX
4. WOLFSSH_FATFS
5. WOLFSSH_ZEPHYR
*/

/*
 * SSH_FXP_OPEN - opens a file by path and returns a file handle
 * SSH_FXP_CLOSE - closes file handle
 * SSH_FXP_READ - reads from file handle
 * SSH_FXP_WRITE - writes to file handle
 * SSH_FXP_REMOVE - removes file by path
 * SSH_FXP_RENAME - renames file by path
 * SSH_FXP_MKDIR - creates directory by path
 * SSH_FXP_RMDIR - removes directory by path
 * SSH_FXP_OPENDIR - opens a directory by path and returns a handle
 * SSH_FXP_READDIR - responds with the next name in the directory, by handle
 * SSH_FXP_STAT - read file attributes by filename, following symbolic links
 * SSH_FXP_LSTAT - read file attributes by path, no following symbolic links
 * SSH_FXP_FSTAT - read file attributes by handle
 * SSH_FXP_SETSTAT - set file attributes by path
 * SSH_FXP_SETFSTAT - set file attributes by handle
*/


/* WOLFSSH_FILE APIs */
WOLFSSH_API int wolfSSH_FILE_open(WOLFSSH_FILE* f,
        const char* path, const char* mode, void* heap);
WOLFSSH_API int wolfSSH_FILE_close(WOLFSSH_FILE* f);
WOLFSSH_API int wolfSSH_FILE_read(WOLFSSH_FILE* f,
        void* ptr, WOLFSSH_SIZE size, WOLFSSH_SIZE nitems,
        WOLFSSH_SIZE* out);
WOLFSSH_API int wolfSSH_FILE_write(WOLFSSH_FILE* f,
        const void* ptr, WOLFSSH_SIZE size, WOLFSSH_SIZE nitems,
        WOLFSSH_SIZE* out);
WOLFSSH_API int wolfSSH_FILE_seek(WOLFSSH_FILE* f,
        WOLFSSH_OFFSET offset, int whence);
WOLFSSH_API int wolfSSH_FILE_tell(WOLFSSH_FILE* f,
        WOLFSSH_OFFSET* offset);
WOLFSSH_API int wolfSSH_FILE_rewind(WOLFSSH_FILE* f);
WOLFSSH_API int wolfSSH_FILE_flush(WOLFSSH_FILE* f);
WOLFSSH_API int wolfSSH_FILE_fstat(WOLFSSH_FILE* f,
        WOLFSSH_FILE_STAT* stat);
WOLFSSH_API int wolfSSH_FILE_fchmod(WOLFSSH_FILE* f,
        WOLFSSH_FILE_MODE mode);

/* File path APIs */
WOLFSSH_API int wolfSSH_FILE_stat(const char* path,
        WOLFSSH_FILE_STAT* stat);
WOLFSSH_API int wolfSSH_FILE_lstat(const char* path,
        WOLFSSH_FILE_STAT* stat);
WOLFSSH_API int wolfSSH_FILE_chmod(const char* path,
        WOLFSSH_FILE_MODE mode);
WOLFSSH_API int wolfSSH_FILE_remove(const char* path);
WOLFSSH_API int wolfSSH_FILE_rename(const char* oldname,
        const char* newname);

/* WOLFSSH_DIR APIs */
WOLFSSH_API int wolfSSH_DIR_open(const char* path,
        WOLFSSH_DIR* d, void* heap);
WOLFSSH_API int wolfSSH_DIR_close(WOLFSSH_DIR* d);
WOLFSSH_API int wolfSSH_DIR_read(WOLFSSH_DIR* d);

/* Directory path APIs */
WOLFSSH_API int wolfSSH_DIR_rmdir(const char* path);
WOLFSSH_API int wolfSSH_DIR_mkdir(const char* path,
        WOLFSSH_FILE_MODE mode);

#endif /* WOLFSSH_WOLFSSH_FILE_H */
