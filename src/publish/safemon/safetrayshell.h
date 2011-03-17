// dummyz@126.com
#pragma once

#include <atltime.h>
#include <atlcoll.h>
#include "safemonitor.h"
#include "miniutil/bkjson.h"
#include "communits/CommFuns.h"
#include "common/utility.h"
#include "kws/KPluginLoader.h"
#include "kws/KwsSetting.h"
#include "kws/KwsSettingOld.h"
#include "kws/simpleipc/KwsIPCWarpperOld.h"
#include "kws/KwsCmptComm.h"

#define		SM_CONFIG_KEY_OPTION						_T("option")
#define		SM_CONFIG_OPTION_PROCESS_MONITOR			_T("process_monitor")
#define		SM_CONFIG_OPTION_RISK____MONITOR			_T("risk_monitor")
#define		SM_CONFIG_OPTION_UDISK___MONITOR			_T("udisk_monitor")
#define		SM_CONFIG_OPTION_LEAK____MONITOR			_T("leak_monitor")
#define     SM_CONFIG_OPTION_BROWSER_MONITOR			_T("browser_monitor")
#define		SM_CONFIG_OPTION_BROWSER_MOD_FISHING		_T("browser_anti_fishing")
#define		SM_CONFIG_OPTION_BROWSER_MOD_MALICIOUS		_T("browser_anti_malicious")
#define		SM_CONFIG_OPTION_KWS_MONITOR				_T("kws_monitor")

class CSafeMonitorTrayShell
{
public:
	CSafeMonitorTrayShell()
	{
		m_bWin64 = IsWin64();

		InitializeCriticalSection(&m_Lock);

		memset(&m_wfadIni, 0, sizeof (m_wfadIni));
	}

	~CSafeMonitorTrayShell(void) 
	{
		DeleteCriticalSection(&m_Lock);
	}

	BOOL IsKwsStdExist()
	{
		BOOL bRet = FALSE;
		int nType = 0;

		return bRet;
	}


	// 监控开关
	// dwMonitorId at [SafeMonitor.h] SM_ID_XX
	BOOL	GetMonitorRun(DWORD dwMonitorID)// const 
	{
		BOOL bRun = FALSE;
		switch ( dwMonitorID )
		{
		case SM_ID_PROCESS:
			/*if ( m_bWin64 )
			{
				bRun = FALSE;
			}
			else*/
			{
				bRun = (BOOL)GetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_PROCESS_MONITOR, FALSE);
			}
			break;

		case SM_ID_RISK:
			bRun = (BOOL)GetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_RISK____MONITOR, FALSE);
			break;

