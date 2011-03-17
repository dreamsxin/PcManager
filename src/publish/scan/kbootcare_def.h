///////////////////////////////////////////////////////////////////////////////
///		@file		kbootcare_def.h
///		@author		yxdh
///		@date		2009-4-27 10:27:58
///		@brief		
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "include/kxe_serialize.h"
#include "ksrusertrustlist.h"

//-------------------------------------------------------------------------

/**
* @brief    KSE的扫描会话句柄
*/
typedef struct _S_SESSION_HANDLE
{
	UUID    name;                   ///< 会话的uuid
	UUID    key;                    ///< 扫描会话的密钥, 用来确认 name 的有效性
} S_SESSION_HANDLE;

KXE_JSON_DDX_BEGIN(S_SESSION_HANDLE)
KXE_JSON_DDX_MEMBER("name", name)
KXE_JSON_DDX_MEMBER("key", key)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义扫描类型
*/
typedef enum _EM_SCAN_TYPE
{
	enum_All = 0,								///< 全部
	enum_Hot_Point,								///< 热点
	enum_Startup_Point,							///< 启动点
	enum_System_Abnormal_Point,					///< 系统异常点
	enum_AV_Point,								///< AV点
    enum_System_Abnormal_Point_Browser,  		///< 系统异常点(用于网盾3.5的浏览器扫描)
    enum_Browser_Home,                          ///< 恢复并锁定浏览器主页设置
    enum_DeskTop_Icon,                          ///< 删除桌面流氓上网图标
    enum_Ie_Link,                               ///< 修复被篡改的IE快捷方式
    enum_Sys_Icon,                              ///< 修复被篡改为流氓网站的系统图标（回收站等）
    enum_Ie_Setting,                            ///< 修复Internet设置及IE功能受限的问题
    enum_Search_engine,                         ///< 修复默认搜索引擎异常问题
    enum_Browser_Link ,                         ///< 修复被篡改的其他浏览器快捷方式
    enum_Abnormal_Url ,                         ///< 删除收藏夹的恶意推广网址
    enum_Http_Protocol ,                        ///< 修复HTTP协议及网页文件关联
    enum_Other_Sys_Abnormal_Point,              ///< 其它
	enum_All_With_Process_Expand_Scan,			///< 全部（并且包含活动进程两层目录PE扫描）
	enum_Mbr_Virus_Point,						///< Mbr病毒
    enum_System_Abnormal_Point_Browser_Without_Startup_Point, ///< 系统异常点(用于网盾3.5的浏览器扫描,先扫浏览器相关的脚本再扫其他脚本不扫启动项)
	enum_System_Repair_Diagnosis,				///< 系统修复体检
	enum_BK_TROJAN_Point,						///< 贝壳木马
	enum_Danger_Plugin,							///< 恶意插件
	enum_Hosts_Abnormal,                        ///< Hosts 异常
	enum_Ie_StartPage_Abnormal,					///< IE 首页异常				
	enum_Memory_And_Critical_Startup_point		///< 内存及关键启动项扫描（卫士体检中使用）
} EM_SCAN_TYPE;

KXE_JSON_DDX_ENUM(EM_SCAN_TYPE)

//-------------------------------------------------------------------------

/**
* @brief 定义扫描点
*/
typedef enum _EM_SCAN_POINT
{
	enum_None_Point = 0x000,

	enum_Hot_Point_MS08067,						///< MS08-067 振荡波
	enum_Hot_Point_MS09002,						///< MS09-002 IE
	enum_Hot_Point_Bot,							///< 肉鸡
	enum_Hot_Point_End,							///< hot point end

	enum_Startup_Point_RegStartup,				///< 常规启动项
	enum_Startup_Point_BrowserPlugIn,			///< 浏览器插件
	enum_Startup_Point_IEPlugIn,				///< IE浏览器插件
	enum_Startup_Point_SystemScripts,			///< 系统脚本
	enum_Startup_Point_FileExt,					///< 文件扩展名关联
	enum_Startup_Point_Services,				///< 系统服务
	enum_Startup_Point_Drivers,					///< 驱动程序
	enum_Startup_Point_WOW,						///< WOW相关项
	enum_Startup_Point_Debug,					///< Debug相关项
	enum_Startup_Point_Protocols,				///< 协议(PROTOCOLS)
	enum_Startup_Point_ActivePlugIn,			///< Active控件
	enum_Startup_Point_IFEO,					///< 映像劫持
	enum_Startup_Point_ScreenSave,				///< 屏幕保护程序
	enum_Startup_Point_SPI,						///< SPI
	enum_Startup_Point_SafeBoot,				///< 安全模式启动项
	enum_Startup_Point_AutorunInf,				///< Autorun.inf
	enum_Startup_Point_SystemIni,				///< 系统ini启动
	enum_Startup_Point_StartupFolder,			///< 启动文件夹
	enum_Startup_Point_SoftwarePulgIn,			///< 第三方软件插件
	enum_Startup_Point_Process,					///< 当前进程
	enum_Startup_Point_RiskFolder,				///< 系统风险位置
	enum_Startup_Point_Hosts,					///< hosts文件
	enum_Startup_Point_TaskScheduler,			///< 计划任务
	enum_Startup_Point_Winlogon,				///< 登陆加载项
	enum_Startup_Point_Rootkit,					///< Rootkit程序
	enum_Startup_Point_Others,					///< 其它启动项
	enum_Startup_Point_End,

	enum_System_Abnormal_Point_Start,			///< 系统异常点开始
	enum_System_Abnormal_Point_End,				///< 系统异常点结束

	enum_Diagnosising,							///< 正在进行安全诊断

} EM_SCAN_POINT;

