#pragma once

#include "ksaferunstrdefine.h"

#define VERSION_KSAFE_RUNOPT_INTERFACE		7
#define STR_KSAFE_RUN_OPTIMIZE_DAT			_T("\\runopt.dat")
#define STR_KSAFE_REG_OPTIMIZE_DAT			_T("\\syscfgopt.dat")

#define KSRUN_DELAYPROTECT_PARAM _T("-delayruncheck")

#define KSRUN_TYPE_STARTUP	1
#define KSRUN_TYPE_SERVICE	2
#define KSRUN_TYPE_TASK		3
#define KSRUN_TYPE_ALL		-1

#define KSRUN_TYPE_SYSCFG	11

#define KSRUN_TYPE_RESTORE_ALL			4
#define KSRUN_TYPE_RESTORE_DELAY_RUN	5

#define KSRUN_TYPE_NONSYSTEM	0x10
#define KSRUN_TYPE_SYSTEM		0x20

#define REG_RUN_TYPE_WINDOW_HKLM		1
#define REG_RUN_TYPE_WINDOW_HKCU		2
#define REG_RUN_TYPE_KSAFE_HKLM			3
#define REG_RUN_TYPE_KSAFE_HKCU			4
#define REG_RUN_TYPE_3XX_HKLM			5
#define REG_RUN_TYPE_3XX_HKCU			6
#define REG_RUN_TYPE_WINDOW_HKLM64		7
#define REG_RUN_TYPE_WINDOW_HKCU64		8
#define REG_RUN_TYPE_KSAFE_HKLM64		9
#define REG_RUN_TYPE_KSAFE_HKCU64		10
#define REG_RUN_TYPE_3XX_HKLM64			11
#define REG_RUN_TYPE_3XX_HKCU64			12

#define DIR_RUN_TYPE_BEGIN				100
#define DIR_RUN_TYPE_WINDOW_COMM		101
#define DIR_RUN_TYPE_WINDOW_USER		102
#define DIR_RUN_TYPE_KSAFE_COMM			103
#define DIR_RUN_TYPE_KSAFE_USER			104
#define DIR_RUN_TYPE_3XX_COMM			105
#define DIR_RUN_TYPE_3XX_USER			106

#define KSRUN_ADVICE_CAN_DISABLE		0x01	//可以禁止
#define KSRUN_ADVICE_DELAY				0x02	//建议延迟
#define KSRUN_ADVICE_CAN_DELAY			0x10	//可以延迟
#define KSRUN_ADVICE_DELAY_SAFE_SOFT	0x20	//安全类软件，不能延迟
#define KSRUN_ADVICE_DELAY_SYSTEM		0x40	//系统类软件，不能延迟
#define KSRUN_ADVICE_CANNOT_DELAY		0x80	//通用软件，不能延迟


//设置类型
#define KSRUN_START_DISABELE	0		//禁用
#define KSRUN_START_ENABLE		0x1		//启用
#define KSRUN_START_DELAY		0x2		//延迟
#define KSRUN_START_DELAY_FAIL  0x3		//延迟失败

#define KSAFE_SYSCONFIG_OPTIMIZE			1	//优化
#define KSAFE_SYSCONFIG_RESTORE_SYSDEF		2	//还原到系统默认
#define KSAFE_SYSCONFIG_RESTORE_INIT		3	//还原到未使用卫士

inline BOOL IsTypeRunEnable(DWORD nType)
{
	return (nType==REG_RUN_TYPE_WINDOW_HKLM||
		nType==REG_RUN_TYPE_WINDOW_HKCU||
		nType==DIR_RUN_TYPE_WINDOW_COMM||
		nType==DIR_RUN_TYPE_WINDOW_USER);
}

inline BOOL IsTypeRunReg(DWORD nType)
{
	return (nType<DIR_RUN_TYPE_BEGIN);
}

inline BOOL IsTypeRunDir(DWORD nType)
{
	return (nType>DIR_RUN_TYPE_BEGIN);
}

