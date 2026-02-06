#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#ifdef WOLFSSL_USER_SETTINGS
#include <wolfssl/wolfcrypt/settings.h>
#else
#include <wolfssl/options.h>
#endif


#include <wolfssh/error.h>
#include <wolfssh/blowfish.h>
#include <wolfssh/pbkdf-bcrypt.h>


#ifdef NO_INLINE
    #include <wolfssh/misc.h>
#else
    #define WOLFSSH_MISC_INCLUDED
    #include "src/misc.c"
#endif


#ifdef WOLFSSH_PBKDF_BCRYPT

const char MagicStringSha512[] = "OxychromaticBlowfishSwatDynamite";


int wolfSSH_pbkdf_bcrypt(const char* pw, word32 pwSz,
        const char* salt, word32 saltSz, word32 rounds,
        byte* key, word32 keySz)
{
    printf("pw:%s pwSz:%u salt:%s saltSz:%u rounds:%u key:%p keySz:%u\n",
            pw, pwSz, salt, saltSz, rounds, key, keySz);

    return WS_SUCCESS;
}

#endif /* WOLFSSH_PBKDF_BCRYPT */
