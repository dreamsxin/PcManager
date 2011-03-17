/***********************************************************************
* @file     kxedatastructdef.h
* @author   liuguifeng <liuguifeng@kingsoft.com>
* @date     2008-6-11
* @brief    KXEngine 大引擎数据结构定义文档
*           本文档定义了大引擎封装向外提供的所有数据和支持的设置的具体结构。
************************************************************************/

#pragma once

#include "include/kxe_serialize.h"


/**
 * @addtogroup kxescan_subsystem_data_struct
 * @{
 */

/**
 * @brief 指定需要扫描的目标类型
 */
enum EM_KXE_SCANTARGET_TYPE
{
    em_Target_None              = 0x00000000,   ///< 无效目标
    em_Target_Win32_Directory   = 0x00010003,   ///< Win32目录,TargetName为路径字符串，TargetID的取值为：0（包含子目录）1（不包含子目录）		 
	em_Target_Win32_File        = 0x00010005,   ///< Win32文件,需要指定TargetName为要扫描文件的全路径，TargetID为空。	
	em_Target_All_Malware       = 0x00010200,   ///< 所有恶意软件对象，TargetName为空，TargetID为空
	em_Target_Malware           = 0x00010201,   ///< 恶意软件对象,TargetName为空，需要指定TargetID为恶意软件ID
	em_Target_All_SysRprPoints,             	///< 所有系统修复点，TargetName为空，TargetID为空
	em_Target_SysRprPoints,       	            ///< 系统修复点,TargetName为空，需要指定TargetID为系统修复点ID
	em_Target_Computer,           	            ///< 我的电脑，TargetName为空，TargetID为空
	em_Target_Autoruns,           	            ///< 自启动对象，TargetName为空，TargetID为空
	em_Target_Critical_Area,       	            ///< 关键区域，TargetName为空，TargetID为空
    em_Target_Removable,                        ///< 移动存储设备
	em_Target_Memory                            ///< 内存扫描
};
KXE_JSON_DDX_ENUM(EM_KXE_SCANTARGET_TYPE)

/**
 * @brief 需要扫描的目标内容
 */
typedef struct _S_KXE_SCAN_TARGET
{
    EM_KXE_SCANTARGET_TYPE		emTargetType;  ///< 需要扫描的类型
    std::wstring				strTargetName; ///< 需要扫描的名字,与emTargetType相关
    DWORD				        dwTargetID;    ///< 需要扫描的ID,与emTargetType相关
}S_KXE_SCAN_TARGET;

/**
 * @brief 扫描的目标路径拼接
 * 返回给界面查询使用
 */
typedef struct _S_KXE_SCAN_PATHS
{
	std::wstring strScanPaths;
} S_KXE_SCAN_PATHS;

KXE_JSON_DDX_BEGIN(S_KXE_SCAN_TARGET)
	KXE_JSON_DDX_MEMBER("TargetType", emTargetType)
	KXE_JSON_DDX_MEMBER("TargetName", strTargetName)
	KXE_JSON_DDX_MEMBER("TargetId", dwTargetID)
KXE_JSON_DDX_END()

KXE_JSON_DDX_BEGIN(S_KXE_SCAN_PATHS)
	KXE_JSON_DDX_MEMBER("ScanPaths", strScanPaths)
KXE_JSON_DDX_END()

/**
 * @brief 指定需要处理的目标
 */ 
typedef struct _S_KXE_PROCESS_TARGET
{
    EM_KXE_SCANTARGET_TYPE		emTargetType;   ///< 需要处理的目标类型
    std::wstring				strTargetName;  ///< 需要处理的目标名字,与emTargetType相关
    DWORD				        dwTargetID;     ///< 需要处理的ID,与emTargetType相关
    _S_KXE_PROCESS_TARGET():
    emTargetType(em_Target_None), 
    dwTargetID(0)
    {}
}S_KXE_PROCESS_TARGET;

KXE_JSON_DDX_BEGIN(S_KXE_PROCESS_TARGET)
	KXE_JSON_DDX_MEMBER("TargetType", emTargetType)
	KXE_JSON_DDX_MEMBER("TargetName", strTargetName)
	KXE_JSON_DDX_MEMBER("TargetId", dwTargetID)
KXE_JSON_DDX_END()