		case SM_ID_UDISK:
			bRun = (BOOL)GetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_UDISK___MONITOR, FALSE);
			break;

		case SM_ID_LEAK:
			bRun = (BOOL)GetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_LEAK____MONITOR, FALSE);
			break;

		case SM_ID_KWS:
			{
				if (IsKwsStdExist())
				{
					kws_old_ipc::KwsSetting setting;
					bRun = setting.IsKwsEnable();
				}
				else
				{
					KwsSetting setting;
					bRun = setting.IsAntiTrojanEnable();
				}
			}
			break;

		case SM_ID_KWS_SAFE_DOWN:
			{
				if (IsKwsStdExist())
				{
					kws_old_ipc::KwsSetting setting;
					bRun = setting.IsAutoScanEnable();
				}
				else
				{
					KwsSetting setting;
					bRun = setting.IsAutoScanEnable();
				}
			}
			break;

        default:
            ATLASSERT(FALSE);
		}
		
		return bRun;
	}

	DWORD	GetMonitorCount()
	{
		return 6; // 增加保护注意修改这个值
	}

	DWORD	GetMonitorRunCount()
	{
		int t = 0;

		if ( GetMonitorRun(SM_ID_LEAK) )            t++;
		if ( GetMonitorRun(SM_ID_PROCESS) )         t++;
		if ( GetMonitorRun(SM_ID_RISK) )            t++;
		if ( GetMonitorRun(SM_ID_UDISK) )           t++;
		if ( GetMonitorRun(SM_ID_KWS) )           t++;
		if ( GetMonitorRun(SM_ID_KWS_SAFE_DOWN) ) t++;
		
		return t;
	}

	//实时保护的个数
	DWORD GetSafeMonitorCount()
	{
		return 4;
	}

	//实时保护开启个数
	DWORD GetSafeMonitorRunCount()
	{
		int t = 0;

		if ( GetMonitorRun(SM_ID_LEAK) )            t++;
		if ( GetMonitorRun(SM_ID_PROCESS) )         t++;
		if ( GetMonitorRun(SM_ID_RISK) )            t++;
		if ( GetMonitorRun(SM_ID_UDISK) )           t++;

		return t;

	}

	BOOL	HaveAnyMonitorRun()
	{
		return GetMonitorRun(SM_ID_LEAK) ||
			GetMonitorRun(SM_ID_PROCESS) ||
			GetMonitorRun(SM_ID_RISK) ||
			GetMonitorRun(SM_ID_UDISK) ||
			GetMonitorRun(SM_ID_KWS) ||
			GetMonitorRun(SM_ID_KWS_SAFE_DOWN);
	}

	BOOL IsAllMonitorRun()
	{
		if ( !GetMonitorRun(SM_ID_LEAK) ||
			!GetMonitorRun(SM_ID_PROCESS) ||
			!GetMonitorRun(SM_ID_RISK) ||
			!GetMonitorRun(SM_ID_UDISK)
			
			)
		{
			return FALSE;
		}

		return TRUE;
	}

   BOOL _IsAllMonitorRun()
   {
       if (!GetMonitorRun(SM_ID_KWS) ||
           !GetMonitorRun(SM_ID_KWS_SAFE_DOWN))
       {
           return FALSE;
       }

       return TRUE;

   }

	void	SetAllMonitorRun(BOOL bRun)
	{
		SetMonitorRun(SM_ID_RISK,            bRun);
		SetMonitorRun(SM_ID_LEAK,            bRun);
		SetMonitorRun(SM_ID_PROCESS,         bRun);
		SetMonitorRun(SM_ID_UDISK,           bRun);		
	}

    void _SetAllMonitorRun(BOOL bRun)
    {
        SetMonitorRun(SM_ID_KWS,			 bRun);
        SetMonitorRun(SM_ID_KWS_SAFE_DOWN,	 bRun);
    }
	
	void	SetMonitorRun(DWORD dwMonitorID, BOOL bRun) 
	{
		switch ( dwMonitorID )
		{
		case SM_ID_PROCESS:
			/*if ( m_bWin64 )
			{
				bRun = FALSE;
			}*/
			SetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_PROCESS_MONITOR, bRun);
			break;

		case SM_ID_RISK:
			SetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_RISK____MONITOR, bRun);
			break;

		case SM_ID_UDISK:
			SetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_UDISK___MONITOR, bRun);
			break;

		case SM_ID_LEAK:
			SetValue(SM_CONFIG_KEY_OPTION, SM_CONFIG_OPTION_LEAK____MONITOR, bRun);
			break;

		case SM_ID_KWS:
			{
				if (IsKwsStdExist())
				{
					kws_old_ipc::KwsSetting setting;
					setting.SetKwsEnable(true);
				}
				else
				{
					KwsSetting setting;
					setting.SetAntiTrojanEnable(bRun);
				}
			}
			break;

		case SM_ID_KWS_SAFE_DOWN:
			{
			}
			break;

        default:
            ATLASSERT(FALSE);
		}

		_NotifyConfigUpdated();
	}

	
	BOOL    GetBwspAnyModRun()
	{
		BOOL bRun = FALSE;

		bRun = (GetMonitorRun(SM_ID_KWS) || GetMonitorRun(SM_ID_KWS_SAFE_DOWN));

		return bRun;
	}

	BOOL    GetBwspAllModRun()
	{
		BOOL bRun = FALSE;

		bRun = GetMonitorRun(SM_ID_KWS) && GetMonitorRun(SM_ID_KWS_SAFE_DOWN);

		return bRun;
	}

	// 设置开机体检
	void	SetStartupCheckup(BOOL bRun)
	{
	}

	BOOL	GetStartupCheckup()
	{
		BOOL bCheckup = FALSE;
		return bCheckup;
	}

	DWORD	GetStartupMonitorRunDelayTime() // 获取开机时延时启动监控的时间, 单位分钟
	{
		DWORD dwResult = 2;
		return min(dwResult, 5);
	}

	static void WhiteListLibUpdated() // 通知 tray 白名单更新
	{
	}

	static void RunOptDelHisUpdated() // 通知 tray 启动项删除历史文件更新
	{
	}

	static void KwsUserProtectionProcessUpdated()
	{
	}

	DWORD	GetUpdateMethod()
	{
		// 0 自动升级
		// 1 是询问升级
		DWORD dwValue = 1;
		return dwValue;
	}

	void	SetUpdateMethod(DWORD dwMethod)
	{
	}

	void	SetAutorunService(BOOL bOn)
	{
		SC_HANDLE hScManager = OpenSCManager(NULL, NULL, GENERIC_WRITE);
		if ( hScManager != NULL )
		{
			SC_HANDLE hService = OpenService(hScManager, _T("KSafeSvc"), GENERIC_WRITE);
			if ( hService != NULL )
			{
				DWORD dwStartType = (bOn ? SERVICE_AUTO_START : SERVICE_DEMAND_START);

				ChangeServiceConfig(hService, 
					SERVICE_NO_CHANGE,
					dwStartType,
					SERVICE_NO_CHANGE,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
					);

				CloseServiceHandle(hService);
				hService = NULL;
			}

			CloseServiceHandle(hScManager);
			hScManager = NULL;
		}
	}

	/** 
	*  设置开机运行tray (修改自已的启动键)
	*/
	void SetAutorunTrayInner(BOOL bOn)
	{
		CRegKey regKey;
		LONG lResult;

		std::wstring strTrayName = _T("KSafeTray");
		KisPublic* kispub = KisPublic::Instance();

		if (kispub->Init())
		{
			if (!kispub->KQueryOEMLPCWSTR(KIS::KOemKsfe::oemc_RunTrayName, strTrayName))
				strTrayName = _T("KSafeTray");
		}

		if (strTrayName.size() == 0)
			strTrayName = _T("KSafeTray");

		lResult = regKey.Create(HKEY_LOCAL_MACHINE, 
			_T("Software\\KSafe\\run\\Disable\\hklm"), 
			NULL, 
			NULL, 
			KEY_WRITE);

		if (lResult == ERROR_SUCCESS)
		{
			if (bOn)
			{
				CString strCmdLine;

				CAppPath::Instance().GetLeidianAppPath(strCmdLine);
				strCmdLine.Insert(0, _T('\"'));
				strCmdLine.Append(_T("\\KSafeTray.exe\" -autorun"));

				regKey.SetStringValue(strTrayName.c_str(), strCmdLine);
			}
			else
			{
				regKey.DeleteValue(strTrayName.c_str());
			}
		}
	}

	// 设置开机运行tray
	void	SetAutorunTray(BOOL bOn)
	{
		//
		CRegKey reg;
		LONG lResult;

		std::wstring strTrayName = _T("KSafeTray");

		if (KisPublic::Instance()->Init())
		{
			if (!KisPublic::Instance()->KQueryOEMLPCWSTR(KIS::KOemKsfe::oemc_RunTrayName, strTrayName))
				strTrayName = _T("KSafeTray");
		}

		if (strTrayName.size() <= 0)
			strTrayName = _T("KSafeTray");

		lResult = reg.Create(HKEY_LOCAL_MACHINE, 
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
			NULL, 
			NULL, 
			KEY_WRITE
			);
		if ( lResult == ERROR_SUCCESS )
		{
			if ( bOn )
			{
				CString strCmdLine;

				CAppPath::Instance().GetLeidianAppPath(strCmdLine);
				strCmdLine.Insert(0, _T('\"'));
				strCmdLine.Append(_T("\\KSafeTray.exe\" -autorun"));

				reg.SetStringValue(strTrayName.c_str(), strCmdLine);
			}
			else
			{
				reg.DeleteValue(strTrayName.c_str());
			}
		}

		if (bOn)  
		{
			 /* 打开开机启动时删除自已的启动键 */
			SetAutorunTrayInner(FALSE); 
		}
		else
		{
			/* 关闭开机启动后，当再次启动主程序时需要提示是否开启 */

			KwsSetting setting;
			setting.SetNeedAutorunHit(TRUE);  
		}

		SetAutorunService(bOn);
	}

	BOOL	GetAutorunTray()
	{
		BOOL bResult = FALSE;
		CRegKey reg;
		LONG lResult;

		lResult = reg.Open(HKEY_LOCAL_MACHINE, 
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
			KEY_READ
			);
		if ( lResult == ERROR_SUCCESS )
		{
			CString strCmdLine;
			DWORD dwCmdLineLen = MAX_PATH + 100;

			std::wstring strTrayName = _T("KSafeTray");
			if (KisPublic::Instance()->Init())
			{
				if (!KisPublic::Instance()->KQueryOEMLPCWSTR(KIS::KOemKsfe::oemc_RunTrayName, strTrayName))
					strTrayName = _T("KSafeTray");
			}

			if (strTrayName.size() <= 0)
				strTrayName = _T("KSafeTray");

			lResult = reg.QueryStringValue(strTrayName.c_str(), strCmdLine.GetBuffer(dwCmdLineLen), &dwCmdLineLen);
			strCmdLine.ReleaseBuffer();

			if ( lResult == ERROR_SUCCESS )
			{
				CString strRightCmdLine;

				CAppPath::Instance().GetLeidianAppPath(strRightCmdLine);
				strRightCmdLine.Insert(0, _T('\"'));
				strRightCmdLine.Append(_T("\\KSafeTray.exe\" -autorun"));

				if ( strRightCmdLine.CompareNoCase(strCmdLine) == 0 )
				{
					bResult = TRUE;
				}
				else
				{
					CRegKey regWrite;

					lResult = regWrite.Create(HKEY_LOCAL_MACHINE, 
						_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
						NULL, 
						NULL, 
						KEY_WRITE
						);
					if ( lResult == ERROR_SUCCESS )
					{
						regWrite.DeleteValue(strTrayName.c_str());
					}
				}
			}
		}

		return bResult;
	}

	BOOL KSafeServerIsOK()
	{
		BOOL fResult = FALSE;
		SC_HANDLE hSCM = NULL, hService = NULL;
		SERVICE_STATUS ss = {0}; 

		hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (hSCM != NULL)
		{
			hService = ::OpenService(hSCM, L"KSafeSvc", SERVICE_QUERY_STATUS);

			if (hService != NULL)
			{
				if (::QueryServiceStatus(hService, &ss))
				{
					if (SERVICE_RUNNING == ss.dwCurrentState)
						fResult = TRUE;		
				}
				::CloseServiceHandle(hService);
			}
			::CloseServiceHandle(hSCM);
		}
		return fResult;
	}

	void	EnableUDiskAutorun(BOOL bAutorun)
	{
	}

	// 游戏免打扰模式
	BOOL	GetGameBypass()
	{
		BOOL bValue = TRUE;
		return bValue;
	}

	void	SetGameBypass(BOOL bOn) 
	{
	}

	BOOL	GetSafeBypass() 
	{
		BOOL bValue = false;
		return bValue;
	}

	void	SetSafeBypass(BOOL bOn)
	{
	}

	void	SetDefaultHomepage(LPCTSTR lpURL)
	{
	}

	CString GetDefaultHomepage()
	{
		CString strHomepage;

		return strHomepage;
	}

	void SetDefaultBrowser(LPCTSTR lpPath)
	{
	}

	CString GetDefaultBrowser()
	{
		CString strBrowser;

		return strBrowser;
	}

	void AddTrustRegPath(LPCTSTR lpRegPath)
	{
	}

	static void AddTrustFilePath(LPCTSTR lpFilePath)
	{
	}

	static void NotifySafeMonitorCtrlUpdated()
	{
	}

	void SetTrayTipInfo(LPCTSTR lpText, DWORD dwTimeout, BOOL bDelayShow)
	{
	}

	void ExitTray(BOOL bForce, BOOL bKillMainApp)
	{
	}

    // WPARAM 升级类型 enum
    // LPARAM 进度 -1失败 0-100进度
	void RunUpdater()
	{
	}

	void SetUpdateObserver(DWORD dwThreadId, DWORD dwMsg)
	{
	}

	BOOL CheckNewVersion(CString& strNewVersion)
	{
		BOOL bCanUpdate = FALSE;

		return bCanUpdate;
	}

	//
	void SetProperty(LPCTSTR lpName, LPCTSTR lpValue)
	{
	}

	void SetFunctionResult(LPCTSTR lpFun, LPCTSTR lpResult)
	{
	}

	void UpdateTest(int nCode)
	{
	}

	void ForceExit()
	{
	}

	void CallUpdate(int nCode)
	{
	}

	static HWND	 GetWnd()
	{
		CString strWndClassName = _T("{5469F950-888A-4bc1-B0B4-72F0159D7ACD}");
		std::wstring strValue;

		if (KisPublic::Instance()->Init())
		{
			int nRet = KisPublic::Instance()->KQueryOEMLPCWSTR(KIS::KOemKsfe::oemc_OtherCheckName, strValue);
			if (nRet)
				strWndClassName.Append(strValue.c_str());

		}

		return FindWindow(strWndClassName, NULL);
	}
	static HWND GetUploadTrayWnd()
	{
		CString strWndClassName = _T("{A67F8456-B51D-4abc-8948-A50706E61E69}");
		std::wstring strValue;

		if (KisPublic::Instance()->Init())
		{
			int nRet = KisPublic::Instance()->KQueryOEMLPCWSTR(KIS::KOemKsfe::oemc_OtherCheckName, strValue);
			if (nRet)
				strWndClassName.Append(strValue.c_str());
		}

		return FindWindow(strWndClassName, NULL);

	}

	void SetUnknownFilesFeedBackLastTime(ULONGLONG ullTime = 0)
	{
	}

	ULONGLONG GetUnknownFilesFeedbackLastTime()
	{
		TCHAR	szBuffer[MAX_PATH] = {0};

		return (ULONGLONG)_ttoi64(szBuffer);
	}

	DWORD GetUnknownFilesFeedbackDiff()
	{
		DWORD dwDefault = 30 * 60;
		return dwDefault;
	}

	// 关键位置保护隔多久做一次全面快照
	__time64_t	GetRiskMonitorSkipXXScanTime()
	{
		TCHAR szValue[MAX_PATH] = {0};
		__time64_t t = 0;
		_stscanf(szValue, _T("%I64X"), &t);

		return t;
	}

	void	SetRiskMonitorSkipXXScanTime(__time64_t t)
	{
	}

	// 扫描服务超时计数
	DWORD	GetScanServerClientTimeoutCount()
	{
		DWORD dwResult = 0;
		return dwResult;
	}

	void	SetScanServerClientTimeoutCount(DWORD dwCount)
	{
	}

	void ShowBalloon( LPCTSTR lpTitle, LPCTSTR lpInfo, UINT uTimeout )
	{// 
	}

	// riskmonitor 自动放行 安全、未知
	void EnableRiskMonitorSafeBypass(BOOL bEnable)
	{
	}

	static void NotifyUploadFileStart()
	{
	}

	static void NotifyConfigUpdated()
	{
		_NotifyConfigUpdated();
	}

