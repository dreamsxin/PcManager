/**
* @file    bkavedef.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-14 15:55
*/

#ifndef BKAVEDEF_H
#define BKAVEDEF_H

#include <assert.h>
#include "skylarkbase.h"
#include "bkdef.h"

NS_SKYLARK_BEGIN

#define BKMOD_NAME_BKAVE        L"ksafeave.dll"

#pragma pack(push)
#pragma pack(4)



// ------------------------------------------------------------
// 文件引擎全局初始化

/// 文件引擎全局初始化信息
struct BKAVE_PLATFORM_INIT
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT32      uAveMask;               ///< [=0]       文件引擎的设置,BKAVE_MASK_xxx

    LPCWSTR     pAVERootDir;            ///< [=NULL]    病毒库根目录,如果为NULL,则为bkave.mod所在目录

    LPCWSTR     pKSGIni;                ///< [=NULL]    signs.ini       的路径,如果为NULL,则为 [root]/signs.ini
    LPCWSTR     pKSGDir;                ///< [=NULL]    *.ksg           的目录,如果为NULL,则为 [root]/ksg

    LPCWSTR     pKAEIni;                ///< [=NULL]    kaecore.ini     的路径,如果为NULL,则为 [root]/kae/kaecore.ini
    LPCWSTR     pKAEDir;                ///< [=NULL]    kae模块         的目录,如果为NULL,则为 [root]/kae

    LPCWSTR     pVNameIni;              ///< [=NULL]    vinfo.ini       的路径,如果为NULL,则为 [root]/vinfo.ini
    LPCWSTR     pVNameDir;              ///< [=NULL]    *.vsg           的目录,如果为NULL,则为 [root]/ksg

    LPCWSTR     pKAECore;               ///< [=NULL]    kaecore.dat     的路径,如果为NULL,则为 [KAEDir]/kaecore.dat
    LPCWSTR     pKAEVName;              ///< [=NULL]    kaevname.dat    的路径,如果为NULL,则为 [KAEDir]/kaevname.dat
    LPCWSTR     pKArchive;              ///< [=NULL]    karchive.dat    的路径,如果为NULL,则为 [KAEDir]/karchive.dat
    LPCWSTR     pKAEUnpak;              ///< [=NULL]    kaeunpak.dat    的路径,如果为NULL,则为 [KAEDir]/kaeunpak.dat

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKAVE_PLATFORM_INIT* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKAVE_PLATFORM_INIT);

    pStruct->uAveMask           = 0;
                                                          
    pStruct->pAVERootDir        = NULL;
                                       
    pStruct->pKSGIni            = NULL;
    pStruct->pKSGDir            = NULL;
                                       
    pStruct->pKAEIni            = NULL;
    pStruct->pKAEDir            = NULL;
                                       
    pStruct->pVNameIni          = NULL;
    pStruct->pVNameDir          = NULL;

    pStruct->pKAECore           = NULL;
    pStruct->pKAEVName          = NULL;
    pStruct->pKArchive          = NULL;
    pStruct->pKAEUnpak          = NULL;
}



// ------------------------------------------------------------
// 文件引擎初始化

/// 文件引擎初始化信息
struct BKAVE_ENGINE_INIT
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    IBKFactory* piFactory;              ///< 类厂,用于获取缓存接口,如果为NULL,内部将不使用缓存

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKAVE_ENGINE_INIT* pStruct)
{
    assert(pStruct);
    pStruct->uSize      = sizeof(BKAVE_ENGINE_INIT);

    pStruct->piFactory  = NULL;
}



// ------------------------------------------------------------