/**
* @brief 文件类型定义
*/
enum EM_KXE_FILE_REALTYPE
{
	em_FileType_Excutable,		///< 可执行文件
	em_FileType_OfficeDoc,		///< RTF文件, 包括Office文件
	em_FileType_Zipped,			///< 压缩文件
	em_FileType_Image,			///< 图形文件
	em_FileType_PDF,			///< PDF文件
	em_FileType_Text,			///< 文本文件
};

KXE_JSON_DDX_ENUM(EM_KXE_FILE_REALTYPE)

/**
 * @brief 指定扫描引擎的相关设置
 */
typedef struct _S_KXE_FILE_ENGINE_OPTION
{
    bool    bScanArchieve;              ///< 是否扫描压缩文件
    bool    bBackupFile;                ///< 是否启用隔离,在清除,删除前的时候则进行隔离
    bool    bAutoFixFile;               ///< 发现病毒时是否自动处理
    bool    bDeleteOnCleanFail;         ///< true: 清除失败后隔离 false: 跳过
	bool    bOnlyDocAndProg;            ///< 是否只扫描程序和文档
    _S_KXE_FILE_ENGINE_OPTION():
    bScanArchieve(true),
    bBackupFile(true),
    bAutoFixFile(true),
    bDeleteOnCleanFail(true),
    bOnlyDocAndProg(false)
    {}
}S_KXE_FILE_ENGINE_OPTION;

KXE_JSON_DDX_BEGIN(S_KXE_FILE_ENGINE_OPTION)
	KXE_JSON_DDX_MEMBER("ScanArchieve", bScanArchieve)
	KXE_JSON_DDX_MEMBER("BackupFile", bBackupFile)
	KXE_JSON_DDX_MEMBER("AutoFixFile", bAutoFixFile)
	KXE_JSON_DDX_MEMBER("DeleteOnCleanFail", bDeleteOnCleanFail)
	KXE_JSON_DDX_MEMBER("OnlyDocAndProg", bOnlyDocAndProg)
KXE_JSON_DDX_END()

/**
 * @brief 指定扫描,监控过滤设置
 */
 typedef struct _S_KXE_FILE_FILTER_OPTION
{
    bool    bEnabledFilterFileSize;                  ///< 是否启用文件大小过滤
    bool    bEnabledFileExtFilter;                   ///< 是否启用文件扩展名过滤
    bool    bEnabledFilePathFilter;                  ///< 是否启用文件路径过滤
    bool    bEnabledAutoUploadFile;                  ///< 是否自动上报样本
    DWORD   dwFileSize;                              ///< 过虑文件大小, 以KB为单位
    std::vector<std::wstring> vecExtFilterList;      ///< 扩展名过虑列表
    std::vector<std::wstring> vecPathFilterList;     ///< 文件路径过滤列表
    _S_KXE_FILE_FILTER_OPTION():
    bEnabledFilterFileSize(false),
    bEnabledFileExtFilter(false),
    bEnabledFilePathFilter(false),
    bEnabledAutoUploadFile(true),
    dwFileSize(0)
    {}
}S_KXE_FILE_FILTER_OPTION;

KXE_JSON_DDX_BEGIN(S_KXE_FILE_FILTER_OPTION)
    KXE_JSON_DDX_MEMBER("EnabledFilterFileSize", bEnabledFilterFileSize)
    KXE_JSON_DDX_MEMBER("EnabledFilterFileExt", bEnabledFileExtFilter)
    KXE_JSON_DDX_MEMBER("EnabledFilterFilePath", bEnabledFilePathFilter)
    KXE_JSON_DDX_MEMBER("EnabledAutoUploadFile", bEnabledAutoUploadFile)
    KXE_JSON_DDX_MEMBER("FilterFileSize", dwFileSize)
    KXE_JSON_DDX_MEMBER("FilterFileExtList", vecExtFilterList)
    KXE_JSON_DDX_MEMBER("FilterFilePathList", vecPathFilterList)
KXE_JSON_DDX_END()


/**
 * @brief ScanSession的状态
 */
