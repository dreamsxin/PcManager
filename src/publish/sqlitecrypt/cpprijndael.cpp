/**
* @file    cpprijndael.cpp
* @brief   ...
* @author  zhangrui
* @date    2009-04-20  13:35
*/

#include "cpprijndael.h"

#include <assert.h>
#include "codecfunc.h"

using namespace WinMod;



void CRijndaelCodec::GenerateKey(const char* lpPassword, int nPasswordLen)
{
    assert(lpPassword);
    CodecGenerateEncryptionKey(lpPassword, nPasswordLen, m_key);
}

void CRijndaelCodec::Encrypt(int nBytes, int nSeed, const BYTE* lpDataIn, BYTE* lpDataOut)
{
    assert(lpDataIn);
    assert(lpDataOut);
    assert(!(nBytes % 16));

    CodecAES(nSeed, TRUE, m_key, lpDataIn, nBytes, lpDataOut);
}

void CRijndaelCodec::Decrypt(int nBytes, int nSeed, const BYTE* lpDataIn, BYTE* lpDataOut)
{
    assert(lpDataIn);
    assert(lpDataOut);
    assert(!(nBytes % 16));

    CodecAES(nSeed, FALSE, m_key, lpDataIn, nBytes, lpDataOut);
}