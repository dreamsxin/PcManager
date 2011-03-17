/**
* @file    bklevel.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-16 21:29
*/

#ifndef BKLEVEL_H
#define BKLEVEL_H

#include "skylarkbase.h"

NS_SKYLARK_BEGIN

//////////////////////////////////////////////////////////////////////////
// Level 的级别

#define SLPTL_LEVEL_GROUP__MIN                                  0       ///< ...
#define SLPTL_LEVEL_GROUP__SERVER_MIN                           0       ///< ...

// 上报(灰名单)
#define SLPTL_LEVEL_GROUP__UPLOAD_MIN                           0       ///< ...
#define SLPTL_LEVEL_MARK___RESERVED                             0       ///< 保留
#define SLPTL_LEVEL_MARK___UNRECOGNISED_SYSTEM                  1       ///< 未上传过的系统签名
#define SLPTL_LEVEL_MARK___UNRECOGNISED_PROCESS                 2       ///< 未上传过的进程签名
#define SLPTL_LEVEL_MARK___UNRECOGNISED_FILE                    3       ///< 未上传过的文件签名
#define SLPTL_LEVEL_GROUP__UPLOAD_MAX                           49      ///< ...

// 黑名单
#define SLPTL_LEVEL_GROUP__BLACK_MIN                            50      ///< ...

#define SLPTL_LEVEL_GROUP__BLACK_AUTO_MIN                       50      ///< 自动判黑的分组
#define SLPTL_LEVEL_MARK___BLACK_AUTO                           50      ///< 病毒(未判断感染型)
#define SLPTL_LEVEL_MARK___BLACK_AUTO_NON_INFECTED              51      ///< 非感染型
#define SLPTL_LEVEL_MARK___BLACK_AUTO_INFECTED                  52      ///< 感染型
#define SLPTL_LEVEL_GROUP__BLACK_AUTO_MAX                       59      

#define SLPTL_LEVEL_GROUP__BLACK_MANUAL_MIN                     60      ///< 手动判黑的分组
#define SLPTL_LEVEL_MARK___BLACK_MANUAL_NON_INFECTED            61      ///< 非感染型
#define SLPTL_LEVEL_MARK___BLACK_MANUAL_INFECTED                62      ///< 感染型
#define SLPTL_LEVEL_GROUP__BLACK_MANUAL_MAX                     69

#define SLPTL_LEVEL_GROUP__BLACK_ABSOLUTELY_MIN                 70      ///< 绝对黑的分组(不需要复查数字签名)
#define SLPTL_LEVEL_MARK___BLACK_ABSOLUTELY_NON_INFECTED        71      ///< 非感染型
#define SLPTL_LEVEL_MARK___BLACK_ABSOLUTELY_INFECTED            72      ///< 感染型
#define SLPTL_LEVEL_GROUP__BLACK_ABSOLUTELY_MAX                 79

#define SLPTL_LEVEL_GROUP__BLACK_HEURIST_MIN                    80      ///< 云端启发式
#define SLPTL_LEVEL_MARK___BLACK_HEURIST_HIGH_RISK              80      ///< 高风险
#define SLPTL_LEVEL_MARK___BLACK_HEURIST_LOW_RISK               81      ///< 低风险
#define SLPTL_LEVEL_GROUP__BLACK_HEURIST_MAX                    89      ///<

#define SLPTL_LEVEL_GROUP__BLACK_MAX                            99      ///< ...

// 灰名单
#define SLPTL_LEVEL_GROUP__GRAY_MIN                             100     ///< ...
#define SLPTL_LEVEL_GROUP__GRAY_UNTRUST_MIN                     100     ///< ...
#define SLPTL_LEVEL_MARK___NEVER_BEEN_SCANNED                   100     ///< 未扫描的文件
#define SLPTL_LEVEL_MARK___SCANNED_ONCE_SUSPICIOUS              101     ///< 扫描过一次,但有疑点的文件
#define SLPTL_LEVEL_MARK___SCANNED_ONCE                         102     ///< 扫描过一次的文件
#define SLPTL_LEVEL_MARK___SCANNED_MORE_THAN_ONCE               103     ///< 至少扫描过两次的文件
#define SLPTL_LEVEL_GROUP__GRAY_UNTRUST_MAX                     119     ///< ...

