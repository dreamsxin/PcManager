// kisupinst.h
// kisupinst实现统一升级程序的单一实例。

#pragma once
#ifndef __KAFE_CHECK_INSTANCE__
#define __KAFE_CHECK_INSTANCE__

#ifndef WM_UPLIVE_BASE
#define WM_UPLIVE_BASE					(WM_USER + 1767)
#endif

#ifndef WM_UPLIVE_SHOW
#define WM_UPLIVE_SHOW					(WM_USER + 1000)
#endif

#ifndef WM_SHOW_RUN1
#define WM_SHOW_RUN1					WM_USER + 1001
#endif

#ifndef WM_SHOW_RUN2
#define WM_SHOW_RUN2					WM_USER + 1002
#endif


class KCheckInstance
{
private:
	explicit KCheckInstance();

#if _MSC_VER <= 1200 // vc6.0
public:
#endif
	~KCheckInstance();

public:
	static KCheckInstance * Instance(void);
	static void Release();
public:
	BOOL CheckFirstInstance(LPCTSTR pszCmdline, IN LPWSTR lpCheckStrName = NULL, IN LPCWSTR lpFileMapStr = NULL);

	BOOL CfgFirstInstance(IN HINSTANCE hInst, 
						  IN HWND hWnd, 
						  IN BOOL bInAutoRun, 
						  IN BOOL bInvalidInst = FALSE);

	BOOL IsFirstInstance();

	void ClearFirstInstance();

	/*
	@ FuncName	: CheckInstanceSelf
	@ Brief		: 检查是否存在多个实例！
	*/
	BOOL CheckInstanceSelf(IN LPCTSTR lpInstanceName);
protected:
	/*
	@ FuncName	: ProcessCmdLine
	@ Brief		: 预先处理命令行！
	*/
	void ProcessCmdLine(IN LPCTSTR pszCmdline, 
						OUT BOOL& bIsExitApp, 
						OUT BOOL& bAutoRun);
	/*
	@ FuncName	: ProcessInstance
	@ Brief		: 处理多实例！
	*/
	BOOL ProcessInstance(IN BOOL bFirstInstance,
						 IN BOOL bIsExitApp,
						 IN BOOL bAutoRun,
						 IN LPCWSTR lpCmdParam);
	/*
	@ FuncName	: CheckInstance
	@ Brief		: 检查实例！
	*/
	BOOL CheckInstance(OUT BOOL& bIsFirstInstance, IN LPCWSTR lpNamStr = NULL, IN LPCWSTR lpFileMapStr = NULL);


private:
	// 升级程序共享内存句柄。
	HANDLE				m_hFileMapping;
	// 是否是第一个实例。
	BOOL				m_bFirstInstance;
	// 实例的互斥体。
	HANDLE				m_hInstanceMutex;
	HANDLE				m_hInstanceMutexSelf;
};


#endif //__UPLIVE_UI_KCHECKINSTANCE_H__
