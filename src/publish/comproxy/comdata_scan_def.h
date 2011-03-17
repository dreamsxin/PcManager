#pragma once
#include "bkxdx/bkxdx.h"
#include "bkxdx/bkxdx_cursor_bkjson.h"    // use bkjson implements
#include "bkxdx/bkxdx_exchange_c.h"       // support c type exchange
#include "bkxdx/bkxdx_exchange_atl.h"     // support ATL class exchange
#include "common_def.h"




// scan mode
typedef enum
{
    BkScanModeNull			= -1,
    BkScanModeFastScan     = 0,     ///< 快速扫描
    BkScanModeBootSystemScan,       ///< 启动扫描
    BkScanModeCheckScan,		    ///< 体检扫描
    BkScanModeCustomScan,           ///< 自定义扫描
    BkScanModeOnlyCustomScan,       ///< 自定义扫描(不包含内存启动项)
    BKScanModeMoveDiskScan,         ///< 移动磁盘扫描
    BkScanModeRightScan,            ///< 右键扫描
    BkScanModeFullScan,             ///< 完全扫描
    BkScanModeIeFix,                ///< IE修复用的扫描
    BkScanModeOnlyMemoryScan,       ///< 内存扫描
    BkScanModeCacheMemory,          ///< 安装cahce的扫描模式
    BkScanModeCacheAutorun,         ///< 安装cache的扫描模式
    BkScanModeNum,
};

// scan step
typedef enum 
{
    BkScanStepNull			= -1,
    BkScanStepMemory		= 0,
    BkScanStepAutorun,
    BkScanStepCriticalDir,
    BkScanStepCustomDir,
    BkScanStepAllDisk,
    BkScanStepMoveDisk,
    BkScanStepNum,
};


typedef enum
{
    BkFileTrackNull         = -1,
    BkFileTrackMemory       = 0,
    BkFileTrackAutorun,
    BkFileTrackCriticalDir,
    BkFileTrackNormalDir,
    BkFileTrackNum,
};


// file state（need clean、clean success、clean failed）
typedef enum
{
    BkFileStateNull				= -1,
    //clean结果
    BkFileStateNoClean          = 0,
    BkFileStateDeleteSuccess,
    BkFileStateRepairSuccess,
    BkFileStateDeleteFailed,
    BkFileStateRepairFailed,
    BkFileStateDeleteReboot,
    BkFileStateRepairReboot,
    BkFileStateNeedDisable,

    //scan结果
    BkFileStateNeedClean        = 1000,
    BkFileStateCannotClean,
    BkFileStateSafe,
    BkFileStateScanFailure,
    BkFileStateDisabled,
};

// virus type
typedef enum
{
    BkVirusTypeNull             = -1,
    BkVirusTypeNeedClean           = 0,
    BkVirusTypeNeedDelete, 
    BkVirusTypeNoOperation, 
    BkVirusTypeNeedDisable,
    BkVirusTypeNum,
};

// request type
typedef enum
{
    BkQueryInfoFileTypeNull			= -1,
    BkQueryInfoFileTypeUnknown		= 0,
    BkQueryInfoFileTypeVirus,
    BkQueryInfoFileTypeSafe,
    BkQueryInfoFileTypeNum,
};

typedef enum
{
    BkScanStateNull         = -1,
    BkScanStateReady        = 0,
    BkScanStateScan,
    BkScanStatePause,
    BkScanStateStop,
    BkScanStateForceStop,
    BkScanStateNum
};



XDX_CLASS_BEGIN(SYSTEMTIME)
    XDX_MEMBER(wYear)
    XDX_MEMBER(wMonth)
    XDX_MEMBER(wDayOfWeek)
    XDX_MEMBER(wDay)
    XDX_MEMBER(wHour)
    XDX_MEMBER(wMinute)
    XDX_MEMBER(wSecond)
    XDX_MEMBER(wMilliseconds)
XDX_CLASS_END

