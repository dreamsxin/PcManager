
#pragma once
#define KPFW_REG								L"Software\\Kingsoft\\kpfw"
#define FLOATWND_SETTING_REG					L"Software\\Kingsoft\\kpfw\\FloatWnd"
#define FLOATWND_SETTING_DELAYSECONDS_REG		L"DelaySeconds"		// 启动延时 托盘菜单打开不延时
#define FLOATWND_SETTING_STARTWITHTRAY_REG		L"StartWithTray"	// 托盘启动时是否启动悬浮窗 0-不启动 1-启动
#define FLOATWND_SETTING_IMAGE_REG				L"Image"			// 悬浮窗背景图
#define FLOATWND_SETTING_TRANSPARENT_REG		L"Transparent"		// 悬浮窗透明度(0 - 100) 0:全透明  100:不透明
#define FLOATWND_SETTING_DESKTOP_RIGHT_REG		L"DeskTopRight"		// 悬浮窗距桌面右边的距离
#define FLOATWND_SETTING_DESKTOP_TOP_REG		L"DeskTopTop"		// 悬浮窗距桌面上面的距离
#define FLOATWND_SETTING_STYLE_REG				L"FlaotWndStyle"	// 悬浮窗距桌面上面的距离
#define FLOATWND_SETTING_REFRESH_REG			L"Refresh"			// 悬浮窗 更新 标志
#define FLOATWND_SETTING_PROMPTNOTANYMORE_REG	L"PromptNotAnyMore" // 悬浮窗透图片设置 不再提示 的标志
#define FLOATWND_SETTING_TIPWND_REG				L"TipWndStyle"		// 悬浮窗透图片设置 不再提示 的标志

// 悬浮窗配置文件
#define FLOATWND_CONFIG_FILEPATH						L"\\cfg\\floatwnd.ini"
#define FLOATWND_CONFIG_SECTION_NAME					L"floatwnd"
#define FLOATWND_CONFIG_DISPLAY_WITH_TRAY_KEY_NAME		L"isdisplaywithtray"
#define FLOATWND_CONFIG_REMAID_DAY_NAME					L"remaidday"
#define FLOATWND_CONFIG_REMAID_MONTH_NAME				L"remaidmonth"
#define FLOATWND_CONFIG_REMAID_DAY_MARK_NAME			L"remaiddaymark"	//本日不在提醒
#define FLOATWND_CONFIG_REMAID_MONTH_MARK_NAME			L"remaidmonthmark"	//本月不再提醒
#define FLOATWND_CONFIG_REMAID_DAY_DATA_NAME			L"remaiddaydata"	//最后一次显示对话框的时间
#define FLOATWND_CONFIG_REMAID_MONTH_DATA_NAME			L"remaidmonthdata"
#define FLOATWND_CONFIG_REMAID_WAITTIME					L"waittime"
#define FLOATWND_CONFIG_DISPLAY_POS_TOP_KEY_NAME		L"postop"
#define FLOATWND_CONFIG_DISPLAY_POS_RIGHT_KEY_NAME		L"posright"
#define FLOATWND_CONFIG_AUTO_CLOSE_FLOATWND_WHEN_DISABLE_NETMON				L"autoclosefloatwndwhendisablenetmon"
#define FLOATWND_CONFIG_REMEMBER_AUTO_CLOSE_FLOATWND_WHEN_DISABLE_NETMON	L"rememberautoclose"
#define FLOATWND_CONFIG_RESOLUTION_COUNT_KEYNAME		L"resolutioncount"
#define FLOATWND_CONFIG_RESOLUTION_KEYNAME_PREFIX		L"ResolutionRecord"
#define FLOATWND_CONFIG_RESOLUTION_WIDTH_KEYNAME		L"resolutionwidth"
#define FLOATWND_CONFIG_RESOLUTION_HEIGHT_KEYNAME		L"resolutionheight"
#define	FLOATWND_CONFIG_OPEN_NETMON_FLOATWND			L"netmonfloatwnd"
#define FLOATWND_CONFIG_OPEN_SYSPERFORMANCE_FLOATWND	L"sysperformancefloatwnd"
#define FLOATWND_CONFIG_TOPMOST							L"topmost"