KXE_JSON_DDX_ENUM(EM_SCAN_POINT)

//-------------------------------------------------------------------------

/**
* @brief 扫描目标
*/
typedef struct _S_SCAN_TARGET
{
	EM_SCAN_TYPE				emTargetType;	///< 扫描的类型
	EM_SCAN_POINT				emTargetPoint;	///< 扫描点
	std::wstring				strLocation;	///< 位置
	std::wstring				strFile;		///< 文件
} S_SCAN_TARGET;

KXE_JSON_DDX_BEGIN(S_SCAN_TARGET)
KXE_JSON_DDX_MEMBER("TargetType", emTargetType)
KXE_JSON_DDX_MEMBER("TargetPoint", emTargetPoint)
KXE_JSON_DDX_MEMBER("Location", strLocation)
KXE_JSON_DDX_MEMBER("File", strFile)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------
//因为EM_SESSION_STATUS里面的枚举和kxedatastructdef.h有冲突,所以改下名
/**
* @brief Session的状态
*/
#ifndef KBOOTCARE_DEF_IN_DUBA
typedef enum _EM_SESSION_STATUS
{
	em_ScanStatus_None          = 0,    ///< 无效状态
	em_ScanStatus_Ready         = 1,    ///< 任务就绪,准备运行(短暂的状态)
	em_ScanStatus_Running       = 2,    ///< 任务正在运行
	em_ScanStatus_Complete      = 3		///< 任务完成(有可能是被中止导致的完成)
} EM_SESSION_STATUS;

#define KQUICK_SCAN_SCANSTATUS_NONE em_ScanStatus_None

#else
typedef enum _EM_SESSION_STATUS
{
	em_Cloud_ScanStatus_None          = 0,    ///< 无效状态
	em_Cloud_ScanStatus_Ready         = 1,    ///< 任务就绪,准备运行(短暂的状态)
	em_Cloud_ScanStatus_Running       = 2,    ///< 任务正在运行
	em_Cloud_ScanStatus_Complete      = 3		///< 任务完成(有可能是被中止导致的完成)
} EM_SESSION_STATUS;

#define KQUICK_SCAN_SCANSTATUS_NONE em_Cloud_ScanStatus_None

#endif

KXE_JSON_DDX_ENUM(EM_SESSION_STATUS);

//-------------------------------------------------------------------------

/**
* @brief 定义当前系统状态
*/
typedef enum _EM_SYSTEM_STATUS
{
	enum_System_Status_None	= 0,				///< 扫描中
	enum_System_Status_Fluent,					///< 流畅
	enum_System_Status_Good,					///< 良好
	enum_System_Status_SAFE,					///< 安全
	enum_System_Status_SOSO,					///< 一般
	enum_System_Status_Risk,					///< 有风险
	enum_System_Status_Sick						///< 已中毒
} EM_SYSTEM_STATUS;

KXE_JSON_DDX_ENUM(EM_SYSTEM_STATUS)

//-------------------------------------------------------------------------

/**
* @brief 定义产品类型
*/
typedef enum _EM_PRODUCT_TYPE
{
	enum_PT_UNKNOWN           = 0,
	enum_PT_ANTIVIRUS         = 1,				///< 杀软
	enum_PT_FIREWALL          = 2,				///< 防火墙
	enum_PT_INTERNETSECURITY  = 3				///< 套装
} EM_PRODUCT_TYPE;

KXE_JSON_DDX_ENUM(EM_PRODUCT_TYPE)

//-------------------------------------------------------------------------

/**
* @brief 定义当前系统安装的杀毒软件信息
*/
typedef struct _S_AV_INFO
{
	std::wstring				strName;		///< 软件名称
	std::wstring				strVersion;		///< 版本信息
	EM_PRODUCT_TYPE				ProductType;	///< 产品类型
	BOOL						bAbnormal;		///< 状态是否异常
	_S_AV_INFO():ProductType(enum_PT_UNKNOWN),
				bAbnormal(0)
	{
	}
} S_AV_INFO;

KXE_JSON_DDX_BEGIN(S_AV_INFO)
KXE_JSON_DDX_MEMBER("Name", strName)
KXE_JSON_DDX_MEMBER("Version", strVersion)
KXE_JSON_DDX_MEMBER("ProductType", ProductType)
KXE_JSON_DDX_MEMBER("IsAbnormal", bAbnormal)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义热点结果
*/
typedef enum _EM_HOT_POINT_RESULT
{
	enum_HOT_POINT_NOT_EXIST	= 0,			///< 存在
	enum_HOT_POINT_EXIST		= 1,			///< 不存在
	enum_HOT_POINT_MAYBE_EXIST	= 2				///< 可能
} EM_HOT_POINT_RESULT;

KXE_JSON_DDX_ENUM(EM_HOT_POINT_RESULT)

//-------------------------------------------------------------------------

/**
* @brief 定义热点描述
*/
typedef struct _S_HOT_POINT
{
	EM_SCAN_POINT				emHotPoint;		///< 扫描点
	EM_HOT_POINT_RESULT			emResult;		///< 结果
} S_HOT_POINT;