private:
	static CString EscapeString(LPCTSTR lpStr)
	{
		CString strPath(lpStr);

		strPath.Replace(_T("\\"), _T("\\\\"));
		strPath.Replace(_T("\""), _T("\\\""));

		return strPath;
	}

	static DWORD CallFunction(LPCTSTR lpJsonFun, DWORD dwTimeout = 2000)
	{
		DWORD dwResult = 0;
		return dwResult;
	}

	void GetFunctionResult(LPCTSTR lpFun, LPTSTR lpResult, DWORD dwLen)
	{
	}

	static void _NotifyConfigUpdated()
	{
	}

	void SetValue(LPCTSTR lpKey, LPCTSTR lpName, LPCTSTR lpValue)
	{
	}

	void SetValue(LPCTSTR lpKey, LPCTSTR lpName, DWORD dwValue)
	{
	}

	LPCTSTR GetValue(LPCTSTR lpKey, LPCTSTR lpName, LPTSTR lpValue, DWORD dwLen, LPCTSTR lpDefault)
	{
		if ( lpValue != NULL && dwLen != 0 )
		{
			*lpValue = 0;
		}

		return lpValue;
	}

	DWORD GetValue(LPCTSTR lpKey, LPCTSTR lpName, DWORD dwDefault)
	{
		int nValue = 0;
		return nValue;

	}

	BOOL IsModified()
	{

		return FALSE;
	}

private:


	static BOOL IsWin64()
	{
		BOOL bRet = FALSE;
		static HMODULE hKernel32 = NULL;
		if( !hKernel32 )
			hKernel32 = ::LoadLibrary(TEXT("Kernel32.DLL"));
		if( !hKernel32 )
			return FALSE;

		typedef BOOL (WINAPI *FunctionIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);
		FunctionIsWow64Process pfnIsWow64Process = NULL;
		pfnIsWow64Process = (FunctionIsWow64Process)GetProcAddress(hKernel32, "IsWow64Process");

		if (NULL == pfnIsWow64Process)
			return FALSE;

		HANDLE hCurrentProcess = GetCurrentProcess();
		pfnIsWow64Process(hCurrentProcess, &bRet);
		return bRet;
	}

private:
	BOOL							m_bWin64;
	CString							m_strIniPath;
	CString							m_strTrayIniPath;
	CString							m_strFunPath;
	CRITICAL_SECTION				m_Lock;
	WIN32_FILE_ATTRIBUTE_DATA		m_wfadIni;
	CAtlMap<CString, CString>		m_strValueMap;
};
