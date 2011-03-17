/**
* @file    bklogdef.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-04 17:56
*/

#ifndef BKLOGDEF_H
#define BKLOGDEF_H

#include "bkcachedef.h"
#include "bkengdef.h"

NS_SKYLARK_BEGIN

// ------------------------------------------------------------
// BKEngine日志

/// BKEngine日志
struct BKENG_SCAN_LOG
{
    UINT32      uSize;              ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT32      uLogID;             ///< 日志的流水号,写入日志时,该项自动生成
    FILETIME    ftLogTime;          ///< 日志录入时间,写入日志时,该项自动生成,注意这里是UTC时间,显示的时候需要转成本地时间

    WCHAR       szPath[MAX_PATH];   ///< 文件路径(超过 MAX_PATH 会被截断)
    WCHAR       szVirusName[64];    ///< 病毒名

    UINT32      uScanResult;        ///< 扫描结果,取值为BKAVE_SCAN_RESULT_xxx
    UINT32      uCleanResult;       ///< 清除结果,取值为BKAVE_CLEAN_RESULT_xxx

    UINT32      uSecLevel;          ///< 文件安全等级,取值为SLPTL_LEVEL_MARK___xxx

    BOOLEAN     bHashed;            ///< 是否有hash结果
    BYTE        byMD5[16];          ///< md5,   仅当 bHashed 不为 FALSE 时,有意义
    UINT32      uCRC32;             ///< crc32, 仅当 bHashed 不为 FALSE 时,有意义

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_SCAN_LOG* pStruct)
{
    assert(pStruct);
    pStruct->uSize  = sizeof(BKENG_SCAN_LOG);

    pStruct->uLogID                     = 0;
    pStruct->ftLogTime.dwHighDateTime   = 0;
    pStruct->ftLogTime.dwLowDateTime    = 0;

    memset(pStruct->szPath,         0, sizeof(pStruct->szPath));
    memset(pStruct->szVirusName,    0, sizeof(pStruct->szVirusName));

    pStruct->uScanResult                = BKENG_SCAN_RESULT_SAFE;
    pStruct->uCleanResult               = BKENG_CLEAN_RESULT_NO_OP;

    pStruct->uSecLevel                  = SLPTL_LEVEL_MARK___RESERVED;

    pStruct->bHashed                    = FALSE;
    memset(pStruct->byMD5,  0, sizeof(pStruct->byMD5));
    pStruct->uCRC32                     = 0;
}

NS_SKYLARK_END

#endif//BKLOGDEF_H