KXE_JSON_DDX_BEGIN(S_HOT_POINT)
KXE_JSON_DDX_MEMBER("HotPoint", emHotPoint)
KXE_JSON_DDX_MEMBER("Result", emResult)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 当前正在扫描的状态
*/
typedef struct _S_SCAN_STATUS
{
	std::vector<S_SCAN_TARGET>	CurrentTarget;                  ///< 当前正在扫描的目标
	EM_SESSION_STATUS			emSessionStatus;                ///< 任务状态
	DWORD                       dwTotalQuantity;                ///< 总的任务量
	DWORD                       dwFinishedQuantity;             ///< 完成的任务量
	EM_SYSTEM_STATUS			emSystemStatus;					///< 当前系统状态
	std::map<EM_SCAN_TYPE, unsigned int> mapFoundThreats;		///< 根据不同的扫描类型, 告诉相关的数量 
	std::vector<S_HOT_POINT>	HotPointList;					///< 热点列表
	S_AV_INFO					AvInfo;							///< 杀软信息
	std::wstring				strVersion;						///< 当前软件版本号
	__time64_t		            tmSessionStartTime;             ///< 扫描启动的时间点
	__time64_t		            tmSessionCurrentTime;           ///< 当前时间点,用于计算已经扫描了多少时间
	__time64_t                  tmSessionEndTime;               ///< 扫描结束的时间点（可能是完成，也可能是被中断，原因见SessionSatus）
	HRESULT                     hErrCode;                       ///< 错误码
	//////////////////////////////////////////////////////////////////////////
	//new data
	DWORD						dwBlackCnt;						///< 黑文件数量	
	DWORD						dwAllCnt;						///< 总文件数量
	unsigned int				uEmergencyFlag;					///< 紧急标志,目前1为发现感染型病毒,建议用户全盘扫描
	//std::vector<std::wstring>	CurrentWhiteList;				///< 当前白文件列表
	_S_SCAN_STATUS():	emSessionStatus(KQUICK_SCAN_SCANSTATUS_NONE),
						dwTotalQuantity(0),
						dwFinishedQuantity(0),
						emSystemStatus(enum_System_Status_None),
						tmSessionStartTime(0),
						tmSessionCurrentTime(0),
						tmSessionEndTime(0),
						hErrCode(0),
						dwBlackCnt(0),
						dwAllCnt(0),
						uEmergencyFlag(0)
	{
	}
} S_SCAN_STATUS;

KXE_JSON_DDX_BEGIN(S_SCAN_STATUS)
KXE_JSON_DDX_MEMBER("CurrentTargetList", CurrentTarget)
KXE_JSON_DDX_MEMBER("SessionStatus", emSessionStatus)
KXE_JSON_DDX_MEMBER("TotalQuantity", dwTotalQuantity)
KXE_JSON_DDX_MEMBER("FinishedQuantity", dwFinishedQuantity)
KXE_JSON_DDX_MEMBER("SystemStatus", emSystemStatus)
KXE_JSON_DDX_MEMBER("FoundedThreatsQuantity", mapFoundThreats)
KXE_JSON_DDX_MEMBER("HotPointList", HotPointList)
KXE_JSON_DDX_MEMBER("AvInfo", AvInfo)
KXE_JSON_DDX_MEMBER("Version", strVersion)
KXE_JSON_DDX_MEMBER("SessionStartTime", tmSessionStartTime)
KXE_JSON_DDX_MEMBER("SessionCurrentTime", tmSessionCurrentTime)
KXE_JSON_DDX_MEMBER("SessionEndTime", tmSessionEndTime)
KXE_JSON_DDX_MEMBER("ErrCode", hErrCode)
KXE_JSON_DDX_MEMBER("BlackCnt", dwBlackCnt)
KXE_JSON_DDX_MEMBER("AllCnt", dwAllCnt)
KXE_JSON_DDX_MEMBER("EmergencyFlag", uEmergencyFlag)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义查询设置
*/

typedef enum _EM_QUERY_LEVEL
{
	enum_QUERY_LEVEL_NONE = 0,			///< all
	enum_QUERY_LEVEL_SAFE,				///< 安全
	enum_QUERY_LEVEL_RISK				///< 风险
} EM_QUERY_LEVEL;

KXE_JSON_DDX_ENUM(EM_QUERY_LEVEL)

//-------------------------------------------------------------------------

typedef struct _S_QUERY_SETTING
{
	DWORD			dwStartIndex;		///< 要查询威胁的起始索引
	DWORD			dwTotalCount;		///< 本次查询最多返回的数量
	EM_SCAN_POINT	ScanPoint;			///< 扫描点
	EM_QUERY_LEVEL	emLevel;			///< 级别
} S_QUERY_SETTING;

KXE_JSON_DDX_BEGIN(S_QUERY_SETTING)
KXE_JSON_DDX_MEMBER("StartIndex", dwStartIndex)
KXE_JSON_DDX_MEMBER("TotalCount", dwTotalCount)
KXE_JSON_DDX_MEMBER("ScanPoint", ScanPoint)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义扫描点项目危险级别
*/
typedef enum _EM_POINT_LEVEL
{
	enum_POINT_LEVEL_VIRUS				= 1001,	///< 病毒程序		危险
	enum_POINT_LEVEL_TROJAN				= 1002,	///< 木马程序		危险
	enum_POINT_LEVEL_BACKDOOR			= 1003,	///< 后门程序		危险
	enum_POINT_LEVEL_WORM				= 1004,	///< 蠕虫程序		危险
	enum_POINT_LEVEL_RISK				= 1005,	///< 风险程序		危险
	enum_POINT_LEVEL_ROOTKIT			= 1006,	///< rootkit程序	可疑
	enum_POINT_LEVEL_UNKNOWN			= 0,	///< 未知			pe可疑
	enum_POINT_LEVEL_DOUBT				= 1,	///< 可疑的(灰)		非pe可疑
	enum_POINT_LEVEL_SAFE				= 2,	///< 安全(白)		全部白
	enum_POINT_LEVEL_DANGER				= 3,	///< 危险(黑)		黑
	enum_POINT_LEVEL_NORMAL				= 4,	///< 一般(非PE)		灰(最新不会抛出)
	enum_POINT_LEVEL_ABNORMAL			= 5,	///< 异常			灰(操作禁用，注册表)
	enum_POINT_LEVEL_SYS_FILE_ABNORMAL	= 6,	///< 系统文件异常	黑并且系统文件
	enum_POINT_LEVEL_NET_DETECT_FAILED	= 7,	///< 联网鉴定失败	
	enum_POINT_LEVEL_REG_REMAIN        	= 8		///< 启动项残留		
} EM_POINT_LEVEL;

