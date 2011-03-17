#pragma once

#include <common/utility.h>
#include <comproxy/comdata_scan_def.h>

#define CFG_VIRSCAN_SETTING     L"scansetting.ini"
#define CFG_KEY_SKYLARK             L"Skylark"
#define CFG_VALUE_JOIN_SKYLARK          L"JoinSkylark"
#define CFG_KEY_SCAN                L"Scan"
#define CFG_VALUE_SCAN_ARCHIVE          L"ScanArchive"
#define CFG_VALUE_SCAN_FILE_TYPE        L"ScanFileType"
#define CFG_VALUE_SCAN_AUTO_CLEAN       L"AutoClean"
#define CFG_VALUE_SCAN_EXT_FASTSCAN     L"ExtFastScan"
#define CFG_VALUE_SCAN_AUTO_EXTSCAN     L"AutoExtScan"

#define CFG_IEFIX_SETTING       L"iefixcfg.ini"
#define CFG_KEY_FIXLIST             L"fixlist"
#define CFG_VALUE_RIGHTMENU             L"rightmenu"
#define CFG_VALUE_TOOLBARICON           L"toolbaricon"
#define CFG_VALUE_THIRDTOOLBAR          L"thirdtoolbar"
#define CFG_VALUE_BHO                   L"bho"
#define CFG_VALUE_SAFEWEB               L"safeweb"
#define CFG_VALUE_PROTOCOL              L"protocol"
#define CFG_VALUE_SEARCHENG             L"searcheng"

#define CFG_BKSAFE_SETTING      L"bksafe.ini"
#define CFG_KEY_OPTION              L"option"
#define CFG_VALUE_AUTO_EXAM             L"autoexam"
#define CFG_VALUE_AUTO_ONCE_EXAM		L"autoexamonce"
#define CFG_VALUE_NO_TIP_RUN_DELETE		L"notiprundel"
#define CFG_VALUE_HAS_RUN_DELETE		L"hasrundel"
#define CFG_VALUE_KWS_NOUSE_CNT			L"kwsnousecnt"
#define CFG_KEY_PROXY				L"proxy"
#define CFG_VALUE_PROXY_TYPE			L"type"
#define CFG_VALUE_PROXY_HOST			L"host"
#define CFG_VALUE_PROXY_PORT			L"port"
#define CFG_VALUE_PROXY_VALIDATE		L"validate"
#define CFG_VALUE_PROXY_USER			L"user"
#define CFG_VALUE_PROXY_PASSWORD		L"password"
#define CFG_KEY_REPORT				L"report"
#define CFG_VALUE_REPORT_DESKTOP		L"desktop"
#define CFG_VALUE_REPORT_FIRSTSCAN		L"ft"


#define	CFG_BK_SOFTMGR_SETTING	L"bksoftmgr.ini"
#define CFG_KEY_SOFTMGR_DOWNLOAD	L"download"
#define CFG_VALUE_STORE_DIR				L"StoreDir"
#define CFG_VALUE_DEFAULT_DIR			L"DefaultDir"
#define CFG_VALUE_HINT_PLUGIN			L"HintPlugin"
#define CFG_KEY_SOFTMGR_INSTALL		L"install"
#define CFG_KEY_SOFTMGR_UPDATE2		L"update_tip"
#		define CFG_VALUE_SOFTMGR_UPDATE_NUM2	L"tip_count"
#		define CFG_VALUE_SOFTMGR_UPDATE_TIME2	L"tip_count_time"
#define CFG_KEY_SOFTMGR_UPDATE		L"update"
#define CFG_VALUE_SOFTMGR_UPDATE_DESKTOP_ICON		L"desktop_icon"
#define CFG_VALUE_SOFTMGR_UPDATE_DESKTOP_ICON_NUM	L"desktop_icon_num"
#define	CFG_VALUE_SOFTMGR_UPDATE_ICON_NUM			L"icon_num"
#define CFG_VALUE_SOFTMGR_UPDATE_NUM	L"updatecount"
#define CFG_VALUE_SOFTMGR_UPDATE_TIME	L"updatetime"
#define	CFG_VALUE_AUTO_INSTALL			L"AutoInstall"
#define	CFG_VALUE_DEL_FILE				L"DelFile"
#define CFG_KEY_SOFTMGR_POWERSWEEP	L"powersweep"
#define	CFG_VALUE_SWEEP_FILE			L"sweepfile"
#define CFG_KEY_SOFTMGR_SHOWCONDITION	L"showcondition"
#define	CFG_VALUE_SOFTMGR_SET_NOPLUG	L"noplug"
#define	CFG_VALUE_SOFTMGR_SET_FREE		L"free"
#define CFG_KEY_SOFTMGR_UNKNOWN		L"unknown"
#define CFG_VALUE_SOFTMGR_UN_OPEN		L"open"		

