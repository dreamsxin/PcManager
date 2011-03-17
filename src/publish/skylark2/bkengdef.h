/**
* @file    bkengdef.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-15 12:17
*/

#ifndef BKENGDEF_H
#define BKENGDEF_H

#include <assert.h>
#include "skylarkbase.h"
#include "bkdef.h"
#include "bklevel.h"

NS_SKYLARK_BEGIN

#define BKMOD_NAME_BKENG        L"ksafeeng.dll"

#pragma pack(push)
#pragma pack(4)



/// 网络超时设置
#define SLPTL_NET_MIN_TIMEOUT               1000        ///< 最小的网络超时
#define SLPTL_NET_MAX_QUERY_TIMEOUT         30000       ///< 最大的查询超时#define SLPTL_NET_MAX_ECHO_TIMEOUT          30000       ///< 最大的握手超时#define SLPTL_NET_ECHO_TIMEOUT              20000       ///< 单次握手的网络超时
#define SLPTL_NET_ECHO_TIMEOUT              20000       ///< 单次握手的网络超时
#define SLPTL_NET_QUERY_TIMEOUT             15000       ///< 单次查询的网络超时
#define SLPTL_NET_UPLOAD_TIMEOUT            600000      ///< 单次上传的网络超时



/// skylark 查询文件的大小限制
#define SKYLARK_QUERY_FILE_MAX_SIZE         (20 * 1024 * 1024)

/// skylark 上传文件的大小限制
#define SKYLARK_UPLOAD_FILE_MAX_SIZE        (20 * 1024 * 1024)

#define SLPTL_FILETYPE__UNKNOWN     0x00000000  ///< 未知文件类型
#define SLPTL_FILETYPE__NON_PE      0x00000001  ///< 非PE文件
#define SLPTL_FILETYPE__TOO_SMALL   0x00000002  ///< 文件太小
#define SLPTL_FILETYPE__TOO_LARGE   0x00000003  ///< 文件太大

#define SLPTL_FILETYPE__PE          0x00010001  ///< PE文件





// ------------------------------------------------------------
// 查杀引擎全局初始化

/// 查杀引擎全局初始化信息
struct BKENG_PLATFORM_INIT
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BYTE        byDeprecated[12];       ///< 不再使用的字段

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_PLATFORM_INIT* pStruct)
{
    assert(pStruct);
    pStruct->uSize = sizeof(BKENG_PLATFORM_INIT);

    memset(pStruct->byDeprecated, 0, sizeof(pStruct->byDeprecated));
}






// ------------------------------------------------------------
// 查杀引擎初始化

/// 文件引擎初始化信息
struct BKENG_ENGINE_INIT
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_ENGINE_INIT* pStruct)
{
    assert(pStruct);
    pStruct->uSize = sizeof(BKENG_ENGINE_INIT);
}






// ------------------------------------------------------------

/// 查杀引擎设置
struct BKENG_SETTINGS
{
    UINT32  uSize;                          ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    // ------------------------------------------------------------
    // 文件引擎的扫描设置

    UINT32  uBWMType;                       ///< [=BKENG_BWMTYPE_GENERAL]       黑白机配置

    UINT64  uMaxFileSize;                   ///< [=ULONG_MAX]                   扫描文件的大小限制

    UINT16  uExtractDepth;                  ///< [=32]                          [暂不支持]解压深度,取值范围为 [8, 64],如果超出则取边界值
    UINT16  uUnpackDepth;                   ///< [=32]                          脱壳深度,取值范围为 [8, 64],如果超出则取边界值

    UINT32  uAveScanMask;                   ///< [=BKAVE_SCAN_MASK_DEFAULT]     扫描设置掩码,取值为 BKAVE_SCAN_MASK_xxx
    UINT32  uAveCleanMask;                  ///< [=BKAVE_CLEAN_MASK_NONE]       清除设置掩码,取值为 BKAVE_CLEAN_MASK_xxx, 需要清除时,通常取 BKAVE_CLEAN_MASK_DEFAULT

