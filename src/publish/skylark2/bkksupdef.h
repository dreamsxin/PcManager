/**
* @file    bkksupdef.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-20 17:09
*/

#ifndef BKKSUPDEF_H
#define BKKSUPDEF_H

#include "bkavedef.h"
#include "bkengdef.h"

NS_SKYLARK_BEGIN

#define BKMOD_NAME_BKKSUP       L"ksignup.dll"

#define BKKSUP_UPCH_DUBA2011    1   ///< 旧的病毒库升级地址
#define BKKSUP_UPCH_WANGDUN     2   ///< 网盾的病毒库升级地址
#define BKKSUP_UPCH_DEFAULT     BKKSUP_UPCH_WANGDUN

// ------------------------------------------------------------
// 病毒库升级初始化信息

/// 病毒库升级初始化信息
struct BKKSUP_INIT
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    LPCWSTR     pAVERootDir;            ///< deprectated 不再使用
    DWORD       dwUpdateChannel;        ///< 升级通道,取值为 BKKSUP_UPCH_xxx

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKKSUP_INIT* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKKSUP_INIT);

    pStruct->pAVERootDir        = NULL;
    pStruct->dwUpdateChannel    = BKKSUP_UPCH_DEFAULT;
}




// ------------------------------------------------------------
// 病毒库升级参数

class IBKEngKsgUpdateCallback;

/// 病毒库升级参数
struct BKKSUP_UPDATE_PARAM
{
    UINT32                      uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN                     bFastCheckVersion;  ///< [=FALSE]   快速判断版本,如果TRUE,则仅仅比较服务端版本,如果为FALSE,会对本地文件作完整性校验
    IBKEngKsgUpdateCallback*    piCallback;         ///< [=NULL]    升级回调
    UINT32                      uRetryTime;         ///< [=0]       重试次数,区间为[0, 3],大于3的取为3

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKKSUP_UPDATE_PARAM* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKKSUP_UPDATE_PARAM);

    pStruct->bFastCheckVersion  = FALSE;
    pStruct->piCallback         = NULL;
    pStruct->uRetryTime         = 0;
}





// ------------------------------------------------------------
// 病毒库升级信息

/// 病毒库升级信息
struct BKKSUP_UPDATE_INSTRUCTION
{
    UINT32              uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN             bNeedUpdate;        ///< 病毒库需要升级

    BKAVE_SIGN_VERSION  localSignVersion;   ///< 本地病毒库版本
    BKAVE_SIGN_VERSION  remoteSignVersion;  ///< 服务端病毒库版本,注意,服务端病毒库又可能比本地病毒库的特征数少.

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKKSUP_UPDATE_INSTRUCTION* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKKSUP_UPDATE_INSTRUCTION);

    pStruct->bNeedUpdate        = FALSE;

    BKENG_INIT(&pStruct->localSignVersion);
    BKENG_INIT(&pStruct->remoteSignVersion);
}




NS_SKYLARK_END

#endif//BKKSUPDEF_H