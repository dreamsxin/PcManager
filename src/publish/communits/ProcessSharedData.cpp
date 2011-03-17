#include "StdAfx.h"
#include "ProcessSharedData.h"
#include "Vista.h"
#include "CommFuns.h"

CProcessSharedData::CProcessSharedData(void)
{
	m_hFileMap = NULL;
	m_lpFileData = NULL;
	m_dwFileSize = 0;
	m_hMutex = NULL;
}

CProcessSharedData::~CProcessSharedData(void)
{
	Close();
}

BOOL CProcessSharedData::Create(LPCTSTR lpName, DWORD dwSize)
{
	assert (m_hFileMap == NULL);
	assert (m_lpFileData == NULL);

	BOOL bResult = FALSE;
	
	CDacl dacl;
	dacl.AddAllowedAce( Sids::World(), FILE_ALL_ACCESS );
	dacl.AddAllowedAce( Sids::Admins(), FILE_ALL_ACCESS  );

	CSecurityDesc sd;
	sd.SetDacl( dacl );
	SetSecurityDescriptorToLowIntegrity(sd);

	CSecurityAttributes sa;
	sa.Set( sd );

	m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,
		&sa,
		PAGE_READWRITE,
		0,
		dwSize,
		lpName
		);
	if ( m_hFileMap != NULL )
	{
		m_lpFileData = MapViewOfFile(m_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE,
			0,
			0,
			0
			);
		if ( m_lpFileData != NULL )
		{
			TCHAR szObjName[MAX_PATH];

			CDacl dacl;
			dacl.AddAllowedAce( Sids::World(), MUTEX_ALL_ACCESS );
			dacl.AddAllowedAce( Sids::Admins(), MUTEX_ALL_ACCESS  );

			CSecurityDesc sd;
			sd.SetDacl( dacl );
			SetSecurityDescriptorToLowIntegrity(sd);

			CSecurityAttributes sa;
			sa.Set( sd );

			_sntprintf(szObjName, MAX_PATH, _T("%s_psd_mutex"), lpName);
			szObjName[MAX_PATH - 1] = 0;
			m_hMutex = CreateMutex(&sa, FALSE, szObjName);
			if ( m_hMutex != NULL )
			{
				bResult = TRUE;
			}

			m_dwFileSize = dwSize;
		}
		else
		{
//			DebugPrintf(_T("CProcessSharedData::Create MapViewOfFile ß∞‹0x%08X"), GetLastError());
		}
	}
	else
	{
//		DebugPrintf(_T("CProcessSharedData::Create CreateFileMapping ß∞‹0x%08X"), GetLastError());
	}

	if ( !bResult )
	{
		Close();
		return FALSE;
	}

	return TRUE;
}

BOOL CProcessSharedData::Open(LPCTSTR lpName, DWORD dwSize)
{
	assert(m_hFileMap == NULL);
	assert(m_lpFileData == NULL);
	BOOL bResult = FALSE;

	m_hFileMap = OpenFileMapping(FILE_MAP_WRITE | FILE_MAP_READ,
		FALSE,
		lpName
		);
	if ( m_hFileMap != NULL )
	{
		m_lpFileData = MapViewOfFile(m_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE,
			0,
			0,
			0
			);
		if ( m_lpFileData != NULL )
		{
			TCHAR szObjName[MAX_PATH];

			_sntprintf(szObjName, MAX_PATH, _T("%s_psd_mutex"), lpName);
			szObjName[MAX_PATH - 1] = 0;

			m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, szObjName);
			if ( m_hMutex != NULL )
			{
				bResult = TRUE;
			}
			else
			{
		//		DebugPrintf(_T("CProcessSharedData::Open: OpenMutex ß∞‹0x%08X"), GetLastError());
			}

			m_dwFileSize = dwSize;
		}
		else
		{
//			DebugPrintf(_T("CProcessSharedData::Open: MapViewOfFile ß∞‹0x%08X"), GetLastError());
		}
	}
	else
	{
//		DebugPrintf(_T("CProcessSharedData::Open: OpenFileMapping ß∞‹0x%08X"), GetLastError());
	}

	if ( !bResult )
	{
		Close();
		return FALSE;
	}

	return bResult;
}

void CProcessSharedData::Close()
{
	if ( m_lpFileData != NULL )
	{
		UnmapViewOfFile(m_lpFileData);
		m_lpFileData = NULL;
	}

	if ( m_hFileMap != NULL )
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}

	if ( m_hMutex != NULL )
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}