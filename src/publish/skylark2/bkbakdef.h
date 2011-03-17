/**
* @file    bkbakdef.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-05 21:21
*/

#ifndef BKBAKDEF_H
#define BKBAKDEF_H

#include "bkdef.h"
#include "bklevel.h"

NS_SKYLARK_BEGIN

#define BKMOD_NAME_BKBAK      L"ksafebak.dll"

#define BKBAK_COMPRESS_DAT_PASSWORD                 "{A56655E0-538D-4381-910D-4D95F2E97CE8}"
#define BKBAK_TEMPFILE_POSTFIX__SLBACKUP            L".kbk"                 ///< 备份文件时的压缩文件后缀
#define BKBAK_TEMPFILE_INF_EXT                      L".inf"                 ///< 备份文件的压缩信息文件后缀

#define BKBAK_UPLOAD_PACK_FILE_NAME                 L"pack.slzip"           ///< 压缩包内的样本文件名
#define BKBAK_UPLOAD_PACK_INFO_FILE_NAME            L"pack.info.slzip"      ///< 压缩包内的信息文件名
#define BKBAK_UPLOAD_PACK_INFO_FILE_MAX_SIZE        (1024 * 1024)           ///< 压缩包内的信息文件的最大长度

#define BKBAK_BACKUP_FILE_DEFAULT_RESERVE_DAYS      30          ///< 默认保留天数
#define BKBAK_BACKUP_FILE_MAX_RESERVE_DAYS          1000        ///< 最多保留天数
#define BKBAK_BACKUP_MAX_FILES                      10000       ///< 最多备份10000个文件
#define BKBAK_BACKUP_MIN_FILES                      20          ///< 最少备份20个文件

#define BKBAK_BACKUP_LEAST_DISK_FREE_BYTES          (1024 * 1024 * 1024)    ///< 隔离区硬盘剩余空间至少1g
#define BKBAK_BACKUP_LEAST_DISK_FREE_PERCENT        5                       ///< 隔离区硬盘剩余空间至少5%


// ------------------------------------------------------------
// 压缩包的信息

/// 压缩包的信息
struct BKBAK_ZIP_INFO
{
    UINT32      uSize;                      ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    DWORD       dwFileAttributes;           ///< 文件属性
    FILETIME    ftCreationTime;             ///< 文件创建时间
    FILETIME    ftLastAccessTime;           ///< 文件最后访问时间
    FILETIME    ftLastWriteTime;            ///< 文件最后写入时间
    DWORD       nFileSizeHigh;              ///< 文件大小的高32位
    DWORD       nFileSizeLow;               ///< 文件大小的低32位
    WCHAR       cFileName[MAX_PATH];        ///< 文件名(不包含文件路径)

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKBAK_ZIP_INFO* pStruct)
{
    assert(pStruct);
    pStruct->uSize = sizeof(BKBAK_ZIP_INFO);

    pStruct->dwFileAttributes                   = FILE_ATTRIBUTE_NORMAL;
    pStruct->ftCreationTime.dwHighDateTime      = 0;
    pStruct->ftCreationTime.dwLowDateTime       = 0;
    pStruct->ftLastAccessTime.dwHighDateTime    = 0;
    pStruct->ftLastAccessTime.dwLowDateTime     = 0;
    pStruct->ftLastWriteTime.dwHighDateTime     = 0;
    pStruct->ftLastWriteTime.dwLowDateTime      = 0;
    pStruct->nFileSizeHigh                      = 0;
    pStruct->nFileSizeLow                       = 0;
    memset(pStruct->cFileName, 0, sizeof(pStruct->cFileName));
}





// ------------------------------------------------------------
// 文件备份参数

/// 文件备份参数
struct BKBAK_BACKUP_PARAM
{
    UINT        uSize;                          ///< 结构体大小

    WCHAR       cVirusName[64];                 ///< 病毒名

    UINT32      uSecLevel;                      ///< 文件的安全等级
    BOOLEAN     bHashed;                        ///< 是否有有效的hash
    BYTE        byMD5[SKYLARK_MD5_BYTES];       ///< 文件的md5,   仅当 bHashed 不为 FALSE 时,有意义
    UINT32      uCRC32;                         ///< 文件的crc32, 仅当 bHashed 不为 FALSE 时,有意义
};
/// 初始化结构体
inline void BKENG_INIT(BKBAK_BACKUP_PARAM* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKBAK_BACKUP_PARAM);

    memset(pStruct->cVirusName,         0, sizeof(pStruct->cVirusName));

    pStruct->uSecLevel      = SLPTL_LEVEL_MARK___RESERVED;
    pStruct->bHashed        = FALSE;
    memset(pStruct->byMD5,  0, sizeof(pStruct->byMD5));
    pStruct->uCRC32         = 0;
}




// ------------------------------------------------------------
// 文件备份ID

/// 文件备份ID
struct BKBAK_BACKUP_ID
{
    UINT    uSize;
    UINT64  uBackupID;          ///< 备份文件的唯一标识;
};
/// 初始化结构体
inline void BKENG_INIT(BKBAK_BACKUP_ID* pStruct)
{
    assert(pStruct);
    pStruct->uSize      = sizeof(BKBAK_BACKUP_ID);

    pStruct->uBackupID  = 0;
}





// ------------------------------------------------------------
// 文件备份信息

/// 文件备份信息
struct BKBAK_BACKUP_INFO
{
    UINT        uSize;                          ///< 结构体大小

