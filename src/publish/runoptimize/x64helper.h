#pragma once
// add wow64 switcher for RunManEngine [11/5/2010 DuBing]
class KWow64Switcher
{
public:
	KWow64Switcher()
	{
		m_pfWow64DisableWow64FsRedirection = NULL;  
		m_pfWow64RevertWow64FsRedirection = NULL;  
		m_pfIsWow64Porcess = NULL;
		m_pOldValue = NULL;  
		m_bCloseRedirection = FALSE;  
		m_hModKrl = NULL;  
	}
	~KWow64Switcher()
	{
		if (m_hModKrl)  
		{
			FreeLibrary(m_hModKrl);  
			m_hModKrl = NULL;  
		}
	}

	BOOL Init()
	{
		BOOL bRet = FALSE;  
		m_hModKrl = LoadLibrary(TEXT("Kernel32.dll"));  
		if (!m_hModKrl)  
		{  
			goto Exit0;  
		}  

		m_pfWow64DisableWow64FsRedirection = (WOW64DISABLEWOW64FSREDIRECTION)GetProcAddress(m_hModKrl, "Wow64DisableWow64FsRedirection");   
		m_pfWow64RevertWow64FsRedirection = (WOW64REVERTWOW64FSREDIRECTION)GetProcAddress(m_hModKrl, "Wow64RevertWow64FsRedirection");  
		m_pfIsWow64Porcess = (FunctionIsWow64Process)GetProcAddress(m_hModKrl , "IsWow64Process");

		if (!m_pfWow64DisableWow64FsRedirection || !m_pfWow64RevertWow64FsRedirection || !m_pfIsWow64Porcess)  
		{  
			goto Exit0;  
		}

		bRet = TRUE;  
	Exit0:  
		return bRet;  
	}

	BOOL IsWin64()
	{
		BOOL bRet = FALSE;

		if (!m_pfIsWow64Porcess)
		{
			bRet = FALSE;
			goto Exit0;
		}
		HANDLE hCurrentProcess = GetCurrentProcess();
		m_pfIsWow64Porcess(hCurrentProcess, &bRet);
Exit0:
		return bRet;
	}

	BOOL IsWin64_2()
	{
		BOOL bRet = FALSE;
		static HMODULE hKernel32 = NULL;
		if( !hKernel32 )
			hKernel32 = ::LoadLibrary(TEXT("Kernel32.DLL"));
		if( !hKernel32 )
		{
			bRet = FALSE;
			goto Exit0;
		}

		typedef BOOL (WINAPI *FunctionIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);
		FunctionIsWow64Process pfnIsWow64Process = NULL;
		pfnIsWow64Process = (FunctionIsWow64Process)GetProcAddress(hKernel32, "IsWow64Process");

		if (NULL == pfnIsWow64Process)
		{
			bRet = FALSE;
			goto Exit0;
		}

		HANDLE hCurrentProcess = GetCurrentProcess();
		pfnIsWow64Process(hCurrentProcess, &bRet);
Exit0:
		return bRet;
	}

	BOOL Close()
	{
		BOOL bRet = FALSE;

		if (m_bCloseRedirection)  
		{  
			bRet = TRUE;  
			goto Exit0;
		}  

		if (!m_pfWow64DisableWow64FsRedirection)  
		{
			bRet = FALSE;
			goto Exit0;
		}

		bRet = m_pfWow64DisableWow64FsRedirection(&m_pOldValue);  
		if (bRet)  
		{  
			m_bCloseRedirection = TRUE;  
			bRet = TRUE;  
			goto Exit0;
		}
Exit0:
		return bRet;
	}

	BOOL Revert()
	{
		BOOL bRet = FALSE;  

		if (!m_bCloseRedirection)  
		{  
			bRet = TRUE;
			goto Exit0;
		}  

		if (!m_pfWow64RevertWow64FsRedirection)  
		{	
			bRet = FALSE;  
			goto Exit0;
		}

		bRet = m_pfWow64RevertWow64FsRedirection(&m_pOldValue);  
		if (bRet)  
		{  
			m_bCloseRedirection = FALSE;  
			bRet = TRUE;  
			goto Exit0;
		}  
Exit0:
		return bRet;
	}

	BOOL HasClosed()
	{
		return m_bCloseRedirection;
	}

private:
	typedef BOOL (__stdcall *WOW64DISABLEWOW64FSREDIRECTION)(PVOID*);
	typedef BOOL (__stdcall *WOW64REVERTWOW64FSREDIRECTION)(PVOID*);
	typedef BOOL (WINAPI *FunctionIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);

	WOW64DISABLEWOW64FSREDIRECTION m_pfWow64DisableWow64FsRedirection;
	WOW64REVERTWOW64FSREDIRECTION  m_pfWow64RevertWow64FsRedirection;
	FunctionIsWow64Process		   m_pfIsWow64Porcess;

	PVOID m_pOldValue;
	BOOL  m_bCloseRedirection;
	HMODULE	m_hModKrl;
};