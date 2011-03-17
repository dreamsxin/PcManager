/**
* @file    codecfunc.c
* @brief   ...
* @author  zhangrui
* @date    2009-04-20  13:46
*/

#include "codecfunc.h"

#include <assert.h>
#include "openssl\rc4.h"
#include "openssl\md5.h"
#include "openssl\aes.h"

#define MD5_HASHBYTES 16


static unsigned char g_padding[] =
"\x28\xBF\x4E\x5E\x4E\x75\x8A\x41\x64\x00\x4E\x56\xFF\xFA\x01\x08\x2E\x2E\x00\xB6\xD0\x68\x3E\x80\x2F\x0C\xA9\xFE\x64\x53\x69\x7A";


void CodecPadPassword(const char* password, int pswdlen, unsigned char pswd[32]);

void CodecGenerateInitialVector(int seed, unsigned char iv[16]);





#define MODMULT(a, b, c, m, s) q = s / a; s = b * (s - a * q) - c * q; if (s < 0) s += m

void CodecGenerateInitialVector(int seed, unsigned char iv[16])
{
  unsigned char initkey[MD5_HASHBYTES];
  int j, q;
  int z = seed + 1;
  for (j = 0; j < 4; j++)
  {
    MODMULT(52774, 40692,  3791, 2147483399L, z);
    initkey[4*j+0] = 0xff &  z;
    initkey[4*j+1] = 0xff & (z >>  8);
    initkey[4*j+2] = 0xff & (z >> 16);
    initkey[4*j+3] = 0xff & (z >> 24);
  }
  MD5((const unsigned char*) initkey, 16, iv);
}



void CodecGenerateEncryptionKey(const char* userPassword, int passwordLength, unsigned char encryptionKey[KEYLENGTH])
{
  unsigned char userPad[32];
  unsigned char ownerPad[32];
  unsigned char ownerKey[32];

  unsigned char mkey[MD5_HASHBYTES];
  unsigned char digest[MD5_HASHBYTES];
  int keyLength = MD5_HASHBYTES;
  int i, j, k;
  MD5_CTX ctx;
  RC4_KEY rc4;

  // Pad passwords
  CodecPadPassword(userPassword, passwordLength, userPad);
  CodecPadPassword("", 0, ownerPad);

  // Compute owner key


  MD5_Init(&ctx);
  MD5_Update(&ctx, ownerPad, 32);
  MD5_Final(digest,&ctx);

  // only use for the input as many bit as the key consists of
  for (k = 0; k < 50; ++k)
  {
    MD5_Init(&ctx);
    MD5_Update(&ctx, digest, keyLength);
    MD5_Final(digest,&ctx);
  }
  memcpy(ownerKey, userPad, 32);
  for (i = 0; i < 20; ++i)
  {
    for (j = 0; j < keyLength ; ++j)
    {
      mkey[j] = (digest[j] ^ i);
    }
    RC4_set_key(&rc4, keyLength, mkey);
    RC4(&rc4, 32, ownerKey, ownerKey);
  }

  // Compute encryption key

  MD5_Init(&ctx);
  MD5_Update(&ctx, userPad, 32);
  MD5_Update(&ctx, ownerKey, 32);
  MD5_Final(digest,&ctx);

  // only use the really needed bits as input for the hash
  for (k = 0; k < 50; ++k)
  {
    MD5_Init(&ctx);
    MD5_Update(&ctx, digest, keyLength);
    MD5_Final(digest, &ctx);
  }
  memcpy(encryptionKey, digest, keyLength);
}



void CodecPadPassword(const char* password, int pswdlen, unsigned char pswd[32])
{
  int m = pswdlen;
  int j;
  int p = 0;

  if (m > 32) m = 32;


  for (j = 0; j < m; j++)
  {
    pswd[p++] = (unsigned char) password[j];
  }
  for (j = 0; p < 32 && j < 32; j++)
  {
    pswd[p++] = g_padding[j];
  }
}



void
CodecAES(int page, BOOL encrypt, unsigned char encryptionKey[MD5_HASHBYTES],
         const unsigned char* datain, int databytes, unsigned char* dataout)
{
  unsigned char initial[MD5_HASHBYTES];
  unsigned char pagekey[MD5_HASHBYTES];
  unsigned char nkey[MD5_HASHBYTES+4+4];
  int keyLength = MD5_HASHBYTES;
  int nkeylen = keyLength + 4 + 4;
  int j;
  int len = 0;
  int ret = 0;
  AES_KEY aesKey;

  for (j = 0; j < keyLength; j++)
  {
    nkey[j] = encryptionKey[j];
  }
  nkey[keyLength+0] = 0xff &  page;
  nkey[keyLength+1] = 0xff & (page >>  8);
  nkey[keyLength+2] = 0xff & (page >> 16);
  nkey[keyLength+3] = 0xff & (page >> 24);

  // AES encryption needs some 'salt'
  nkey[keyLength+4] = 0x73;
  nkey[keyLength+5] = 0x41;
  nkey[keyLength+6] = 0x6c;
  nkey[keyLength+7] = 0x54;

  MD5(nkey, nkeylen, pagekey);
  CodecGenerateInitialVector(page, initial);

  if (encrypt)
  {
    ret = AES_set_encrypt_key(pagekey, sizeof(pagekey) * 8, &aesKey);
    assert(!ret);
    AES_cbc_encrypt(datain, dataout, databytes, &aesKey, initial, AES_ENCRYPT);
  }
  else
  {
    ret = AES_set_decrypt_key(pagekey, sizeof(pagekey) * 8, &aesKey);
    assert(!ret);
    AES_cbc_encrypt(datain, dataout, databytes, &aesKey, initial, AES_DECRYPT);
  }

  
  // It is a good idea to check the error code
  if (len < 0)
  {
    // AES: Error on encrypting.
  }
}