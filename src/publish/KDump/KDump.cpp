#include "stdafx.h"
#include "common/utility.h"
#include "KDump.h"
#include "skylark2/midgenerator.h"

KDump KDump::s_Instance;


KDump::KDump():m_bInited(FALSE)
{
	m_bInited = InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
	m_bRegistered = FALSE;
}

KDump::~KDump()
{

}

BOOL KDump::Install()
{
	if( !m_bInited ) return FALSE;
	CString sPath;
	CAppPath::Instance().GetLeidianAppPath(sPath);
	sPath += L"\\kdump.dll";

	EnterCriticalSection(&m_cs);
	if( m_bRegistered )
	{
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}

	CString     strMid;
    Skylark::CMidGenerator::Instance().GetMid( strMid );

	typedef void (__stdcall *PFN_KxEOpenDumpMonitorEx2)(bool, const wchar_t*, DWORD, DWORD);
    typedef void (__stdcall *PFN_KxESetTrialMode)(bool);
    HMODULE hLib = LoadLibrary(sPath);
    if (hLib)
    {
		PFN_KxEOpenDumpMonitorEx2 OpenKingsoftDumpEx2 = NULL;
		PFN_KxESetTrialMode KxESetTrialMode = NULL;     
		OpenKingsoftDumpEx2 = (PFN_KxEOpenDumpMonitorEx2)GetProcAddress(hLib, "KxEOpenDumpMonitorEx2");
		KxESetTrialMode = (PFN_KxESetTrialMode)GetProcAddress(hLib, "KxESetTrialMode");
		if (OpenKingsoftDumpEx2)
		{
			OpenKingsoftDumpEx2(true, strMid, 0x111000, 0);
			m_bRegistered = TRUE;
			
		}
	}
	LeaveCriticalSection(&m_cs);
	return m_bRegistered;
}