typedef struct _BK_SCAN_INFO 
{
    DWORD	    ScanMode;						//扫描类型
    SYSTEMTIME	StartTime;						//扫描开始时间
    DWORD		ScanTime;						//扫描持续时间
    BOOL		ScanFinished;					//扫描结束标志 
    DWORD		Progress;						//扫描进度
    int		    Step;							//扫描步骤（1.系统内存。2.自启动项。3.系统关键目录、全部目录以及自定义目录等）
    DWORD		ScanFileCount;					//已扫描文件计数
    DWORD		VirusCount;						//扫描到的病毒计数
    DWORD		CleanedVirus;					//已清除的病毒计数
    DWORD		UnkownCount;					//未知文件计数
    DWORD		TrustCount;						//可信文件计数
    DWORD		SafeCount;						//安全文件计数
    DWORD		RebootCount;					//需要重启清除计数
    DWORD		CleanFailedCount;				//清除失败计数
    DWORD		QueryFailedCount;				//查询失败计数
    DWORD		CantCleanVirus;                 //不可清除病毒计数
    CString		LastScanFileName;               //上次扫描的文件全路径
    int         ScanState;                      //扫描状态
    void    Reset()
    {
        ScanMode = BkScanModeNull;
        ::ZeroMemory( &StartTime, sizeof(StartTime) );
        ScanTime = 0;
        ScanFinished = FALSE;
        Progress = 0;
        Step = BkScanStepNull;
        ScanFileCount = 0;
        VirusCount = 0;
        CleanedVirus = 0;
        UnkownCount = 0;
        TrustCount = 0;
        SafeCount = 0;
        RebootCount = 0;
        CleanFailedCount = 0;
        QueryFailedCount = 0;
        CantCleanVirus = 0;
        LastScanFileName = L"";
        ScanState = BkScanStateReady;
    }

    void    Copy( const struct _BK_SCAN_INFO& src )
    {
        ScanMode = src.ScanMode;
        StartTime = src.StartTime;
        ScanTime = src.ScanTime;
        ScanFinished = src.ScanFinished;
        Progress = src.Progress;
        Step = src.Step;
        ScanFileCount = src.ScanFileCount;
        VirusCount = src.VirusCount;
        CleanedVirus = src.CleanedVirus;
        UnkownCount = src.UnkownCount;
        TrustCount = src.TrustCount;
        SafeCount = src.SafeCount;
        RebootCount = src.RebootCount;
        CleanFailedCount = src.CleanFailedCount;
        QueryFailedCount = src.QueryFailedCount;
        CantCleanVirus = src.CantCleanVirus;
        LastScanFileName = src.LastScanFileName;
        ScanState = src.ScanState;
    }
}BK_SCAN_INFO, *PBK_SCAN_INFO;

XDX_CLASS_BEGIN(BK_SCAN_INFO)
    XDX_MEMBER(ScanMode)
    XDX_MEMBER(StartTime)
    XDX_MEMBER(ScanTime)
    XDX_MEMBER(ScanFinished)
    XDX_MEMBER(Progress)
    XDX_MEMBER(Step)
    XDX_MEMBER(ScanFileCount)
    XDX_MEMBER(VirusCount)
    XDX_MEMBER(CleanedVirus)
    XDX_MEMBER(UnkownCount)
    XDX_MEMBER(TrustCount)
    XDX_MEMBER(SafeCount)
    XDX_MEMBER(RebootCount)
    XDX_MEMBER(CleanFailedCount)
    XDX_MEMBER(QueryFailedCount)
    XDX_MEMBER(CantCleanVirus)
    XDX_MEMBER(LastScanFileName)
    XDX_MEMBER(ScanState)
XDX_CLASS_END