inline BOOL IsHKLMRunType(DWORD nType)
{
	return (nType==REG_RUN_TYPE_WINDOW_HKLM || nType==REG_RUN_TYPE_KSAFE_HKLM);
}

inline BOOL IsSameRunType(DWORD nType, DWORD nTypeX)
{
	if (nTypeX==nType)
		return TRUE;

	if (nType==REG_RUN_TYPE_WINDOW_HKLM && nTypeX==REG_RUN_TYPE_KSAFE_HKLM )
		return TRUE;

	if (nType==REG_RUN_TYPE_WINDOW_HKCU && nTypeX==REG_RUN_TYPE_KSAFE_HKCU )
		return TRUE;

	if (nTypeX==REG_RUN_TYPE_WINDOW_HKLM && nType==REG_RUN_TYPE_KSAFE_HKLM )
		return TRUE;

	if (nTypeX==REG_RUN_TYPE_WINDOW_HKCU && nType==REG_RUN_TYPE_KSAFE_HKCU )
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	if (nType==DIR_RUN_TYPE_WINDOW_COMM && nTypeX==DIR_RUN_TYPE_KSAFE_COMM )
		return TRUE;

	if (nType==DIR_RUN_TYPE_WINDOW_USER && nTypeX==DIR_RUN_TYPE_KSAFE_USER )
		return TRUE;

	if (nTypeX==DIR_RUN_TYPE_WINDOW_COMM && nType==DIR_RUN_TYPE_KSAFE_COMM )
		return TRUE;

	if (nTypeX==DIR_RUN_TYPE_WINDOW_USER && nType==DIR_RUN_TYPE_KSAFE_USER )
		return TRUE;

	//////////////////////////////////////////////////////////////////////////
	if (nType==REG_RUN_TYPE_WINDOW_HKLM64 && nTypeX==REG_RUN_TYPE_KSAFE_HKLM64 )
		return TRUE;

	if (nType==REG_RUN_TYPE_WINDOW_HKCU64 && nTypeX==REG_RUN_TYPE_KSAFE_HKCU64 )
		return TRUE;

	if (nTypeX==REG_RUN_TYPE_WINDOW_HKLM64 && nType==REG_RUN_TYPE_KSAFE_HKLM64 )
		return TRUE;

	if (nTypeX==REG_RUN_TYPE_WINDOW_HKCU64 && nType==REG_RUN_TYPE_KSAFE_HKCU64 )
		return TRUE;

	return FALSE;
}

inline DWORD GetInitRegType(DWORD nNewType)
{
	if (nNewType == REG_RUN_TYPE_KSAFE_HKLM)
		return REG_RUN_TYPE_WINDOW_HKLM;
	else if (nNewType == REG_RUN_TYPE_KSAFE_HKCU)
		return REG_RUN_TYPE_WINDOW_HKCU;
	else if (nNewType == DIR_RUN_TYPE_KSAFE_COMM)
		return DIR_RUN_TYPE_WINDOW_COMM;
	else if (nNewType == DIR_RUN_TYPE_KSAFE_USER)
		return DIR_RUN_TYPE_WINDOW_USER;
	else if (nNewType == REG_RUN_TYPE_KSAFE_HKLM64)
		return REG_RUN_TYPE_WINDOW_HKLM64;
	else if (nNewType == REG_RUN_TYPE_KSAFE_HKCU64)
		return REG_RUN_TYPE_WINDOW_HKCU64;
	else 
		return nNewType;
}

inline CString GetFileName(LPCTSTR lpFullPath)
{
	if ( NULL == lpFullPath )
		return L"";

	LPCTSTR lpstr = _tcsrchr(lpFullPath,_T('\\'));

	if (lpstr==NULL)
		return CString(lpFullPath);
	else
		return CString(lpstr+1);
}

//NoAutoDisable
//StopIt

