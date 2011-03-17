/*
dummyz@126.com
2009/12/10
*/
#ifndef _SAFE_MONITOR_INC_
#define _SAFE_MONITOR_INC_ // 其他地方使用，不能使用#pragma once

/*
1、危险注册表操作行为监控
2、危险进程创建监控
3、溢出行为监控
*/

#include <assert.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//									const value defined
////////////////////////////////////////////////////////////////////////////////////////////////

#define BKSAFE_MONITOR_NUMBER		4	// 基础防护4 个

// 监控器标识
#define SM_ID_INVAILD				0
#define SM_ID_PROCESS				1 // 进程监控
#define SM_ID_RISK					2 // 风险操作监控
#define SM_ID_UDISK					4 // u盘自动运行拦截
#define SM_ID_VIRUS					5
#define SM_ID_LEAK                  6 // 漏洞防护
#define SM_ID_RESCAN				SM_ID_VIRUS // 回扫
#define SM_ID_KWS					11 //网盾浏览器保护
#define SM_ID_KWS_SAFE_DOWN			12 //下载保护泡泡
#define SM_ID_KWS_URL_MON			13 // url 防火墙
#define SM_ID_REPORT				14 // 异步数据提交
#define SM_ID_ARP                   15

// 监控器运行状态
#define SM_STATUS_INVALID			0
#define SM_STATUS_RUN				1
#define SM_STATUS_STOP				2
#define SM_STATUS_PAUSE				3

// 监视器支持的控制
#define SM_CTRL_TRUST				0x0001			// 信任
#define SM_CTRL_BLOCK				0x0002			// 阻止
#define SM_CTRL_CLEAN				0x0004			// 阻止的同时执行清除操作
#define SM_CTRL_REMEMBER			0x00010000		// 记住操作，不在提示
#define SM_CTRL_HISTROY				0x00020000		// 操作来自历史
#define SM_CTRL_FAST_SCAN			0x00040000		// 同时执行了快速扫描

#define IS_SM_CTRL_TRUST(x)			(((x) & SM_CTRL_TRUST) == SM_CTRL_TRUST)
#define IS_SM_CTRL_BLOCK(x)			(((x) & SM_CTRL_BLOCK) == SM_CTRL_BLOCK)
#define IS_SM_CTRL_CLEAN(x)			(((x) & SM_CTRL_CLEAN) == SM_CTRL_CLEAN)
#define IS_SM_CTRL_REMERBER(x)		(((x) & SM_CTRL_REMEMBER) == SM_CTRL_REMEMBER)
#define IS_SM_CTRL_HISTROY(x)		(((x) & SM_CTRL_HISTROY) == SM_CTRL_HISTROY)
#define IS_SM_CTRL_FAST_SCAN(x)		(((x) & SM_CTRL_FAST_SCAN) == SM_CTRL_FAST_SCAN)

// 监视器捕捉到的动作
#define SM_ACTION_NOTING			0
#define SM_ACTION_ADD				1		// 新建
#define SM_ACTION_DELETE			2		// 删除
#define SM_ACTION_MODIFY			3		// 修改

// 清除结果
#define SM_CLEAN_DELETE				1
#define SM_CLEAN_REPAIR				2
#define SM_CLEAN_FAILED				0x00010000
#define SM_CLEAN_SUCCESS			0x00020000
#define SM_CLEAN_REBOOT				0x00030000
#define GET_SM_CLEAN_OPERATOR(x)	((x) & 0xffff)
#define GET_SM_CLEAN_RESULT(x)		((x) & 0xffff0000)

//文件类型
enum SM_FILE_TYPE {
	SM_FILE_TYPE_UNKNOWN		= 0x00000000,
	SM_FILE_TYPE_SAFE			= 0x00010000,
	SM_FILE_TYPE_VIRUS			= 0x00020000
};

enum SM_FT_VIRUS  // 病毒的子类型
{
	SM_FT_VIRUS_TROJAN = 1,
	SM_FT_VIRUS_THREAT,
	SM_FT_VIRUS_MALWARE,
	SM_FT_VIRUS_WORM,
	SM_FT_VIRUS_ADWARE,
	SM_FT_VIRUS_RISK_LOW,// 风险程序
	SM_FT_VIRUS_RISK_MED,
	SM_FT_VIRUS_RISK_HIG,
};

enum SM_FT_UNKNOWN // 未知的子类型
{
	SM_FT_UNKNOWN_NET_ERROR		= 1,
	SM_FT_UNKNOWN_TIMEOUT,
	SM_FT_UNKNOWN_SCAN_ERROR,
	SM_FT_UNKNOWN_SCAN_RESULT_ERROR
};

inline DWORD GET_SM_FILE_TYPE(DWORD dwFileTyppe)			
{
	return (dwFileTyppe & 0xffff0000);
}

