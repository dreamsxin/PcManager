///////////////////////////////////////////////////////////////
//
//	Filename: 	PathDef.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-3-18  18:58
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _PathDef_h_
#define _PathDef_h_


#define REGKEY_KWS			     "SOFTWARE\\kingsoft\\KSWSVC"
#define REGKEY_PROPATH			 "ProgramPath"

#define REGKEY_KWSSERVICE       "SYSTEM\\CurrentControlSet\\Services\\Kingsoft Antivirus WebShield Service"

#define REGVALUE_ENABLE		     "en"             // 是否开启保护

#define KWS_SWITCHS				 "switchs"
#define KWS_SWITCHS_DEFVAL       0xFFFF
#define KWS_SWITCH_FISHING		 0x0001			  // 钓鱼开关
#define KWS_SWITCH_ABICON		 0x0002			  // 地址栏图标开关
#define KWS_SWITCH_UPREPORT		 0x0004			  // upreport开关
#define KWS_SWITCH_TIPSBANNER    0x0008           // IE提示广告过滤开关
#define KWS_SWITCH_PROBRO		 0x0010           // 浏览器锁定功能开关
#define KWS_SWITCH_TROJAN        0x0020           /* 木马开关 */
// 读不到值时默认其它开关开启但地址栏图标开关及提示广告过滤关闭
#define KWS_SWITCHS_DEFVAL       (0xFFFF & ~KWS_SWITCH_ABICON) 

#define KWS_SEARCHENGINE_BAIDU      0x0001
#define KWS_SEARCHENGINE_GOOGLE     0x0002
#define KWS_SEARCHENGINE_SOGOU      0x0004
#define KWS_SEARCHENGINE_BING       0x0008
#define KWS_SEARCHENGINE_SOSO       0x0010
#define KWS_SEARCHENGINE_YOUDAO     0x0020

#define REGVALUE_FIRSTRUN	     "FirstRun"       // 记录是否已经提示过用户网盾会闪框
#define REGVALUE_USETRAYICON     "usetrayicon"    // 是否启用托盘图标
#define REGVALUE_USETRAYICON_W    L"usetrayicon"    // 是否启用托盘图标
#define REGVALUE_SHOWBAIDUTIP	 L"ShowBaiduTip"		//根据是否是百度第一次启动而弹出相应的提醒框
#define REGVALUE_CLOSEBAIDUSHOWED	L"CloseBDShowed"	//关闭百度的告知窗口是否弹出
#define REGVALUE_SHINE           "enshine"        // 是否闪框
#define REGVALUE_COLOR_SAFE      "crsafe"         // 安全时闪框颜色
#define REGVALUE_COLOR_WARN      "crwarn"         // 警告时闪框颜色
#define REGVALUE_COLOR_DANGER    "crdanger"       // 危险时闪框颜色
#define REGVALUE_SHINELEVL       "shinelevel"     // 闪框时长

#define REGVALUE_SHIELDPROCESS   "shieldprocess"  // 用户自定义需要保护的进程 

#define UPDATE_PROGRAM_NAME		 "kwsupd.exe"

#define KWS_SETUP_CONFIG_FILE_NAME L"kwssetup.ini" // 安装完成后读取的配置文件
#define KWS_SETTING_FILE		L"kws.ini"
#define KWS_SETTING_KEY			L"setting"
#define KWS_UPDATE_KEY			L"update"

#define KWSVALUE_LASTUPDATETIME		L"LastUpdate"	// 最后一次升级时间
#define KWSVALUE_LASTFEEDBACKTIME		L"LastFeedback"	// 最后一次用户反馈时间

#define KWSVALUE_SETUP_TIME				L"setupTime"	// 安装时间
#define KWSVALUE_DANGER_COUNT		L"dangerCount"	// 威胁总数

#define KWSVALUE_HIDETRAYBALLOON		L"hideballoon"	// 保护程序时托盘闪动后是否提示	
#define KWSVALUE_SHOWTRAYSHINE			L"showtrayshine"	// 保护程序时托盘闪动