#define NETMON_DLG_PROCESS_NAME				L"\\netmon.exe"

//#define FLOATWND_BACKGROUND_IMAGE			L"\\webui\\splock\\floatwnd\\floatwnd.png"
//#define FLOATWND_TIP_BACKGROUND_IMAGE		L"\\webui\\splock\\floatwnd\\floatwndtip.png"
#define FLOATWND_TIP_DEFAULT_ICO			L"\\webui\\splock\\floatwnd\\defaulticon.ico"
#define FLOATWND_TIP_DEFAULT_ICO_SMALL		L"\\webui\\splock\\floatwnd\\defaulticon_small.ico"
// 默认值
#define FLOATWND_SETTING_TRANSPARENT_DEFAULT 100
#define FLOATWND_SETTING_START_WITH_TRAY_DEFAULT TRUE
#define FLOATWND_SETTING_IMAGE_PATH_DEFAULT L""
#define FLOATWND_SETTING_POS_TOP_DEFAULT 200
#define FLOATWND_SETTING_POS_RIGHT_DEFAULT 350
#define FLOATWND_SETTING_POS_DEFAULT_TO_BOTTOM 100


enum FloatWndStyle {
	KPFW_FLOATWND_STYLE_VERTICAL = 0,		// 悬浮窗 竖向 显示
	KPFW_FLOATWND_STYLE_HORIZONTAL = 1		// 悬浮窗 横向 显示
};

struct FloatStyle
{
	FloatWndStyle style;
};


enum FloatWndTipDirection
{
	ENUM_FLOATWND_TIP_DIRECTION_ABOVE = 0,  
	ENUM_FLOATWND_TIP_DIRECTION_BELOW = 1,
	ENUM_FLOATWND_TIP_DIRECTION_LEFT =  2,
	ENUM_FLOATWND_TIP_DIRECTION_RIGHT = 3
};

// 定义事件
#define EVENT_OPEN_TIP_DLG								L"Global\\{5CD6EFE3-9432-4374-86CD-8FA9C1AAB96B}"
#define EVENT_CLOSE_TIP_DLG								L"Global\\{75D82824-961D-4bd4-B9B7-C9154A781C94}"
#define EVENT_OPEN_FLOATWND_DLG							L"Global\\{65A984EB-65DB-427a-8E68-EEBF7019A902}"
#define EVENT_CLOSE_FLOATWND_DLG						L"Global\\{C82DA7CB-6CEE-4090-ACF3-D94A90F03866}"
#define EVENT_NETMON_DLG_EXIT							L"Global\\{0196B688-8935-4497-A9F1-FCFF0A49AB1B}"
#define EVENT_NETMON_DLG_FLOATWND_DISPLAY_STATUS_TEXT	L"Global\\{0077CA83-ACFB-4c71-8B17-5A8736137044}"

//  [2/21/2011 zhangbaoliang]
#define EVENT_SYSTEM_PERFORMANCE_DLG_UPDATE_FLOATWND_STATUS			L"Global\\{7E6CCD5C-E23D-4821-84BE-19E06BA8FE43}"
#define EVENT_SYSTEM_PERFORMANCE_CLOSE_TIP_DLG						L"Global\\{46C70B52-2E14-44de-916A-FEC7815919ED}"
#define EVENT_OPEN_NETMON_TIP_DLG									L"Global\\{E14400C9-7FE9-4cb4-B7B9-DD64632C2109}"
#define EVENT_CLOSE_NETMON_TIP_DLG									L"Global\\{1092D32F-A246-42b0-AEB5-2EFB5EA46607}"



// 关于Tip窗体一个值的定义
#define TOOLTIPWND_SPACE_FOR_FLOATWND		5	// 悬浮窗Tip窗体和悬浮窗的距离
#define TOOLTIPWND_HEIGHT					194	// Tip窗体的高
#define TOOLTIPWND_WIDTH					213 // Tip窗体的宽
//  [2/19/2011 zhangbaoliang]
#define TOOLTIPWND_NETMON_HEIGHT			219//170
#define TOOLTIPWND_NETMON_WIDTH				272//224

#define TOOLTIPWND_SYSPERFORMANCE_HEIGHT	240//200
#define TOOLTIPWND_SYSPERFORMANCE_WIDTH		272//224

