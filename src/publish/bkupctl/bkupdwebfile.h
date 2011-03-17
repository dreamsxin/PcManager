//////////////////////////////////////////////////////////////////////////
//   File Name: downloadfile.h
//     Creator: Zhang Xiaoxuan
//     Version: 2009.6.1 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "md5/md5.h"
#include <atlfile.h>

/// hex工具类模板
template <class T_AtlString>
class CHexUtilT
{
public:

    typedef typename T_AtlString        StringType; ///< typedef for string type
    typedef typename T_AtlString::XCHAR CharType;   ///< typedef for char type

    /**
    * @brief    从二进制数据转换成十六进制数字的字符串
    * @param    pbBuf       二进制数据
    * @param    dwBufLen    二进制数据的长度(字节数)
    * @param    strHex      返回转换后得到的十六进制数字的字符串
    */
    static void HexFromBuffer(const BYTE* pbBuf, DWORD dwBufLen, StringType& strHex)
    {
        CharType szFormat[] = {'%', '0', '2', 'x', '\0'};

        strHex.Empty();
        for (DWORD i = 0; i < dwBufLen; ++i)
        {
            strHex.AppendFormat(szFormat, pbBuf[i]);
        }
    }

    /**
    * @brief    从十六进制数字的字符串转换成二进制数据
    * @param    pbBuf       二进制数据
    * @param    dwBufLen    缓冲区的长度(字节数)
    * @param    lpHex       要转换的十六进制数字字符串
    */
    static HRESULT HexToBuffer(BYTE* pbBuf, DWORD dwBufLen, const CharType* lpHex)
    {
        ::ZeroMemory(pbBuf, dwBufLen);
        const CharType* lpHexEnd = lpHex + dwBufLen * 2;
        while (lpHex < lpHexEnd && lpHex[0] && lpHex[1])
        {
            if (!lpHex[0] || !lpHex[1])
                return E_INVALIDARG;

            BYTE highHalfByte = HexNumberValue(lpHex[0]);
            BYTE lowHalfByte  = HexNumberValue(lpHex[1]);
            if (UCHAR_MAX == highHalfByte||
                UCHAR_MAX == lowHalfByte)
                return E_FAIL;

            *pbBuf = highHalfByte << 4 | lowHalfByte;

            lpHex += 2;
            ++pbBuf;
        }       

        return S_OK;
    }

    /// 计算十六进制数字的值
    static BYTE HexNumberValue(CharType ch)
    {
        switch (ch)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (BYTE)ch - '0';
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return (BYTE)ch - 'a' + 10;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return (BYTE)ch - 'A' + 10;
        }

        return UCHAR_MAX;
    }
};

typedef CHexUtilT<CStringA> CHexUtilA;  ///< typedef for multi bytes
typedef CHexUtilT<CStringW> CHexUtilW;  ///< typedef for wide char
typedef CHexUtilT<CString>  CHexUtil;   ///< typedef for T-type


/// md5 hash 工具类
#define SKYLARK_MD5_BYTES   16
class CSLMD5
{
public:

    /// md5 init
    void Init()
    {
        MD5Init(&m_ctx);
    }

    void Update(const void* pbData, DWORD dwLen)
    {
        MD5Update(&m_ctx, ( const unsigned char* )pbData, dwLen);
    }

    void Final(BYTE pbResult[SKYLARK_MD5_BYTES])
    {
        MD5Final(pbResult, &m_ctx);
    }

    template <class T_AtlString>
    void FinalAsHex(T_AtlString& strHex)
    {
        BYTE buf[SKYLARK_MD5_BYTES];
        Final(buf);
        CHexUtilT<T_AtlString>::HexFromBuffer(buf, sizeof(buf), strHex);
    }

    static void NullFinal(BYTE pbResult[SKYLARK_MD5_BYTES])
    {
        CSLMD5 ctx;
        ctx.Init();
        ctx.Final(pbResult);
    }

    template <class T_AtlString>
    static void NullFinalAsString(T_AtlString& strHex)
    {
        BYTE buf[SKYLARK_MD5_BYTES];
        NullFinal(buf);
        CHexUtilT<T_AtlString>::HexFromBuffer(buf, sizeof(buf), strHex);
    }

protected:
    struct MD5Context m_ctx;  ///< context
};



interface IBkUpdWebFile
{
    // Call NeedHash before SetSize
    virtual HRESULT WINAPI NeedHash(BOOL bNeedHash) = 0;
    // Call SetSize before Write
    virtual HRESULT WINAPI SetSize(DWORD dwNewLen) = 0;
    virtual HRESULT WINAPI SetPos(DWORD dwNewPos) = 0;
    virtual HRESULT WINAPI Write(LPBYTE pbyBuffer, DWORD dwSize) = 0;
    virtual HRESULT WINAPI Close() = 0;
    // Call GetHash After Close
    virtual HRESULT WINAPI GetHash(CString &strHash) = 0;
};