typedef struct _BK_CLEAN_INFO
{
    BK_SCAN_INFO    ScanInfo;                   //查杀信息（界面需要更新数据）
    DWORD           OperatedVirus;              //被处理过的病毒数
    BOOL	        CleanFinished;              //清除结束标志
    CString	        LastCleanFileName;          //上次清除的文件全路径

    void    Reset()
    {
        OperatedVirus = 0;
        CleanFinished = 0;
        LastCleanFileName = L"";
        ScanInfo.Reset();
    }

    void    Copy( const struct _BK_CLEAN_INFO& src )
    {
        OperatedVirus = src.OperatedVirus;
        CleanFinished = src.CleanFinished;
        LastCleanFileName = src.LastCleanFileName;
        ScanInfo.Copy( src.ScanInfo );
    }
}BK_CLEAN_INFO, *PBK_CLEAN_INFO;

XDX_CLASS_BEGIN(BK_CLEAN_INFO)
    XDX_MEMBER(ScanInfo)
    XDX_MEMBER(OperatedVirus)
    XDX_MEMBER(CleanFinished)
    XDX_MEMBER(LastCleanFileName)
XDX_CLASS_END

typedef struct _BK_FILE_RESULT
{
    CString		FileName;		                //文件名
    DWORD		Type;                           //扫描结果类型
    DWORD		Status;			                //文件状态（需要清除、清除成功、清除失败等）
    DWORD		Track;			                //文件来源（内存，启动项、系统关键目录或者是普通文件枚举等）
    DWORD		VirusType;		                //病毒类型
    CString		MD5;			                //文件MD5
    DWORD		CRC;			                //文件CRC
    CString		VirusName;		                //病毒名
    DWORD       SecLevel;                       //文件安全等级,引擎返回
    BOOL        bHashed;                        //是否有hash,FALSE表示文件有错误
}BK_FILE_RESULT, *PBK_FILE_RESULT;

XDX_CLASS_BEGIN(BK_FILE_RESULT)
    XDX_MEMBER(FileName)
    XDX_MEMBER(Type)
    XDX_MEMBER(Status)
    XDX_MEMBER(Track)
    XDX_MEMBER(VirusType)
    XDX_MEMBER(MD5)
    XDX_MEMBER(CRC)
    XDX_MEMBER(VirusName)
    XDX_MEMBER(SecLevel)
    XDX_MEMBER(bHashed)
XDX_CLASS_END

typedef struct _BK_FILE_INFO
{
    DWORD			Index;                      //编号
    BK_FILE_RESULT	FileResult;                 //文件信息。
}BK_FILE_INFO, *PBK_FILE_INFO;

XDX_CLASS_BEGIN(BK_FILE_INFO)
    XDX_MEMBER(Index)
    XDX_MEMBER(FileResult)
XDX_CLASS_END



//////////////////////////////////////////////////////////////////////////
// 扫描策略
#define SCAN_POLICY_AVE                         0x00000001		///< 是否使用本地引擎
#define SCAN_POLICY_CLOUD                       0x00000002		///< 是否联网查询
#define SCAN_POLICY_CACHE                       0x00000004		///< 使用cache
#define SCAN_POLICY_BLACKCACHE                  0x00000008		///< 是否使用黑缓存
#define SCAN_POLICY_RESERVED_1                  0x00000010      ///< 是否是监控模式,监控模式会使用较快的扫描设置
#define SCAN_POLICY_FORCE_BOOTDELETE            0x00000020      ///< 当需要删除文件时,强制重启后删除
//#define SCAN_POLICY_DISABLE_OPEN_FILE_FOR_HASH  0x00000040      ///< 禁止打开文件进行hash
//#define SCAN_POLICY_DISABLE_WRITE_CACHE         0x00000080      ///< 禁止写缓存
//#define SCAN_POLICY_DISABLE_WINTRUST            0x00000100      ///< 禁止验证数字签名
//#define SCAN_POLICY_M_ARCHIVED					0x00000200		///< [监控用]扫描时会将Settings.dwScanningMode强行设置上KAV_O_M_ARCHIVED KAV_O_M_MAILBASES KAV_O_M_MAILPLAIN这三个标记
#define SCAN_PLOICY_FOR_RESCAN                  0x00000400      ///< 回扫设置:禁止查询联网缓存,强制联网

