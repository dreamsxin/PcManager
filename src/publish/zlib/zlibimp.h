/**
* @file    ksupdecode.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-19 15:22
*/

#ifndef DECODE_H
#define DECODE_H
#include <zlib/zlib123/zlib.h>

class CZlib
{
public:
    static CZlib& Instance()
    {
        static CZlib   _ins;
        return  _ins;
    }
    HRESULT InitZlib();
    void    UninitZlib();
    HRESULT DecodeFile( 
        LPCWSTR lpwszSrcFile, 
        LPCWSTR lpwszDstFile, 
        DWORD dwDstSize = 0 
        );
    HRESULT EncodeFile( 
        LPCWSTR lpwszSrcFile,
        LPCWSTR lpwszDstFile,
        int level = Z_DEFAULT_COMPRESSION
        );

    HRESULT DecodeGZipBuffer(
        LPBYTE pbySrcBuffer, 
        LONG lSrcBuffSize,
        LPBYTE *ppbyDstBuffer, 
        LONG *plDstBuffSize
        );
private:
    CComAutoCriticalSection m_lock;
};

#endif//DECODE_H