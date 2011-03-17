/**
* @file    slhashutil.h
* @brief   ...
* @author  zhangrui
* @date    2008-12-22  01:23
*/

#ifndef SLHASHUTIL_H
#define SLHASHUTIL_H

#include <assert.h>
#include <atlstr.h>
#include "openssl\md5.h"
#include "openssl\sha.h"
#include "skylarkbase.h"

NS_SKYLARK_BEGIN

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
        assert(pbBuf);
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
        assert(pbBuf);
        assert(lpHex);

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
class CSLMD5
{
public:

    /// md5 init
    void Init()
    {
        MD5_Init(&m_ctx);
    }


    /**
    * @brief md5 update
    * @param    pbData  二进制数据
    * @param    dwLen   数据长度(字节数)
    */
    void Update(const void* pbData, DWORD dwLen)
    {
        assert(pbData);
        MD5_Update(&m_ctx, pbData, dwLen);
    }


    /**
    * @brief md5 final
    * @param    pbResult    hash值
    */
    void Final(BYTE pbResult[SKYLARK_MD5_BYTES])
    {
        assert(pbResult);
        MD5_Final(pbResult, &m_ctx);
    }


    /**
    * @brief 以十六进制数字字符串形式返回hash结果
    * @param    strHex      hash值
    */
    template <class T_AtlString>
    void FinalAsHex(T_AtlString& strHex)
    {
        BYTE buf[SKYLARK_MD5_BYTES];
        Final(buf);
        CHexUtilT<T_AtlString>::HexFromBuffer(buf, sizeof(buf), strHex);
    }



    /**
    * @brief 返回0长度数据的hash值
    * @param    pbResult    hash值
    */
    static void NullFinal(BYTE pbResult[SKYLARK_MD5_BYTES])
    {
        CSLMD5 ctx;
        ctx.Init();
        ctx.Final(pbResult);
    }

    /**
    * @brief 以十六进制数字字符串形式返回0长度数据的hash值
    * @param    strHex      hash值
    */
    template <class T_AtlString>
    static void NullFinalAsString(T_AtlString& strHex)
    {
        BYTE buf[SKYLARK_MD5_BYTES];
        NullFinal(buf);
        CHexUtilT<T_AtlString>::HexFromBuffer(buf, sizeof(buf), strHex);
    }

protected:
    MD5state_st m_ctx;  ///< context
};





/// sha1 hash 工具类
class CSLSHA1
{
public:

    /// sha1 init
    void Init()
    {
        SHA1_Init(&m_ctx);
    }

    /**
    * @brief sha1 update
    * @param    pbData  二进制数据
    * @param    dwLen   数据长度(字节数)
    */
    void Update(const void* pbData, DWORD dwLen)
    {
        assert(pbData);
        SHA1_Update(&m_ctx, (const unsigned char*)pbData, dwLen);
    }

    /**
    * @brief sha1 final
    * @param    pbResult    hash值
    */
    void Final(BYTE pbResult[20])
    {
        assert(pbResult);
        SHA1_Final(pbResult, &m_ctx);
    }

    /**
    * @brief 以十六进制数字字符串形式返回hash结果
    * @param    strHex      hash值
    */
    template <class T_AtlString>
    void FinalAsHex(T_AtlString& strHex)
    {
        BYTE buf[20];
        Final(buf);
        CHexUtilT<T_AtlString>::HexFromBuffer(buf, sizeof(buf), strHex);
    }

    /**
    * @brief 返回0长度数据的hash值
    * @param    pbResult    hash值
    */
    static void NullFinal(BYTE pbResult[20])
    {
        CSLSHA1 ctx;
        ctx.Init();
        ctx.Final(pbResult);
    }

    /**
    * @brief 以十六进制数字字符串形式返回0长度数据的hash值
    * @param    strHex      hash值
    */
    template <class T_AtlString>
    static void NullFinalAsHex(T_AtlString& strHex)
    {
        BYTE buf[20];
        NullFinal(buf);
        CHexUtilT<T_AtlString>::HexFromBuffer(buf, sizeof(buf), strHex);
    }

protected:
    SHA_CTX m_ctx; ///< context
};

NS_SKYLARK_END

#endif//SLHASHUTIL_H