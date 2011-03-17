/**
* @file    slbase64util.h
* @brief   ...
* @author  zhangrui
* @date    2008-12-22  02:49
*/


#ifndef SLBASE64UTIL_H
#define SLBASE64UTIL_H

#include <atlstr.h>
#include <atlenc.h>
#include <atlcoll.h>
#include "skylarkbase.h"

NS_SKYLARK_BEGIN

/// base64工具类
class CBase64Util
{
public:

    /**
    * @brief    base64编码
    * @param    lpData      要编码的数据
    * @param    nBytes      明文的长度
    * @param    strBase64   编码后的base64字符串
    */
    static HRESULT B64Encode(const void* lpData, int nBytes, CStringA& strBase64)
    {
        if (!lpData)
            return E_POINTER;

        int nDstLen = Base64EncodeGetRequiredLength(nBytes, ATL_BASE64_FLAG_NOCRLF);


        BOOL bRet = Base64Encode(
            (const BYTE*)lpData,
            nBytes,
            strBase64.GetBuffer(nDstLen),
            &nDstLen,
            ATL_BASE64_FLAG_NOCRLF);
        if (!bRet)
        {
            strBase64.ReleaseBuffer(0);
            return E_FAIL;
        }

        strBase64.ReleaseBuffer(nDstLen);
        return S_OK;
    }



    /**
    * @brief    base64解码
    * @param    lpBase64    要解码的base64字符串
    * @param    nBase64Len  base64字符串的长度
    * @param    data        解码后的明文
    */
    static HRESULT B64Decode(LPCSTR lpBase64, int nBase64Len, CAtlArray<BYTE>& data)
    {
        if (!lpBase64)
            return E_POINTER;

        int nDstLen = Base64DecodeGetRequiredLength(nBase64Len);

        data.SetCount(nDstLen);
        BOOL bRet = Base64Decode(
            lpBase64,
            nBase64Len,
            data.GetData(),
            &nDstLen);
        data.SetCount(nDstLen);

        if (!bRet)
        {
            data.RemoveAll();
            return E_FAIL;
        }

        return S_OK;
    }

};

NS_SKYLARK_END

#endif//SLBASE64UTIL_H
