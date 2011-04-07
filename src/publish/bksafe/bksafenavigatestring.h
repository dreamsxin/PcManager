#pragma once

#define BKSFNS_TRAYSTART                        L"TrayStart"
#define BKSFNS_UI                               L"UI"
#   define BKSFNS_PAGE_EXAM                     L"Exam"
#		define BKSFNS_FRCE_STARTEXAM			L"ForceExam"
#		define BKSFNS_STOPEXAM					L"stopexam"
#   define BKSFNS_PAGE_VIRSCAN                  L"VirScan"
#       define BKSFNS_VIRSCAN_SHOW              L"ShowVirScan"
#       define BKSFNS_VIRSCAN_FAST              L"Fast"
#       define BKSFNS_VIRSCAN_FULL              L"Full"
#       define BKSFNS_VIRSCAN_CUSTOM            L"Custom"
#       define BKSFNS_VIRSCAN_SHOW_REPORT_LOG   L"ReportLog"
#		define BKSFNS_VIRSCAN_FREE				L"FreeVirus" 
#		define BKSFNS_VIRSCAN_SYSFIX			L"SysFix" 
#		define BKSFNS_VIRSCAN_PLUG				L"TrjPlug" 
#   define BKSFNS_PAGE_LEAKSCAN                 L"LeakScan"
#		define BKSFNS_LEAKSCAN_FIX_FROMLIB		L"fixfromlib"	//泡泡上点击“立即修复”，直接进入修复流程
#   define BKSFNS_PAGE_LEAKSCAN2                L"LeakScan2"	// Require Rescan 
#   define BKSFNS_PAGE_FREEVIRUS                L"PageFreeVirus"
#   define BKSFNS_PAGE_PLUGINSCAN               L"PluginScan"
#       define BKSFNS_PLUGINSCAN_SCAN           L"Scan"
#   define BKSFNS_PAGE_PROTECTION               L"Protection"
#		define BKSFNS_PAGE_PROTECTION_COMMON	L"Common"
#		define BKSFNS_PAGE_PROTECTION_BANDT		L"BlockAndTrust"
#		define BKSFNS_PAGE_PROTECTION_HISTORY	L"History"
#   define BKSFNS_PAGE_IEFIX                    L"IEFix"
#		define BKSFNS_PAGE_IEFIX1				L"IEFix1"
#		define BKSFNS_PAGE_SAFEMON				L"IEFixSm"
#		define BKSFNS_PAGE_IESET				L"IESet"
#		define BKSFNS_PAGE_IECLEAN				L"IEClean"
#		define BKSFNS_PAGE_IESAFE				L"IESafe"
#		define BKSFNS_PAGE_BWS					L"BWS"
#		define BKSFNS_PAGE_BWS_HISTORY			L"BWSHistory"
#		define BKSFNS_PAGE_KWS					L"KWS"
#		define BKSFNS_PAGE_BLOCKLOG             L"kwsblocklogui"
#          define BKSFNS_TAB_FS                 L"flish"
#          define BKSFNS_TAB_AD                 L"advert"
#          define BKSFNS_TAB_TJ                 L"trojan"
#          define BKSFNS_TAB_SM                 L"safemonlog"
#   define BKSFNS_PAGE_SYSTEM_OPTIMIZE          L"SysOpt"
#   define BKSFNS_PAGE_SYSTEM_CLEAR	            L"KClear"
#		define BKSFNS_SYSOPT_CLR_REG			L"ClrReg"
#		define BKSFNS_SYSOPT_CLR_HENJI			L"ClrHenji"
#		define BKSFNS_SYSOPT_CLR_RUBBISH		L"ClrRub"
#		define BKSFNS_SYSOPT_RUN_OPT			L"RunOpt"
#		define BKSFNS_SYSOPT_ONEKEY_RUN_OPT		L"OneKeyRunOpt"
#		define BKSFNS_SYSOPT_SETTING			L"SysoptSetting"
#       define BKSFNS_SYSOPT_CLR_ONEKEY			L"ClrOnekey"
#		define BKSFNS_SYSOPT_CLR_SHOUSHEN		L"ClrShoushen"
#       define BKSFNS_SYSOPT_BIG_FILE           L"BigFile"
#		define BKSFNS_SYSOPT_ONEKEY_RUN_OPT		L"OneKeyRunOpt"
#		define BKSFNS_SYSOPT_NET_MONITOR		L"NetMonitor"
#		define BKSFNS_SYSOPT_SYSTOOLS			L"KSafeTools"
#		define BKSFNS_SYSOPT_PERFOPT			L"KSafePerfOpt"
/*#		define BKSFNS_SYSOPT_SETTING			L"SysoptSetting"*/
#   define BKSFNS_SETTING                       L"Setting"
#       define BKSFNS_SETTING_PAGE_COMMON       L"Common"
#       define BKSFNS_SETTING_PAGE_VIRSCAN      L"VirScan"
#       define BKSFNS_SETTING_PAGE_VULSCAN      L"VulScan"
#       define BKSFNS_SETTING_PAGE_PROTECTION   L"Protection"
#       define BKSFNS_SETTING_PAGE_IEFIX        L"IEFix"
#   define BKSFNS_SETTING_CHANGE                L"SettingChange"
#   define BKSFNS_ECHO_FAILED                   L"EchoFail"
#   define BKSFNS_AVE_INSTALL_FINISH            L"avok"
#   define BKSFNS_SOFTMGR                       L"SoftMgr"
#		define	BKSFNS_SOFTMGR_PAGE_ZJBB		L"SoftBibei"
#		define	BKSFNS_SOFTMGR_PAGE_DAQUAN		L"SoftDaquan"
#		define	BKSFNS_SOFTMGR_PAGE_UNINSTALL	L"SoftUninstall"
#		define	BKSFNS_SOFTMGR_PAGE_UPDATE		L"SoftUpdate"
#		define	BKSFNS_SOFTMGR_PAGE_HOTGAME		L"SoftHotGame"
#	define BKSFNS_KWS							L"kws"
#		define BKSFNS_KWS_SETTING				L"kwssetting"
#       define BKSFNS_KWS_DLSUI                 L"kwsdownloadsafeui"
/* #    define BKSFNS_KWS_BLOCKLOG              L"kwsblocklogui" */
#define BKSFNS_URL                              L"URL"
#define BKSFNS_JOIN_SKYLARK                     L"JoinSkylark"
#define BKSFNS_QUIT                             L"{B2C96360-9354-4686-A04B-0F9870D892C9}"
//hub  
#		define BKSFNS_SETTING_PAGE_SYSOPT		L"SysOpt"  //设置页面中的系统优化页面
#		define BKSFNS_SETTING_PAGE_KWS			L"SysKws"
#		define BKSFNS_SETTING_PAGE_SOFTMGR		L"SoftMgr"
#			define BKSFNS_SETTING_PAGE_SOFTMGR_UPDATE		L"Update"
//hub end
#define BKSFNS_MAKE(a, b) (a L"_" b)
#define BKSFNS_MAKE_3(a, b, c) (a L"_" b L"_" c)