#define CFG_KEY_SOFTMGR_MAJOR_UPDATE_TIP	L"major_update_tip"
#define CFG_KEY_SOFTMGR_UPDATE_TIP		L"update_tip"
#define	CFG_VALUE_UPDATE_TIP			L"tip_type"
#define	CFG_VALUE_UPDATE_TIP_DELAY_TIME	L"tip_delay_second"
#define	CFG_VALUE_UPDATE_TIP_DELAY_TIME_FOR_AUTORUN	L"tip_delay_second_for_autorun"

#define CFG_VULFIX_FILENAME		_T("vulfix.ini")
#define CFG_VULFIX_SECTION_MAIN _T("main")


class BKSafeConfig
{
public:
    BKSafeConfig()
    {
    }

    ~BKSafeConfig()
    {
    }

	/// 软件管理设置 --------------------------------------------------------------------------
	
	static	void GetStoreDir( CString & strDir )
	{
		WCHAR szSystemPath[MAX_PATH] = {0};
		GetSystemDirectory(szSystemPath, MAX_PATH-1);
		CString strSysPath;
		strSysPath.Format(_T("%s"), szSystemPath);
		strSysPath = strSysPath.Left(3);
		strSysPath.Append( _T( "ksDownloads" ) );

		_LoadConfigStr( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_DOWNLOAD, CFG_VALUE_STORE_DIR, strDir, strSysPath );

	}

	static void SetStortDir( LPCTSTR szDir )
	{
		_SaveConfigStr( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_DOWNLOAD, CFG_VALUE_STORE_DIR, szDir );
	}

	static int	GetHintPlugin()
	{
		/*
		0 -- 不提示插件信息， 直接下载
		1 -- 提示插件信息
		*/
		int	nValue = 0;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_DOWNLOAD, CFG_VALUE_HINT_PLUGIN, nValue, 0 );

