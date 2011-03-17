/* -------------------------------------------------------------------------
// 文件名		:	common/commondefine.h
// 创建人		:	chenghui
// 创建时间		:	2009-3-31 17:34:01
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __COMMON_COMMONDEFINE_H__
#define __COMMON_COMMONDEFINE_H__


#define KWS_SERVICE_NAME  L"Kingsoft Antivirus WebShield Service"


// -------------------------------------------------------------------------
#define KWSCMD_MAINVER		L"beta"

// 主界面的互斥量定义
#define KWS_MUTEX L"AFX_KWSMAIN2_CPP__2E3C367A-9ED5-4d83-B23B-5FD132BDE717__"

// 主界面命令行定义
#define KWSCMD_INSTALL		L"/install"			// 安装调用
#define KWSCMD_UNINSTALL	L"/uninstall"		// 卸载调用

#define KWSCMD_SHOWMAIN			L"/showmain"		// 显示主界面
#define KWSCMD_SHOWSETTING		L"/showsetting"		// 显示设置页面
#define KWSCMD_SHOWTRUSTLIST	L"/showtrustlist"	//显示设置的信任列表页
#define KWSCMD_DELTRUSTURL		L"/deltrusturl"		//删除一条信任url，并显示信任列表页
#define KWSCMD_SHOWADFILTER		L"/showadfilter"	// 显示拦截
#define KWSCMD_SHOWADFILTER_KSAFE		L"/do:UI_kwssetting"	// 显示拦截
#define KWSCMD_SHOWFEEDBACK		L"/showfeedback"	// 显示反馈页
#define KWSCMD_SHOWLOG			L"/showlog"			// 显示日志页
#define KWSCMD_SHOWWHITEBLACK	L"/showwhiteblack"			// 显示黑白名单页
#define KWSCMD_SHOWLSP			L"/showlsp"			// 显示首页保护页
#define KWSCMD_FEEDBACK			L"/feedback"			// 显示首页保护页

#define KWSCMD_SCANFILE			L"/scanfile"       // 扫描文件

#define KWSCMD_SHOWDNSF			L"/showdownfile"	//打开下载安全页

// 原来随机启动的命令行。为了兼容，主界面遇到这个命令行会直接把托盘启动，自己退出。
#define KWSCMD_STARTUP			L"/start" 

#define KWSCMD_CLOSE			L"/close"		// 关闭主界面（托盘调用）
#define KWSCMD_ENABLE			L"/kwsenable"	// 开启保护（托盘调用）
#define KWSCMD_DISABLE			L"/kwsdisable"	// 关闭保护（托盘调用）
//----------------------------------------------------------------------------------
// 托盘的exe
#define KWSTIP_FILENAME			L"kwstray.exe"
#define KWSTRAY_MUTEX			L"{5BA2D87A-A0C4-4e0e-883E-9993AB132272}"

// 托盘的命令行
#define KWSTRAY_CMD_STARTUP		L"/start"		// 开机启动
#define KWSTRAY_CMD_POP			L"/pop"			// 因弹气泡启动tray（服务触发）
#define KWSTRAY_CMD_SHOWTRAY	L"/showtray"	// 因设置显示而启动tray或因主界面启动显示（服务触发）

#define KWSTRAY_CMD_CLOSE		L"/close"		// 关闭托盘
#define KWSTRAY_CMD_INSTALL		L"/install"		// 安装调用

#define KWSTRAY_CMD_CLOSE_ANOTHER	L"/cl_another"	//关闭其他实例

#define KWSTRAY_CMD_DISUPREP    L"/disuprep"  // 关闭upreport

//----------------------------------------------------------------------------------
//增加鼠镖中“访问”网址的命令
//Add by: Xu yixin 20100317

#define KWSTRAY_CMD_SOW_SHELLEXECUTE	L"/sow_shellexecute"

//----------------------------------------------------------------------------------

// 主界面的exe
#define KWSMAIN_FILENAME		L"kwsmain.exe"
#define KWSTIP_FILENAME_A		 "kwstray.exe"
#define KWSTIP_REGPATH_A		 "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define KWSTIP_REGKEY_W			L"kwstray"
#define KWSTIP_REGKEY_A			 "kwstray"

// pop
#define KWSPOP_DLL_NAME			L"kwspop.dll"

// kwsupd.exe
#define KWSUPDATE_EXE			L"kwsupd.exe"
// -------------------------------------------------------------------------
// 保护功能是否开启的MUTEX，提供给第三方使用者查询保护状态
#define KWS_ENABLE_MUTEX		L"Global\\KWS_ENABLE_MUTEX_658B4D40-E61C-48b1-858A-0DA882FD9C67"

// 鉴定网盾是否安装在机器上的注册表位置
#define KWS_CHECK_INSTALL_KEY "SOFTWARE\\Kingsoft\\KSWSVC"
#define KWS_CHECK_INSTALL_VAL "installpath"

// 网盾卸载重启标志
#define	KWS_REBOOT_KEY_PATH "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
#define	KWS_REBOOT_KEY_NAME	"kwsuninstalldel"

// -------------------------------------------------------------------------
// 新版的界面exe相关的参数定义
#define KWSMAIN_HTMLPANEL_MAIN			L"panel=0"
#define KWSMAIN_HTMLPANEL_SETTING		L"panel=1"
#define KWSMAIN_HTMLPANEL_WHITEBLACK	L"panel=2"
#define KWSMAIN_HTMLPANEL_LOG			L"panel=3"
#define KWSMAIN_HTMLPANEL_FEEDBACK		L"panel=4"
#define KWSMAIN_HTMLPANEL_LSP			L"panel=5"

/// 2009.11.06 yaojianfeng
/// 为方便恶意库文件信息独立升级，恶意库文件将拥有独立的外网服务器及索引文件信息
/// 以下宏用于配置本地配置文件存放的恶意库下载地址的配置项及键值，以及服务器上的
/// 索引文件名信息
#define KWS_MALICE_DOWNLOAD_CONFIG_SECTION	L"LibUpdate"
#define KWS_MALICE_DOWNLOAD_CONFIG_KEY	L"URL"
#define KWS_MALICE_DOWNLOAD_INDEX_FILE L"kws_malice.index"

/// 2010.01.08 yaojianfeng
/// 急救箱捆绑定义宏
#define KWS_KSM_FUSION_DOWNLOAD_LOCAL_DIR_NAME L"ksys"
#define KWS_KSM_FUSION_DOWNLOAD_FILE_NAME L"ksysdiag.exe"
#define KWS_KSM_FUSION_REPAIR_FILE_NAME L"RepairKXE.exe"
#define KWS_KSM_FUSION_UNINSTALL_PARAM L"/uninstall"
#define KWS_KSM_FUSION_DOWNLOAD_PROCESS_PARAM L"/update"
#define KWS_KSM_DOWNLOAD_URL L"http://i2u.www.duba.net/i2u/kws3upd/ksys/ksysdiag.exe"
#define KWS_KSM_REPAIR_URL L"http://i2u.www.duba.net/i2u/kws3upd/ksys/RepairKXE.exe"

/// 广告拦截日志文件名字
#define KWS_ADBLOG   L"adblog.log"

// -------------------------------------------------------------------------
// $Log: $

#endif /* __COMMON_COMMONDEFINE_H__ */