#define BKSAFE_DDE_WND_CLASS L"{C234B468-9B74-443b-8CAC-BCA631D38D68}"

namespace BeikeSafe
{
    inline BOOL IsRunning()
    {
        return (NULL != ::FindWindow(BKSAFE_DDE_WND_CLASS, NULL));
    }

    inline BOOL Navigate(LPCTSTR lpszMessage, int nSize)
    {
        if (nSize > 255)
            return FALSE;

        HWND hWndDDE = ::FindWindow(BKSAFE_DDE_WND_CLASS, NULL);
        if (NULL == hWndDDE)
            return FALSE;

        ATOM atomDDE = ::GlobalAddAtom(lpszMessage);
        if (0 == atomDDE)
            return FALSE;

        return ::PostMessage(hWndDDE, WM_APP, (WPARAM)atomDDE, (LPARAM)nSize);
    }

    inline BOOL Navigate(LPCTSTR lpszMessage)
    {
        int nSize = (int)wcslen(lpszMessage);

        if (nSize > 255)
            return FALSE;

        HWND hWndDDE = ::FindWindow(BKSAFE_DDE_WND_CLASS, NULL);
        if (NULL == hWndDDE)
            return FALSE;

        ATOM atomDDE = ::GlobalAddAtom(lpszMessage);
        if (0 == atomDDE)
            return FALSE;

        return ::PostMessage(hWndDDE, WM_APP, (WPARAM)atomDDE, (LPARAM)nSize);
    }
}