		return	nValue;
	}

	static void SetHintPlugin( int nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_DOWNLOAD, CFG_VALUE_HINT_PLUGIN, nValue );
	}

	static void SetShowNoPlugSet( int nValue )
	{
		/*
		0 -- 显示有插件的软件
		1 -- 不显示有插件的软件
		*/
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_SHOWCONDITION, CFG_VALUE_SOFTMGR_SET_NOPLUG, nValue );
	}

	static int GetShowNoPlugSet()
	{
		/*
		0 -- 显示有插件的软件
		1 -- 不显示有插件的软件
		*/
		int nValue = 0;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_SHOWCONDITION, CFG_VALUE_SOFTMGR_SET_NOPLUG, nValue, 0 );

		return nValue;
	}

	static void SetShowFreeSoftSet( int nValue )
	{
		/*
		0 -- 显示收费的软件
		1 -- 不显示收费的软件
		*/
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_SHOWCONDITION, CFG_VALUE_SOFTMGR_SET_FREE, nValue );
	}

	static int GetShowFreeSoftSet()
	{
		/*
		0 -- 显示有插件的软件
		1 -- 不显示有插件的软件
		*/
		int nValue = 0;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_SHOWCONDITION, CFG_VALUE_SOFTMGR_SET_FREE, nValue, 0 );

		return nValue;
	}

	static int	GetAutoInstall()
	{
		/*
		0 -- 下载完成后不自动开始安装
		1 -- 下载完成后自动开始安装
		*/
		int	nValue = 1;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_INSTALL, CFG_VALUE_AUTO_INSTALL, nValue, 1 );

		return	nValue;
	}

	static void	SetAutoInstall( int nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_INSTALL, CFG_VALUE_AUTO_INSTALL, nValue );
	}

	static int	GetPowerSweepFile()
	{
		/*
		0 -- 删除到回收站
		1 -- 彻底删除文件
		*/
		int	nValue = 1;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_POWERSWEEP, CFG_VALUE_SWEEP_FILE, nValue, 1 );	

		return	nValue;
	}

	static void	SetPowerSweepFile( int nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_POWERSWEEP, CFG_VALUE_SWEEP_FILE, nValue );
	}

	static int	GetUnKnownFileOpen()
	{
		/*
		0 -- 关闭未知文件对话框
		1 -- 开启未知文件对话框
		*/
		int	nValue = 1;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UNKNOWN, CFG_VALUE_SOFTMGR_UN_OPEN, nValue, 1 );	

		return	nValue;
	}

	static void	SetUnKnownFileOpen( int nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UNKNOWN, CFG_VALUE_SOFTMGR_UN_OPEN, nValue );
	}

	static int	GetDelFile()
	{
		/*
		0 -- 安装后安装文件一直保留
		1 -- 安装完成后立即删除安装文件
		2 -- 已下载的安装文件一周后自动删除
		*/
		int		nValue = 2;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_INSTALL, CFG_VALUE_DEL_FILE, nValue, 2 );
		
		return	nValue;
	}

	static void SetDelFile( int	nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_INSTALL, CFG_VALUE_DEL_FILE, nValue );
	}

	static int GetUpdatePointOutDelayTimeForAutoRun()
	{
		// 开机启动托盘后，弹泡提示软件升级的延迟时间
		// 默认为3分钟
		int		nValue = 1;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE_TIP, CFG_VALUE_UPDATE_TIP_DELAY_TIME_FOR_AUTORUN, nValue, 180 );

		return	nValue;
	}

	static int GetUpdatePointOutDelayTime()
	{
		// 手动启动托盘后，弹泡提示软件升级的延迟时间
		// 默认为1分钟
		int		nValue = 1;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE_TIP, CFG_VALUE_UPDATE_TIP_DELAY_TIME, nValue, 60 );

		return	nValue;
	}

	static void SetMajorUpdatePointOut( int nValue )
	{
		/*
		0 -- 不提醒重要软件更新
		1 -- 提醒重要软件更新
		*/
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_MAJOR_UPDATE_TIP, CFG_VALUE_UPDATE_TIP, nValue );
	}

	static int GetMajorUpdatePointOut()
	{
		/*
		0 -- 不提醒重要软件更新
		1 -- 提醒重要软件更新
		*/
		int nValue = 0;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_MAJOR_UPDATE_TIP, CFG_VALUE_UPDATE_TIP, nValue, 1 );

		return nValue;
	}

	static int	GetUpdatePointOut()
	{
		/*
		0 -- 每天在桌面提示一次可以升级的软件
		1 -- 每周在桌面提示一次可以升级的软件
		2 -- 不再显示软件升级的桌面提示
		*/
		int		nValue = 1;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE_TIP, CFG_VALUE_UPDATE_TIP, nValue, 1 );

		return	nValue;
	}

	static void SetUpdatePointOut( int	nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE_TIP, CFG_VALUE_UPDATE_TIP, nValue );
	}

	static int	GetDesktopIconNum()
	{
		int		nValue = 0;
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_ICON_NUM, nValue, 0 );

		return	nValue;
	}

	static void SetDesktopIconNum( int	nValue )
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_ICON_NUM, nValue );
	}

	static void SetSoftUpdateDesktopIcon(BOOL bIcon, BOOL bNum)
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_DESKTOP_ICON, bIcon );
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_DESKTOP_ICON_NUM, bNum );
	}

	static void GetSoftUpdateDesktopIcon(BOOL& bIcon, BOOL& bNum)
	{
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_DESKTOP_ICON, bIcon, 0 );
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_DESKTOP_ICON_NUM, (int&)bNum, 0 );
	}

	static void SetSoftUpdateCount(DWORD nNumber, DWORD nTime)
	{
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_NUM, nNumber );
		_SaveConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_TIME, nTime );
	}
	
	static void GetSoftUpdateCount(int& nNumber, DWORD & nTime)
	{
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_NUM, nNumber, 0 );
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE, CFG_VALUE_SOFTMGR_UPDATE_TIME, (int&)nTime, 0 );
	}

	static void GetSoftUpdateCount2(int& nNumber, DWORD & nTime)
	{
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE2, CFG_VALUE_SOFTMGR_UPDATE_NUM2, nNumber, 0 );
		_LoadConfig( CFG_BK_SOFTMGR_SETTING, CFG_KEY_SOFTMGR_UPDATE2, CFG_VALUE_SOFTMGR_UPDATE_TIME2, (int&)nTime, 0 );
	}

	/// end -----------------------------------------------------------------------------------

    static int GetAutoExam()
    {
		/*
		0 自动体检
		1 每天一次体检
		2 手动体检
		*/
        int nValue = 1;

        _LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_AUTO_EXAM, nValue, 1);

        return nValue;
    }

