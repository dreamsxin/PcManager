

#include "StdAfx.h"
#include "Vista.h"
#include <AccCtrl.h>
#include <AclAPI.h>

/*
CEveryoneSA::CEveryoneSA(DWORD dwAccessPermissions)
{
    m_lpACL = NULL;
	m_lpEveryoneSID = NULL;
	m_lpSD = NULL;

	m_sa.lpSecurityDescriptor = NULL;
	m_sa.bInheritHandle = FALSE;
	m_sa.nLength = 0;

	Init(dwAccessPermissions);
}

CEveryoneSA::~CEveryoneSA()
{
	if( m_lpEveryoneSID != NULL )
    {
        FreeSid(m_lpEveryoneSID);
		m_lpEveryoneSID = NULL;
    }
	
    if ( m_lpACL != NULL )
    {
        LocalFree(m_lpACL);
		m_lpACL = NULL;
    }
	
    if ( m_lpSD != NULL )
    {
        LocalFree(m_lpSD);
		m_lpSD = NULL;
    }
}

void CEveryoneSA::Init(DWORD dwAccessPermissions)
{
	do
    {
		SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
		EXPLICIT_ACCESS ea[1];
		DWORD dwRet;

        // Create a well-known SID for the Everyone group.
		if( !AllocateAndInitializeSid(&SIDAuthWorld,
			1,
			SECURITY_WORLD_RID,
			0, 0, 0, 0, 0, 0, 0,
			&m_lpEveryoneSID)
			)
        {
            break;
        }
		
        // Initialize an EXPLICIT_ACCESS structure for an ACE.
        ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
        ea[0].grfAccessPermissions	= dwAccessPermissions;
        ea[0].grfAccessMode			= SET_ACCESS;
        ea[0].grfInheritance		= NO_INHERITANCE;
        ea[0].Trustee.TrusteeForm	= TRUSTEE_IS_SID;
        ea[0].Trustee.TrusteeType	= TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[0].Trustee.ptstrName		= (LPTSTR)m_lpEveryoneSID;
		
        // Create a new ACL that contains the new ACEs.
        dwRet = SetEntriesInAcl(1, ea, NULL, &m_lpACL);
        if( ERROR_SUCCESS != dwRet )
        {
            break;
        }
		
        // Initialize a security descriptor. 
        m_lpSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
			SECURITY_DESCRIPTOR_MIN_LENGTH
			);
        if( NULL == m_lpSD )
        {
            break;
        }
		
        if( !InitializeSecurityDescriptor(m_lpSD, SECURITY_DESCRIPTOR_REVISION ) )
        { 
            break;
        }
		
        // Add the ACL to the security descriptor.
        if (!SetSecurityDescriptorDacl(m_lpSD,
			TRUE,    // bDaclPresent flag 
			m_lpACL,
			FALSE)
			)  // not a default DACL
        {
            break;
        }
		
        // Initialize a security attributes structure.
        m_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        m_sa.lpSecurityDescriptor = m_lpSD;
        m_sa.bInheritHandle = FALSE;
	} while ( FALSE );
}

PSECURITY_ATTRIBUTES CEveryoneSA::FileAllAccess()
{
	static CEveryoneSA sa(FILE_ALL_ACCESS);
	return sa;
}

PSECURITY_ATTRIBUTES CEveryoneSA::EventAllAccess()
{
	static CEveryoneSA sa(EVENT_ALL_ACCESS);
	return sa;
}

PSECURITY_ATTRIBUTES CEveryoneSA::MutexAllAccess()
{
	static CEveryoneSA sa(MUTEX_ALL_ACCESS);
	return sa;
}
*/

BOOL WINAPI	IsVistaLater()
{
	OSVERSIONINFO osInfo;
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	::GetVersionEx(&osInfo);

	return osInfo.dwMajorVersion >= 6;
}

BOOL WINAPI	IsOpenUAC()
{
	BOOL bResult = FALSE;

    if( IsVistaLater() )
    {
		LONG	lResult;
		HKEY	hKey;
		DWORD	dwEnableLUA;
		DWORD	dwType = REG_DWORD;
		DWORD	dwSize = sizeof( DWORD );

        lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\"),
			0,
			KEY_READ,
			&hKey
			);
        if( lResult == ERROR_SUCCESS )
        {
            lResult = RegQueryValueEx(hKey,
				_T("EnableLUA"),
				NULL,
				&dwType,
				(BYTE*)&dwEnableLUA,
				&dwSize
				);
			bResult = (lResult == ERROR_SUCCESS) && (0 != dwEnableLUA);

            RegCloseKey(hKey);
        }
    }

    return bResult;
}

BOOL EnablePrivilege(HANDLE hToken, LPCTSTR szPrivName)
{
	TOKEN_PRIVILEGES tkp;
	
	LookupPrivilegeValue(NULL, szPrivName, &tkp.Privileges[0].Luid);//修改进程权限
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);//通知系统修改进程权限

	return((GetLastError() == ERROR_SUCCESS));
}

typedef BOOL (WINAPI *PFN_ChangeWindowMessageFilter)(UINT uMessage, int nFlag);
BOOL WINAPI	Vista_ChangeWindowMessageFilter(UINT uMessage, int nFlag)
{
	HMODULE hUserMod;
	PFN_ChangeWindowMessageFilter pfnChangeWindowMessageFilter;

	hUserMod = GetModuleHandle(_T("user32.dll"));
	pfnChangeWindowMessageFilter = (PFN_ChangeWindowMessageFilter)GetProcAddress(hUserMod, "ChangeWindowMessageFilter");

	return (pfnChangeWindowMessageFilter != NULL) && pfnChangeWindowMessageFilter(uMessage, nFlag);
}