KXE_JSON_DDX_ENUM(EM_POINT_LEVEL)

//-------------------------------------------------------------------------

/**
* @brief 定义扫描点项目处理类型
*/
typedef enum _S_POINT_PROCESS_TYPE
{
	enum_PROCESS_TYPE_CLEAN				= 0,	///< 清除
	enum_PROCESS_TYPE_FIX,						///< 修复
	enum_PROCESS_TYPE_KEEP,						///< 保留
	enum_PROCESS_TYPE_DISABLE,					///< 禁用
	enum_PROCESS_TYPE_UPLOAD					///< 上传
} S_POINT_PROCESS_TYPE;

KXE_JSON_DDX_ENUM(S_POINT_PROCESS_TYPE)

//-------------------------------------------------------------------------

/**
* @brief 定义启动点项目信息
*/
typedef struct _S_STARTUP_POINT
{
	EM_SCAN_POINT				emType;			///< 扫描点
	DWORD						dwID;			///< 索引
	std::wstring				strItemName;	///< 名称
	std::wstring				strLocation;	///< 位置(注册表)
	EM_POINT_LEVEL				emLevel;		///< 危险级别
	std::wstring				strFile;		///< 文件路径
	std::wstring				strRegValue;	///< 注册表键值
	S_POINT_PROCESS_TYPE		emStatus;		///< 状态
	std::wstring				strCmd;			///< 命令行
} S_STARTUP_POINT;

KXE_JSON_DDX_BEGIN(S_STARTUP_POINT)
KXE_JSON_DDX_MEMBER("Type", emType)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Name", strItemName)
KXE_JSON_DDX_MEMBER("Location", strLocation)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_MEMBER("File", strFile)
KXE_JSON_DDX_MEMBER("RegValue", strRegValue)
KXE_JSON_DDX_MEMBER("Status", emStatus)
KXE_JSON_DDX_MEMBER("Cmd", strCmd)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义系统异常点项目信息
*/
typedef struct _S_SYSTEM_ABNORMAL_ITEM
{
	DWORD					dwID;				///< 子脚本索引
	std::wstring			strName;			///< 子脚本名称
	std::wstring			strDescript;		///< 子脚本描述
	std::wstring			strAbnormalList;	///< 扫描到的所有 Url 或文件路径
    std::wstring			strScriptUuid;	    ///< 脚本 uuid

} S_SYSTEM_ABNORMAL_ITEM;

KXE_JSON_DDX_BEGIN(S_SYSTEM_ABNORMAL_ITEM)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Name", strName)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_SYSTEM_ABNORMAL
{
	std::vector<S_SYSTEM_ABNORMAL_ITEM>	ItemList;	///< 子项列表
	DWORD								dwID;		///< 索引
	std::wstring						strName;	///< 名称
	EM_POINT_LEVEL						emLevel;	///< 危险级别
	S_POINT_PROCESS_TYPE				emAdvice;	///< 建议操作
} S_SYSTEM_ABNORMAL;

KXE_JSON_DDX_BEGIN(S_SYSTEM_ABNORMAL)
KXE_JSON_DDX_MEMBER("ItemList", ItemList)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Name", strName)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_MEMBER("Status", emAdvice)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_MALICIOUS_ITEM
{
	std::wstring				strType;	///< 类型（Url / 文件路径 / ...）
	unsigned int				uCount;		///< 该类型的值的数目
	std::vector<std::wstring>	vecValue;	///< 值列表

	_S_MALICIOUS_ITEM() : uCount(0)
	{

	}
} S_MALICIOUS_ITEM;

KXE_JSON_DDX_BEGIN(S_MALICIOUS_ITEM)
KXE_JSON_DDX_MEMBER("InfoType", strType)
KXE_JSON_DDX_MEMBER("InfoCount", uCount)
KXE_JSON_DDX_MEMBER("InfoList", vecValue)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _MALICIOUS_INFO
{
	unsigned int					uItemCount;		///< 类型数目
	std::vector<S_MALICIOUS_ITEM>	vecItem;		///< 类型的值列表

	_MALICIOUS_INFO() : uItemCount(0)
	{

	}
} MALICIOUS_INFO;

KXE_JSON_DDX_BEGIN(MALICIOUS_INFO)
KXE_JSON_DDX_MEMBER("ItemCount", uItemCount)
KXE_JSON_DDX_MEMBER("ItemList", vecItem)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义扫描的引擎类型
*/
typedef enum _EM_SCAN_ENGINE_TYPE
{
	enum_SCAN_ENGINE_INVALID_TYPE,         ///< 无效类型
	enum_SCAN_ENGINE_FILEENGINE_TYPE,      ///< 文件引擎
	enum_SCAN_ENGINE_KCLOUD_TYPE,          ///< 金山云引擎
	enum_SCAN_ENGINE_BEIKE_TYPE,           ///< 卫士云引擎
	enum_SCAN_ENGINE_SYSTEM_TYPE,           ///< 系统修复引擎
	enum_SCAN_ENGINE_KSAFE_PLUG_TYPE      ///< 卫士插件引擎
} EM_SCAN_ENGINE_TYPE;