// 	static BOOL GetAutoExamOnce()
// 	{
// 		BOOL bValue = FALSE;
// 		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_AUTO_ONCE_EXAM, bValue, TRUE);
// 	}

    static void SetAutoExam(int nSet)
    {
        _SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_AUTO_EXAM, nSet);
    }

	//记录体检中未安装网盾次数
	static int GetKwsNoUseCnt()
	{
		int nValue = 0;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_KWS_NOUSE_CNT, nValue, 0);
		return nValue;
	}

	static void SetKwsNoUseCnt(int nSet)
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_KWS_NOUSE_CNT, nSet);
	}

// 	static void SetAutoExamOnce(int nSet)
// 	{
// 		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_AUTO_ONCE_EXAM, nSet);
// 	}

    static BOOL GetAutoReportUnknown()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SKYLARK, CFG_VALUE_JOIN_SKYLARK, bValue, TRUE);

        return bValue;
    }

    static void SetAutoReportUnknown(int nSet)
    {
        _SaveConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SKYLARK, CFG_VALUE_JOIN_SKYLARK, nSet);
    }

	static BOOL GetHasRunDelete()
	{
		BOOL bHas = FALSE;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_HAS_RUN_DELETE, bHas, FALSE);
		return bHas;
	}

	static void SetHasRunDelete(BOOL bHas)
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_HAS_RUN_DELETE, bHas);
	}

	static BOOL GetNoTipRunDelete()
	{
		BOOL bValue = FALSE;

		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_NO_TIP_RUN_DELETE, bValue, FALSE);

		return bValue;
	}

	static void SetNoTipRunDelete(BOOL bNoTip)
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_OPTION, CFG_VALUE_NO_TIP_RUN_DELETE, bNoTip);
	}


    static BOOL GetAutoClean()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_AUTO_CLEAN, bValue, FALSE);

        return bValue;
    }

    static void SetAutoClean(int nSet)
    {
        _SaveConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_AUTO_CLEAN, nSet);
    }

	static BOOL GetNeedMsgExtFastScan()
	{
		BOOL bValue = FALSE;

		_LoadConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_EXT_FASTSCAN, bValue, FALSE);

		return bValue;
	}

	static void SetAutoExpandScan( BOOL bExt )
	{
		_SaveConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_AUTO_EXTSCAN, bExt );
	}

	static BOOL GetAutoExpandScan()
	{
		BOOL bValue = FALSE;

		_LoadConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_AUTO_EXTSCAN, bValue, FALSE);

		return bValue;
	}

	static void SetNeedMsgExtFastScan( BOOL bExt )
	{
		_SaveConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_EXT_FASTSCAN, bExt );
	}

    static int GetVirScanFileType()
    {
        int nValue = FALSE;

        _LoadConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_FILE_TYPE, nValue, BkScanFileTypeExe);

        return nValue;
    }

    static void SetVirScanFileType(int nSet)
    {
        _SaveConfig(CFG_VIRSCAN_SETTING, CFG_KEY_SCAN, CFG_VALUE_SCAN_FILE_TYPE, nSet);
    }

    static int GetIEFixRightMenu()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_RIGHTMENU, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixRightMenu(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_RIGHTMENU, bSet);
    }

    static BOOL GetIEFixToolbarIcon()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_TOOLBARICON, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixToolbarIcon(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_TOOLBARICON, bSet);
    }

    static BOOL GetIEFixThirdToolbar()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_THIRDTOOLBAR, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixThirdToolbar(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_THIRDTOOLBAR, bSet);
    }

    static BOOL GetIEFixBHO()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_BHO, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixBHO(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_BHO, bSet);
    }

    static BOOL GetIEFixSafeWeb()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_SAFEWEB, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixSafeWeb(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_SAFEWEB, bSet);
    }

    static BOOL GetIEFixProtocol()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_PROTOCOL, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixProtocol(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_PROTOCOL, bSet);
    }

    static BOOL GetIEFixSearchEngine()
    {
        BOOL bValue = FALSE;

        _LoadConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_SEARCHENG, bValue, TRUE);

        return bValue;
    }

    static void SetIEFixSearchEngine(BOOL bSet = TRUE)
    {
        _SaveConfig(CFG_IEFIX_SETTING, CFG_KEY_FIXLIST, CFG_VALUE_SEARCHENG, bSet);
    }
	
	
	///////////////////////////////////////////////
	// 漏洞修复
	// 下载路径 
	static BOOL Get_Vulfix_IniFile( CString& strFileName )
	{
		return _GetConfigFileName(CFG_VULFIX_FILENAME, strFileName);
	}	
	
	static INT Get_Vulfix_ProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault )
	{
		CString strIniFile;
		INT nVal = nDefault;
		if( _GetConfigFileName(CFG_VULFIX_FILENAME, strIniFile) )
		{
			nVal = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, strIniFile );
		}
		return nVal;
	}
	
	static VOID Set_Vulfix_ProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nVal)
	{
		CString strIniFile;
		if( _GetConfigFileName(CFG_VULFIX_FILENAME, strIniFile) )
		{
			CString	str;
			str.Format(_T("%d"), nVal);
			WritePrivateProfileString(lpAppName, lpKeyName, str, strIniFile );
		}
	}
	
	static VOID Get_Vulfix_ProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, CString &strVal, LPCTSTR szDefault=NULL )
	{
		if(szDefault)
			strVal = szDefault;

		CString strIniFile;
		if( _GetConfigFileName(CFG_VULFIX_FILENAME, strIniFile) )
		{
			TCHAR buf[MAX_PATH] = {0};
			GetPrivateProfileString(lpAppName, lpKeyName, szDefault, buf, MAX_PATH-1, strIniFile );
			strVal = buf;
		}
	}
	
	static VOID Set_Vulfix_ProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR szVal)
	{
		CString strIniFile;
		if( _GetConfigFileName(CFG_VULFIX_FILENAME, strIniFile) )
		{
			WritePrivateProfileString(lpAppName, lpKeyName, szVal, strIniFile );
		}
	}
	
	static void Get_Vulfix_DownloadPath( CString &strPath )
	{
		TCHAR szPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		PathAppend(szPath, _T("..\\hotfix"));		
		return Get_Vulfix_ProfileString(_T("Main"), _T("downpath"), strPath, szPath);
	}

	static void Set_Vulfix_DownloadPath(LPCTSTR szPath)
	{
		return Set_Vulfix_ProfileString(_T("Main"), _T("downpath"), szPath);
	}

	static BOOL Get_Vulfix_SaveDownFile()
	{
		return Get_Vulfix_ProfileInt(_T("Main"), _T("savedown"), 1);
	}

	static VOID Set_Vulfix_SaveDownFile(BOOL bSave)
	{
		return Set_Vulfix_ProfileInt(_T("Main"), _T("savedown"), bSave);
	}

	static void Get_Vulfix_LastScanTime( CString &strTime )
	{
		return Get_Vulfix_ProfileString(_T("Main"), _T("lastscan"), strTime, NULL);
	}
	
	static void Set_Vulfix_LastScanTime(LPCTSTR szTime)
	{
		return Set_Vulfix_ProfileString(_T("Main"), _T("lastscan"), szTime);
	}
	
	// Vul - download config 
	static BOOL Get_Vulfix_Download_UseLocal()
	{
		return Get_Vulfix_ProfileInt(_T("download"), _T("use_local"), 0);
	}
	static BOOL Get_Vulfix_Download_DownloadIfNotExists()
	{
		return Get_Vulfix_ProfileInt(_T("download"), _T("down_if_not_exists"), 1);
	}
	static void Get_Vulfix_Download_LocalPath(CString &strLocalPath)
	{
		return Get_Vulfix_ProfileString(_T("download"), _T("local_path"), strLocalPath);
	}
	
	static void Get_MainDlg_LastCloseTab( CString &strTab )
	{
		CString strIniFile;
		if( _GetConfigFileName(CFG_BKSAFE_SETTING, strIniFile) )
		{
			TCHAR buf[64] = {0};
			GetPrivateProfileString(_T("Main"), _T("lastclosetab"), _T(""), buf, 63, strIniFile);
			strTab = buf;
		}
	}

	static void Set_MainDlg_LastCloseTab(LPCTSTR szTab)
	{
		CString strIniFile;
		if( _GetConfigFileName(CFG_BKSAFE_SETTING, strIniFile) )
		{
			WritePrivateProfileString(_T("Main"), _T("lastclosetab"), szTab, strIniFile );
		}
	}
	
	static BOOL Get_Protection_Trunoff_Warning()
	{
		BOOL bValue = FALSE;

		_LoadConfig(CFG_BKSAFE_SETTING, _T("ui_warning"), _T("protection_trunoff"), bValue, TRUE);

		return bValue;
	}

	static void Set_Protection_Trunoff_Warning(BOOL bWarning)
	{
		_SaveConfig(CFG_BKSAFE_SETTING, _T("ui_warning"), _T("protection_trunoff"), bWarning);
	}
	
	
	static BOOL Get_KwsProtection_Trunoff_Warning()
	{
		BOOL bValue = FALSE;

		_LoadConfig(CFG_BKSAFE_SETTING, _T("ui_warning"), _T("kws_protection_trunoff"), bValue, TRUE);

		return bValue;
	}

	static void Set_KwsProtection_Trunoff_Warning(BOOL bWarning)
	{
		_SaveConfig(CFG_BKSAFE_SETTING, _T("ui_warning"), _T("kws_protection_trunoff"), bWarning);
	}
	

	static BOOL GetProductDataPath(CString& strPath, BOOL bCreate = FALSE)
	{
		return CAppPath::Instance().GetLeidianDataPath(strPath, bCreate);
	}

	// 木马快速扫描最后一次提示重启时间 [8/5/2010 zhangbaoliang]
	static CString GetVirusScanRebootTime()
	{
		CString strTime(L"");
		_LoadConfigStr(CFG_BKSAFE_SETTING, _T("Main"), _T("last_reboot"), strTime, L"");
		return strTime;

	}
	
	static void SetVirusScanRebootTime(CString strTime)
	{
		_SaveConfigStr(CFG_BKSAFE_SETTING, L"Main", L"last_reboot", strTime);
	}
	// 获取代理信息 [1/11/2011 zhangbaoliang]

	static void GetProxyInfo(CString& strHost, CString& strPort, CString& strUser, CString& strPwd, int& nProxyType, int& nVerify)
	{
		strHost = strPort = strUser = strPwd = L"";
		nVerify = nProxyType = -1;
		//type
		_LoadConfig(CFG_BKSAFE_SETTING, _T("proxy"), _T("type"), nProxyType, -1);
		//host
		_LoadConfigStr(CFG_BKSAFE_SETTING, L"proxy", L"host", strHost, L"");
		//port
		_LoadConfigStr(CFG_BKSAFE_SETTING, L"proxy", L"port", strPort, L"");
		//verify
		_LoadConfig(CFG_BKSAFE_SETTING, _T("proxy"), _T("validate"), nVerify, -1);
		//user
		_LoadConfigStr(CFG_BKSAFE_SETTING, L"proxy", L"user", strUser, L"");
		//pwd
		_LoadConfigStr(CFG_BKSAFE_SETTING, L"proxy", L"password", strPwd, L"");
	}

