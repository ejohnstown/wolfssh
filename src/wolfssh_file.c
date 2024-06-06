/* wolfssh_file.c
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


#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssh/wolfssh_file.h>
#include <wolfssh/port.h>
#include <wolfssh/error.h>

int wolfSSH_FILE_open(WOLFSSH_FILE* f, const char* path, const char* mode,
        void* heap)
{
    WOLFSSH_UNUSED(heap);
    f->file = fopen(path, mode);
    if (!f->file) {
        return WS_BAD_FILE_E;
    }
    return WS_SUCCESS;
}

int wolfSSH_FILE_close(WOLFSSH_FILE* f)
{
    if (f && f->file) {
        fclose(f->file);
        f->file = NULL;
    }

    return WS_SUCCESS;
}

int wolfSSH_FILE_read(WOLFSSH_FILE* f,
        void* ptr, WOLFSSH_SIZE size, WOLFSSH_SIZE nitems,
        WOLFSSH_SIZE* out)
{
    size_t read;

    read = fread(ptr, size.size, nitems.size, f->file);
    out->size = (word32)read;

    return WS_SUCCESS;
}

int wolfSSH_FILE_write(WOLFSSH_FILE* f,
        const void* ptr, WOLFSSH_SIZE size, WOLFSSH_SIZE nitems,
        WOLFSSH_SIZE* out)
{
    size_t sent;

    sent = fwrite(ptr, size.size, nitems.size, f->file);
    out->size = (word32)sent;

    return WS_SUCCESS;
}

int wolfSSH_FILE_seek(WOLFSSH_FILE* f, WOLFSSH_OFFSET offset, int whence)
{
    int ret = WS_BAD_ARGUMENT;

    if (f && f->file) {
        off_t o;

        o = offset.offset[0] | (off_t)offset.offset[1] << 32;
        ret = fseeko(f->file, o, whence);
        if (ret >= 0) {
            ret = WS_SUCCESS;
        }
    }

    return ret;
}

int wolfSSH_FILE_tell(WOLFSSH_FILE* f, WOLFSSH_OFFSET* offset)
{
    int ret = WS_BAD_ARGUMENT;

    if (f && f->file && offset) {
        off_t o;

        o = ftello(f->file);
        if (o >= 0) {
            offset->offset[0] = (word32)o;
            offset->offset[1] = (word32)(o >> 32);
            ret = WS_SUCCESS;
        }
    }

    return ret;
}

int wolfSSH_FILE_rewind(WOLFSSH_FILE* f)
{
    int ret = WS_BAD_ARGUMENT;

    if (f && f->file) {
        rewind(f->file);
        ret = WS_SUCCESS;
    }

    return ret;
}

int wolfSSH_FILE_flush(WOLFSSH_FILE* f)
{
    int ret = WS_BAD_ARGUMENT;

    if (f && f->file) {
        ret = fflush(f->file);
        if (ret >= 0) {
            ret = WS_SUCCESS;
        }
    }

    return ret;
}