#define KWSVALUE_SCANED_KPP			L"scanedkpp"		// 是否扫描过密保

#define KWSVALUE_LASTURLQUERY			L"LastUrlQuery"		// 最后一次查询用户相关的被挂马url的时间

#define KWSVALUE_COLLECT			L"uic"		// 是否收集过用户信息

#define KWSVALUE_SEARCHENGINE       L"searchengine"

#define KWSVALUE_NEEDBLOCKDLG		L"needblockdlg"

#define KWSVALUE_LASTREPORTTIME     L"LastRep"

#define KWSVALUE_NEEDRCMKPP         L"needrcmkpp"  // 是否需要推荐密保
#define KWSVALUE_LASTRCMKPPTIME     L"lastrcmkpp"  // 最后推荐密保的时间

#define KWSVALUE_LASTCOLUSINTIME    L"lastcousin"  // 最后收集用户信息的时间

#define KWSVALUE_LASTCOLINFOCTIME	L"lastcolinfoc"		// 最后一次收集竞品的时间

#define KWS_BIND_CONFIG_FILE_NAME L"kws_bind.cfg" // 金山网盾带其它产品的绑定配置文件名

#define KWS_UPDATE_CONFIG L"update.cfg"		// 金山网盾升级配置文件

#define REGVALUE_VERSIONTYPE        "vt"		// 网盾版本类型
#define KWS_VT_STANDARD				0x00		// 独立版
#define KWS_VT_DUBA					0x04		// 毒霸版
#define KWS_VT_MAXTHON			    0x06		// 傲游版
#define KWS_VT_SOGOU				0x08		// 搜狗版

#define KWS_STARTPAGE				L"sp"			//锁定首页
#define KWS_LOCKSTARTPAGE			L"locksp"		//是否开启锁定首页
#define KWS_LOCKSPFIRSTFLAG			L"lockspfirstflag" //首页锁定后写这个值

#define KWS_ENABLE_DOWN_FILEAUTO_SCAN			L"auto_scan_downfile"	// 开启自动扫描
#define KWS_ENABLE_DOWN_FILEAUTO_CLEARVIRUS		L"auto_clean_virus"		// 开启自动清除病毒
#define KWS_SHOW_SAFE_POP						L"show_safe_pop"		// 显示安全的文件的泡泡
#define KWS_SHOW_GRAY_POP						L"show_gray_pop"		// 显示未知的文件的泡泡
#define KWS_DOWN_FILE_IMPORT_FLAG				L"down_file_import_flag"	// 导入文件标志
#define KWS_DOWN_NEED_SHOW_TIP_POP				L"show_tip_pop"			//是否显示tip泡泡
#define KWS_ENABLE_DOWNFILE_HELPER              L"auto_downfile_helper"  // 启用文件传输助手功能


#define KWS_DP						L"dp"		   // 是否开启下载保护
#define KWS_DLSOFT					L"dlsoft"      // 下载保护

#define KWS_HF						L"hfil"			// 网页净化

#define KWSVALUE_NEEDRCMLSP         L"needrcmlsp"  // 是否需要推荐首页保护

#define KWS_SVT						L"svt"			// 专版
#define KWS_SVTNAME					L"svtname"		// 专版名字
#define KWS_URLCHN					L"urlchn"		// url渠道
#define KWS_SVTURL					L"svturl"		// 专版url

#define KWS_SETUPTIME				L"SetupTime"	//安装时间
#define KWS_UNINTALLTIME			L"UninstallTime" //卸载时间
#define KWS_INSTALLSERVER			L"http://kwsdata.duba.net/wdreporter/Install"
#define KWS_UNINSTALLSERVER			L"http://kwsdata.duba.net/wdreporter/Uninstall"

#define KWS_UDFAILED_TIMES			L"failedtimes"
#define KWS_UDFAILED_TIME			L"failedtime"