//proxy 设置部分
	//type
	static int GetProxyType()
	{
		int nValue = 0;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_TYPE, nValue, 0);
		return nValue;
	}

	static void SetProxyType( int nType )
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_TYPE, nType );
	}

	//prot
	static int GetProxyPort()
	{
		int nValue = 0;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_PORT, nValue, 0);
		return nValue;
	}

	static void SetProxyPort( int nType )
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_PORT, nType );
	}

	//validate
	static int GetProxyValidate()
	{
		int nValue = 0;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_VALIDATE, nValue, 0);
		return nValue;
	}

	static void SetProxyValidate( BOOL bValidate )
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_VALIDATE, bValidate );
	}

	//host
	static CString GetProxyHost()
	{
		CString strHost(L"");
		_LoadConfigStr(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_HOST, strHost, L"");
		return strHost;
	}

	static void SetProxyHost(CString strHost)
	{
		_SaveConfigStr(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_HOST, strHost);
	}

	//user 
	static CString GetProxyUser()
	{
		CString strUser(L"");
		_LoadConfigStr(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_USER, strUser, L"");
		return strUser;
	}

	static void SetProxyUser(CString strUser)
	{
		_SaveConfigStr(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_USER, strUser);
	}

	//password 
	static CString GetProxyPassWord()
	{
		CString strPassWord(L"");
		_LoadConfigStr(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_PASSWORD, strPassWord, L"");
		return strPassWord;
	}

	static void SetProxyPassWord(CString strPassWord)
	{
		_SaveConfigStr(CFG_BKSAFE_SETTING, CFG_KEY_PROXY, CFG_VALUE_PROXY_PASSWORD, strPassWord);
	}

	//report desk info
	static int GetReportDeskInfo()
	{
		int nReport = 0;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_REPORT, CFG_VALUE_REPORT_DESKTOP, nReport, 0);
		return nReport;
	}

	static void SetReportDeskInfo( int nReport )
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_REPORT, CFG_VALUE_REPORT_DESKTOP, nReport );
	}

	//report first scan
	static int GetReportFirstScan()
	{
		int nFirstScan = 0;
		_LoadConfig(CFG_BKSAFE_SETTING, CFG_KEY_REPORT, CFG_VALUE_REPORT_FIRSTSCAN, nFirstScan, 1 );
		
		return nFirstScan;
	}

	static void SetReportFirstScan( int nFirstScan )
	{
		_SaveConfig(CFG_BKSAFE_SETTING, CFG_KEY_REPORT, CFG_VALUE_REPORT_FIRSTSCAN, nFirstScan );
	}

