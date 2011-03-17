/**
* @file    slfeed.h
* @brief   ...
* @author  zhangrui
* @date    2009-03-02  15:21
*/

#ifndef SLFEED_H
#define SLFEED_H

#include <unknwn.h>
#include "skylarkbase.h"
#include "slfeedschema.h"
#include "slfeedcode.h"
#include "slfeedaudit.h"



inline HRESULT WINAPI SkylarkFeedAudit(DWORD, DWORD, DWORD, DWORD = 1)
{
    return S_FALSE;
}

inline HRESULT WINAPI SkylarkFeedDebugLog(DWORD, DWORD, DWORD, LPCWSTR, DWORD = 1)
{
    return S_FALSE;
}

inline HRESULT WINAPI SkylarkFeedStatistic(DWORD, DWORD, DWORD, DWORD = 1)
{
    return S_FALSE;
}

inline HRESULT WINAPI SkylarkInitializeFeeder(DWORD, DWORD)
{
    return S_FALSE;
}

inline HRESULT WINAPI SkylarkUninitializeFeeder(DWORD = 1000)
{
    return S_FALSE;
}

inline HRESULT WINAPI SkylarkGetMid(LPWSTR, DWORD)
{
    return S_FALSE;
}


inline DWORD WINAPI SkylarkGetProductID()
{
    return SLK_PID_KSAFE;
}

inline DWORD WINAPI SkylarkGetChannelID()
{
    return 0;
}

inline DWORD WINAPI SkylarkGetFeedVer()
{
    return 0;
}

inline HRESULT WINAPI SkylarkRenewFeedVer(DWORD)
{
    return S_FALSE;
}




#define SKYLARK_MID_BUF_SIZE        64      ///< mid的buf长度(比实际数据长度长)

#define SLFEED_METHOD_IMMEDIATELY   0       ///< 立即发送(阻塞)
#define SLFEED_METHOD_ASYNC         1       ///< 异步发送(非阻塞)
#define SLFEED_METHOD_OFFLINE       2       ///< 脱机保存(延迟发送)

#if 0

/**
* @brief    向信息收集服务器发送审计日志
* @param    dwProductID     产品ID
* @param    dwChannelID     通道ID,用来区分
* @param    dwActionID      活动ID
* @param    dwFeedMethod    发送方式,取值为SLFEED_METHOD_XXX
@verbatim
审计数据包含的内容
    [自动获取] 
    mid         包含版本识别的mid
    ip          服务端自动获取
    time        服务端自动获取
    ver         客户端版本号

    [参数填写]
    pid         产品id
    chid        通道号,识别产品的分发途径
    act         活动
@endverbatim
*/
EXTERN_C HRESULT WINAPI SkylarkFeedAudit(
    DWORD   dwProductID,
    DWORD   dwChannelID,
    DWORD   dwActionID,
    DWORD   dwFeedMethod = SLFEED_METHOD_ASYNC);
/// typedef for SkylarkFeedAudit
typedef HRESULT (WINAPI *PFN_SkylarkFeedAudit)(
    DWORD   dwProductID,
    DWORD   dwChannelID,
    DWORD   dwActionID,
    DWORD   dwFeedMethod);



/**
* @brief    向信息收集服务器发送日志
* @param    dwSchemaID      信息的主题id,由info数据库的管理者统一分配,见slfeedschema.h
* @param    dwInfoCode      信息码,主题的所有者自行定义,通常是预定义的值,见slfeedschema.h
* @param    dwInfoParam     信息参数,主题的所有者自行定义,通常是易变的值,比如GetLastError()的返回值
* @param    lpszInfoMessage 字符串信息,主题的所有者自行定义,使用该参数可能导致服务器数据量大增
* @param    dwFeedMethod    发送方式,取值为SLFEED_METHOD_XXX
@verbatim
调试日志包含的内容
    [自动获取] 
    mid         包含版本识别的mid
    ip          服务端自动获取
    time        服务端自动获取
    ver         客户端版本号

    [参数填写]
    schema
    code
    param
    message
@endverbatim
*/
EXTERN_C HRESULT WINAPI SkylarkFeedDebugLog(
    DWORD   dwSchemaID,
    DWORD   dwInfoCode,
    DWORD   dwInfoParam,
    LPCWSTR lpszInfoMessage,
    DWORD   dwFeedMethod = SLFEED_METHOD_ASYNC);
/// typedef for SkylarkFeedDebugLog
typedef HRESULT (WINAPI *PFN_SkylarkFeedDebugLog)(
    DWORD   dwSchemaID,
    DWORD   dwInfoCode,
    DWORD   dwInfoParam,
    LPCWSTR lpszInfoMessage,
    DWORD   dwFeedMethod);






/**
* @brief    向信息收集服务器发送统计日志
* @param    dwSchemaID      信息的主题id,由info数据库的管理者统一分配,见slfeedschema.h
* @param    dwInfoCode      信息码,主题的所有者自行定义,通常是预定义的值,见slfeedschema.h
* @param    dwInfoParam     信息参数,主题的所有者自行定义,通常是易变的值,比如GetLastError()的返回值
* @param    dwFeedMethod    发送方式,取值为SLFEED_METHOD_XXX
*/
EXTERN_C HRESULT WINAPI SkylarkFeedStatistic(
    DWORD   dwSchemaID,
    DWORD   dwInfoCode,
    DWORD   dwInfoParam,
    DWORD   dwFeedMethod = SLFEED_METHOD_ASYNC);
