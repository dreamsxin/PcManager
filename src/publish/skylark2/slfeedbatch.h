/**
* @file    slfeedbatch.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-22  11:41
*/

#ifndef SLFEEDBATCH_H
#define SLFEEDBATCH_H

#include <wininet.h>
#include <unknwn.h>
#include "skylarkbase.h"
#include "slfeed.h"

NS_SKYLARK_BEGIN

/// 上传feed日志的接口
class __declspec(uuid("EC3C9CE5-FD9D-40d2-A8D8-E681C17A6780"))
IFeedBatch: public IUnknown
{
public:

    /// ...
    virtual HRESULT STDMETHODCALLTYPE Post(HINTERNET hInetSession = NULL) = 0;

    /// ...
    virtual HRESULT STDMETHODCALLTYPE PostEx(
        LPCWSTR     lpszMid,
        DWORD       dwProductID,
        DWORD       dwChannelID,
        DWORD       dwFeedVer,
        HINTERNET   hInetSession = NULL) = 0;
};


/// 批量上传feed日志的接口
class __declspec(uuid("CC86DBC7-35EA-4160-9B4D-3DA546190BD9"))
IFeedDebugLogBatch: public IFeedBatch
{
public:

    /**
    * @brief    向信息收集服务器发送日志
    * @param    dwSchemaID      信息的主题id,由info数据库的管理者统一分配,见slfeedschema.h
    * @param    dwInfoCode      信息码,主题的所有者自行定义,通常是预定义的值,见slfeedschema.h
    * @param    dwInfoParam     信息参数,主题的所有者自行定义,通常是易变的值,比如GetLastError()的返回值
    * @param    lpszInfoMessage 字符串信息,主题的所有者自行定义,使用该参数可能导致服务器数据量大增
    * @note     原则上,使用批量日志时,应该尽可能发送同类型日志,以便服务端作分表操作
    */
    virtual HRESULT STDMETHODCALLTYPE FeedDebugLog(
        DWORD   dwSchemaID,
        DWORD   dwInfoCode,
        DWORD   dwInfoParam,
        LPCWSTR lpszInfoMessage) = 0;
};

NS_SKYLARK_END


/// class Factory
EXTERN_C HRESULT WINAPI SkylarkCreateFeedDebugLogBatch(Skylark::IFeedDebugLogBatch** ppiObj);
/// typedef SkylarkCreateSecurityProductInfo
typedef HRESULT (WINAPI *PFN_SkylarkCreateFeedDebugLogBatch)(Skylark::IFeedDebugLogBatch** ppiObj);


#endif//SLFEEDBATCH_H