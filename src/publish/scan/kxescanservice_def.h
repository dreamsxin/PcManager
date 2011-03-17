/***********************************************************************
* @file   kxescanservice_def.h
* @author luopeng
* @date   2008-10-13
* @brief  
************************************************************************/

#pragma once
#include <string>
#include "include/kxe_serialize.h"
#include "kxedatastructdef.h"

/**
 * @defgroup kxescan_subsystem_data_struct KXEngine Scan SubSystem Data Struct
 * @{
 */

/**
 * @brief 扫描的属性
 */
enum EM_KXE_GENERIC_SCAN_ATTRIBUTES
{
	em_Sess_Attr_None             = (ULONG)0x00000000,   ///< 无特殊属性
	em_Sess_Attr_Transaction_Log  = (ULONG)0x00000001,   ///< 保留事务日志,以便会话关闭后可以被恢复
	em_Sess_Attr_Resume_Session   = (ULONG)0x00000002,   ///< 恢复一个之前曾被打开的会话
	em_Sess_Attr_Low_Resource_Use = (ULONG)0x00000004   ///< 低资源占用,速度较慢
};
KXE_JSON_DDX_ENUM(EM_KXE_GENERIC_SCAN_ATTRIBUTES);



/**
* @brief 扫描的属性
*/
enum EM_KXE_SCAN_DISK_STATUS
{
	em_Wait             = (ULONG)0x00000000,   ///< 等待扫描
	em_Scanning		    = (ULONG)0x00000001,   ///< 正在扫描
	em_Finish			= (ULONG)0x00000002,   ///< 完成扫描
};
KXE_JSON_DDX_ENUM(EM_KXE_SCAN_DISK_STATUS);


/**
* @brief 设置扫描发起者状态
*/
enum EM_KXE_GENERIC_SCAN_LAUCH
{
    em_KXE_GENERIC_SCAN_NONE = 0,					 // 初始化
    em_KXE_GENERIC_SCAN_MAINUI = 1,					 // 1：用户从主程序界面启动查杀
    em_KXE_GENERIC_SCAN_RIGHTMENU = 2,				 // 2：用户在文件夹上右键选择查杀
    em_KXE_GENERIC_SCAN_SCHEME = 3,					 // 3：定时查毒；
    em_KXE_GENERIC_SCAN_SCREENSAVER = 4,             // 4：屏保查毒；
    em_KXE_GENERIC_SCAN_EMBEDDING = 5,               // 5：由其他程序（MSN、迅雷等）调用毒霸查杀；
    em_KXE_GENERIC_SCAN_COMMANDLINE = 6,			 // 6: 使用命令行查杀（2、3、4、5的情况）；
};
KXE_JSON_DDX_ENUM(EM_KXE_GENERIC_SCAN_LAUCH);


/**
 * @brief    KSE的扫描会话控制块
 */
typedef struct _S_KXE_GENERICSCAN_SESSION_BLOCK
{
    UUID        name;               ///< 会话的uuid,由调用方指定.\n
                                    ///< 如果指定了 em_Sess_Attr_Resume_Session,
                                    ///< 并且,之前具有相同 name 的任务指定了 em_Sess_Attr_Transaction_Log, \n
                                    ///< 那么,该任务会被恢复,以便继续扫描或处理,\n
                                    ///< 当扫描结束后,该sessionID还可以被用来取回查杀结果.\n
                                    ///< 如果 name 为全0,那么内部会为其随机生成一个uuid.\n
                                    ///< \n
                                    ///< 对于大多数查杀任务,可以每次生成一个新的uuid,
                                    ///< 并在会话完成以后,丢弃该uuid.\n

    EM_KXE_GENERIC_SCAN_ATTRIBUTES attributes; ///< 会话的属性掩码
    DWORD						   dwTargetID; ///< 扫描目标类型,用于创建不同的网络查询通道
    EM_KXE_GENERIC_SCAN_LAUCH      emLauchWay;   ///< 发起扫描的方式
    _S_KXE_GENERICSCAN_SESSION_BLOCK():dwTargetID(0), attributes(em_Sess_Attr_None),emLauchWay(em_KXE_GENERIC_SCAN_NONE)
    {}
}S_KXE_GENERICSCAN_SESSION_BLOCK;