#define KWS_MOUSEPRO				L"mousepro"				//鼠镖开关

#define KWS_BROWSERPRO				L"bspro"				//浏览器开关
#define KWS_BROWSERPATH				L"bspt"					//浏览器路径

#define KWSVALUE_NEEDRCMKSM			L"needrcmksm"			// 推荐使用急救箱修复
#define KWSVALUE_LASTRCMKSMTIME		L"lastrcmksm"		// 最后推荐急救箱的时间

#define KWSVALUE_LASTSCANANTISF     L"lastscanas"       //最后扫描是否安装杀软的时间

#define KWSVALUE_LASTFALSE_UPDATE	L"lastfalse"

#define KWSVALUE_LASTEEUTIME		L"lasteeu"			// 最后扫描恶意首页的时间

#define KWSADGUIDOVERSION           L"AdIdVer"           // 广告的Guid和version

#define KWSVALUE_SPMODIFYCOUNT      L"spmdcount"        //主页被改的次数

#define KWSVALUE_SHOW360SE			L"ssep"		// 360浏览器弹泡提示开关

#define KWSVALUE_RENAVIGATECOUNT    L"renv"     //主页被改后,网盾强转到主页的次数

#define KWSVALUE_MAXNOTIFY          L"maxntdis"    //MAXTHON浏览器锁定泡泡是否禁止

#define KWSVALUE_LOCKSPNOTIFY       L"lockspntdis" //推荐主页锁定泡泡是否禁止
#define KWSVALUE_ADV_URL_BLOCK_DATE _T("ubd")           // 广告链接被拦截的日期

#define KWS_TRAY_SHOW_CFGED			"iconConfiged"			//是否已经设置过将托盘图标显示在外面

#define KWSVALUE_LASTSCANSPNTTIME   L"lastscannt"   //上次推主页扫描弹泡的时间

#define	KWSVALUE_NEEDCHECKAD        L"checkad"			//是否第一次检查广告订阅

#define KWSVALUE_ENABLE_ANTIADVLOG  _T("EnableAntiAdvLog")


#define KWSVALUE_RCM_ADV_DATE       L"timercmadvf"  //最后次推荐广告过滤时间 
#define KWSVALUE_LAST_RECOMMEND     L"lastrecmd"   // 上次推荐广告过滤规则的时间

#define KWSVALUE_NEEDCSC			L"needcsc"			//是否需要修改快捷方式
#define KWSVALUE_NEEDFIXMXCFG		L"needfmxc"			//是否需要修改傲游版设置

#define KWS_SDBROWSERPATH				L"sdbspt"					//自定义浏览器路径

#define KWS_LOCKMAXSP               L"lockmaxsp" //锁定傲游主页
#define KWS_DISMAXLOCKPOP           L"dismaxlkpop" //不再弹提示傲游锁定主页泡泡
#define KWS_BSPPOP                  L"bsppop" //主页锁定拦截泡泡标识

#define	KWSVALUE_MINISITE			L"minisite"			//是否第一次显示mini向导
#define	KWSVALUE_MXMINISITE			L"mxminisite"			//是否第一次显示mxmini向导
#define	KWSVALUE_MXMINISITE2		L"mxminisite2"			//是否第一次显示mxmini2向导
#define	KWSVALUE_MINI36				L"mini36"			//是否第一次显示mini向导

#define KWSVALUE_ONEKEY				L"onekey"
#define KWSVALUE_ONEKEY_WAITTIME	L"onekeytime"

#define KWSVALUE_UPD_STATUS         L"updstatus"




#define KWS_CFG_AntiAd_IsEnable      L"kws.antiad.is_enable"
#define KWS_CFG_AntiAd_IsHaveAnyRule L"kws.antiad.is_have_any_rule"

#define KWSVALUE_NEED_AUTORUN_HIT    L"need_autorun_hit"


#endif // _PathDef_h_
