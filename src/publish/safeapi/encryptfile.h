/**
* @file    encryptfile.h
* @brief   ...
* @author  bbcallen
* @date    2010-03-05 23:26
*/

#ifndef ENCRYPTFILE_H
#define ENCRYPTFILE_H

#include <atlfile.h>

class CEncryptFile : public CAtlFile
{
public:
    HRESULT Read(
        __out_bcount(nBufSize) LPVOID pBuffer,
        DWORD nBufSize) throw()
    {
       HRESULT  hr = S_OK;

       hr = __super::Read( pBuffer, nBufSize );

       if ( SUCCEEDED( hr ) )
       {
           PBYTE   pTmp = ( PBYTE )pBuffer;
           for ( DWORD count = 0; count < nBufSize; count++ )
           {
               pTmp[ count ] = pTmp[ count ] ^ ENCRYPTION_CHAR;
           }
       }

       return hr;
    }

    HRESULT Write(
        __in_bcount(nBufSize) LPCVOID pBuffer,
        __in DWORD nBufSize,
        __out_opt DWORD* pnBytesWritten = NULL) throw()
    {
        HRESULT hr = S_OK;

        if ( pBuffer )
        {
            PBYTE  pTmp = ( PBYTE )pBuffer;

            for ( DWORD count = 0; count < nBufSize; count++ )
            {
                pTmp[ count ] = pTmp[ count ] ^ ENCRYPTION_CHAR;
            }
        }

        return __super::Write( pBuffer, nBufSize, pnBytesWritten );
    }
};

#endif//ENCRYPTFILE_H