    // ------------------------------------------------------------
    // 云查杀的扫描设置

    // 云查杀设置
    UINT8   uRetryTimes;                    ///< [=0]                           重试次数,区间为[0, 3],大于3的取为3
    UINT32  uCloudMask;                     ///< [=BKCLOUD_MASK_DEFAULT]        云查杀的设置掩码

    // 缓存掩码
    UINT32  uEnableQueryCacheMask;          ///< [=BKENG_CACHE_MASK_ALL]        取值为 BKENG_CACHE_MASK_xxx
    UINT32  uEnableWriteCacheMask;          ///< [=BKENG_CACHE_MASK_ALL]        取值为 BKENG_CACHE_MASK_xxx
    
    // 扫描掩码
    UINT32  uEnableLocalScanMask;           ///< [=BKENG_LOCAL_SCAN_MASK_ALL]   取值为 BKENG_LOCAL_SCAN_MASK_xxx
    UINT32  uEnableReviewScanMask;          ///< [=BKENG_REVIEW_SCAN_MASK_ALL]  取值为 BKENG_REVIEW_SCAN_MASK_xxx

    // 网络超时设置
    UINT32  uNetworkTimeout;                ///< [=SLPTL_NET_TIMEOUT]           握手的超时设置,单位为毫秒,最小值为3000毫秒,默认30000毫秒(30秒)
    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_SETTINGS* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKENG_SETTINGS);


    // 文件引擎的设置
    pStruct->uBWMType           = BKENG_BWMTYPE_GENERAL;    

    pStruct->uMaxFileSize       = ULONG_MAX;                

    pStruct->uExtractDepth      = 32;                       
    pStruct->uUnpackDepth       = 32;         

    pStruct->uAveScanMask       = BKAVE_SCAN_MASK_DEFAULT;
    pStruct->uAveCleanMask      = BKAVE_CLEAN_MASK_NONE;


    // 云查杀的扫描设置
    pStruct->uRetryTimes            = 1;
    pStruct->uCloudMask             = BKCLOUD_MASK_DEFAULT;

    pStruct->uEnableQueryCacheMask  = BKENG_CACHE_MASK_ALL;
    pStruct->uEnableWriteCacheMask  = BKENG_CACHE_MASK_ALL;

    pStruct->uEnableLocalScanMask   = BKENG_LOCAL_SCAN_MASK_ALL;
    pStruct->uEnableReviewScanMask  = BKENG_REVIEW_SCAN_MASK_DEFAULT;

    pStruct->uNetworkTimeout        = SLPTL_NET_QUERY_TIMEOUT;
}




// ------------------------------------------------------------
// 查杀引擎扫描参数

/// 查杀引擎扫描参数
struct BKENG_SCAN_PARAM
{
    UINT32                  uSize;              ///< 结构体大小

    const WIN32_FIND_DATAW* pFindData;          ///< 枚举文件时候的结构体,如果不为NULL,可以加速扫描速度
    DWORD                   dwScanMask;         ///< 扫描掩码,取值为BKENG_SCAN_MASK_xxx的组合
    IUnknown*               piReserved;         ///< 保留
    DWORD                   dwScanWeight;       ///< 扫描权重,用来计算进度
    void*                   pvFlag;             ///< 调用者定义的绑定参数,回调时原样返回
    DWORD                   dwHighWaterMark;    ///< 高水位线,当等待扫描的文件超过 dwHighWaterMark 时,会阻塞,直到文件数低于 dwHighWaterMark,如果为0,则不阻塞
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_SCAN_PARAM* pStruct)
{  
    assert(pStruct);
    pStruct->uSize              = sizeof(BKENG_SCAN_PARAM);

    pStruct->pFindData          = NULL;
    pStruct->dwScanMask         = BKENG_SCAN_MASK_NONE;
    pStruct->piReserved         = NULL;
    pStruct->dwScanWeight       = 0;
    pStruct->pvFlag             = NULL;
    pStruct->dwHighWaterMark    = 5000;
}