class CKsafeRunInfo
{
public:
	CKsafeRunInfo()
	{	
		nID		= 0;
		bEnable = FALSE;
		nType	= 0;
		nAdvice	= 4;
		bSystem = FALSE;
		bNoDealyShow = FALSE;
		nRegType= 0;
		bUnknown= TRUE;
		bDisabledBy3xx = FALSE;
		bFileIsExist = TRUE;
		bHasIgnored = FALSE;
		bShowRemain = TRUE;
		nExtraFlag = 0;
		nUserCount = 0;
	}
	~CKsafeRunInfo(){}

	BOOL IsAdviseStop()
	{
		return (nAdvice==0);
	}

	BOOL IsDisabledBy3XX()
	{
		return bDisabledBy3xx;
	}

	LPCTSTR GetAdviceStr()
	{
		LPCTSTR	lpstr = _T("维持现状");

		switch (nAdvice)
		{
		case 0:
			lpstr = _T("建议禁止");
			break;
		case 1:
			lpstr = _T("建议启动");
			break;
		case 2:
			lpstr = _T("必须启动");
			break;
		case 3:
			lpstr = _T("建议启动");
			break;
		case 4:
			{
				lpstr = _T("维持现状");
				if(nExtraFlag & KSRUN_ADVICE_CAN_DISABLE)
					lpstr = _T("可以禁止");
				if (nType == KSRUN_TYPE_STARTUP	&& IsAdviceDelay())
					lpstr = _T("建议延迟");
				break;
			}
		default:
			break;
		}
		if (!bFileIsExist && nType != KSRUN_TYPE_SERVICE)
			lpstr = _T("建议删除");
		return lpstr;
	}

	BOOL IsAdviceDelay()//建议延迟，建议文字为“建议延迟”
	{
		return (nAdvice == 4) && (nExtraFlag & KSRUN_ADVICE_DELAY);
	}
	
	BOOL IsCanDelay()	//可以延迟，出现在一键优化中
	{
		return ((bEnable == KSRUN_START_ENABLE)
			&& !(CannotDelay_SafeSoft() || CannotDelay_Sys() || CannotDelay())
			&& (nType == KSRUN_TYPE_STARTUP)
			&& !(nAdvice == 1 || nAdvice == 2)
			&& !(IsAdviseStop())
			&& (nID != 0));
	}

	BOOL CannotDelay()
	{
		return (nExtraFlag & KSRUN_ADVICE_CANNOT_DELAY);
	}

	BOOL CannotDelay_Sys()
	{
		return (nExtraFlag & KSRUN_ADVICE_DELAY_SYSTEM) || nAdvice == 2;
	}

	BOOL CannotDelay_SafeSoft()
	{
		return (nExtraFlag & KSRUN_ADVICE_DELAY_SAFE_SOFT);
	}

public:
	DWORD		nID;
	DWORD		nType;			
	DWORD		nAdvice;	
	BOOL		bEnable;
	BOOL		bNoDealyShow;	
	BOOL		bUnknown;
	BOOL		bSystem;
	BOOL		bDisabledBy3xx;
	BOOL		bFileIsExist;
	BOOL		bHasIgnored;
	BOOL		bShowRemain;
	DWORD		nRegType;
	DWORD		nJobType;
	DWORD		nExtraFlag;	//"可以禁止"标志
	CString		strName;			
	CString		strDesc;		
	CString		strPath;		//启动项的全路径
	CString		strExePath;		//启动项lnk指向文件的全路径
	CString		strValue;		//run项的注册表值
	CString		strParam;		
	CString		strDisplay;		//显示的名字
	CString		strWarning;		//禁止的警告
	CString		strOpenRate;	//开启率
	DWORD		nUserCount;		//使用人数
};

class IEnumRunCallBack
{
public:
	virtual BOOL FindRunItem(CKsafeRunInfo* pInfo) = 0;
	virtual void BeginScan(DWORD nType) = 0;
	virtual void EndScan() = 0;
	virtual void RestoreEnd() = 0;
	virtual void PostRestoreRunItem(DWORD nType,BOOL bEnable,LPCTSTR lpName) = 0;
};