KXE_JSON_DDX_ENUM(EM_SCAN_ENGINE_TYPE)

//-------------------------------------------------------------------------

/**
* @brief 定义需要修复的危险项信息
*/
typedef struct _S_FIX_ITEM
{
	EM_SCAN_TYPE				emType;			///< 类型
	DWORD						dwID;			///< 索引
	std::wstring				strItemName;	///< 名称
	EM_POINT_LEVEL				emLevel;		///< 危险级别
	S_POINT_PROCESS_TYPE		Advice;			///< 建议操作
	S_POINT_PROCESS_TYPE		ActionType;		///< 具体操作

	_S_FIX_ITEM():
	emType(enum_All),
		dwID(0),
		emLevel(enum_POINT_LEVEL_UNKNOWN),
		Advice(enum_PROCESS_TYPE_CLEAN),
		ActionType(enum_PROCESS_TYPE_CLEAN)
	{
	}
} S_FIX_ITEM;

KXE_JSON_DDX_BEGIN(S_FIX_ITEM)
KXE_JSON_DDX_MEMBER("Type", emType)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Name", strItemName)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_MEMBER("Advice", Advice)
KXE_JSON_DDX_MEMBER("ActionType", ActionType)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义历史信息
*/
typedef struct _S_HISTORY_ITEM
{
	__time64_t		            tmHistoryTime;  ///< 时间点
	std::vector<S_FIX_ITEM>		FixedList;		///< 具体操作
} S_HISTORY_ITEM;

KXE_JSON_DDX_BEGIN(S_HISTORY_ITEM)
KXE_JSON_DDX_MEMBER("HistoryTime", tmHistoryTime)
KXE_JSON_DDX_MEMBER("FixedList", FixedList)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义扩展历史信息（包含了某次处理的状态：已修复/已还原）
*/
typedef enum _EM_HISTORY_ITEM_STATE
{
	enum_HISTORY_ITEM_STATE_INVALID				= 0,
	enum_HISTORY_ITEM_STATE_FIXED				= 1,		///< 已修复
	enum_HISTORY_ITEM_STATE_RESTORED			= 2,		///< 已被用户还原
	enum_HISTORY_ITEM_STATE_CANNOT_BE_RESTORED	= 3			///< 无法还原（如修复时没有备份导致）
} EM_HISTORY_ITEM_STATE;

KXE_JSON_DDX_ENUM(EM_HISTORY_ITEM_STATE)

//-------------------------------------------------------------------------

/*
* @brief	定义对象的处理类型
* @remark	！！！必须与 [IDispatch.h][_PROCESS_AT] 保持一致 ！！！
*/
typedef enum _EM_OBJECT_PROCESS_TYPE
{
	enum_PROCESS_AT_UNDEFINED								= -1,	// 非法类型

	enum_PROCESS_AT_BOOT_DELETE_FILE						= 0,	// 删除文件

	enum_PROCESS_AT_BOOT_CLEAN_REG_KEY						= 1,
	enum_PROCESS_AT_BOOT_CLEAN_REG_VALUE					= 2,

	enum_PROCESS_AT_BOOT_REPLACE_REG_SZ_VALUE				= 3,
	enum_enum_PROCESS_AT_BOOT_REPLACE_REG_MULTI_SZ_VALUE	= 4,
	enum_PROCESS_AT_BOOT_REPLACE_REG_VALUE_OTHER_TYPE		= 5,
	enum_PROCESS_AT_BOOT_CLEAN_INFECTED_FILE				= 6,	// 修复被感染文件
	enum_PROCESS_AT_JUST_RECORED							= 7		// 仅记录信息，并不产生处理（目前记录的是被禁用的启动项所关联的灰文件）
} EM_OBJECT_PROCESS_TYPE;

KXE_JSON_DDX_ENUM(EM_OBJECT_PROCESS_TYPE)

//-------------------------------------------------------------------------

/*
* @brief	定义历史管理器额外传出信息（界面展示）
* @remark	
*/
typedef struct _S_PROCESS_OBJECT_INFO
{
	DWORD					dwIndex;		///< 子项编号，从 0 开始
	EM_OBJECT_PROCESS_TYPE	processType;	///< 子项类型
	std::wstring			strInfo_1;		///< processType 不同，此字段代表意义不同
											///< -----------------------------------------------------
											///<	processType		|		含义
											///< -----------------------------------------------------
											///<		0			|	文件路径	
											///<		1			|	注册表键名路径( key path )
											///<		2			|	同上
											///<		3			|	同上
											///<		4			|	同上
											///<		5			|	未定义
											///<		6			|	文件路径
											///<		7			|	文件路径
	std::wstring			strInfo_2;		///< processType 不同，此字段代表意义不同
											///< -----------------------------------------------------
											///<	processType		|		含义
											///< -----------------------------------------------------
											///<		0			|	未定义
											///<		1			|	未定义
											///<		2			|	注册表键值名( value name)
											///<		3			|	同上
											///<		4			|	同上
											///<		5			|	未定义
											///<		6			|	未定义
											///<		7			|	未定义
	std::wstring			strInfo_3;		///< processType 不同，此字段代表意义不同
											///< -----------------------------------------------------
											///<	processType		|		含义
											///< -----------------------------------------------------
											///<		0			|	未定义
											///<		1			|	未定义
											///<		2			|	未定义
											///<		3			|	设置的新值
											///<		4			|	同上
											///<		5			|	未定义
											///<		6			|	未定义
											///<		7			|	未定义
	std::wstring			strVirusName;	///< 病毒名，仅当此项为病毒时才不空

	_S_PROCESS_OBJECT_INFO() : dwIndex(0),
		processType(enum_PROCESS_AT_UNDEFINED)
	{
	}
} S_PROCESS_OBJECT_INFO;