    FILETIME    ftBackupTime;                   ///< 备份时间

    WCHAR       cFileOriginalPath[MAX_PATH];    ///< 文件的原始路径(超过MAX_PATH会被截断)
    WCHAR       cFileName[MAX_PATH];            ///< 文件名(不含路径)
    WCHAR       cVirusName[64];                 ///< 病毒名

    UINT32      uSecLevel;                      ///< 文件的安全等级
    BOOLEAN     bHashed;                        ///< 是否有有效的hash
    BYTE        byMD5[SKYLARK_MD5_BYTES];       ///< 文件的md5,   仅当 bHashed 不为 FALSE 时,有意义
    UINT32      uCRC32;                         ///< 文件的crc32, 仅当 bHashed 不为 FALSE 时,有意义

    DWORD       dwFileAttributes;               ///< 文件属性
    FILETIME    ftCreationTime;                 ///< 文件创建时间
    FILETIME    ftLastAccessTime;               ///< 文件最后访问时间
    FILETIME    ftLastWriteTime;                ///< 文件最后写入时间
    DWORD       nFileSizeHigh;                  ///< 文件大小的高32位
    DWORD       nFileSizeLow;                   ///< 文件大小的低32位

    DWORD       nCompressedSizeHigh;            ///< 压缩后的文件大小的高32位
    DWORD       nCompressedSizeLow;             ///< 压缩后的文件大小的低32位
};
/// 初始化结构体
inline void BKENG_INIT(BKBAK_BACKUP_INFO* pStruct)
{
    assert(pStruct);
    pStruct->uSize                              = sizeof(BKBAK_BACKUP_INFO);

    pStruct->ftBackupTime.dwHighDateTime        = 0;
    pStruct->ftBackupTime.dwLowDateTime         = 0;

    memset(pStruct->cFileOriginalPath,  0, sizeof(pStruct->cFileOriginalPath));
    memset(pStruct->cFileName,          0, sizeof(pStruct->cFileName));
    memset(pStruct->cVirusName,         0, sizeof(pStruct->cVirusName));

    pStruct->uSecLevel                          = SLPTL_LEVEL_MARK___RESERVED;
    pStruct->bHashed                            = FALSE;
    memset(pStruct->byMD5, 0, sizeof(pStruct->byMD5));
    pStruct->uCRC32                             = 0;

    pStruct->dwFileAttributes                   = FILE_ATTRIBUTE_NORMAL;
    pStruct->ftCreationTime.dwHighDateTime      = 0;
    pStruct->ftCreationTime.dwLowDateTime       = 0;
    pStruct->ftLastAccessTime.dwHighDateTime    = 0;
    pStruct->ftLastAccessTime.dwLowDateTime     = 0;
    pStruct->ftLastWriteTime.dwHighDateTime     = 0;
    pStruct->ftLastWriteTime.dwLowDateTime      = 0;
    pStruct->nFileSizeHigh                      = 0;
    pStruct->nFileSizeLow                       = 0;

    pStruct->nCompressedSizeHigh                = 0;
    pStruct->nCompressedSizeLow                 = 0;
}







// ------------------------------------------------------------
// 文件备份信息概要

/// 文件备份信息概要
struct BKBAK_BACKUP_SHORT_INFO
{
    UINT        uSize;                          ///< 结构体大小

    FILETIME    ftBackupTime;                   ///< 备份时间
    WCHAR       cVirusName[64];                 ///< 病毒名

    UINT32      uSecLevel;                      ///< 文件的安全等级
    BOOLEAN     bHashed;                        ///< 是否有有效的hash
    BYTE        byMD5[SKYLARK_MD5_BYTES];       ///< 文件的md5,   仅当 bHashed 不为 FALSE 时,有意义
    UINT32      uCRC32;                         ///< 文件的crc32, 仅当 bHashed 不为 FALSE 时,有意义
};
/// 初始化结构体
inline void BKENG_INIT(BKBAK_BACKUP_SHORT_INFO* pStruct)
{
    assert(pStruct);
    pStruct->uSize                              = sizeof(BKBAK_BACKUP_SHORT_INFO);

    pStruct->ftBackupTime.dwHighDateTime        = 0;
    pStruct->ftBackupTime.dwLowDateTime         = 0;

    memset(pStruct->cVirusName,         0, sizeof(pStruct->cVirusName));

    pStruct->uSecLevel                          = SLPTL_LEVEL_MARK___RESERVED;
    pStruct->bHashed                            = FALSE;
    memset(pStruct->byMD5, 0, sizeof(pStruct->byMD5));
    pStruct->uCRC32                             = 0;
}


inline void BKENG_COPY(BKBAK_BACKUP_SHORT_INFO* pTo, const BKBAK_BACKUP_INFO* pFrom)
{
    assert(pTo);
    assert(pFrom);

    pTo->ftBackupTime   = pFrom->ftBackupTime;
    StrCpyNW(pTo->cVirusName, pFrom->cVirusName, _countof(pTo->cVirusName));
    pTo->cVirusName[_countof(pTo->cVirusName) - 1] = L'\0';

    pTo->uSecLevel  = pFrom->uSecLevel;
    pTo->bHashed    = pFrom->bHashed;
    memcpy(pTo->byMD5, pFrom->byMD5, sizeof(pTo->byMD5));
    pTo->uCRC32     = pFrom->uCRC32;
}







NS_SKYLARK_END

#endif//BKBAKDEF_H