class IEnumRunCallBackNull : public IEnumRunCallBack
{
	virtual BOOL FindRunItem(CKsafeRunInfo* pInfo)=0;
	virtual void BeginScan(DWORD nType){return;}
	virtual void EndScan(){return;}
	virtual void RestoreEnd(){return;}
	virtual void PostRestoreRunItem(DWORD nType,BOOL bEnable,LPCTSTR lpName){return;}
};

class IEnumRunCallBackNull2 : public IEnumRunCallBackNull
{
public:
	virtual BOOL FindRunItem(CKsafeRunInfo* pInfo) = 0;
	virtual void BeginScan(DWORD nType){return;}
	virtual void EndScan(){return;}
	virtual void RestoreEnd() {return;}
	virtual void PostRestoreRunItem(DWORD nType,BOOL bEnable,LPCTSTR lpName) = 0;
	virtual void EndScan(DWORD dwFlags) = 0;
	virtual BOOL FindRunItem(CKSafeSysoptRegInfo* pInfo) = 0;
};

class IRunDatQueryEng
{
public:
	virtual BOOL GetInfoByPathValue(DWORD nType, LPCTSTR lpPath, LPCTSTR lpFullPath, KSRunInfo** ppInfo) = 0;
	virtual VOID DeleteInfo(KSRunInfo* pInfo) = 0;
};

class IRunOptimizeEng
{
public:
	virtual VOID EnumRunInfo(DWORD dwFlags,IEnumRunCallBack* pCallback )=0;
	virtual BOOL ControlRun(BOOL bEnable, CKsafeRunInfo* pInfo) = 0;
	virtual VOID RestoreRun(DWORD nType,IEnumRunCallBack* pCallback) = 0;
	virtual BOOL DeleteRun(CKsafeRunInfo* pInfo) = 0;
	virtual DWORD GetRunCount(DWORD nFlags) = 0;
	virtual VOID EnumRunInfoForExam(DWORD dwFlags,IEnumRunCallBack* pCallback )=0;
};

class IRunOptimizeEng2: public IRunOptimizeEng
{
public:
	virtual VOID EnumRunInfo(DWORD dwFlags,IEnumRunCallBack* pCallback )=0;
	virtual BOOL ControlRun(BOOL bEnable, CKsafeRunInfo* pInfo)=0;  //增加一种类型为延迟，暂不改定义，因为BOOL实际继承的是INT
	virtual VOID RestoreRun(DWORD nType,IEnumRunCallBack* pCallback)=0;
	virtual BOOL DeleteRun(CKsafeRunInfo* pInfo)=0;
	virtual DWORD GetRunCount(DWORD nFlags)=0;
	virtual VOID EnumRunInfoForExam(DWORD dwFlags,IEnumRunCallBack* pCallback )=0;
	virtual VOID EnumSysConfigInfo(DWORD dwFlags, IEnumRunCallBackNull2* pCallback) = 0;
	virtual BOOL ControlSysConfig(DWORD dwFlags, CKSafeSysoptRegInfo* pSysCfgInfo) = 0;
	virtual VOID RestoreSysCfg(DWORD dwFlags, IEnumRunCallBack* pCallback) = 0;
};

typedef IRunOptimizeEng* (WINAPI *PFNGetRunOptEng)(DWORD);
typedef VOID (WINAPI *PFNFreeRunOptEng)(IRunOptimizeEng* );

typedef IRunDatQueryEng* (WINAPI *PFNGetQueryEng)(DWORD);
typedef VOID (WINAPI *PFNFreeQueryEng)(IRunDatQueryEng* );

typedef VOID (WINAPI *PFNStatitics)();
typedef IRunOptimizeEng2* (WINAPI *PFNGetRunOptEng2)(DWORD);

