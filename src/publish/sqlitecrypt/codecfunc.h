/**
* @file    codecfunc.h
* @brief   ...
* @author  zhangrui
* @date    2009-04-20  13:47
*/

#ifndef CODECFUNC_H
#define CODECFUNC_H

#include <wtypes.h>

#define KEYLENGTH 16

/// Generate encryption key
EXTERN_C void CodecGenerateEncryptionKey(const char* userPassword, int passwordLength, unsigned char encryptionKey[KEYLENGTH]);

EXTERN_C void CodecAES(int page, BOOL encrypt, unsigned char encryptionKey[KEYLENGTH],
              const unsigned char* datain, int datalen, unsigned char* dataout);


#endif//CODECFUNC_H