KXE_JSON_DDX_BEGIN(S_KXE_GENERICSCAN_SESSION_BLOCK)
	KXE_JSON_DDX_MEMBER("Name", name)
	KXE_JSON_DDX_MEMBER("Attributes", attributes)
    KXE_JSON_DDX_MEMBER("TargetID", dwTargetID)
    KXE_JSON_DDX_MEMBER("LauchWay", emLauchWay)
KXE_JSON_DDX_END()
/**
 * @brief    KSE的扫描会话句柄
 */
typedef struct _S_KXE_GENERICSCAN_HANDLE
{
    UUID    name;                   ///< 会话的uuid
    UUID    key;                    ///< 扫描会话的密钥,用来确认 name 的有效性
}S_KXE_GENERICSCAN_HANDLE;

KXE_JSON_DDX_BEGIN(S_KXE_GENERICSCAN_HANDLE)
	KXE_JSON_DDX_MEMBER("name", name)
	KXE_JSON_DDX_MEMBER("key", key)
KXE_JSON_DDX_END()

/**
 * @brief SignLib相关设置
 */
typedef struct _S_KXE_SIGN_LIB
{
	UUID SignLibKey;                            ///< 所选病毒库的Key，用来验证要加载的病毒库。
	EM_KXE_KSESIGN_TYPE	SignLibType;            ///< 病毒库的种类
	EM_KXE_SIGNLIB_POLICY_TYPE SignPolicyType;  ///< 病毒库策略
    _S_KXE_SIGN_LIB():SignLibType(em_SignType_AllVirusSign), SignPolicyType(em_SignPolicy_AllSign){}
}S_KXE_SIGN_LIB;

KXE_JSON_DDX_BEGIN(S_KXE_SIGN_LIB)
	KXE_JSON_DDX_MEMBER("SignLibKey", SignLibKey)
	KXE_JSON_DDX_MEMBER("SignLibType", SignLibType)
	KXE_JSON_DDX_MEMBER("SignPolicyType", SignPolicyType)
KXE_JSON_DDX_END()

/**
 * @brief 用于对指定的ScanHandle进行MalwareThreadList的相关操作
 */
typedef struct _S_KXE_QUERY_THREAT
{
	DWORD dwStartIndex;///<要查询威胁的起始索引
	DWORD dwTotalCount;///<本次查询最多返回的数量
    _S_KXE_QUERY_THREAT():dwStartIndex(0), dwTotalCount(-1){}
}S_KXE_QUERY_THREAT;

KXE_JSON_DDX_BEGIN(S_KXE_QUERY_THREAT)
	KXE_JSON_DDX_MEMBER("StartIndex", dwStartIndex)
	KXE_JSON_DDX_MEMBER("TotalCount", dwTotalCount)
KXE_JSON_DDX_END()

/**
 * @brief 用于对指定的ScanHandle处理对应的威肋内容
 *        如果bClearAllThread为TRUE, emProcessRequest为em_Process_Ignore，则ThreatIndexList对应的列表不会被处理
 */
typedef struct _S_KXE_PROCESS_SCAN_TARGET
{
    BOOL bClearAllThread;                           ///< 是否清除所有威胁
	EM_KXE_THREAT_PROCESS_REQUEST emProcessRequest; ///< 要发起的处理请求动作方式
	std::vector<DWORD> ThreatIndexList;             ///< 提交处理请求的威胁索引的列表（威胁索引在查询威胁时获得）
    _S_KXE_PROCESS_SCAN_TARGET():bClearAllThread(false), emProcessRequest(em_Process_Do){}
}S_KXE_PROCESS_SCAN_TARGET;