enum EM_KXE_SCANSESSION_STATUS
{
    em_KXE_ScanStatus_None          = 0,    ///< 无效状态
	em_KXE_ScanStatus_Ready         = 1,    ///< 任务就绪,准备运行(短暂的状态)
	em_KXE_ScanStatus_Running       = 2,    ///< 任务正在运行
	em_KXE_ScanStatus_Paused        = 3,    ///< 任务被暂停
	em_KXE_ScanStatus_Complete      = 4,    ///< 任务完成(有可能是被中止导致的完成)
	em_KXE_ScanStatus_NetDetecting  = 5,    ///< 等待网络检测
};

KXE_JSON_DDX_ENUM(EM_KXE_SCANSESSION_STATUS);

/**
 * @brief 威肋类型定义
 */
enum EM_KXE_THREAT_TYPE
{
    em_ThreatType_MalwareInSystem,		///< 系统恶意软件
    em_ThreatType_FileVirus,			///< 文件病毒
    em_ThreatType_SystemRepairPoint,	///< 系统修复点
};

KXE_JSON_DDX_ENUM(EM_KXE_THREAT_TYPE)

/**
 * @brief 当前正在扫描的状态
 */
typedef struct _S_KXE_SCAN_STATUS
{
    S_KXE_SCAN_TARGET           CurrentTarget;                  ///< 当前正在扫描关键区域，见发起扫描的目标定义
    EM_KXE_SCANSESSION_STATUS   emSessionStatus;                ///< 任务状态
    DWORD                       dwTotalQuantity;                ///< 总的任务量
    DWORD                       dwFinishedQuantity;             ///< 完成的任务量
    DWORD                       dwFindThread;                   ///< 发现威胁的总数
    DWORD                       dwProcessSucceed;               ///< 成功处理威胁的总数
	std::map<EM_KXE_THREAT_TYPE, unsigned int> mapFoundThreats; ///< 根据不同的威胁种类,告诉相关的数量 
    __time64_t		            tmSessionStartTime;             ///< 扫描启动的时间点
    __time64_t		            tmSessionCurrentTime;           ///< 扫描用时
    __time64_t                  tmSessionEndTime;               ///< 扫描结束的时间点（可能是完成，也可能是被中断，原因见SessionSatus）
    HRESULT                     hErrCode;                       ///< 错误码
    _S_KXE_SCAN_STATUS():
    emSessionStatus(em_KXE_ScanStatus_None),
    dwTotalQuantity(0),
    dwFinishedQuantity(0),
    dwFindThread(0),
    dwProcessSucceed(0),
    tmSessionStartTime(0),
    tmSessionCurrentTime(0),
    tmSessionEndTime(0),
    hErrCode(0)
    {}
}S_KXE_SCAN_STATUS;

KXE_JSON_DDX_BEGIN(S_KXE_SCAN_STATUS)
	KXE_JSON_DDX_MEMBER("CurrentTarget", CurrentTarget)
	KXE_JSON_DDX_MEMBER("SessionStatus", emSessionStatus)
	KXE_JSON_DDX_MEMBER("TotalQuantity", dwTotalQuantity)
	KXE_JSON_DDX_MEMBER("FinishedQuantity", dwFinishedQuantity)
    KXE_JSON_DDX_MEMBER("FindThread", dwFindThread)
    KXE_JSON_DDX_MEMBER("ProcessSucced", dwProcessSucceed)
	KXE_JSON_DDX_MEMBER("FoundedThreatsQuantity", mapFoundThreats)
	KXE_JSON_DDX_MEMBER("SessionStartTime", tmSessionStartTime)
    KXE_JSON_DDX_MEMBER("SessionCurrentTime", tmSessionCurrentTime)
	KXE_JSON_DDX_MEMBER("SessionEndTime", tmSessionEndTime)
    KXE_JSON_DDX_MEMBER("ErrCode", hErrCode)
KXE_JSON_DDX_END()

/**
 * @brief 注册表的描述
 */
typedef struct _S_KXE_REG_ITEM
{
    std::wstring strKey;             ///< 注册表键
    std::wstring strValue;           ///< 注册表值
}S_KXE_REG_ITEM;

KXE_JSON_DDX_BEGIN(S_KXE_REG_ITEM)
	KXE_JSON_DDX_MEMBER("key", strKey)
	KXE_JSON_DDX_MEMBER("value", strValue)
KXE_JSON_DDX_END()

/**
 * @brief 进程描述
 */
typedef struct _S_KXE_PROCESS_ITEM
{
    DWORD        dwProcessID;        ///< 进程的ID
    std::wstring strImagePath;       ///< 进程映像的全路径
    _S_KXE_PROCESS_ITEM():dwProcessID(0){}
}S_KXE_PROCESS_ITEM;