#define STR_RUNOPT_DLL_NAME _T("krunopt.dll")
#define STR_RUNOPT_GET_ENG	("a")
#define STR_RUNOPT_DEL_ENG	("b")
#define STR_RUNOPT_GET_QENG	("c")
#define STR_RUNOPT_DEL_QENG	("d")

#define STR_RUNOPT_STATITICS ("e")
#define STR_RUNOPT_STATITICS2 ("f")
#define STR_RUNOPT_GET_ENG2	  ("g")


class CRunOptLoader
{
public:
	CRunOptLoader(CString strAppPath = L"")
	{
		m_hModDLL	= NULL;
		pfnGet		= NULL;
		pfnFree		= NULL;
		pfnGetQEng	= NULL;
		pfnFreeQEng = NULL;

		CString strDllPath = strAppPath;
		if (TRUE == strDllPath.IsEmpty())
		{
			strDllPath = STR_RUNOPT_DLL_NAME;
		}
		else
		{
			strDllPath.Append(L"\\");
			strDllPath.Append(STR_RUNOPT_DLL_NAME);
		}

		m_hModDLL	= ::LoadLibrary(strDllPath);
		if ( m_hModDLL != NULL )
		{
			pfnGet  = (PFNGetRunOptEng)::GetProcAddress( m_hModDLL, STR_RUNOPT_GET_ENG);
			pfnFree = (PFNFreeRunOptEng)::GetProcAddress( m_hModDLL, STR_RUNOPT_DEL_ENG);
			pfnGetQEng  = (PFNGetQueryEng)::GetProcAddress( m_hModDLL, STR_RUNOPT_GET_QENG);
			pfnFreeQEng = (PFNFreeQueryEng)::GetProcAddress( m_hModDLL, STR_RUNOPT_DEL_QENG);
			pfnStatics	= (PFNStatitics)::GetProcAddress(m_hModDLL,STR_RUNOPT_STATITICS2);
			pfnGet2		= (PFNGetRunOptEng2)::GetProcAddress(m_hModDLL, STR_RUNOPT_GET_ENG2);
		}
	}
	virtual ~CRunOptLoader()
	{
		if ( m_hModDLL != NULL )
		{
			::FreeLibrary(m_hModDLL);
			m_hModDLL = NULL;
		}
	}

	static CRunOptLoader* GetGlobalPtr(CString strAppPath = L"")
	{
		static CRunOptLoader loader(strAppPath);
		return &loader;
	}

	IRunOptimizeEng* GetOptEng()
	{
		if ( pfnGet != NULL )
			return pfnGet(VERSION_KSAFE_RUNOPT_INTERFACE);

		return NULL;	
	}

	VOID FreeOptEng(IRunOptimizeEng* pEng)
	{
		if ( pfnFree != NULL )
			pfnFree(pEng);
	}


	IRunDatQueryEng* GetQueryEng()
	{
		if ( pfnGetQEng != NULL )
			return pfnGetQEng(VERSION_KSAFE_RUNOPT_INTERFACE);

		return NULL;	
	}

	VOID FreeQueryEng(IRunDatQueryEng* pEng)
	{
		if ( pfnFreeQEng != NULL )
			pfnFreeQEng(pEng);
	}

	VOID Statitics()
	{
		if ( pfnStatics != NULL )
			pfnStatics();
	}

	IRunOptimizeEng2* GetOptEng2()
	{
		if (pfnGet2 != NULL)
			return pfnGet2(VERSION_KSAFE_RUNOPT_INTERFACE);
		
		return NULL;
	}

protected:
	HMODULE				m_hModDLL;
	PFNGetRunOptEng		pfnGet;
	PFNFreeRunOptEng	pfnFree;

	PFNGetQueryEng		pfnGetQEng;
	PFNFreeQueryEng		pfnFreeQEng;
	PFNStatitics		pfnStatics;
	PFNGetRunOptEng2	pfnGet2;
};

