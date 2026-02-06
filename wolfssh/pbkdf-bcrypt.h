#ifdef WOLFSSL_USER_SETTINGS
#include <wolfssl/wolfcrypt/settings.h>
#else
#include <wolfssl/options.h>
#endif
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssh/visibility.h>

#ifndef _WOLFSSH_PBKDF_BCRYPT_H_
#define _WOLFSSH_PBKDF_BCRYPT_H_

#ifdef __cplusplus
extern "C" {
#endif


WOLFSSH_API int wolfSSH_pbkdf_bcrypt(
        const char* pw, word32 pwSz,
        const char* salt, word32 saltSz,
        word32 rounds,
        byte* key, word32 keySz);


#ifdef __cplusplus
}
#endif

#endif /* _WOLFSSH_PBKDF_BCRYPT_H_ */