inline DWORD GET_SM_SUB_FILE_TYPE(DWORD dwFileType)
{
	DWORD f = dwFileType & 0xffff;
	return (f <= 0xff) ? f : 0;
}

inline SM_FILE_TYPE MAKE_SM_FILE_TYPE(DWORD dwFileType, DWORD dwSubFileType, DWORD dwSecLevel = 0)
{
	dwSubFileType = dwSubFileType + (dwSecLevel << 8);
	assert(dwSubFileType <= 0xFFFF);

	return (SM_FILE_TYPE)(dwFileType | dwSubFileType);
}

// 文件扫描引擎
enum SM_FILE_ENGINE
{
	SM_FILE_ENGINE_UNKNOWN									=	0x00000000,
	SM_FILE_ENGINE_VIRUS									=	0x00000001, // 本地引擎
	SM_FILE_ENGINE_LOCAL									=	SM_FILE_ENGINE_VIRUS,
	SM_FILE_ENGINE_CLOUND									=	0x00000002, // 云端引擎
	SM_FILE_ENGINE_PLUGIN									=	0x00000004, // 插件引擎
	SM_FILE_ENGINE_KNOWLEDGELIB								=	0x00000008, // 知识库引擎
	SM_FILE_ENGINE_HEURISTIC								=	0x00000010, // 启发
	SM_FILE_ENGINE_RUNDATA		    						=	0x00000020, // 启动项库
	SM_FILE_ENGINE_VERIFY_SIGN								=	0x00000080, // 签名
	SM_FILE_ENGINE_RISK_SIGN								=	0x00000100, // 黑“特征”，检测风险安装包

	SM_FILE_ENGINE_ALL										=	0xFFFFFFFF
};

////////////////////////////////////////////////////////////////////////////////////////////////
//									interface define
////////////////////////////////////////////////////////////////////////////////////////////////
#include "SafeMonitorEvent.h"
#include <assert.h>

class ISafeMonitorObserver;
class ISafeMonitorManager;
class CSafeMonitorEvent;

class ISafeMonitorObserver
{
public:
	virtual DWORD __stdcall MonitorEvent(CSafeMonitorEvent* lpEvent) = 0;
};

class ISafeMonitorManager : public ISafeMonitorObserver
{
public:
	virtual HRESULT __stdcall Init() = 0;
	virtual HRESULT __stdcall Uninit() = 0;

	virtual HRESULT __stdcall Startup(ISafeMonitorObserver* lpObserver) = 0;
	virtual HRESULT __stdcall Shutdown() = 0;

	// SM_ID_RISK 支持 'default_homepage', 'default_browser'
	virtual HRESULT __stdcall SetProperty(DWORD dwMonitorId, LPCTSTR lpName, LPCTSTR lpValue) = 0;
	virtual HRESULT __stdcall GetProperty(DWORD dwMonitorId, LPCTSTR lpName, LPTSTR& lpValue) = 0;

	// SM_ID_ALL 支持 'status', 
	// set 'plugin_lib_updated', 'knowledge_lib_updated', 'riskpoint_lib_updated'
	virtual HRESULT __stdcall SetPropertyInt(DWORD dwMonitorId, LPCTSTR lpName, DWORD dwValue) = 0;
	virtual HRESULT __stdcall GetPropertyInt(DWORD dwMonitorId, LPCTSTR lpName, DWORD& dwValue) = 0;
};

#define SMM_DLL_NAME	_T("ksfmon.dll")
#define SMM_FUN_NAME	"A1"
#define GCPSR_FUN_NAME	"D1"

typedef ISafeMonitorManager* (WINAPI* PFN_GetSafeMonitorManager)();
ISafeMonitorManager* WINAPI GetSafeMonitorManager();


//////////////////////////////////////////////////////////////////////////
// CSafeMonitorLoader

class CSafeMonitorLoader
{
public:
	CSafeMonitorLoader()
	{
		m_pfnGet = NULL;

		m_hModule = LoadLibrary(SMM_DLL_NAME);
		if ( m_hModule != NULL )
		{
			m_pfnGet = (PFN_GetSafeMonitorManager)GetProcAddress(m_hModule, SMM_FUN_NAME);
		}
	}
	
	~CSafeMonitorLoader()
	{
		FreeLibrary(m_hModule);
	}

	operator bool ()
	{
		return (m_hModule != NULL) && (m_pfnGet != NULL);
	}

	ISafeMonitorManager* GetManager()
	{
		assert(m_pfnGet != NULL);
		return (m_pfnGet != NULL) ? m_pfnGet() : NULL;
	}


protected:
	PFN_GetSafeMonitorManager m_pfnGet;
	HMODULE		m_hModule;
};


#endif /* _SAFE_MONITOR_INC_ */ 