KXE_JSON_DDX_BEGIN(S_KXE_PROCESS_SCAN_TARGET)
    KXE_JSON_DDX_MEMBER("ClearAllThread", bClearAllThread)
	KXE_JSON_DDX_MEMBER("ProcessRequest", emProcessRequest)
	KXE_JSON_DDX_MEMBER("ThreatIndexList", ThreatIndexList)
KXE_JSON_DDX_END()

/**
* @brief 用于对指定的ScanHandle查询处理对应的威肋内容
*/
typedef struct _S_KXE_QUERY_PROCESS_TARGET
{
    EM_KXE_THREAT_TYPE emThreatType;   ///<要查询的威胁类型
    std::vector<DWORD> vecThreatList;  ///<威胁ID列表
}S_KXE_QUERY_PROCESS_TARGET;

KXE_JSON_DDX_BEGIN(S_KXE_QUERY_PROCESS_TARGET)
    KXE_JSON_DDX_MEMBER("QueryProcess", emThreatType)
    KXE_JSON_DDX_MEMBER("ThreatList", vecThreatList)
KXE_JSON_DDX_END()

/**
 * @brief 文件病毒SDK描述
 */
typedef struct _S_KXE_FILEVIRUS_RESULT
{
	DWORD    dwFileSize;                      ///< 此结构的大小,在作传入参数时,需要初始化
	BOOL     bFoundVirus;                     ///< 标志是否发现病毒
	BOOL     bVirusInWrapper;                 ///< 是压缩包内存在病毒
    DWORD    dwThreatID;                      ///< 此ID为威胁库内存在的ID
    WCHAR    szVirusDescription[MAX_PATH];    ///< 病毒描述
    EM_KXE_THREAT_PROCESS_RESULT eResult;     ///< 处理结果
	int      nErrCode;                        ///< 处理错误码,非0时需要检查
}S_KXE_FILEVIRUS_RESULT;
/**
 * @}
 */


/**
* @brief 文件引擎相关信息描述
*/
typedef struct _S_KXE_FILE_ENGINE_INFO
{
    DWORD    dwSize;                          ///< 本结构体大小，在作传入参数时，需要初始化
    UINT32   uBWMVersion;                     ///< 文件引擎黑白机配置文件版本，YYYYMMDDBB格式
    UINT32   uPolicyID;                       ///< 文件引擎病毒库策略文件ID
    UINT32   uSignVersion;                    ///< 文件引擎病毒库版本号，YYYYMMDDBB格式
    INT32    nSignCount;                      ///< 文件引擎病毒库特征数量
}S_KXE_FILE_ENGINE_INFO;
/**
* @}
*/



/**
* @brief 进度配置信息
*/
typedef struct _S_KXE_PROGRESS_OPTION
{
	DWORD dwUpdateDays;         ///< 全盘扫描文件总数失效的天数
	DWORD dwSampleSize;         ///< 设置样本的大小	
}S_KXE_PROGRESS_OPTION;
KXE_JSON_DDX_BEGIN(S_KXE_PROGRESS_OPTION)
KXE_JSON_DDX_MEMBER("UpdateDays", dwUpdateDays)
KXE_JSON_DDX_MEMBER("SampleSize", dwSampleSize)
KXE_JSON_DDX_END()
/**
* @}
*/

/**
* @brief 获取进度信息
*/
typedef struct _S_KXE_PROGRESS_INFO
{
	DWORD dwTimeProgress;         ///< 剩余的时间(ms)
	DWORD dwStreamProgress;       ///< 获取基于数据流的进度  (0 - 100)      	
	DWORD dwFilesProgress;        ///< 获取基于文件数的进度  (0 - 100)
	DWORD dwFilesTotalCount;      ///< 获取扫描文件的总数	
    _S_KXE_PROGRESS_INFO():
    dwTimeProgress(0),
    dwStreamProgress(0),
    dwFilesProgress(0),
    dwFilesTotalCount(0)
    {}
}S_KXE_PROGRESS_INFO;
KXE_JSON_DDX_BEGIN(S_KXE_PROGRESS_INFO)
KXE_JSON_DDX_MEMBER("RestTime",		dwTimeProgress)
KXE_JSON_DDX_MEMBER("DataProgress", dwStreamProgress)
KXE_JSON_DDX_MEMBER("FileProgress", dwFilesProgress)
KXE_JSON_DDX_MEMBER("TotalCount",	dwFilesTotalCount)
KXE_JSON_DDX_END()
/**
* @}
*/