private:

    static BOOL _GetConfigFileName(LPCTSTR lpszCfgName, CString& strFileName)
    {
        HRESULT hRet = CAppPath::Instance().GetLeidianCfgPath(strFileName);

        if (FAILED(hRet))
            return FALSE;

        strFileName += L'\\';
        strFileName += lpszCfgName;

		::SetFileAttributes(strFileName, FILE_ATTRIBUTE_NORMAL);

        return TRUE;
    }

    static BOOL _LoadConfig(LPCTSTR lpszCfgName, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, int &nValue, int nDefault)
    {
        CString strPath;

        BOOL bRet = _GetConfigFileName(lpszCfgName, strPath);
        if (!bRet)
            return FALSE;

        nValue = (int)::GetPrivateProfileInt(lpszKeyName, lpszValueName, nDefault, strPath);
        
        return TRUE;
    }

	static BOOL _LoadConfigStr(LPCTSTR lpszCfgName, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, CString &strVal, LPCTSTR szDefault=NULL )
	{
		if(szDefault)
			strVal = szDefault;

		CString strPath;
		BOOL bRet = _GetConfigFileName(lpszCfgName, strPath);
		if (!bRet)
			return FALSE;

		TCHAR buf[MAX_PATH] = {0};
		GetPrivateProfileString(lpszKeyName, lpszValueName, szDefault, buf, MAX_PATH-1, strPath );
		strVal = buf;

		return TRUE;
	}


    static BOOL _SaveConfig(LPCTSTR lpszCfgName, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, int nValue)
    {
        CString strPath, strValue;

        BOOL bRet = _GetConfigFileName(lpszCfgName, strPath);
        if (!bRet)
            return FALSE;

        strValue.Format(L"%d", nValue);

        return ::WritePrivateProfileString(lpszKeyName, lpszValueName, strValue, strPath);
    }

	static BOOL _SaveConfigStr(LPCTSTR lpszCfgName, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, LPCTSTR szValue)
	{
		CString strPath;

		BOOL bRet = _GetConfigFileName(lpszCfgName, strPath);
		if (!bRet)
			return FALSE;

		return ::WritePrivateProfileString(lpszKeyName, lpszValueName, szValue, strPath);
	}


};