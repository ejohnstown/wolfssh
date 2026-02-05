#ifdef WOLFSSL_USER_SETTINGS
#include <wolfssl/wolfcrypt/settings.h>
#else
#include <wolfssl/options.h>
#endif
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssh/visibility.h>

#ifndef _WOLFSSH_BLOWFISH_H_
#define _WOLFSSH_BLOWFISH_H_

#define KEYBYTES     8
#define MAXKEYBYTES 56          /* 448 bits */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blf_ctx {
    word32 S[4][256],P[18];
} blf_ctx;

WOLFSSH_API void blf_key_init(blf_ctx *bc, byte *key, int len);
WOLFSSH_API void blf_key_cleanup(blf_ctx *bc);
WOLFSSH_API void blf_enc(blf_ctx *bc, word32 *data, int blocks);
WOLFSSH_API void blf_dec(blf_ctx *bc, word32 *data, int blocks);

#ifdef __cplusplus
}
#endif

#endif /* _WOLFSSH_BLOWFISH_H_ */