KXE_JSON_DDX_BEGIN(S_KXE_PROCESS_ITEM)
	KXE_JSON_DDX_MEMBER("ProcessId", dwProcessID)
	KXE_JSON_DDX_MEMBER("ImagePath", strImagePath)
KXE_JSON_DDX_END()

/**
 * @brief 威助的处理结果
 */
enum EM_KXE_THREAT_PROCESS_RESULT
{
    em_Threat_Process_No_Op                            =  0x00000001,   ///< 未处理
    em_Threat_Process_Unknown_Fail                     =  0x80000001,   ///< 未知失败
 
    em_Threat_Process_Delay                            =  0x00000002,   ///< 延迟处理
    em_Threat_Process_Skip                             =  0x00000003,   ///< 跳过

    // 文件相关的结果
    em_Threat_Process_Clean_File_Success               =  0x00000004,   ///< 清除(修复)文件成功
    em_Threat_Process_Clean_File_Fail                  =  0x80000004,   ///< 清除(修复)文件失败
    em_Threat_Process_Delete_File_Success              =  0x00000005,   ///< 删除文件成功
    em_Threat_Process_Delete_File_Fail                 =  0x80000005,   ///< 删除文件失败

    em_Threat_Process_Reboot_Clean_File_Success        =  0x00000006,   ///< 重启后清除文件(调用成功)
    em_Threat_Process_Reboot_Clean_File_Fail           =  0x80000006,   ///< 重启后清除文件(调用失败)
    em_Threat_Process_Reboot_Delete_File_Success       =  0x00000007,   ///< 重启后删除文件(调用成功)
    em_Threat_Process_Reboot_Delete_File_Fail          =  0x80000007,   ///< 重启后删除文件(调用失败)
    em_Threat_Process_Rename_File_Success              =  0x00000008,   ///< 重命名文件成功
    em_Threat_Process_Rename_File_Fail                 =  0x80000008,   ///< 重命名文件失败

    em_Threat_Process_Quarantine_File_Success          =  0x00000009,   ///< 隔离文件成功
    em_Threat_Process_Quarantine_File_Fail             =  0x80000009,   ///< 隔离文件失败
    em_Threat_Process_Reboot_Quarantine_File_Success   =  0x0000000A,   ///< 重启后隔离文件(调用成功)
    em_Threat_Process_Reboot_Quarantine_File_Fail      =  0x8000000A,   ///< 重启后隔离文件(调用失败)

	em_Threat_Process_File_NoExist					   =  0x81000001,    ///< 文件不存在

	em_Threat_Process_File_InWPL					   =  0x00000020,    ///< 文件在纯白名单中
	em_Threat_Process_File_Restore_Success			   =  0x00000021,    ///< 该文件已经被恢复

};

KXE_JSON_DDX_ENUM(EM_KXE_THREAT_PROCESS_RESULT)

/**
 * @brief 恶意软件描述
 */
typedef struct _S_KXE_MALWARE_THREAT
{
	DWORD                            dwFoundThreatIndex;     ///< 标示了机器上发现的一个威助
	DWORD                            dwThreatID;             ///< 此ID为威胁库内存在的ID
    std::vector<S_KXE_REG_ITEM>      AffectedRegList;        ///< 受影响的注册表项
    std::vector<std::wstring>        FilesToBeFixed;         ///< 需要被修复的文件
    std::vector<std::wstring>        FilesToBeDeleted;       ///< 需要被删除的文件
    std::vector<S_KXE_PROCESS_ITEM>  AffectedProcessList;    ///< 受影响的进程
    EM_KXE_THREAT_PROCESS_RESULT     eResult;                ///< 处理结果
    HRESULT                          hErrCode;               ///< 错误码
}S_KXE_MALWARE_THREAT;

KXE_JSON_DDX_BEGIN(S_KXE_MALWARE_THREAT)
	KXE_JSON_DDX_MEMBER("FoundThreatIndex", dwFoundThreatIndex)
	KXE_JSON_DDX_MEMBER("ThreatId", dwThreatID)
	KXE_JSON_DDX_MEMBER("AffectedRegList", AffectedRegList)
	KXE_JSON_DDX_MEMBER("FilesToBeFixed", FilesToBeFixed)
	KXE_JSON_DDX_MEMBER("FilesToBeDeleted", FilesToBeDeleted)
	KXE_JSON_DDX_MEMBER("AffectedProcessList", AffectedProcessList)
	KXE_JSON_DDX_MEMBER("Result", eResult)
	KXE_JSON_DDX_MEMBER("ErrorCode", hErrCode)