/// 文件引擎设置
struct BKAVE_ENGINE_SETTINGS
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT32      uBWMType;               ///< [=BKENG_BWMTYPE_GENERAL]   黑白机配置

    UINT64      uMaxFileSize;           ///< [=ULONG_MAX]               扫描文件的大小限制

    UINT16      uExtractDepth;          ///< [=32]                      [暂不支持]解压深度,取值范围为 [8, 64],如果超出则取边界值
    UINT16      uUnpackDepth;           ///< [=32]                      [暂不支持]脱壳深度,取值范围为 [8, 64],如果超出则取边界值

    UINT32      uAveScanMask;           ///< [=BKAVE_SCAN_MASK_DEFAULT] 扫描设置掩码,取值为 BKAVE_SCAN_MASK_xxx
    UINT32      uAveCleanMask;          ///< [=BKAVE_CLEAN_MASK_NONE]   清除设置掩码,取值为 BKAVE_CLEAN_MASK_xxx, 需要清除时,通常取 BKAVE_CLEAN_MASK_DEFAULT

    // ------------------------------------------------------------ 

    // 缓存掩码
    UINT32      uEnableQueryCacheMask;  ///< [=BKENG_CACHE_MASK_ALL]    取值为 BKENG_CACHE_MASK_xxx
    UINT32      uEnableWriteCacheMask;  ///< [=BKENG_CACHE_MASK_ALL]    取值为 BKENG_CACHE_MASK_xxx

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKAVE_ENGINE_SETTINGS* pStruct)
{                                          
    assert(pStruct);    
    pStruct->uSize                  = sizeof(BKAVE_ENGINE_SETTINGS);

    pStruct->uBWMType               = BKENG_BWMTYPE_GENERAL;

    pStruct->uMaxFileSize           = ULONG_MAX;            

    pStruct->uExtractDepth          = 32;                   
    pStruct->uUnpackDepth           = 32;                   

    pStruct->uAveScanMask           = BKAVE_SCAN_MASK_DEFAULT;
    pStruct->uAveCleanMask          = BKAVE_CLEAN_MASK_NONE;

    pStruct->uEnableQueryCacheMask  = BKENG_CACHE_MASK_ALL;
    pStruct->uEnableWriteCacheMask  = BKENG_CACHE_MASK_ALL;
}





// ------------------------------------------------------------

/// 文件引擎扫描参数
struct BKAVE_SCAN_PARAM
{
    UINT32      uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    HRESULT     hrTag;              ///< [=ULONG_MAX]   计算文件标签的返回值, ULONG_MAX 表示从未计算过
    DWORD       dwFileTag;          ///< [=0]           文件标签

    BOOLEAN     bValidAveFT;        ///< [=FALSE]       是否提供缓存的ave文件类型,仅当 dwFileTag 和 uHintAveFT 有效时为TRUE
    UINT32      uHintAveFT;         ///< [=0]           提供缓存的ave文件类型

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKAVE_SCAN_PARAM* pStruct)
{                                          
    assert(pStruct);    
    pStruct->uSize                  = sizeof(BKAVE_SCAN_PARAM);

    pStruct->hrTag                  = ULONG_MAX;
    pStruct->dwFileTag              = 0;

    pStruct->bValidAveFT            = FALSE;
    pStruct->uHintAveFT             = 0;
}






// ------------------------------------------------------------
// 文件引擎扫描结果

/// 文件引擎设置
struct BKAVE_ENGINE_RESULT
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN     bIsVirus;               ///< 是否是病毒
    UINT32      uThreatID;              ///< 病毒ID
    WCHAR       szVirusName[64];        ///< 病毒名
    UINT32      uScanResult;            ///< 扫描结果,取值为BKENG_SCAN_RESULT_xxx
    UINT32      uCleanResult;           ///< 清除结果,取值为BKENG_CLEAN_RESULT_xxx

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKAVE_ENGINE_RESULT* pStruct)
{  
    assert(pStruct);
    pStruct->uSize          = sizeof(BKAVE_ENGINE_RESULT);

    pStruct->bIsVirus       = FALSE;
    pStruct->uThreatID      = 0;
    pStruct->uScanResult    = BKENG_SCAN_RESULT_SAFE;
    pStruct->uCleanResult   = BKENG_CLEAN_RESULT_NO_OP;
    memset(pStruct->szVirusName, 0, sizeof(pStruct->szVirusName));
}











#pragma pack(pop)
NS_SKYLARK_END

#endif//BKAVEDEF_H