// ------------------------------------------------------------
// 查杀引擎hash扫描参数

/// 查杀引擎hash扫描参数
struct BKENG_HASH_SCAN_PARAM
{
    UINT32      uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN     bUseHex;            ///< =[FALSE]

    BYTE        byMD5[16];          ///< md5,           仅当 bUseHex 为 FALSE 时有效
    UINT32      uCRC32;             ///< crc32,         仅当 bUseHex 为 FALSE 时有效

    WCHAR       szHex[64];          ///< hash的hex形式, 仅当 bUseHex 为 TRUE  时有效
    
    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_HASH_SCAN_PARAM* pStruct)
{  
    assert(pStruct);
    pStruct->uSize          = sizeof(BKENG_HASH_SCAN_PARAM);

    pStruct->bUseHex        = FALSE;

    memset(pStruct->byMD5, 0, sizeof(pStruct->byMD5));
    pStruct->uCRC32         = 0;

    memset(pStruct->szHex, 0, sizeof(pStruct->szHex));
}




// ------------------------------------------------------------
// 查杀引擎扫描结果

/// 查杀引擎设置
struct BKENG_RESULT
{
    UINT32      uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN     bIsVirus;           ///< 是否是病毒
    WCHAR       szVirusName[64];    ///< 病毒名
    UINT32      uScanResult;        ///< 扫描结果,取值为BKAVE_SCAN_RESULT_xxx
    UINT32      uCleanResult;       ///< 扫描结果,取值为BKAVE_CLEAN_RESULT_xxx

    UINT32      uSecLevel;          ///< 文件安全等级,取值为SLPTL_LEVEL_MARK___xxx
    BOOLEAN     bNeedUpload;        ///< 文件需要上传

    BOOLEAN     bHashed;            ///< 是否有hash结果
    BYTE        byMD5[16];          ///< md5
    UINT32      uCRC32;             ///< crc32

    HRESULT     hrLastError;        ///< 最后一次操作的错误码
    // ------------------------------------------------------------
    // added 2010.07.15

    DWORD       dwUsedScanWeight;   ///< 已经消耗的扫描权重
    DWORD       dwTotalScanWeight;  ///< 总扫描权重
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_RESULT* pStruct)
{  
    assert(pStruct);
    pStruct->uSize          = sizeof(BKENG_RESULT);

    pStruct->bIsVirus       = FALSE;
    pStruct->uScanResult    = BKENG_SCAN_RESULT_SAFE;
    pStruct->uCleanResult   = BKENG_CLEAN_RESULT_NO_OP;
    memset(pStruct->szVirusName, 0, sizeof(pStruct->szVirusName));

    pStruct->uSecLevel      = SLPTL_LEVEL_MARK___RESERVED;
    pStruct->bNeedUpload    = FALSE;

    pStruct->bHashed        = FALSE;
    memset(pStruct->byMD5, 0, sizeof(pStruct->byMD5));
    pStruct->uCRC32         = 0;

    pStruct->hrLastError    = S_OK;
    pStruct->dwUsedScanWeight   = 0;    pStruct->dwTotalScanWeight  = 0;}








#define BKENG_UPLOAD_AUTO                  1       ///< 自动上传
#define BKENG_UPLOAD_MANUAL                2       ///< 手动上传
#define BKENG_UPLOAD_FORCE                 3       ///< 强制上传


// ------------------------------------------------------------
// 上传参数

/// 上传参数
struct BKENG_UPLOAD_PARAM
{
    UINT32      uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT32      uUploadChannel;         ///< [=0]                   上传通道号
    UINT32      uUploadAct;             ///< [=BKENG_UPLOAD_AUTO]   上传动作参数,取值为 BKENG_UPLOAD_xxx
    BOOLEAN     bUploadZippedFile;      ///< [=FALSE]               上传已经压缩过的文件

