#ifndef _WOLFSSH_PBKDF_BCRYPT_H_
#define _WOLFSSH_PBKDF_BCRYPT_H_

#ifdef WOLFSSL_USER_SETTINGS
#include <wolfssl/wolfcrypt/settings.h>
#else
#include <wolfssl/options.h>
#endif
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssh/settings.h>

#ifdef __cplusplus
extern "C" {
#endif


WOLFSSH_API int wolfSSH_pbkdf_bcrypt(
        const byte* pw, word32 pwSz,
        const byte* salt, word32 saltSz,
        word32 rounds,
        byte* key, word32 keySz);


#ifdef WOLFSSH_TEST_INTERNAL

typedef struct blf_ctx {
    word32 S[4][256], P[18];
} blf_ctx;

WOLFSSH_API void wolfSSH_TestBlfInit(blf_ctx *bc);
WOLFSSH_API void wolfSSH_TestBlfKeyInit(blf_ctx *bc, byte *key, int len);
WOLFSSH_API void wolfSSH_TestBlfKeyCleanup(blf_ctx *bc);
WOLFSSH_API void wolfSSH_TestBlfEnc(blf_ctx *bc, word32 *data, int blocks);
WOLFSSH_API void wolfSSH_TestBlfDec(blf_ctx *bc, word32 *data, int blocks);

#endif /* WOLFSSH_TEST_INTERNAL */


#ifdef __cplusplus
}
#endif

#endif /* _WOLFSSH_PBKDF_BCRYPT_H_ */
