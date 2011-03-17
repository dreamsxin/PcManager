#pragma once

class CGetGlobalState
{
public:
	CGetGlobalState(void);
	~CGetGlobalState(void);

	VOID		InitModule(HMODULE hMod);
public:
	LPCTSTR		GetModulePath();
	LPCTSTR		GetModuleDir();
	LPCTSTR		GetTempDir();
	LPCTSTR		GetWindowsDir();
	LPCTSTR		GetSystem32Dir();
	LPCTSTR		GetProgramDir();
	LPCTSTR		GetCommonAppDataDir();
	LPCTSTR		GetAppDataDir();
	LPCTSTR		GetUserStartupDir();
	LPCTSTR		GetAllStartupDir();

	LPCTSTR		GetUserinitPath();
	LPCTSTR		GetIExplorerPath();
	LPCTSTR		GetExplorerPath();
	LPCTSTR		GetSysExplorerPath();	// system32目录下explorer.exe的路径
	LPCTSTR		GetRundll32Path();
	LPCTSTR		GetRegSrvPath();

	LPCTSTR		GetWinlogonPath();
	
	LPCTSTR		GetUserStartMenuProgDir();		// 开始菜单，程序目录
	LPCTSTR		GetCommonStartMenuProgDir();	

	LPCTSTR		GetUserDesktopDir();	// 桌面
	LPCTSTR		GetCommonDesktopDir();

	LPCTSTR		GetQuickLanchDir();		// 快速启动

	LPCTSTR		GetUserStartMenuRootDir();		// 启动菜单根目录
	LPCTSTR		GetCommonStartMenuRootDir();

	LPCTSTR		GetUserStartRunDir();
	LPCTSTR		GetCommonStartRunDir();

	LPCTSTR		GetUserFavoriteDir();
	LPCTSTR		GetCommonFavoriteDir();

	BOOL		IsWindowGENT(){ return TRUE; }		// 非win95、98系统，NT4以上的系统
	BOOL		IsWindowNT4(){ return m_dwMajorVer==4; }
	BOOL		IsWindowVista(){return m_dwMajorVer==6;}
	BOOL		IsWindowXp(){return (m_dwMajorVer==5 && m_dwMinVer ==1);}
	BOOL		IsWindow2K(){return (m_dwMajorVer==5 && m_dwMinVer ==0);}
	BOOL		IsWindow5x(){return m_dwMajorVer==5;}	// 是不是5.X系列的
	BOOL		IsWindowGExp(){return (m_dwMajorVer>5)||(m_dwMajorVer==5&&m_dwMinVer>0);}	// 是不是大于XP
	BOOL		IsWindowGE2k(){return m_dwMajorVer>=5;}	// 是不是大于2K
	BOOL		IsWindow7x(){return m_dwMajorVer==7;}	
	BOOL		IsWindow2K3(){return (m_dwMajorVer==5 && m_dwMinVer==2);}
	BOOL		IsWOW64(){return m_bIsWOW64;}

	BOOL		FixPathLastSpec(LPTSTR lpPath);	// 把最后一位 '\' 去掉

	VOID		GetLongPath(LPCTSTR lpFile, CString& strLongFile);

protected:
	BOOL	GetIsWOW64();
protected:
	HMODULE		m_hMod;
	DWORD		m_dwMajorVer;
	DWORD		m_dwMinVer;
	BOOL		m_bIsWOW64;
};

CGetGlobalState* GetGlobalState();
