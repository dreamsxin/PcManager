//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KOSVersion.cpp
//  Creator     :   YangXD(yxdh)
//  Date        :   2008-1-24 15:14:33
//  Comment     :   Implementation of the OS_VER namespace.
//
//////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KOSVersion.h"
#include <Windows.h>


bool OS_VER::IsNTPlatform()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;


	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
	if(!bOsVersionInfoEx)
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi)) 
			return false;
	}


	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool OS_VER::_CheckNTVersion(unsigned long ulMajorVersion, unsigned long ulMinorVersion)
{
	OSVERSIONINFO osi;


	ZeroMemory(&osi, sizeof(OSVERSIONINFO));
	osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osi))
		return false;

	if (VER_PLATFORM_WIN32_NT != osi.dwPlatformId)
		return false;
	

	return (
		(osi.dwMajorVersion > ulMajorVersion) 
		|| 
		(osi.dwMajorVersion == ulMajorVersion && osi.dwMinorVersion >= ulMinorVersion));
}

bool OS_VER::IsWinXPOrLater()
{
	return OS_VER::_CheckNTVersion(5, 1);
}

bool OS_VER::IsWin2KOrLater()
{
	return OS_VER::_CheckNTVersion(5, 0);
}