KXE_JSON_DDX_BEGIN(S_PROCESS_OBJECT_INFO)
KXE_JSON_DDX_MEMBER("ID", dwIndex)
KXE_JSON_DDX_MEMBER("ProcessType", processType)
KXE_JSON_DDX_MEMBER("Info_1", strInfo_1)
KXE_JSON_DDX_MEMBER("Info_2", strInfo_2)
KXE_JSON_DDX_MEMBER("Info_3", strInfo_3)
KXE_JSON_DDX_MEMBER("VirusName", strVirusName)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_HISTORY_ITEM_INFO : public _S_FIX_ITEM
{
	EM_HISTORY_ITEM_STATE				state;				///< 处理状态：已修复/已还原
	std::vector<S_PROCESS_OBJECT_INFO>	vecProcessItems;	///< 这一项包含的具体处理对象信息列表

	_S_HISTORY_ITEM_INFO() : state(enum_HISTORY_ITEM_STATE_INVALID)
	{
	}
} S_HISTORY_ITEM_INFO;

KXE_JSON_DDX_BEGIN(S_HISTORY_ITEM_INFO)
KXE_JSON_DDX_MEMBER("Type", emType)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Name", strItemName)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_MEMBER("Advice", Advice)
KXE_JSON_DDX_MEMBER("ActionType", ActionType)
KXE_JSON_DDX_MEMBER("ItemState", state)
KXE_JSON_DDX_MEMBER("ProcessItems", vecProcessItems)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_HISTORY_ITEM_EX
{
	__time64_t							tmHistoryTime;  ///< 时间点
	std::vector<S_HISTORY_ITEM_INFO>	FixedList;		///< 具体操作
} S_HISTORY_ITEM_EX;

KXE_JSON_DDX_BEGIN(S_HISTORY_ITEM_EX)
KXE_JSON_DDX_MEMBER("HistoryTime", tmHistoryTime)
KXE_JSON_DDX_MEMBER("FixedList", FixedList)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/*
* 历史管理器还原功能的结构定义
*/
typedef struct _S_RECOVER_ITEM
{
	__time64_t							tmHistoryTime;  ///< 时间点
	std::vector<DWORD>					vecItemIDs;		///< 本时间点下还原的 Item 列表
} S_RECOVER_ITEM;

KXE_JSON_DDX_BEGIN(S_RECOVER_ITEM)
KXE_JSON_DDX_MEMBER("HistoryTime", tmHistoryTime)
KXE_JSON_DDX_MEMBER("RecoverList", vecItemIDs)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义需要修复的危险项信息(数据项比S_FIX_ITEM多)
*/
typedef struct _S_FIX_ITEM_EX : public _S_FIX_ITEM
{
	//////////////////////////////////////////////////////////////////////////
	//new data
	std::wstring				strFile;				///< 文件
	std::wstring				strSubKey;				///< 子键
	std::wstring				strLocation;			///< 位置
	std::wstring				strDescript;			///< 描述
	MALICIOUS_INFO				MaliciousInfo;			///< 所有威胁列表
	unsigned int				uIndex;					///< 修复项的索引值
	unsigned int				uFixedStatus;			///< 处理的状态,0是未处理,其他值是已经处理
	unsigned int				uSelectStatus;			///< 选择的状态,0是未选择,其他值是已经选择
	EM_SCAN_ENGINE_TYPE         scanEngineType;			///< 扫描的引擎类型
	unsigned int				uIsFileNonPe;			///< 文件是否为非 PE，非 0 为非 PE，0 则为 PE。默认为 PE，即此字段默认为 0
	std::wstring				strScriptGuid;			///< 脚本 UUID
	BOOL						bScriptCanBeTrusted;	///< 标识此脚本是否能够被信任
	DWORD						dwSubItemID;			///< 此项所属的小类 ID（三期界面展现优化）

	_S_FIX_ITEM_EX():
		uIndex(0),
		uFixedStatus(0),
		uSelectStatus(0),
		uIsFileNonPe(0),
		bScriptCanBeTrusted(FALSE),
		dwSubItemID(0)
	{
	}
} S_FIX_ITEM_EX;

KXE_JSON_DDX_BEGIN(S_FIX_ITEM_EX)
KXE_JSON_DDX_MEMBER("Type", emType)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Name", strItemName)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_MEMBER("Advice", Advice)
KXE_JSON_DDX_MEMBER("ActionType", ActionType)
KXE_JSON_DDX_MEMBER("File", strFile)
KXE_JSON_DDX_MEMBER("SubKey", strSubKey)
KXE_JSON_DDX_MEMBER("Location", strLocation)
KXE_JSON_DDX_MEMBER("Descript", strDescript)
KXE_JSON_DDX_MEMBER("MaliciousList", MaliciousInfo)
KXE_JSON_DDX_MEMBER("Index", uIndex)
KXE_JSON_DDX_MEMBER("FixedStatus", uFixedStatus)
KXE_JSON_DDX_MEMBER("SelectStatus", uSelectStatus)
KXE_JSON_DDX_MEMBER("ScanEngineType", scanEngineType)
KXE_JSON_DDX_MEMBER("IsFileNonPe", uIsFileNonPe)
KXE_JSON_DDX_MEMBER("ScriptGuid", strScriptGuid)
KXE_JSON_DDX_MEMBER("ScriptCanBeTrusted", bScriptCanBeTrusted)
KXE_JSON_DDX_MEMBER("SubItemID", dwSubItemID)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义需要修改选择状态的具体项
*/
typedef struct _S_CHANGE_SELECT_ITEM
{
	unsigned int				uIndex;			///< 索引
	unsigned int				uSelectStatus;	///< 0是取消选择
	unsigned int				uChangeAction;  ///< 0表示ActionType无效,否则是有效
	S_POINT_PROCESS_TYPE		ActionType;		///< 具体操作
	

	_S_CHANGE_SELECT_ITEM():
		uIndex(0),
		uSelectStatus(0),
		uChangeAction(0),
		ActionType(enum_PROCESS_TYPE_CLEAN)
	{
	}
} S_CHANGE_SELECT_ITEM;