KXE_JSON_DDX_END()


/**
* @brief   病毒的查询类型
*/
enum EM_KXE_THREAT_CHECKING_TYPE
{
    em_KXE_ThreatCheckedByFileEngine,   ///< 文件引擎报毒
    em_KXE_ThreatCheckedByCloud,        ///< 云查杀报毒
};

KXE_JSON_DDX_ENUM(EM_KXE_THREAT_CHECKING_TYPE)


/**
 * @brief   压纹,脱壳文件数据结构
 */
typedef struct _S_KXE_ARCHIVE_THREAT
{
    std::wstring                 strFileName;           ///< 压缩,文件名
    std::wstring                 strFullPath;           ///< 虚拟全路径
    std::wstring                 strThreatName;         ///< 威胁名称
    EM_KXE_THREAT_PROCESS_RESULT eResult;               ///< 处理结果
	DWORD						 dwThreatType;          ///< 病毒类型
    EM_KXE_THREAT_CHECKING_TYPE  eThreatCheckingType;   ///< 报毒类型
    _S_KXE_ARCHIVE_THREAT():
    eResult(em_Threat_Process_No_Op),
    dwThreatType(0),
    eThreatCheckingType(em_KXE_ThreatCheckedByFileEngine)
    {}
}S_KXE_ARCHIVE_THREAT;
KXE_JSON_DDX_BEGIN(S_KXE_ARCHIVE_THREAT)
    KXE_JSON_DDX_MEMBER("FileName", strFileName)
    KXE_JSON_DDX_MEMBER("FullPath", strFullPath)
    KXE_JSON_DDX_MEMBER("ThreatName", strThreatName)
    KXE_JSON_DDX_MEMBER("Result", eResult)
	KXE_JSON_DDX_MEMBER("ThreatType", dwThreatType)
    KXE_JSON_DDX_MEMBER("ThreatCheckingType", eThreatCheckingType)
KXE_JSON_DDX_END()

typedef struct _S_KXE_FILEVIRUS_THREAT
{
	DWORD                               dwFoundThreatIndex;     ///< 标示了机器上发现的一个威助
	DWORD                               dwThreatID;             ///< 此ID为威胁库内存在的ID
	bool                                bFileInWrapper;         ///< 是否是压缩包中的文件(不包括RTF)
	std::wstring                        strFileFullPath;        ///< 病毒文件的全路径
	std::wstring                        strVirusDescription;    ///< 病毒描述
	EM_KXE_THREAT_PROCESS_RESULT        eResult;                ///< 处理结果
	HRESULT                             hErrCode;               ///< 错误码
	std::vector<S_KXE_ARCHIVE_THREAT>  virtualFile;            ///< 压缩,脱壳文件列表
	DWORD			                    dwThreatType;			///< 病毒类型
    EM_KXE_THREAT_CHECKING_TYPE         eThreatCheckingType;    ///< 报毒类型
    _S_KXE_FILEVIRUS_THREAT():
    dwFoundThreatIndex(-1),
    dwThreatID(0),
    bFileInWrapper(false),
    eResult(em_Threat_Process_No_Op),
    hErrCode(0),
    dwThreatType(0),
    eThreatCheckingType(em_KXE_ThreatCheckedByFileEngine)
    {}
}S_KXE_FILEVIRUS_THREAT;
KXE_JSON_DDX_BEGIN(S_KXE_FILEVIRUS_THREAT)
    KXE_JSON_DDX_MEMBER("FoundThreatIndex", dwFoundThreatIndex)
    KXE_JSON_DDX_MEMBER("ThreatId", dwThreatID)
    KXE_JSON_DDX_MEMBER("FileInWrapper", bFileInWrapper)
    KXE_JSON_DDX_MEMBER("FileFullPath", strFileFullPath)
    KXE_JSON_DDX_MEMBER("VirusDesciption", strVirusDescription)
    KXE_JSON_DDX_MEMBER("Result", eResult)
    KXE_JSON_DDX_MEMBER("ErrCode", hErrCode)
    KXE_JSON_DDX_MEMBER("VirtualFiles", virtualFile)
    KXE_JSON_DDX_MEMBER("ThreatType", dwThreatType)
    KXE_JSON_DDX_MEMBER("ThreatCheckingType", eThreatCheckingType)