#define SLPTL_LEVEL_GROUP__GRAY_TRUST_MIN                       120     ///< ...
#define SLPTL_LEVEL_MARK___TEMP_FALSE_POSITIVE                  130     ///< 临时认为是误报的文件
#define SLPTL_LEVEL_MARK___SCANNED_ENOUGH                       150     ///< 扫描了足够长时间的文件
#define SLPTL_LEVEL_MARK___WIN_TRUST__DEPRECATE                 160     ///< (不再使用)(本地)包含微软信任的数字签名的文件
#define SLPTL_LEVEL_MARK___WHITE_COEXIST                        168     ///< 根据与白文件共存性统计判定为无威胁的文件
#define SLPTL_LEVEL_GROUP__GRAY_TRUST_MAX                       169     ///< ...
#define SLPTL_LEVEL_GROUP__GRAY_MAX                             169     ///< ...

// 白名单
#define SLPTL_LEVEL_GROUP__TRUST_MIN                            170     ///< ...
#define SLPTL_LEVEL_MARK___MANUALLY_WHITE                       170     ///< 人工鉴定确认是正常文件
#define SLPTL_LEVEL_MARK___MANUALLY_FALSE_POSITIVE              171     ///< 人工鉴定确认是误报的文件
#define SLPTL_LEVEL_MARK___WHITE_SCANNED_ENOUGH                 172     ///< 扫描足够次数,认为可以判白
#define SLPTL_LEVEL_MARK___DOWNLOADED_FROM_TRUSTED_WEBSITES     180     ///< 从可信的站点下载的软件
#define SLPTL_LEVEL_MARK___DOWNLOADED_FROM_OFFICIAL_WEBSITES    181     ///< 从官方网站下载的软件
#define SLPTL_LEVEL_MARK___DISTRIBUTED_BY_MICROSOFT             182     ///< 微软发布的文件
#define SLPTL_LEVEL_MARK___DISTRIBUTED_BY_COPARTNER             183     ///< 合作伙伴提交的文件
#define SLPTL_LEVEL_MARK___DISTRIBUTED_BY_BEIKE_CORP            184     ///< 贝壳发布的文件
#define SLPTL_LEVEL_MARK___MANUALLY_TRUST                       185     ///< 人工确认为可信任文件

// add by yangjun 2010.11.12 为解决毒霸白缓存时间过短问题
#define SLPTL_LEVEL_MARK___DUBA_TRUST_3DAY                      186     ///< 毒霸认为可信要缓存3天
#define SLPTL_LEVEL_MARK___DUBA_TRUST_5DAY                      187     ///< 毒霸认为可信要缓存5天
#define SLPTL_LEVEL_MARK___DUBA_TRUST_7DAY                      188     ///< 毒霸认为可信要缓存7天
#define SLPTL_LEVEL_MARK___DUBA_TRUST_10DAY                     189     ///< 毒霸认为可信要缓存10天
// add end

#define SLPTL_LEVEL_GROUP__TRUST_MAX                            199     ///< ...
#define SLPTL_LEVEL_GROUP                                               ///< ...

#define SLPTL_LEVEL_GROUP__SERVER_MAX                           199     ///< ...



// 本地使用的level
#define SLPTL_LEVEL_GROUP__LOCAL_MIN                            800     ///< ...

#define SLPTL_LEVEL_GROUP__LOCAL_WHITE_MIN                      800     ///< ...
#define SLPTL_LEVEL_MARK___LOCAL_WHITE_FILE_NAME                801     ///< 本地白名单文件名
#define SLPTL_LEVEL_MARK___LOCAL_WIN_TRUST                      802     ///< 本地可信的数字签名
#define SLPTL_LEVEL_MARK___LOCAL_WIN_TRUST_FAKE_SYS             803     ///< 本地可信的数字签名,但非白即黑的判黑病毒
#define SLPTL_LEVEL_MARK___LOCAL_WIN_TRUST_HEURIST              804     ///< 本地可信的数字签名,但启发式判黑
#define SLPTL_LEVEL_MARK___LOCAL_WIN_TRUST_CLOUD_HIGH_RISK      805     ///< 本地可信的数字签名,但云端报高风险
#define SLPTL_LEVEL_MARK___LOCAL_WIN_TRUST_CLOUD_LOW_RISK       806     ///< 本地可信的数字签名,但云端报低风险
#define SLPTL_LEVEL_MARK___LOCAL_WIN_TRUST_BOTH_HEURIST         807     ///< 本地可信的数字签名,但云端启发和本地启发都报毒
#define SLPTL_LEVEL_MARK___LOCAL_KSCLOUD_TRUST                  808     ///< 毒霸云可信
#define SLPTL_LEVEL_GROUP__LOCAL_WHITE_MAX                      849     ///< ...


