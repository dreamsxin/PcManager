/**
* @file    bkcachedef.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-16 17:36
*/

#ifndef BKCACHEDEF_H
#define BKCACHEDEF_H

#include <algorithm>
#include <atltime.h>
#include "skylarkbase.h"
#include "bklevel.h"

NS_SKYLARK_BEGIN

#define BKMOD_NAME_BKCACHE      L"kcache.dll"

#pragma pack(push)
#pragma pack(4)

// ------------------------------------------------------------
// 文件引擎的接口版本




// 缓存时间                                                   天    时  分 秒
#define BKCACHE_DEFAULT_REMAIN__LONG                CTimeSpan(365,  0,  0,  0).GetTimeSpan()    ///< 长缓存


#define BKCACHE_DEFAULT_REMAIN__BLACK               CTimeSpan(0,    0,  5,  0).GetTimeSpan() 
#define BKCACHE_DEFAULT_REMAIN__UPLOAD              0
#define BKCACHE_DEFAULT_REMAIN__GRAY                CTimeSpan(0,    1,  0,  0).GetTimeSpan()
#define BKCACHE_DEFAULT_REMAIN__TRUST               CTimeSpan(365,  0,  0,  0).GetTimeSpan()

// add by yangjun 2010.11.12 为解决毒霸白缓存时间过短问题
#define BKCACHE_REMAIN_KSWHITE_3DAYS                CTimeSpan(3,  0,  0,  0).GetTimeSpan()
#define BKCACHE_REMAIN_KSWHITE_5DAYS                CTimeSpan(5,  0,  0,  0).GetTimeSpan()
#define BKCACHE_REMAIN_KSWHITE_7DAYS                CTimeSpan(7,  0,  0,  0).GetTimeSpan()
#define BKCACHE_REMAIN_KSWHITE_10DAYS               CTimeSpan(10,  0,  0,  0).GetTimeSpan()
// add end

#define BKCACHE_DEFAULT_REMAIN__PATH_TO_HASH        CTimeSpan(365,  0,  0,  0).GetTimeSpan()

#define BKCACHE_DEFAULT_REMAIN__WIN_TRUST           CTimeSpan(365,  0,  0,  0).GetTimeSpan()
#define BKCACHE_DEFAULT_REMAIN__FILE_TYPE           CTimeSpan(365,  0,  0,  0).GetTimeSpan()

///< 未知文件不缓存
#define BKCACHE_DEFAULT_REMAIN__UNRECOGNISED_FILE   0

///< 从未被扫描的文件缓存1分钟
#define BKCACHE_DEFAULT_REMAIN__NEVER_BEEN_SCANNED  CTimeSpan(0,    0,  1,  0).GetTimeSpan()






// 预定义的缓存ID
#define BKCACHE_STD_NULL                    0       ///< 缓存 无效的
#define BKCACHE_STD_PATH_TO_HASH            1       ///< 缓存 path - hash
#define BKCACHE_STD_HASH_TO_LEVEL           2       ///< 缓存 hash - level
#define BKCACHE_STD_PATH_TO_WINTRUST        3       ///< 缓存 path - wintrust
#define BKCACHE_STD_PATH_TO_NONPE           4       ///< 缓存 path - non-pe
#define BKCACHE_STD_PATH_TO_AVEFT           5       ///< 缓存 path - ave filetype
#define BKCACHE_STD_PATH_TO_AVEBWM          6       ///< 缓存 path - ave bwm result

#define BKCACHE_STD_SCAN_LOG                100     ///< 日志 查杀日志
#define BKCACHE_STD_PATH_TO_DELETE_TRACE    101     ///< 日志 清除跟踪日志,用以检查是否是重复清除

#define BKCACHE_STD_EXCLUDE_FILES           200     ///< 文件排除路径




#define BKCACHE_IMPORT_DB_PASSWORD          L"IMPORT-DB"




// ------------------------------------------------------------
// path->hash 缓存的设置
struct BKCACHE_SETTINGS_PATH_TO_HASH
{
    UINT32  uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN bAsyncUpdate;           ///< [=TRUE]        异步更新模式, 需要调用Flush才会把数据写入数据库
    UINT64  uExpireTime;            ///< [=ULLONG_MAX]  过期时间, 单位为秒, 默认永不过期

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKCACHE_SETTINGS_PATH_TO_HASH* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKCACHE_SETTINGS_PATH_TO_HASH);

    pStruct->bAsyncUpdate   = TRUE;
    pStruct->uExpireTime    = ULLONG_MAX;
}




// ------------------------------------------------------------
// path->hash 缓存记录
struct BKCACHE_PATH_TO_HASH
{
    UINT32  uSize;                      ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT64  uTimeStamp;                 ///< 记录写入的时间戳
    BYTE    byMD5[SKYLARK_MD5_BYTES];   ///< md5
    UINT32  uCRC32;                     ///< crc32
    UINT32  uTag;                       ///< 文件的校验码

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKCACHE_PATH_TO_HASH* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKCACHE_PATH_TO_HASH);

    pStruct->uTimeStamp     = 0;
    memset(pStruct->byMD5, 0, sizeof(pStruct->byMD5));
    pStruct->uCRC32         = 0;
    pStruct->uTag           = 0;
}