#define TOOLTIPWND_ALL_HEIGHT				382//336
#define TOOLTIPWND_ALL_WIDTH				272//224


enum FloatWndToolTipStyle
{
	FLOATWND_TOOLTIP_Top = 0,				// Tip窗体在悬浮窗的上边
	FLOATWND_TOOLTIP_Bottom = 1,			// Tip窗体在悬浮窗的下边
	FLOATWND_TOOLTIP_Left = 2,				// Tip窗体在悬浮窗的左边
	FLOATWND_TOOLTIP_Right = 3				// Tip窗体在悬浮窗的右边
};

struct FloatTipWndStyle
{
	FloatWndToolTipStyle style;
};

typedef enum emTrustMode
{
	enumTM_UnVerfiy     = 0,
	enumTM_Unknown		= 1 ,						// 当前未知程序
	enumTM_Safe			= 2 ,						// 可信认证自动放行
	enumTM_Danger		= 3 ,						// 泡泡记住放行
} TrustMode;

enum enumProcessType
{
	enumProcessTypeKingsoft,	//金山的服务进程
	enumProcessTypeOS,			//操作系统的进程
	enumProcessTypeService,		//其他服务进程
	enumProcessTypeOther		//其他普通进程
};


//////////////////////////////////////////////////////////////////////////
//系统性能监控配置信息

// 系统性能监控相关的事件定义 [2/15/2011 zhangbaoliang]
#define EVENT_SYSTEM_PERFORMANCE_OPEN_FLOATWND_DLG					L"Global\\{7ADD1C71-178D-4e3b-BAE2-9C8F5405DB5D}"
#define EVENT_SYSTEM_PERFORMANCE_CLOSE_FLOATWND_DLG					L"Global\\{6D271389-2FBC-473f-B87F-1DAAE8061ADC}"
#define EVENT_SYSTEM_PERFORMANCE_FLOATWND_EXIT						L"Global\\{EC90D243-1DD4-4284-B20C-16E0F1035A31}"


#define SYS_PERFORMANCE_FLOATWND_CONFIG_FILEPATH					L"\\cfg\\floatwnd.ini"
#define SYS_PERFORMANCE_FLOATWND_CONFIG_SECTION_NAME				L"sys_performance_floatwnd"
#define SYS_PERFORMANCE_FLOATWND_CONFIG_DISPLAY_WITH_TRAY_KEY_NAME	L"sys_performance_floatwnd_isdisplaywithtray"


#define SYS_PERFORMANCE_FLOATWND_CONFIG_RESOLUTION_COUNT_KEYNAME	L"sys_performance_resolutioncount"
#define SYS_PERFORMANCE_FLOATWND_CONFIG_RESOLUTION_KEYNAME_PREFIX	L"sys_performance_ResolutionRecord"
#define SYS_PERFORMANCE_FLOATWND_CONFIG_RESOLUTION_WIDTH_KEYNAME	L"sys_performance_resolutionwidth"
#define SYS_PERFORMANCE_FLOATWND_CONFIG_RESOLUTION_HEIGHT_KEYNAME	L"sys_performance_resolutionheight"

#define SYS_PERFORMANCE_FLOATWND_CONFIG_DISPLAY_POS_TOP_KEY_NAME	L"sys_performance_postop"
#define SYS_PERFORMANCE_FLOATWND_CONFIG_DISPLAY_POS_RIGHT_KEY_NAME	L"sys_performance_posright"


// 关于Tip窗体一个值的定义
#define SYS_PERFORMANCE_TOOLTIPWND_SPACE_FOR_FLOATWND		5	// 悬浮窗Tip窗体和悬浮窗的距离
#define SYS_PERFORMANCE_TOOLTIPWND_HEIGHT					246	// Tip窗体的高
#define SYS_PERFORMANCE_TOOLTIPWND_WIDTH					213 // Tip窗体的宽


//窗口位置
#define SYS_PERFORMANCE_FLOATWND_SETTING_POS_RIGHT_DEFAULT				180
#define SYS_PERFORMANCE_FLOATWND_SETTING_POS_DEFAULT_TO_BOTTOM			130
#define SYS_PERFORMANCE_FLOATWND_SETTING_POS_TOP_DEFAULT				200