#define SLPTL_LEVEL_GROUP__LOCAL_BLACK_MIN                      850     ///< ...
#define SLPTL_LEVEL_MARK___LOCAL_INFECTED_DELPHI                860     ///< 被感染的delphi
#define SLPTL_LEVEL_MARK___LOCAL_HEURIST                        861     ///< 本地启发式
#define SLPTL_LEVEL_MARK___LOCAL_AVENGINE                       862     ///< 本地杀毒引擎发现的病毒
#define SLPTL_LEVEL_MARK___LOCAL_AVP                            863     ///< 本地卡巴引擎发现的病毒
#define SLPTL_LEVEL_MARK___LOCAL_FAKE_SYS_FILE                  864     ///< 本地非白即黑的判黑病毒
#define SLPTL_LEVEL_MARK___LOCAL_HEURIST_WITH_CLOUD_LOW_RISK    865     ///< 本地启发和云端启发低风险同时报毒
#define SLPTL_LEVEL_MARK___LOCAL_KSCLOUD_BLACK                  868     ///< 毒霸云认为是病毒
#define SLPTL_LEVEL_GROUP__LOCAL_BLACK_MAX                      899     ///< ...


// 本地错误
#define SLPTL_LEVEL_GROUP__LOCAL_ERROR_MIN                      900     ///< ...
#define SLPTL_LEVEL_MARK___LOCAL_RESERVED                       900     ///< 保留
#define SLPTL_LEVEL_MARK___LOCAL_FAILED_TO_HASH_FILE            901     ///< 计算文件hash的时候发生错误
#define SLPTL_LEVEL_MARK___LOCAL_FAILED_TO_QUERY_FILE           902     ///< 查询文件level的时候发生错误
#define SLPTL_LEVEL_MARK___LOCAL_FAILED_TO_HASH_PROCESS         903     ///< 计算进程hash的时候发生错误
#define SLPTL_LEVEL_MARK___LOCAL_FAILED_TO_QUERY_PROCESS        904     ///< 查询进程hash的时候发生错误
#define SLPTL_LEVEL_MARK___LOCAL_FAILED_TO_HASH_SYSTEM          905     ///< 计算系统hash的时候发生错误
#define SLPTL_LEVEL_MARK___LOCAL_FAILED_TO_QUERY_SYSTEM         906     ///< 查询系统hash的时候发生错误
#define SLPTL_LEVEL_MARK___LOCAL_NON_PE_FILE                    910     ///< 非PE文件
#define SLPTL_LEVEL_MARK___LOCAL_TOO_SMALL_FILE                 911     ///< 文件太小
#define SLPTL_LEVEL_MARK___LOCAL_TOO_LARGE_FILE                 912     ///< 文件太大
#define SLTPL_LEVEL_MARK___LOCAL_NON_EXE_FILE                   931     ///< 本地不可执行的文件
#define SLPTL_LEVEL_GROUP__LOCAL_ERROR_MAX                      949     ///< ...

#define SLPTL_LEVEL_GROUP__LOCAL_GRAY_MIN                       950     ///< ...
#define SLPTL_LEVEL_MARK___LOCAL_FAKE_SYS_GRAY_TRUST            950     ///< 本地sfmkb的可信灰文件
#define SLPTL_LEVEL_MARK___LOCAL_FAKE_SYS_GRAY_UNTRUST          975     ///< 本地sfmkb的不可信灰文件
#define SLPTL_LEVEL_MARK___LOCAL_NO_NEED_SRV_HEURIST_HIGH_RISK  976     ///< 本地不需要的云端启发结果(高风险)
#define SLPTL_LEVEL_MARK___LOCAL_NO_NEED_SRV_HEURIST_LOW_RISK   977     ///< 本地不需要的云端启发结果(低风险)
#define SLPTL_LEVEL_GROUP__LOCAL_GRAY_MAX                       999     ///< ...

#define SLPTL_LEVEL_GROUP__LOCAL_MAX                            999     ///< ...



#define SLPTL_LEVEL_GROUP__MAX                                  999     ///< ...



/// 灰名单,缓存1小时
inline BOOL SLLevelIsGray(DWORD dwLevel)
{
    return dwLevel >= SLPTL_LEVEL_GROUP__GRAY_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__GRAY_MAX;
}

///// 不可信的灰名单，需要提交给本地引擎
//inline BOOL SLLevelIsGrayUnTrust(DWORD dwLevel)
//{
//    if (dwLevel >= SLPTL_LEVEL_GROUP__GRAY_UNTRUST_MIN &&
//        dwLevel <= SLPTL_LEVEL_GROUP__GRAY_UNTRUST_MAX)
//        return TRUE;
//
//    if (dwLevel == SLPTL_LEVEL_MARK___LOCAL_FAKE_SYS_GRAY_UNTRUST)
//        return TRUE;
//
//    return FALSE;
//}

