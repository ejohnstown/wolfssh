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
#include <wolfssl/wolfcrypt/hash.h>


#ifdef NO_INLINE
    #include <wolfssh/misc.h>
#else
    #define WOLFSSH_MISC_INCLUDED
    #include "src/misc.c"
#endif


#ifdef WOLFSSH_PBKDF_BCRYPT

static void expand_key_full(const word32* pw, const word32* salt, blf_ctx* ctx)
{
    WOLFSSH_UNUSED(pw);
    WOLFSSH_UNUSED(salt);
    WOLFSSH_UNUSED(ctx);
}


static void expand_key_part(const word32* part, blf_ctx* ctx)
{
    WOLFSSH_UNUSED(part);
    WOLFSSH_UNUSED(ctx);
}


static void expensive_blowfish_setup(
        const word32* password,
        const word32* salt,
        word32 rounds, blf_ctx* ctx)
{
    word32 i;

    expand_key_full(password, salt, ctx);

    for (i = 0; i < rounds; i++) {
        expand_key_part(password, ctx);
        expand_key_part(salt, ctx);
    }
}


/*
 * Preprocess the password and salt with a SHA-512 hash of each.
 * The output is expanded to 256 bits.
 * The magic string is 256 bits.
 */
int wolfSSH_pbkdf_bcrypt(const byte* pw, word32 pwSz,
        const byte* salt, word32 saltSz, word32 rounds,
        byte* key, word32 keySz)
{
    blf_ctx ctx;

    printf("pw:\"%s\" pwSz:%u salt:\"%s\" saltSz:%u "
            "rounds:%u key:%p keySz:%u\n",
            pw, pwSz, salt, saltSz, rounds, key, keySz);

    blf_init(&ctx);

    {
        word32 pwHash[WC_SHA512_DIGEST_SIZE/sizeof(word32)];
        word32 saltHash[WC_SHA512_DIGEST_SIZE/sizeof(word32)];
        word32 i;

        wc_Sha512Hash(pw, pwSz, (byte*)pwHash);
        wc_Sha512Hash(salt, saltSz, (byte*)saltHash);

        for (i = 0; i < 16; i++) {
            ato32((byte*)pwHash + (i * 4), pwHash + i);
            ato32((byte*)saltHash + (i * 4), saltHash + i);
        }

        /* 512-bits, 64-bytes. Blowfish works on 64-bit blocks. */
        expensive_blowfish_setup(pwHash, saltHash, rounds, &ctx);

        ForceZero(pwHash, sizeof(pwHash));
        ForceZero(saltHash, sizeof(saltHash));
    }

    {
        byte c[32] = "OxychromaticBlowfishSwatDynamite";
        word32 i;

        for (i = 0; i < 64; i++) {
            blf_enc(&ctx, (word32*)c, 1);
        }

        WMEMCPY(key, c, sizeof(c));
    }

    blf_key_cleanup(&ctx);

    return WS_SUCCESS;
}

#endif /* WOLFSSH_PBKDF_BCRYPT */