/**
* @brief 磁盘扫描项统计信息
*/
typedef struct _S_KXE_DISKSCANITEM_INFO
{
	std::wstring strDriverName;			 ///< 驱动器名称
	DWORD		 dwScanItems;			 ///< 已经扫描项的总数
	DWORD        dwFindThreats;          ///< 发现的病毒数
    int			 nStatus;				 ///< 表示当前的状态  参考EM_KXE_SCAN_DISK_STATUS
    _S_KXE_DISKSCANITEM_INFO():
    dwScanItems(0),
    dwFindThreats(0),
    nStatus(em_Wait)
    {}
}S_KXE_DISKSCANITEM_INFO;
KXE_JSON_DDX_BEGIN(S_KXE_DISKSCANITEM_INFO)
KXE_JSON_DDX_MEMBER("DriverName",		strDriverName)
KXE_JSON_DDX_MEMBER("ScanItems",		dwScanItems)
KXE_JSON_DDX_MEMBER("FindThreats",		dwFindThreats)
KXE_JSON_DDX_MEMBER("Status",			nStatus)
KXE_JSON_DDX_END()
/**
* @}
*/


/**
* @brief 统计处理结果状态信息
*/
typedef struct _S_KXE_PROCESS_RESULT_INFO
{
	DWORD		 dwUndisposed;			 ///< 未处理的总数
	DWORD		 dwProcessed;			 ///< 已经处理的总数
	DWORD		 dwUnsuccessed;          ///< 处理失败的总数
	DWORD        dwRebootClean;          ///< 重起后清除的总数
	DWORD        dwRebootDelete;		 ///< 重起后删除的总数
    _S_KXE_PROCESS_RESULT_INFO():
    dwUndisposed(0),
    dwProcessed(0),
    dwUnsuccessed(0),
    dwRebootClean(0),
    dwRebootDelete(0)
    {}
}S_KXE_PROCESS_RESULT_INFO;
KXE_JSON_DDX_BEGIN(S_KXE_PROCESS_RESULT_INFO)
KXE_JSON_DDX_MEMBER("Undisposed",	dwUndisposed)
KXE_JSON_DDX_MEMBER("Processed",	dwProcessed)
KXE_JSON_DDX_MEMBER("Unsuccessed",	dwUnsuccessed)
KXE_JSON_DDX_MEMBER("RebootClean",	dwRebootClean)
KXE_JSON_DDX_MEMBER("RebootDelete",	dwRebootDelete)
KXE_JSON_DDX_END()


/**
* @brief 全盘扫描的时候，状态信息的查询
*/
typedef struct _S_KXE_FULLDISK_STATUS_INFO
{
	S_KXE_SCAN_STATUS		ScanStatus;
	S_KXE_PROGRESS_INFO		Progress;
	std::vector<S_KXE_DISKSCANITEM_INFO> DiskInfo;
}S_KXE_FULLDISK_STATUS_INFO;
KXE_JSON_DDX_BEGIN(S_KXE_FULLDISK_STATUS_INFO)
KXE_JSON_DDX_MEMBER("ScanStatus",	ScanStatus)
KXE_JSON_DDX_MEMBER("Progress",	    Progress)
KXE_JSON_DDX_MEMBER("DiskInfo",		DiskInfo)
KXE_JSON_DDX_END()

/**
* @brief 恢复文件的时候使用
*/
typedef struct _S_KXE_RESTOREFILE_LIST
{
	std::vector<unsigned int> FileIndexList;
}S_KXE_RESTOREFILE_LIST;
KXE_JSON_DDX_BEGIN(S_KXE_RESTOREFILE_LIST)
KXE_JSON_DDX_MEMBER("FileIndexList",FileIndexList)
KXE_JSON_DDX_END()