/// typedef for SkylarkFeedStatistic
typedef HRESULT (WINAPI *PFN_SkylarkFeedStatistic)(
    DWORD   dwSchemaID,
    DWORD   dwInfoCode,
    DWORD   dwInfoParam,
    DWORD   dwFeedMethod);






/**
* @brief    向信息收集服务器发送报告
* @param    dwSchemaID      信息的主题id,由info数据库的管理者统一分配,见slfeedschema.h
* @param    dwInfoCode      信息码,主题的所有者自行定义,通常是预定义的值,见slfeedschema.h
* @param    dwInfoParam     信息参数,主题的所有者自行定义,通常是易变的值,比如GetLastError()的返回值
* @param    lpszInfoMessage 字符串信息,暂定最大长度为 65535 bytes
* @param    dwFeedMethod    发送方式,取值为SLFEED_METHOD_XXX
*/
EXTERN_C HRESULT WINAPI SkylarkFeedReport(
    DWORD   dwSchemaID,
    DWORD   dwInfoCode,
    DWORD   dwInfoParam,
    LPCWSTR lpszInfoMessage,
    DWORD   dwFeedMethod = SLFEED_METHOD_ASYNC);
/// typedef for SkylarkFeedStatistic
typedef HRESULT (WINAPI *PFN_SkylarkFeedReport)(
    DWORD   dwSchemaID,
    DWORD   dwInfoCode,
    DWORD   dwInfoParam,
    LPCWSTR lpszInfoMessage,
    DWORD   dwFeedMethod);







// 以下为Feeder的控制函数

/**
* @brief    向信息收集服务器发送异步日志
* @param    bKeepFileIfFailedToSend     如果为FALSE,那么发送失败后,会删除发送失败的日志
*/
EXTERN_C HRESULT WINAPI SendOfflineFeedReport(BOOL bKeepFileIfFailedToSend);
/// typedef for SendOfflineFeedReport
typedef HRESULT (WINAPI *PFN_SendOfflineFeedReport)(BOOL bKeepFileIfFailedToSend);




/**
* @brief        初始化全局的feeder,所有未发出的日志都会被丢弃
* @param        dwProductID         产品id
* @param        dwChannelID         产品通道id
* @note         SkylarkInitialize   会调用该函数
*/
EXTERN_C HRESULT WINAPI SkylarkInitializeFeeder(DWORD dwProductID, DWORD dwChannelID);
/// typedef for SkylarkInitializeFeeder
typedef HRESULT (WINAPI *PFN_SkylarkInitializeFeeder)();

/**
* @brief        反初始化全局的feeder,所有未发出的日志都会被丢弃
* @param[in]    dwMilliSeconds      超时时间,单位为毫秒
* @note         SkylarkUninitialize 会调用该函数
*/
EXTERN_C HRESULT WINAPI SkylarkUninitializeFeeder(DWORD dwMilliSeconds = 1000);
/// typedef for SkylarkUninitializeFeeder
typedef HRESULT (WINAPI *PFN_SkylarkUninitializeFeeder)(DWORD dwMilliSeconds);

/**
* @brief        获取mid
* @param[out]   szMid       返回mid
* @param[in]    cchBufLen   mid
* @note         Mid 字符串长度为40,建议buf长度至少为41
*/
EXTERN_C HRESULT WINAPI SkylarkGetMid(LPWSTR szMid, DWORD cchBufLen);
/// typedef for SkylarkGetMid
typedef HRESULT (WINAPI *PFN_SkylarkGetMid)(LPWSTR szMid, DWORD cchBufLen);



/**
* @brief        获取产品id
*/
EXTERN_C DWORD WINAPI SkylarkGetProductID();
/// typedef for SkylarkGetFeedChannel
typedef DWORD (WINAPI *PFN_SkylarkGetProductID)();

/**
* @brief        获取产品通道号
*/
EXTERN_C DWORD WINAPI SkylarkGetChannelID();
/// typedef for SkylarkGetFeedChannel
typedef DWORD (WINAPI *PFN_SkylarkGetChannelID)();


/**
* @brief        获取feed版本号
*/
EXTERN_C DWORD WINAPI SkylarkGetFeedVer();
/// typedef for SkylarkGetFeedVer
typedef DWORD (WINAPI *PFN_SkylarkGetFeedVer)();

/**
* @brief        更新feed版本号,通常由升级程序调用
* @param[in]    dwVersion           设置新的mid
*/
EXTERN_C HRESULT WINAPI SkylarkRenewFeedVer(DWORD dwVersion);
/// typedef for SkylarkRenewFeedVer
typedef HRESULT (WINAPI *PFN_SkylarkRenewFeedVer)(DWORD dwVersion);



/**
* @brief        以mid为种子,获取hash值
* @param[in]    dwMaxHash   hash的最大取值, 参考的实现为 crc32(mid) % (dwMaxHash + 1)
*/
EXTERN_C DWORD WINAPI SkylarkFeedHashCodeByMid(DWORD dwMaxHash);
/// typedef for SkylarkFeedHashCodeByMid
typedef DWORD (WINAPI *PFN_SkylarkFeedHashCodeByMid)(DWORD dwMaxHash);

#endif




#endif//SLFEED_H