/// 可信的灰名单，不提交给本地引擎
inline BOOL SLLevelIsGrayTrust(DWORD dwLevel)
{
    if (dwLevel >= SLPTL_LEVEL_GROUP__GRAY_TRUST_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__GRAY_TRUST_MAX)
        return TRUE;

    if (dwLevel == SLPTL_LEVEL_MARK___LOCAL_FAKE_SYS_GRAY_TRUST)
        return TRUE;

    return FALSE;
}

/// 自定鉴定的黑名单,需要验证数字签名
inline BOOL SLLevelIsBlackAuto(DWORD dwLevel)
{
    if (dwLevel >= SLPTL_LEVEL_GROUP__BLACK_AUTO_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__BLACK_AUTO_MAX)
        return TRUE;

    if (dwLevel >= SLPTL_LEVEL_GROUP__BLACK_HEURIST_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__BLACK_HEURIST_MAX)
        return TRUE;

    if (dwLevel >= SLPTL_LEVEL_GROUP__LOCAL_BLACK_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__LOCAL_BLACK_MAX)
        return TRUE;

    return FALSE;
}

/// 手动鉴定的黑名单,需要验证数字签名
inline BOOL SLLevelIsBlackManual(DWORD dwLevel)
{
    if (dwLevel >= SLPTL_LEVEL_GROUP__BLACK_MANUAL_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__BLACK_MANUAL_MAX)
        return TRUE;

    return FALSE;
}

/// 绝对的黑名单,不需要验证数字签名
inline BOOL SLLevelIsBlackAbsolutely(DWORD dwLevel)
{
    if (dwLevel >= SLPTL_LEVEL_GROUP__BLACK_ABSOLUTELY_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__BLACK_ABSOLUTELY_MAX)
        return TRUE;

    return FALSE;
}

/// ...
//inline BOOL SLLevelIsBlack(DWORD dwLevel)
//{
//    if (SLLevelIsBlackAuto(dwLevel))
//        return TRUE;
//
//    if (SLLevelIsBlackManual(dwLevel))
//        return TRUE;
//
//    return FALSE;
//}

/// 已知要报毒的黑文件
inline BOOL SLLevelIsBlackKnown(DWORD dwLevel)
{
    if (SLLevelIsBlackAuto(dwLevel))
        return TRUE;

    if (SLLevelIsBlackManual(dwLevel))
        return TRUE;

    if (SLLevelIsBlackAbsolutely(dwLevel))
        return TRUE;

    return FALSE;
}

/// 已知的感染型
inline BOOL SLLevelIsInfected(DWORD dwLevel)
{
    if (SLPTL_LEVEL_MARK___BLACK_AUTO_INFECTED == dwLevel)
        return TRUE;

    if (SLPTL_LEVEL_MARK___BLACK_MANUAL_INFECTED == dwLevel)
        return TRUE;

    if (SLPTL_LEVEL_MARK___BLACK_ABSOLUTELY_INFECTED == dwLevel)
        return TRUE;

    return FALSE;
}

/// 可信任的白文件
inline BOOL SLLevelIsTrusted(DWORD dwLevel)
{
    if (dwLevel >= SLPTL_LEVEL_GROUP__TRUST_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__TRUST_MAX)
        return TRUE;

    if (dwLevel >= SLPTL_LEVEL_GROUP__LOCAL_WHITE_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__LOCAL_WHITE_MAX)
        return TRUE;

    return FALSE;
}

/// ...
inline BOOL SLLevelIsServer(DWORD dwLevel)
{
    return dwLevel >= SLPTL_LEVEL_GROUP__SERVER_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__SERVER_MAX;
}

/// ...
inline BOOL SLLevelIsLocalError(DWORD dwLevel)
{
    return dwLevel >= SLPTL_LEVEL_GROUP__LOCAL_ERROR_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__LOCAL_ERROR_MAX;
}

/// ...
inline BOOL SLLevelIsLocalWhite(DWORD dwLevel)
{
    return dwLevel >= SLPTL_LEVEL_GROUP__LOCAL_WHITE_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__LOCAL_WHITE_MAX;
}

/// ...
inline BOOL SLLevelIsLocalBlack(DWORD dwLevel)
{
    return dwLevel >= SLPTL_LEVEL_GROUP__LOCAL_BLACK_MIN &&
        dwLevel <= SLPTL_LEVEL_GROUP__LOCAL_BLACK_MAX;
}

NS_SKYLARK_END

#endif//BKLEVEL_H