#define SCAN_POLICY_STD_SCAN        ( SCAN_POLICY_CLOUD | SCAN_POLICY_CACHE )

#define SCAN_POLICY_																		
///< 当需要删除文件时,强制重启后删除
///< 禁止打开文件进行hash
///< 禁止验证数字签名
///< 禁止写缓存

																								
																								
																								
typedef enum																					
{																								
    CleanFailedNull         = -1,
    CleanFailedDefOp        = 0,
    CleanFailedDelete,
    CleanFailedNum          
};

#define MAX_SCAN_SIZE 0x1400000

typedef enum
{
    BkScanFileTypeNull      = -1,
    BkScanFileTypeAllFile   = 0,
    BkScanFileTypeExe       = 1,
    BkScanFileTypeNum,
};

typedef struct _BK_SCAN_SETTING
{
    DWORD   dwSize;
    UINT64  ScanFileSize;                       ///< [=ULONG_MAX]                   扫描文件的大小限制
    int     nScanFileType;                        //是否过滤其它,无危险文件。
    BOOL	bScanArchive;                        //是否扫描压缩包
    BOOL	bAutoClean;                          //是否在扫描过程中发现病毒自动清除	
    int     CleanFailedOp;                      //清除失败处理
    DWORD   nScanPolicy;                        //扫描策略（1.查缓存, 2.查文件引擎, 3. 云查杀引擎 ）		
    BOOL    bQuarantineVirus;                    //[=TRUE]是否隔离病毒
    void    Reset()
    {
        dwSize = sizeof( struct _BK_SCAN_SETTING );
        ScanFileSize    = MAX_SCAN_SIZE;
        nScanFileType   = BkScanFileTypeAllFile;
        bScanArchive    = FALSE;
        bAutoClean      = FALSE;
        CleanFailedOp   = CleanFailedDefOp;
        nScanPolicy     = SCAN_POLICY_STD_SCAN;
        bQuarantineVirus = TRUE;
    }

}BK_SCAN_SETTING, *PBK_SCAN_SETTING;

XDX_CLASS_BEGIN(BK_SCAN_SETTING)  
    XDX_MEMBER(dwSize)
    XDX_MEMBER(ScanFileSize)
    XDX_MEMBER(nScanFileType)
    XDX_MEMBER(bScanArchive)
    XDX_MEMBER(bAutoClean)
    XDX_MEMBER(CleanFailedOp)
    XDX_MEMBER(nScanPolicy)
    XDX_MEMBER(bQuarantineVirus)
XDX_CLASS_END

typedef struct _BK_FORCE_CLEAN_DETAIL
{
    CString strVirusName;
    DWORD   dwSecLevel;
    DWORD   dwScanPolicy;
    BOOL    bNeedUpload;
    DWORD   dwTrack;
}BK_FORCE_CLEAN_DETAIL, *PBK_FORCE_CLEAN_DETAIL;

XDX_CLASS_BEGIN(BK_FORCE_CLEAN_DETAIL)  
    XDX_MEMBER(strVirusName)
    XDX_MEMBER(dwSecLevel)
    XDX_MEMBER(dwScanPolicy)
    XDX_MEMBER(bNeedUpload)
    XDX_MEMBER(dwTrack)
XDX_CLASS_END

typedef struct _BK_SCAN_ADDITIONAL_PATH
{
    CString strFilePath;
    BOOL    bRecursive;
}BK_SCAN_ADDITIONAL_PATH, *PBK_SCAN_ADDITIONAL_PATH;

XDX_CLASS_BEGIN(BK_SCAN_ADDITIONAL_PATH)  
    XDX_MEMBER(strFilePath)
    XDX_MEMBER(bRecursive)
XDX_CLASS_END