KXE_JSON_DDX_BEGIN(S_CHANGE_SELECT_ITEM)
KXE_JSON_DDX_MEMBER("Index", uIndex)
KXE_JSON_DDX_MEMBER("SelectStatus", uSelectStatus)
KXE_JSON_DDX_MEMBER("ChangeAction", uChangeAction)
KXE_JSON_DDX_MEMBER("ActionType", ActionType)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义修改选择状态设置信息
*/
typedef struct _S_CHANGE_SELECT_SETTING
{
	_S_CHANGE_SELECT_SETTING():
		dwSelectAll(0)
	{
	}

	DWORD dwSelectAll;		///< 0是忽略, 1是选择全部, 2是取消选择全部,
							///< 执行了全选或取消全选的操作后,会继续对下面的详细列表进行操作
	std::vector<S_CHANGE_SELECT_ITEM>	ItemList;	///< 对每项进行操作
} S_CHANGE_SELECT_SETTING;

KXE_JSON_DDX_BEGIN(S_CHANGE_SELECT_SETTING)
KXE_JSON_DDX_MEMBER("SelectAll", dwSelectAll)
KXE_JSON_DDX_MEMBER("ItemList", ItemList)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_SELECT_RESULT_STATISTICS
{
	_S_SELECT_RESULT_STATISTICS():	uTotalCount(0),
									uSelectedCount(0)
	{
	}

	unsigned int uTotalCount;
	unsigned int uSelectedCount;
}S_SELECT_RESULT_STATISTICS;
KXE_JSON_DDX_BEGIN(S_SELECT_RESULT_STATISTICS)
KXE_JSON_DDX_MEMBER("TotalCount", uTotalCount)
KXE_JSON_DDX_MEMBER("SelectedCount", uSelectedCount)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_FIX_PROC_RESULT_STATISTICS
{
	_S_FIX_PROC_RESULT_STATISTICS():
		uTotalCount(0),
		uProcessCount(0),
		uEmergencyFlag(0),
		uNotProcessCount(0)
	{
	}

	unsigned int uTotalCount;		///< 总数
	unsigned int uProcessCount;		///< 刚处理的数量
	unsigned int uEmergencyFlag;	///< 紧急标志,目前1为发现感染型病毒,建议用户全盘扫描
	unsigned int uNotProcessCount;	///< 未处理的标记

}S_FIX_PROC_RESULT_STATISTICS;
KXE_JSON_DDX_BEGIN(S_FIX_PROC_RESULT_STATISTICS)
KXE_JSON_DDX_MEMBER("TotalCount", uTotalCount)
KXE_JSON_DDX_MEMBER("ProcessCount", uProcessCount)
KXE_JSON_DDX_MEMBER("EmergencyFlag", uEmergencyFlag)
KXE_JSON_DDX_MEMBER("NotProcessCount", uNotProcessCount)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef enum _EM_TRUST_LIST_OPERATION
{
	enum_TRUST_LIST_INVALID		= 0,	///< 无效操作类型
	enum_TRUST_LIST_ADD			= 1,	///< 增加某类型的若干信任项
	enum_TRUST_LIST_DELETE		= 2,	///< 删除某类型的若干信任项
	enum_TRUST_LIST_REPLACE		= 3		///< 替换信任列表的某一类的所有项
} EM_TRUST_LIST_OPERATION;
KXE_JSON_DDX_ENUM(EM_TRUST_LIST_OPERATION)

//-------------------------------------------------------------------------

KXE_JSON_DDX_ENUM(EM_TRUST_ITEM_TYPE)

//-------------------------------------------------------------------------