    BYTE        byMD5[16];              ///< [=0]                   文件md5,当且仅当 bUploadZippedFile 为 TRUE 时, 该值必须有效
    UINT32      uCRC32;                 ///< [=0]                   文件crc,当且仅当 bUploadZippedFile 为 TRUE 时, 该值必须有效

    BOOLEAN     bUploadNonPEFile;       ///< [=FALSE]               上传非PE文件

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_UPLOAD_PARAM* pStruct)
{  
    assert(pStruct);
    pStruct->uSize              = sizeof(BKENG_UPLOAD_PARAM);

    pStruct->uUploadChannel     = 0;
    pStruct->uUploadAct         = BKENG_UPLOAD_AUTO;
    pStruct->bUploadZippedFile  = FALSE;

    memset(pStruct->byMD5, 0, sizeof(pStruct->byMD5));
    pStruct->uCRC32             = 0;

    pStruct->bUploadNonPEFile   = FALSE;
}



//////////////////////////////////////////////////////////////////////////
// 握手状态
#define BKENG_ECHO_SUCCESS              0       ///< 成功

#define BKENG_ECHO_UNKNOWN_ERROR        1       ///< 未知错误
#define BKENG_ECHO_LOCAL_ERROR          2       ///< 本地错误
#define BKENG_ECHO_NETWORK_ERROR        3       ///< 远程错误
#define BKENG_ECHO_SERVER_ERROR         4       ///< 服务端错误
#define BKENG_ECHO_SERVICE_UNAVALIABLE  5       ///< 服务不可用

#define BKENG_ECHO_UNRECOGNISED_EXE     6       ///< 未知的可执行文件
#define BKENG_ECHO_BLACK_EXE            7       ///< 可执行文件是木马

#define BKENG_ECHO_INTERRUPTED          8       ///< 握手被中断
#define BKENG_ECHO_LOCAL_COMM_ERROR     10      ///< 本地服务通信失败




// ------------------------------------------------------------

/// 查杀引擎设置
struct BKENG_ECHO_SETTINGS
{
    UINT32  uSize;                          ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT8   uRetryTimes;                    ///< [=1]                       重试次数,区间为[0, 3],大于3的取为3
    UINT32  uNetworkTimeout;                ///< [=SLPTL_NET_ECHO_TIMEOUT]  握手的超时设置,单位为毫秒,最小值为3000毫秒,默认30000毫秒(30秒)

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_ECHO_SETTINGS* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKENG_ECHO_SETTINGS);


    // 文件引擎的设置
    pStruct->uRetryTimes        = 3;
    pStruct->uNetworkTimeout    = SLPTL_NET_ECHO_TIMEOUT;
}



// ------------------------------------------------------------
// 握手信息
/// 握手信息
struct BKENG_ECHO_RESULT
{
    UINT32      uSize;                      ///< 结构体大小

    UINT32      uEchoStatus;                ///< 握手状态取值见 BKENG_ECHO_XXX
    HRESULT     hEchoCode;                  ///< 握手操作的错误码,遵循HRESULT规范

    __time64_t  serverTime;                 ///< 服务器时间

    // ------------------------------------------------------------
    // added after svn r567

    BOOLEAN     bTrustProgram;              ///< 用来进行握手验证的文件是否可信
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_ECHO_RESULT* pStruct)
{  
    assert(pStruct);
    pStruct->uSize              = sizeof(BKENG_ECHO_RESULT);

    pStruct->uEchoStatus        = BKENG_ECHO_UNKNOWN_ERROR;
    pStruct->hEchoCode          = E_FAIL;

    pStruct->serverTime         = 0;

    pStruct->bTrustProgram      = FALSE;
}






// ------------------------------------------------------------
// 杀毒引擎加载设置
struct BKENG_AVE_LOAD
{
    UINT32      uSize;              ///< 结构体大小
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_AVE_LOAD* pStruct)
{  
    assert(pStruct);
    pStruct->uSize              = sizeof(BKENG_AVE_LOAD);
}






#pragma pack(pop)
NS_SKYLARK_END

#endif//BKENGDEF_H