class CBkUpdWebFileBase
    : public IBkUpdWebFile
{
public:
    CBkUpdWebFileBase()
        : m_bCalcHash(FALSE)
    {
    }

    virtual ~CBkUpdWebFileBase()
    {
    }

    HRESULT WINAPI NeedHash(BOOL bNeedHash)
    {
        m_bCalcHash = bNeedHash;
        m_ctx.Init();

        return S_OK;
    }

    HRESULT WINAPI GetHash(CString &strHash)
    {
        if (!m_bCalcHash)
            return E_FAIL;

        m_ctx.FinalAsHex(strHash);

        return S_OK;
    }
protected:
    BOOL m_bCalcHash;
    CSLMD5 m_ctx;
};

class CBkUpdWebFile
    : public CBkUpdWebFileBase
{
public:
    CBkUpdWebFile()
    {
    }

    HRESULT SetFileName(LPCTSTR lpszFileName)
    {
        m_strFileName = lpszFileName;

        return S_OK;
    }

    HRESULT GetFileName( CString& strFileName )
    {
        strFileName = m_strFileName;

        return S_OK;
    }

    HRESULT WINAPI SetSize(DWORD dwNewLen)
    {
        m_theFile.Close();

        HRESULT hRet = m_theFile.Create(
            m_strFileName, 
            FILE_GENERIC_READ | FILE_GENERIC_WRITE, 
            0, 
            CREATE_ALWAYS
            );
        if (FAILED(hRet))
            return hRet;

        return m_theFile.SetSize(dwNewLen);
    }

    HRESULT WINAPI SetPos(DWORD dwNewPos)
    {
        return m_theFile.Seek(dwNewPos, FILE_BEGIN);
    }

    HRESULT WINAPI Write(LPBYTE pbyBuffer, DWORD dwSize)
    {
        HRESULT hRet = m_theFile.Write(pbyBuffer, dwSize);

        if (SUCCEEDED(hRet) && m_bCalcHash)
            m_ctx.Update(pbyBuffer, dwSize);

        return hRet;
    }

    HRESULT WINAPI Close()
    {
        m_theFile.Close();

        return S_OK;
    }

protected:

    CString m_strFileName;

    CAtlFile m_theFile;
};


class CBkUpdWebMemoryFile
    : public CBkUpdWebFileBase
{
public:
    CBkUpdWebMemoryFile()
        : m_dwPointer(0)
        , m_dwSize(0)
        , m_pbyBuffer(NULL)
    {
    }

    ~CBkUpdWebMemoryFile()
    {
        if (m_pbyBuffer)
        {
            delete[] m_pbyBuffer;
            m_pbyBuffer = NULL;
        }
    }

    HRESULT WINAPI SetSize(DWORD dwNewLen)
    {
        if (m_pbyBuffer)
            delete[] m_pbyBuffer;

        m_pbyBuffer = new BYTE[dwNewLen];
        ::ZeroMemory(m_pbyBuffer, dwNewLen);

        m_dwSize = dwNewLen;

        return S_OK;
    }

    HRESULT WINAPI SetPos(DWORD dwNewPos)
    {
        if (dwNewPos >= m_dwSize)
            return E_INVALIDARG;
        else
        {
            m_dwPointer = dwNewPos;
            return S_OK;
        }
    }

    HRESULT WINAPI Write(LPBYTE pbyBuffer, DWORD dwSize)
    {
        if (!m_pbyBuffer)
            return E_INVALIDARG;

        if (m_dwPointer + dwSize > m_dwSize)
            return E_INVALIDARG;

        memcpy((LPBYTE)m_pbyBuffer + m_dwPointer, pbyBuffer, dwSize);

        m_dwPointer += dwSize;

        if (m_bCalcHash)
            m_ctx.Update(pbyBuffer, dwSize);

        return S_OK;
    }

    HRESULT WINAPI Close()
    {
        return S_OK;
    }

    LPBYTE GetBuffer()
    {
        return m_pbyBuffer;
    }

    DWORD GetSize()
    {
        return m_dwSize;
    }

    HRESULT SaveToFile(LPCTSTR lpszFileName)
    {
        HRESULT hRet = E_FAIL;
        CAtlFile file;

        if (!m_pbyBuffer)
            goto Exit0;

        hRet = file.Create(
            lpszFileName, 
            FILE_GENERIC_READ | FILE_GENERIC_WRITE, 
            0, 
            CREATE_ALWAYS
            );
        if (FAILED(hRet))
            goto Exit0;

        hRet = file.SetSize(m_dwSize);
        if (FAILED(hRet))
            goto Exit0;

        hRet = file.Seek(0, FILE_BEGIN);
        if (FAILED(hRet))
            goto Exit0;

        hRet = file.Write(m_pbyBuffer, m_dwSize);
        if (FAILED(hRet))
            goto Exit0;

Exit0:

        file.Close();

        return hRet;
    }

protected:

    DWORD m_dwPointer;
    DWORD m_dwSize;
    LPBYTE m_pbyBuffer;
};