// ------------------------------------------------------------
// hash->level 缓存的设置
struct BKCACHE_SETTINGS_HASH_TO_LEVEL
{
    UINT32  uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN bAsyncUpdate;           ///< [=TRUE]        异步更新模式, 需要调用Flush才会把数据写入数据库
    UINT64  vecExpireTime[256];     ///< [=...]         256个level的过期时间, 单位为秒

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKCACHE_SETTINGS_HASH_TO_LEVEL* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKCACHE_SETTINGS_HASH_TO_LEVEL);

    pStruct->bAsyncUpdate   = TRUE;
    memset(pStruct->vecExpireTime, 0, sizeof(pStruct->vecExpireTime));
    assert(SLPTL_LEVEL_GROUP__UPLOAD_MAX   < _countof(pStruct->vecExpireTime));
    assert(SLPTL_LEVEL_GROUP__BLACK_MAX    < _countof(pStruct->vecExpireTime));
    assert(SLPTL_LEVEL_GROUP__GRAY_MAX     < _countof(pStruct->vecExpireTime));
    assert(SLPTL_LEVEL_GROUP__TRUST_MAX    < _countof(pStruct->vecExpireTime));

    // 未知文件
    std::fill(
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__UPLOAD_MIN,
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__UPLOAD_MAX + 1,
        BKCACHE_DEFAULT_REMAIN__UPLOAD);

    // 黑文件
    std::fill(
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__BLACK_MIN,
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__BLACK_MAX + 1,
        BKCACHE_DEFAULT_REMAIN__BLACK);

    // 灰文件
    std::fill(
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__GRAY_MIN,
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__GRAY_MAX + 1,
        BKCACHE_DEFAULT_REMAIN__GRAY);

    // 白文件
    std::fill(
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__TRUST_MIN,
        pStruct->vecExpireTime + SLPTL_LEVEL_GROUP__TRUST_MAX + 1,
        BKCACHE_DEFAULT_REMAIN__TRUST);

    // 单独再调整一下毒霸白缓存时间
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___DUBA_TRUST_3DAY]   = BKCACHE_REMAIN_KSWHITE_3DAYS;
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___DUBA_TRUST_5DAY]   = BKCACHE_REMAIN_KSWHITE_5DAYS;
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___DUBA_TRUST_7DAY]   = BKCACHE_REMAIN_KSWHITE_7DAYS;
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___DUBA_TRUST_10DAY]  = BKCACHE_REMAIN_KSWHITE_10DAYS;

    // 需要上传的文件单独设置
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___UNRECOGNISED_FILE]   = BKCACHE_DEFAULT_REMAIN__UNRECOGNISED_FILE;
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___NEVER_BEEN_SCANNED]  = BKCACHE_DEFAULT_REMAIN__NEVER_BEEN_SCANNED;                         

    // 云启发不缓存
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___BLACK_HEURIST_HIGH_RISK]  = 0;
    pStruct->vecExpireTime[SLPTL_LEVEL_MARK___BLACK_HEURIST_LOW_RISK]   = 0;
}


// ------------------------------------------------------------
// hash->level 缓存记录
struct BKCACHE_HASH_TO_LEVEL
{
    UINT32  uSize;          ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT64  uTimeStamp;     ///< 记录写入的时间戳
    UINT32  uLevel;         ///< 安全级别
    WCHAR   szInfo[64];     ///< 信息

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKCACHE_HASH_TO_LEVEL* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKCACHE_HASH_TO_LEVEL);

    pStruct->uTimeStamp     = 0;
    pStruct->uLevel         = SLPTL_LEVEL_MARK___RESERVED;
    memset(pStruct->szInfo, 0, sizeof(pStruct->szInfo));
}

















// ------------------------------------------------------------
// path->hresult 缓存的设置
struct BKCACHE_SETTINGS_PATH_TO_HRESULT
{
    UINT32  uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    BOOLEAN bAsyncUpdate;           ///< [=TRUE]        异步更新模式, 需要调用Flush才会把数据写入数据库
    UINT64  uExpireTime;            ///< [=ULLONG_MAX]  过期时间, 单位为秒, 默认永不过期

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKCACHE_SETTINGS_PATH_TO_HRESULT* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKCACHE_SETTINGS_PATH_TO_HRESULT);

    pStruct->bAsyncUpdate   = TRUE;
    pStruct->uExpireTime    = ULLONG_MAX;
}


// ------------------------------------------------------------
// hash->hresult 缓存记录
struct BKCACHE_PATH_TO_HRESULT
{
    UINT32  uSize;          ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT32  hResult;        ///< 可以是任何含义,目前使用的方式包括WinTrust返回值,PE判定结果
    UINT64  uTimeStamp;     ///< 记录写入的时间戳
    UINT32  uTag;           ///< 文件的标记,

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKCACHE_PATH_TO_HRESULT* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKCACHE_PATH_TO_HRESULT);

    pStruct->hResult        = 0;
    pStruct->uTimeStamp     = 0;
    pStruct->uTag           = 0;
}






#pragma pack(pop)
NS_SKYLARK_END

#endif//BKCACHEDEF_H