KXE_JSON_DDX_END()

/**
* @brief 威胁处理结果状态
*/
typedef struct _S_KXE_THREAT_PROCESS_RESULT
{
    DWORD   dwThreadIndex;                  ///< 威胁索引ID
    EM_KXE_THREAT_PROCESS_RESULT eResult;   ///< 处理结果
    std::vector<EM_KXE_THREAT_PROCESS_RESULT>
                                 vecResult; ///< 压缩包,脱壳文件处理结果
    _S_KXE_THREAT_PROCESS_RESULT():dwThreadIndex(-1),eResult(em_Threat_Process_No_Op){}
}S_KXE_THREAT_PROCESS_RESULT;
KXE_JSON_DDX_BEGIN(S_KXE_THREAT_PROCESS_RESULT)
    KXE_JSON_DDX_MEMBER("ThreatIndex", dwThreadIndex)
    KXE_JSON_DDX_MEMBER("Result", eResult)
    KXE_JSON_DDX_MEMBER("ResultExt", vecResult)
KXE_JSON_DDX_END()

/**
 * @brief 系统修复描述
 */
typedef struct _S_KXE_SYSREP_THREAT
{
	DWORD                            dwFoundThreatIndex;     ///< 标示了机器上发现的一个威助
	DWORD                            dwThreatID;             ///< 此ID为威胁库内存在的ID
    _S_KXE_SYSREP_THREAT():dwFoundThreatIndex(-1), dwThreatID(0){}
}S_KXE_SYSREP_THREAT;

KXE_JSON_DDX_BEGIN(S_KXE_SYSREP_THREAT)	
	KXE_JSON_DDX_MEMBER("ThreatId", dwThreatID)	
KXE_JSON_DDX_END()

/**
 * @brief 针对威胁需要处理的动作
 */
enum EM_KXE_THREAT_PROCESS_REQUEST
{
    em_Process_Do,       ///< 处理这些威胁
	em_Process_Ignore    ///< 忽略这些威胁
};

KXE_JSON_DDX_ENUM(EM_KXE_THREAT_PROCESS_REQUEST)

/**
 * @brief 引擎类型枚举
 */
enum EM_KXE_KSEENGINE_TYPE
{
    em_EngineType_FileEngine,       ///< 文件引擎
    em_EngineType_SpywareEngine,	///< 恶意软件引擎
    em_EngineType_SysRepEngine      ///< 系统修复引擎
};

KXE_JSON_DDX_ENUM(EM_KXE_KSEENGINE_TYPE)

/**
 * @brief 病毒库策略枚举
 */
enum EM_KXE_SIGNLIB_POLICY_TYPE
{
	em_SignPolicy_AllSign			///< 使用所有的KS3库（全库策略）
};

KXE_JSON_DDX_ENUM(EM_KXE_SIGNLIB_POLICY_TYPE)

/**
 * @brief 特征库的种类（可以在开始查杀前指定需要使用的病毒库）\n
 *        每类特征库和升级的单元对应
 */
enum EM_KXE_KSESIGN_TYPE
{
	//病毒库（0x00000FFF）
    em_SignType_AllVirusSign	= 0x00000000,			///< 通用病毒库全库
	em_SignType_OnlineVirusSign	= 0x00000001,			///< 在线杀毒病毒库全库
	em_SignType_QQSign			= 0x00000002,			///< QQ木马专用库
	//恶意软件库（0x000FF000）
    em_SignType_AllSpywareSign  = 0x00001000			///< 通用恶意软件库
	
};

KXE_JSON_DDX_ENUM(EM_KXE_KSESIGN_TYPE)

/**
 * @brief 引擎版本信息描述
 */
