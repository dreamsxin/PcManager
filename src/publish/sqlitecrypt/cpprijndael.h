/**
* @file    .cpprijndaelh
* @brief   ...
* @author  zhangrui
* @date    2009-04-20  13:29
*/

#ifndef _CPPRIJNDAELH
#define _CPPRIJNDAELH

#include <wtypes.h>

#ifndef NS_WINMOD_BEGIN
#define NS_WINMOD_BEGIN    namespace WinMod {   ///< declare namespace WinMod
#define NS_WINMOD_END      };                   ///< declare namespace Skylark
#endif//NS_WINMOD_BEGIN

NS_WINMOD_BEGIN


#define WINMOD_RIJNDAEL_KEY_LENGTH 16
#define WINMOD_RIJNDAEL_MD5_BYTES  16


class CRijndaelCodec
{
public:
    /// generate key
    void GenerateKey(const char* lpPassword, int nPasswordLen);

    /// The input array length must be a multiple of 16 bytes, the remaining part is DISCARDED.
    void Encrypt(int nBytes, int nSeed, const BYTE* lpDataIn, BYTE* lpDataOut);

    /// The input array length must be a multiple of 16 bytes, the remaining part is DISCARDED.
    void Decrypt(int nBytes, int nSeed, const BYTE* lpDataIn, BYTE* lpDataOut);

private:
    BYTE      m_key[WINMOD_RIJNDAEL_KEY_LENGTH];
};








NS_WINMOD_END

#endif//_CPPRIJNDAELH