#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#ifdef WOLFSSL_USER_SETTINGS
#include <wolfssl/wolfcrypt/settings.h>
#else
#include <wolfssl/options.h>
#endif

#include <stdio.h>

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
    word32 i;
    word32 datal;
    word32 datar;
    word32 saltIdx;

    /* XOR password into P-array */
    for (i = 0; i < 18; i++) {
        ctx->P[i] ^= pw[i % 16];
    }

    /* Encrypt P-array using salt as data */
    saltIdx = 0;
    for (i = 0; i < 18; i += 2) {
        datal = salt[saltIdx % 16];
        datar = salt[(saltIdx + 1) % 16];
        saltIdx += 2;

        Blowfish_encipher(ctx, &datal, &datar);

        ctx->P[i] = datal;
        ctx->P[i + 1] = datar;
    }

    /* Encrypt S-boxes using salt as data */
    for (i = 0; i < 4; i++) {
        word32 j;
        for (j = 0; j < 256; j += 2) {
            datal = salt[saltIdx % 16];
            datar = salt[(saltIdx + 1) % 16];
            saltIdx += 2;

            Blowfish_encipher(ctx, &datal, &datar);

            ctx->S[i][j] = datal;
            ctx->S[i][j + 1] = datar;
        }
    }
}


static void expand_key_part(const word32* part, blf_ctx* ctx)
{
    word32 i;
    word32 datal;
    word32 datar;

    /* XOR part into P-array */
    for (i = 0; i < 18; i++) {
        ctx->P[i] ^= part[i % 16];
    }

    /* Re-encrypt P-array using current state */
    datal = 0;
    datar = 0;
    for (i = 0; i < 18; i += 2) {
        Blowfish_encipher(ctx, &datal, &datar);
        ctx->P[i] = datal;
        ctx->P[i + 1] = datar;
    }

    /* Re-encrypt S-boxes using current state */
    for (i = 0; i < 4; i++) {
        word32 j;
        for (j = 0; j < 256; j += 2) {
            Blowfish_encipher(ctx, &datal, &datar);
            ctx->S[i][j] = datal;
            ctx->S[i][j + 1] = datar;
        }
    }
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
        byte cStr[32] = "OxychromaticBlowfishSwatDynamite";
        word32 c[8];
        word32 i;

        for (i = 0; i < 8; i++) {
            ato32(cStr + (i * 4), c + i);
        }

        for (i = 0; i < 64; i++) {
            blf_enc(&ctx, c, 4);
        }

        for (i = 0; i < 8; i++) {
            c32toa(c[i], key + (i * 4));
        }
    }

    blf_key_cleanup(&ctx);

    return WS_SUCCESS;
}

#endif /* WOLFSSH_PBKDF_BCRYPT */
