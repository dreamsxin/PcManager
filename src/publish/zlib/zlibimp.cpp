/**
* @file    ksupdecode.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-01-19 15:23
*/

#include "stdafx.h"
#include "zlibimp.h"
#include <atlfile.h>
#include <common\utility.h>
#include "winmod/winpath.h"


#define ZLIB_SINGLE_FILE_MAX_SIZE         (1000 * 1000 * 1000)  ///< 单个文件的最大体积
#define ZLIB_DECOMPRESS_INIT_BUFF_SIZE    (1000 * 1000)       ///< 解压缩输出的初始体积

typedef int (*PFN_uncompress)(
    Bytef *dest,   
    uLongf *destLen,
    const Bytef *source, 
    uLong sourceLen
    );
#define FN_uncompress "uncompress"

typedef int (*PFN_compress2)(
    Bytef *dest,
    uLongf *destLen,
    const Bytef *source,
    uLong sourceLen,
    int level
    );
#define FN_compress2 "compress2"

typedef int (*PFN_compress)(
    Bytef *dest,
    uLongf *destLen,
    const Bytef *source,
    uLong sourceLen
    );
#define FN_compress "compress"

typedef int (*PFN_inflate)(
    z_streamp strm, 
    int flush
    );
#define FN_inflate "inflate"

typedef int (*PFN_inflateEnd)(
    z_streamp strm
    );
#define FN_inflateEnd "inflateEnd"

typedef int (*PFN_inflateInit_)(
    z_streamp strm,
    const char *version, 
    int stream_size
    );
#define FN_inflateInit_ "inflateInit_"

typedef int (*PFN_inflateInit2_)(
    z_streamp strm,
    int windowBits,
    const char *version, 
    int stream_size
    );
#define FN_inflateInit2_ "inflateInit2_"

#define zlib_inflateInit(strm) \
    zlib_inflateInit_((strm),                ZLIB_VERSION, sizeof(z_stream))
#define zlib_inflateInit2(strm, windowBits) \
    zlib_inflateInit2_((strm), (windowBits), ZLIB_VERSION, sizeof(z_stream))

PFN_compress            zlib_compress       = NULL;
PFN_compress2           zlib_compress2      = NULL;
PFN_uncompress          zlib_uncompress     = NULL;
PFN_inflate             zlib_inflate        = NULL;
PFN_inflateEnd          zlib_inflateEnd     = NULL;
PFN_inflateInit_        zlib_inflateInit_   = NULL;
PFN_inflateInit2_       zlib_inflateInit2_  = NULL;




#if defined(USE_STATIC_ZLIB)
#include "zlib123/zlib.h"
#ifdef _DEBUG
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif
#else
#ifdef _DEBUG
#define ZLIB_FILE_NAME  L"zlib1d.dll"
#else
#define ZLIB_FILE_NAME  L"zlib1.dll"
#endif
HMODULE g_hZlib = NULL;
#endif

HRESULT CZlib::InitZlib()
{
#if defined(USE_STATIC_ZLIB)
	zlib_compress       = compress;
	zlib_compress2      = compress2;
	zlib_uncompress     = uncompress;
	zlib_inflate        = inflate;
	zlib_inflateEnd     = inflateEnd;
	zlib_inflateInit_   = inflateInit_;
	zlib_inflateInit2_  = inflateInit2_;
	return S_OK;
#else
	HRESULT             hr = S_OK;
	WinMod::CWinPath    modpath;

    hr = CAppPath::Instance().GetLeidianAppPath( modpath.m_strPath );
    if ( FAILED( hr ) )
    {
        return hr;
    }

    modpath.Append( ZLIB_FILE_NAME );

    m_lock.Lock();
    if ( NULL == g_hZlib )
    {
        g_hZlib = LoadLibrary( modpath );
        if ( NULL == g_hZlib )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_compress = ( PFN_compress )GetProcAddress( g_hZlib, FN_compress );
        if ( NULL == zlib_compress )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_compress2 = ( PFN_compress2 )GetProcAddress( g_hZlib, FN_compress2 );
        if ( NULL == zlib_compress )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_uncompress = ( PFN_uncompress )GetProcAddress( g_hZlib, FN_uncompress );
        if ( NULL == zlib_uncompress )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_inflate = ( PFN_inflate )GetProcAddress( g_hZlib, FN_inflate );
        if ( NULL == zlib_inflate )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_inflateEnd = ( PFN_inflateEnd )GetProcAddress( g_hZlib, FN_inflateEnd );
        if ( NULL == zlib_inflateEnd )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_inflateInit_ = ( PFN_inflateInit_ )GetProcAddress( g_hZlib, FN_inflateInit_ );
        if ( NULL == zlib_inflateInit_ )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }

        zlib_inflateInit2_ = ( PFN_inflateInit2_ )GetProcAddress( g_hZlib, FN_inflateInit2_ );
        if ( NULL == zlib_inflateInit2_ )
        {
            hr = HRESULT_FROM_WIN32( GetLastError() );
            goto Exit0;
        }
    }