typedef struct _S_KXE_ENGINEINFO_ITEM
{
    EM_KXE_KSEENGINE_TYPE   emEngineType;   ///< 引擎类型
    DWORD                   dwHighVersion;  ///< 引擎版本高32位
    DWORD                   dwLowVersion;   ///< 引擎版本低32位
    _S_KXE_ENGINEINFO_ITEM():
    emEngineType(em_EngineType_FileEngine),
    dwHighVersion(0),
    dwLowVersion(0)
    {}
}S_KXE_ENGINEINFO_ITEM;

KXE_JSON_DDX_BEGIN(S_KXE_ENGINEINFO_ITEM)
	KXE_JSON_DDX_MEMBER("EngineType", emEngineType)
	KXE_JSON_DDX_MEMBER("HighVersion", dwHighVersion)
	KXE_JSON_DDX_MEMBER("LowVersion", dwLowVersion)
KXE_JSON_DDX_END()

/**
 * @brief 病毒库版本信息描述
 */
typedef struct _S_KXE_SIGNINFO_ITEM
{
    EM_KXE_KSESIGN_TYPE     emSignType;     ///< 病毒库类型
    DWORD                   dwHighVersion;  ///< 病毒库版本高32位
    DWORD                   dwLowVersion;   ///< 病毒库版本低32位
    _S_KXE_SIGNINFO_ITEM():emSignType(em_SignType_AllVirusSign), dwHighVersion(0), dwLowVersion(0){}
}S_KXE_SIGNINFO_ITEM;

KXE_JSON_DDX_BEGIN(S_KXE_SIGNINFO_ITEM)
	KXE_JSON_DDX_MEMBER("SignType", emSignType)
	KXE_JSON_DDX_MEMBER("HighVersion", dwHighVersion)
	KXE_JSON_DDX_MEMBER("LowVersion", dwLowVersion)
KXE_JSON_DDX_END()

/**
* @brief 文件监控设置改变的事件ID,绑定的事件类型为KXE_FILEMON_SIGN_EVENT
* @see KXE_UPDATE_SIGN_EVENT
*/
#define KXE_FILEMON_SIGN_VERSION_EVENT 1

/**
* @brief 文件监控设置改变事件
*/

/**
* @brief 通知文件监控设置改变事件的数据结构
*/
typedef struct _KXE_FILEMON_SIGN_EVENT
{
    int nNullValue;   ///< do nothing
    _KXE_FILEMON_SIGN_EVENT():nNullValue(0){}
}KXE_FILEMON_SIGN_EVENT;

KXE_JSON_DDX_BEGIN(KXE_FILEMON_SIGN_EVENT)
    KXE_JSON_DDX_MEMBER("value", nNullValue)
KXE_JSON_DDX_END()


/**
* @brief 网络代理类型
*        
*/
enum EM_KXE_NETPROXY_TYPE
{
    EM_KXE_NETPROXY_TYPE_NOPROXY = 0x00000000, ///< 没有设置代理
    EM_KXE_NETPROXY_TYPE_HTTP	 = 0x00000001, ///< Http代理
    EM_KXE_NETPROXY_TYPE_SOCKET5 = 0x00000002   ///< Socket5代理
};

KXE_JSON_DDX_ENUM(EM_KXE_NETPROXY_TYPE)

/**
* @brief 设置代理的相关信息
*/
typedef struct _KXE_CLOUD_PROXY_INFO
{
    EM_KXE_NETPROXY_TYPE emProxyType;
    std::wstring strProxyHost;
    DWORD dwProxyPort;
    std::wstring strProxyUsername;
    std::wstring strProxyPassword;
    _KXE_CLOUD_PROXY_INFO():dwProxyPort(80)
    {}
}KXE_CLOUD_PROXY_INFO;

KXE_JSON_DDX_BEGIN(KXE_CLOUD_PROXY_INFO)
KXE_JSON_DDX_MEMBER("ProxyType", emProxyType)
KXE_JSON_DDX_MEMBER("ProxyHost", strProxyHost)
KXE_JSON_DDX_MEMBER("ProxyPort", dwProxyPort)
KXE_JSON_DDX_MEMBER("ProxyUsername", strProxyUsername)
KXE_JSON_DDX_MEMBER("ProxyPassword", strProxyPassword)
KXE_JSON_DDX_END()

KXE_DECLARE_EVENT_ID_TYPE(
                          enum_SubSystem_Scan,
                          KXE_FILEMON_SIGN_VERSION_EVENT,
                          KXE_FILEMON_SIGN_EVENT
                          );


/**
 * @}
 */
