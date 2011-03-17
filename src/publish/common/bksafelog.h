#include "iefix/RegisterOperate.h"
#include "communits/ThreadLangLocale.h"
#include "common/utility.h"

#pragma once

#define REG_KSAFE	L"SOFTWARE\\KSafe"

namespace BKSafeLog
{
	class BKSafeModuleLog
	{
	public:
		BKSafeModuleLog(DWORD dwModuleLog, CString strName, CString strFlag)
		{
			dwModule = dwModuleLog;
			strLogName = strName;
			strDebugFlag = strFlag;
		}
		~BKSafeModuleLog(){}
		DWORD		dwModule;
		CString		strLogName;
		CString		strDebugFlag;
	};

	enum
	{
		enumBKSafeTrayLog	= 0,
		enumBKSafeUpliveLog,
		enumBKSafeExamLog,
		enumBKSafeMainDlgLog,
		enumBKSafeNetMonitorLog,
		enumBKSafeRunoptLog,
		enumBKSafeFloatWndLog,


		enumBKSafeUnknownModule = -1
	};

	class CBKafeLogModuleInfo
	{
	public:
		static CBKafeLogModuleInfo* GetPtr()
		{
			static CBKafeLogModuleInfo info;
			return &info;
		}
		~CBKafeLogModuleInfo(){};
	public:

		int AddLogModule(DWORD dwModule, BKSafeModuleLog& moduleInfo)
		{
			if (m_mapModule.FindKey(dwModule) < 0)
				m_mapModule.Add(dwModule, moduleInfo);

			return m_mapModule.GetSize();
		}
		CSimpleMap<DWORD, BKSafeModuleLog>& GetModuleMap()
		{
			return m_mapModule;
		}
	private:
		CSimpleMap<DWORD, BKSafeModuleLog> m_mapModule;
		CBKafeLogModuleInfo()
		{
			m_mapModule.RemoveAll();
		}
	};



	class CBKSafeLog
	{
	public:
		CBKSafeLog()
		{
			m_mapModule.RemoveAll();

			//Init Log Module
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeTrayLog,		BKSafeModuleLog(enumBKSafeTrayLog,		L"tray.log",		L"traydebug"));
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeUpliveLog,	BKSafeModuleLog(enumBKSafeUpliveLog,	L"uplive.log",		L"uplivedebug"));
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeExamLog,		BKSafeModuleLog(enumBKSafeExamLog,		L"exam.log",		L"examdebug"));
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeMainDlgLog,	BKSafeModuleLog(enumBKSafeMainDlgLog,	L"ksafemain.log",	L"ksafemaindebug"));
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeNetMonitorLog,BKSafeModuleLog(enumBKSafeNetMonitorLog,	L"ksafenetmonitor.log",	L"netmonitordebug"));
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeRunoptLog,	BKSafeModuleLog(enumBKSafeRunoptLog,	L"ksaferunopt.log",	L"ksaferunoptdebug"));
			CBKafeLogModuleInfo::GetPtr()->AddLogModule(enumBKSafeFloatWndLog,	BKSafeModuleLog(enumBKSafeFloatWndLog,	L"bksafefloatwnd.log",	L"bksafefloatwnddebug"));
			//end Init

			m_dwLogModule = enumBKSafeUnknownModule;

			m_bCanWriteLog = FALSE;
		}
		~CBKSafeLog(){}
	public:
		void WriteLog(LPCTSTR pszFormat, ...)
		{
			USE_CHINESE_LANG_LOCAL;
			USES_CONVERSION;

			if (FALSE == IsCanWriteLog())
				return;

			CString strTrace = L"";
			va_list args;
			va_start(args, pszFormat);
			strTrace.FormatV(pszFormat, args);
			va_end(args);

			if (TRUE == strTrace.IsEmpty())
				return;

			char szBuf[1000] = {'\0'};
			//StringCbPrintfA(szBuf, sizeof(szBuf), "%s", W2A(strTrace.GetBuffer(-1)));
			_snprintf(szBuf, sizeof(szBuf), "%s", W2A(strTrace.GetBuffer(-1)));
			strTrace.ReleaseBuffer(-1);

			FILE *pFile = fopen(W2A(m_strLogPath.GetBuffer(MAX_PATH)), ("a+b"));
			m_strLogPath.ReleaseBuffer(MAX_PATH);

			if(pFile == NULL)
				return;

			SYSTEMTIME timeCur;
			GetLocalTime(&timeCur);
			char szTime[100] = {0};
			//StringCbPrintfA(szTime, sizeof(szTime), ("%d-%d-%d %d:%d:%d:%d  "), timeCur.wYear, timeCur.wMonth, timeCur.wDay,timeCur.wHour, timeCur.wMinute, timeCur.wSecond, timeCur.wMilliseconds);
			_snprintf(szTime, sizeof(szTime), ("%d-%d-%d %d:%d:%d:%d  "), timeCur.wYear, timeCur.wMonth, timeCur.wDay,timeCur.wHour, timeCur.wMinute, timeCur.wSecond, timeCur.wMilliseconds);

			fwrite((szTime), strlen(szTime), 1, pFile);
			fwrite((szBuf),strlen(szBuf), 1, pFile);
			fwrite(("\r\n"), 2, 1,pFile);
			fclose(pFile);
		}


		BOOL IsCanWriteLog()
		{
			return m_bCanWriteLog;
		}

		DWORD SetLogForModule(DWORD dwModule)
		{
			DWORD d = m_dwLogModule;
			m_dwLogModule = dwModule;

			int nIndex = CBKafeLogModuleInfo::GetPtr()->GetModuleMap().FindKey(dwModule);
			if (nIndex >= 0)
			{
				BKSafeModuleLog moduleLog = CBKafeLogModuleInfo::GetPtr()->GetModuleMap().GetValueAt(nIndex);
				//init write log 
				CString strLog;
				CAppPath::Instance().GetLeidianLogPath(strLog);
				m_strLogPath = strLog;
				m_strLogPath.Append(L"\\");
				m_strLogPath.Append(moduleLog.strLogName);

				DWORD dwValue = 0;
				RegisterOperate::CRegisterOperate::GetRegOperatePtr()->GetRegValue(HKEY_LOCAL_MACHINE, REG_KSAFE, moduleLog.strDebugFlag, dwValue);
				m_bCanWriteLog = dwValue;
				//end init

			}

			return d;
		}
	private:
		CString		m_strLogPath;
		BOOL		m_bCanWriteLog;
		DWORD		m_dwLogModule;
		CSimpleMap<DWORD, BKSafeModuleLog> m_mapModule;
	};
}