Exit0:
    if ( FAILED( hr ) )
    {
        zlib_compress = NULL;
        zlib_uncompress = NULL;

        FreeLibrary( g_hZlib );
        g_hZlib = NULL;
    }

    m_lock.Unlock();
    return hr;
#endif
}

void CZlib::UninitZlib()
{
#if defined(USE_STATIC_ZLIB)
#else
    m_lock.Lock();

    if ( g_hZlib )
    {
        FreeLibrary( g_hZlib );
        g_hZlib = NULL;
    }

    m_lock.Unlock();
#endif
}

HRESULT CZlib::DecodeFile( 
    LPCWSTR lpwszSrcFile, 
    LPCWSTR lpwszDstFile, 
    DWORD dwDstSize 
    )
{
    if ( zlib_uncompress )
    {
        CAtlFile hInFile;
        HRESULT hr = hInFile.Create(
            lpwszSrcFile,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_DELETE,
            OPEN_EXISTING);
        if (FAILED(hr))
            return hr;

        CAtlFile hOutFile;
        hr = hOutFile.Create(
            lpwszDstFile,
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_DELETE,
            CREATE_ALWAYS);
        if (FAILED(hr))
            return hr;

        ULONGLONG uInFileSize = 0;
        hr = hInFile.GetSize(uInFileSize);
        if (FAILED(hr))
            return hr;

        // 0长度文件不需要解压
        if (0 == uInFileSize)
            return S_OK;

        // 太大的文件不解压
        if (uInFileSize > ZLIB_SINGLE_FILE_MAX_SIZE)
            return E_FAIL;

        // 读取输入
        CAtlArray<BYTE> bufRead;
        bufRead.SetCount((size_t)uInFileSize);
        if (uInFileSize != bufRead.GetCount())
            return E_OUTOFMEMORY;


        hr = hInFile.Read(bufRead.GetData(), (DWORD)bufRead.GetCount());
        if (FAILED(hr))
            return hr;

        // 准备解压
        ULONGLONG uOutFileSize = 0;
        if ( 0 == dwDstSize )
        {
            uOutFileSize = max(uInFileSize, ZLIB_DECOMPRESS_INIT_BUFF_SIZE);
        }
        else
        {
            uOutFileSize = ( ULONGLONG )dwDstSize;
        }

        CAtlArray<BYTE> bufWrite;
        try
        {
            while (uOutFileSize <= ZLIB_SINGLE_FILE_MAX_SIZE)
            {
                bufWrite.SetCount(0);
                bufWrite.SetCount((DWORD)uOutFileSize);
                if (uOutFileSize != bufWrite.GetCount())
                    return E_OUTOFMEMORY;

                DWORD dwDecompressSize = DWORD(uOutFileSize);
                int nRet = zlib_uncompress(
                    bufWrite.GetData(),
                    &dwDecompressSize,
                    bufRead.GetData(),
                    (int)bufRead.GetCount());
                if (nRet == Z_OK && dwDecompressSize <= uOutFileSize)
                {
                    bufWrite.SetCount(dwDecompressSize);
                    break;
                }

                if (nRet != Z_BUF_ERROR)
                    return E_FAIL;

                uOutFileSize *= 2;
            }	
        }
        catch (...)
        {
            return E_FAIL;
        }

        hr = hOutFile.Write(bufWrite.GetData(), (DWORD)bufWrite.GetCount());
        if (FAILED(hr))
            return hr;

        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

HRESULT CZlib::EncodeFile( LPCWSTR lpwszSrcFile, LPCWSTR lpwszDstFile, int level )
{
    if ( zlib_compress )
    {
        CAtlFile hInFile;
        HRESULT hr = hInFile.Create(
            lpwszSrcFile,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_DELETE,
            OPEN_EXISTING);
        if (FAILED(hr))
            return hr;

        CAtlFile hOutFile;
        hr = hOutFile.Create(
            lpwszDstFile,
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_DELETE,
            CREATE_ALWAYS);
        if (FAILED(hr))
            return hr;

        ULONGLONG uInFileSize = 0;
        hr = hInFile.GetSize(uInFileSize);
        if (FAILED(hr))
            return hr;

        // 0长度文件不需要压缩
        if (0 == uInFileSize)
            return S_OK;

        // 太大的文件不压缩
        if (uInFileSize >  ZLIB_SINGLE_FILE_MAX_SIZE ) //假定压缩比为4:1
            return E_FAIL;

        // 读取输入
        CAtlArray<BYTE> bufRead;
        bufRead.SetCount((size_t)uInFileSize);
        if (uInFileSize != bufRead.GetCount())
            return E_OUTOFMEMORY;

        hr = hInFile.Read(bufRead.GetData(), (DWORD)bufRead.GetCount());
        if (FAILED(hr))
            return hr;

        // 准备压缩
        ULONGLONG uOutFileSize = max(uInFileSize, ZLIB_DECOMPRESS_INIT_BUFF_SIZE);
        CAtlArray<BYTE> bufWrite;
        try
        {
            while (uOutFileSize <= ZLIB_SINGLE_FILE_MAX_SIZE )
            {
                bufWrite.SetCount(0);
                bufWrite.SetCount((DWORD)uOutFileSize);
                if (uOutFileSize != bufWrite.GetCount())
                    return E_OUTOFMEMORY;

                DWORD dwcompressSize = DWORD(uOutFileSize);
                int nRet = zlib_compress2(
                    bufWrite.GetData(),
                    &dwcompressSize,
                    bufRead.GetData(),
                    (int)bufRead.GetCount(),
                    level
                    );
                if (nRet == Z_OK && dwcompressSize <= uOutFileSize)
                {
                    bufWrite.SetCount(dwcompressSize);
                    break;
                }

                if (nRet != Z_BUF_ERROR)
                    return E_FAIL;

                uOutFileSize *= 2;
            }	
        }
        catch (...)
        {
            return E_FAIL;
        }

        hr = hOutFile.Write(bufWrite.GetData(), (DWORD)bufWrite.GetCount());
        if (FAILED(hr))
            return hr;

        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

int inflate_read(LPBYTE pbySrcBuffer, LONG lSrcBuffSize, LPBYTE *ppbyDstBuffer, LONG *plDstBuffSize, BOOL bGZip)
{
#   define  CHUNK           4096

    int     nRet            = Z_DATA_ERROR;
    LONG    lReadSize       = 0;
    BYTE    pbyBufferTmp[CHUNK];
    LONG    lTotalSize      = 0;
    LPBYTE  pbyDstBuffer    = NULL;

    z_stream strm;

    strm.zalloc     =  Z_NULL;
    strm.zfree      =  Z_NULL;
    strm.opaque     =  Z_NULL;
    strm.avail_in   =  0;
    strm.next_in    =  Z_NULL;

    if (bGZip)
        nRet = zlib_inflateInit2(&strm, 47);
    else
        nRet = zlib_inflateInit(&strm);

    if (nRet != Z_OK)
        return nRet;

    strm.avail_in = lSrcBuffSize;
    strm.next_in  = pbySrcBuffer;

    do
    {
        strm.avail_out = CHUNK;
        strm.next_out  = pbyBufferTmp;

        nRet = zlib_inflate(&strm, Z_NO_FLUSH);

        switch (nRet)
        {
            case Z_NEED_DICT:
                nRet = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                goto Exit0;
        }

        lReadSize    = CHUNK - strm.avail_out;
        lTotalSize  += lReadSize;
        pbyDstBuffer = (LPBYTE)realloc(pbyDstBuffer, lTotalSize);
        memcpy(pbyDstBuffer + lTotalSize - lReadSize, pbyBufferTmp, lReadSize);
    }
    while (strm.avail_out == 0);

Exit0:

    zlib_inflateEnd(&strm);

    if (Z_STREAM_END == nRet)
    {
        *ppbyDstBuffer = pbyDstBuffer;
        *plDstBuffSize = lTotalSize;
    }
    else
    {
        if (pbyDstBuffer)
        {
            free(pbyDstBuffer);
            pbyDstBuffer = NULL;
        }
        *ppbyDstBuffer = NULL;
        *plDstBuffSize = 0;
    }

    return (Z_STREAM_END == nRet) ? Z_OK : Z_DATA_ERROR;
}

HRESULT CZlib::DecodeGZipBuffer(
    LPBYTE pbySrcBuffer, 
    LONG lSrcBuffSize,
    LPBYTE *ppbyDstBuffer, 
    LONG *plDstBuffSize
    )
{
    if (NULL == zlib_inflate)
        return E_FAIL;

    int nRet = inflate_read(pbySrcBuffer, lSrcBuffSize, ppbyDstBuffer, plDstBuffSize, TRUE);

    return (Z_OK == nRet) ? S_OK : E_FAIL;
}