typedef struct _S_TRUST_LIST
{
	EM_TRUST_LIST_OPERATION		operation;		///< 操作类型
	EM_TRUST_ITEM_TYPE			itemType;		///< 信任项目类型
	std::vector<std::wstring>	vecItemList;	///< 项目列表
} S_TRUST_LIST;
KXE_JSON_DDX_BEGIN(S_TRUST_LIST)
KXE_JSON_DDX_MEMBER("Operation", operation)
KXE_JSON_DDX_MEMBER("Type", itemType)
KXE_JSON_DDX_MEMBER("ItemList", vecItemList)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_HISTORY_RECORD_ITEM
{
	__time64_t					tmTime;			///< 处理时间
	DWORD						dwItemID;		///< 处理项 ID
} S_HISTORY_RECORD_ITEM;
KXE_JSON_DDX_BEGIN(S_HISTORY_RECORD_ITEM)
KXE_JSON_DDX_MEMBER("Time", tmTime)
KXE_JSON_DDX_MEMBER("ItemID", dwItemID)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_REMOVE_HISTORY_RECORD_OPTION
{
	bool								bClearAll;		///< 标识是否清空所有历史处理记录
	std::vector<S_HISTORY_RECORD_ITEM>	vecRecords;		///< （当 bClearAll 为假时）指定要清空的历史处理记录列表
} S_REMOVE_HISTORY_RECORD_OPTION;
KXE_JSON_DDX_BEGIN(S_REMOVE_HISTORY_RECORD_OPTION)
KXE_JSON_DDX_MEMBER("ClearAll", bClearAll)
KXE_JSON_DDX_MEMBER("Records", vecRecords)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义启动项小类扫描状态
*/
typedef enum _EM_SCAN_ITEM_STATUS
{
	enum_SCAN_ITEM_STATUS_UNDEFINED			= -1,			///< 无效状态
	enum_SCAN_ITEM_STATUS_TO_BE_STARTED		= 0,			///< 尚未开始
	enum_SCAN_ITEM_STATUS_SCANNING,							///< 正在扫描
	enum_SCAN_ITEM_STATUS_NET_DIAGNOSISING,					///< 已枚举完毕，正在进行网络检测
	enum_SCAN_ITEM_STATUS_COMPLETED							///< 扫描完成
} EM_SCAN_ITEM_STATUS;

KXE_JSON_DDX_ENUM(EM_SCAN_ITEM_STATUS)

//-------------------------------------------------------------------------

/**
* @brief 定义启动项小类详细信息
*/
typedef struct _S_SCAN_ITEM_DETAIL
{
	DWORD									dwID;				///< 编号（唯一）
	EM_SCAN_ITEM_STATUS						emScanStatus;		///< 扫描状态
	DWORD									dwTotalCount;		///< 扫描总数
	EM_POINT_LEVEL							emLevel;			///< 危险级别，其值为此小类中发现所有异常项中级别最具威胁的一项的危险级别
	DWORD									dwThreatCount;		///< 发现威胁数目

	_S_SCAN_ITEM_DETAIL() : dwID(0),
		emScanStatus(enum_SCAN_ITEM_STATUS_UNDEFINED),
		dwTotalCount(0),
		emLevel(enum_POINT_LEVEL_UNKNOWN),
		dwThreatCount(0)
	{
	}
} S_SCAN_ITEM_DETAIL;

KXE_JSON_DDX_BEGIN(S_SCAN_ITEM_DETAIL)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("TotalCount", dwTotalCount)
KXE_JSON_DDX_MEMBER("ScanStatus", emScanStatus)
KXE_JSON_DDX_MEMBER("Level", emLevel)
KXE_JSON_DDX_MEMBER("ThreatCount", dwThreatCount)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义启动项小类详细信息
*/
typedef struct _S_SCAN_ITEM_SIMPLE_DETAIL
{
	std::wstring							strName;			///< 名称
	DWORD									dwID;				///< 编号（唯一）
	std::wstring							strDecription;		///< 描述

	_S_SCAN_ITEM_SIMPLE_DETAIL() : dwID(0)
	{
	}
} S_SCAN_ITEM_SIMPLE_DETAIL;

KXE_JSON_DDX_BEGIN(S_SCAN_ITEM_SIMPLE_DETAIL)
KXE_JSON_DDX_MEMBER("Name", strName)
KXE_JSON_DDX_MEMBER("ID", dwID)
KXE_JSON_DDX_MEMBER("Description", strDecription)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

/**
* @brief 定义启动项大类详细信息
*/
typedef struct _S_SCAN_GROUP_DETAIL
{
	std::wstring								strName;		///< 名称
	std::vector<S_SCAN_ITEM_SIMPLE_DETAIL>		vecSubItemList;	///< 小类列表，其中键为小类编号
} S_SCAN_GROUP_DETAIL;

KXE_JSON_DDX_BEGIN(S_SCAN_GROUP_DETAIL)
KXE_JSON_DDX_MEMBER("Name", strName)
KXE_JSON_DDX_MEMBER("SubItemList", vecSubItemList)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef struct _S_SCAN_CATEGORY_DETAIL
{
	std::wstring									strName;			///< 总分类名称
	std::vector<S_SCAN_GROUP_DETAIL>				vecScanItem;		///< 大类列表
} S_SCAN_CATEGORY_DETAIL;

KXE_JSON_DDX_BEGIN(S_SCAN_CATEGORY_DETAIL)
KXE_JSON_DDX_MEMBER("Name", strName)
KXE_JSON_DDX_MEMBER("ItemList", vecScanItem)
KXE_JSON_DDX_END()

//-------------------------------------------------------------------------

typedef enum _EM_COMMON_STATUS_ENABLE_FLAGS
{
	enum_dwHistoryRecordCount			= 0x01,					///< 成员 dwHistoryRecordCount 有效
} EM_COMMON_STATUS_ENABLE_FLAGS;

typedef struct _S_COMMON_STATUS_INFO
{
	__int64									nEnableFlags;				///< 标识哪些成员是有效的，为标志 EM_COMMON_STATUS_ENABLE_FLAGS 的逻辑和
	DWORD									dwHistoryRecordCount;		///< （处理）历史记录条数
	_S_COMMON_STATUS_INFO() :
	nEnableFlags(0),
	dwHistoryRecordCount(0)
	{
	}
} S_COMMON_STATUS_INFO;

KXE_JSON_DDX_BEGIN(S_COMMON_STATUS_INFO)
KXE_JSON_DDX_MEMBER("EnableFlags", nEnableFlags)
KXE_JSON_DDX_MEMBER("RecordCount", dwHistoryRecordCount)
KXE_JSON_DDX_END()
