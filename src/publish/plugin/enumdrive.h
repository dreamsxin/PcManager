#pragma once

#include <atlcoll.h>
#include <atlstr.h>
#include <WinIoCtl.h>

class CEnumDrive
{
public:
	CEnumDrive()
	{
		UINT uOldErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS);
		
		for ( char root='c'; root <= 'z'; root++)
		{
			DWORD		nType;
			TCHAR		szDriver[] = _T("C:");
			szDriver[0] += (root-'c');
			nType = GetDriveType(szDriver);

			if (nType==DRIVE_REMOVABLE||nType==DRIVE_FIXED)
			{
				if (IsDriveExist(szDriver[0]))
					m_listdrive.Add(szDriver);
			}
		}
		// »Ö¸´ErrorMode;
		SetErrorMode(uOldErrorMode);
	}
	~CEnumDrive()
	{
		m_listdrive.RemoveAll();
	}

	int GetCount()
	{
		return m_listdrive.GetSize();
	}

	LPCTSTR operator[](int iIndex)
	{
		return m_listdrive[iIndex];
	}

	BOOL IsFileInCorrectDrive(LPCTSTR lpFileName)
	{
		if (lpFileName==NULL)
			return FALSE;

		if (_tcslen(lpFileName)<2)
			return FALSE;

		for (int i = 0; i < m_listdrive.GetSize(); i++)
		{
			if ( _tcsnicmp(m_listdrive[i],lpFileName,2)==0)
				return TRUE;
		}

		return FALSE;
	}

protected:
	BOOL IsDriveExist(TCHAR cRoot)
	{
		WCHAR szRootPath[MAX_PATH] = L"\\\\?\\";
		szRootPath[4] = cRoot;
		szRootPath[5] = L':';
		szRootPath[6] = L'\0';

		BOOL bAccessible = FALSE;
		CAtlFile hDevice;
		HRESULT hr = hDevice.Create(
			szRootPath,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			OPEN_EXISTING);
		if (SUCCEEDED(hr))
		{
			DWORD dwBytesAct  = 0;
			bAccessible = ::DeviceIoControl(  
				hDevice,  
				IOCTL_DISK_CHECK_VERIFY,  
				NULL,  
				0,  
				NULL,  
				0,  
				&dwBytesAct,  
				NULL);
		}

		return bAccessible;
	}

protected:
	CSimpleArray<CString>	m_listdrive;
};