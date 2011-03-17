//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KSysInfo.h"
#include "IniFile.h"

//////////////////////////////////////////////////////////////////////////

int CKSysInfo::Get()
{
	int nRetCode = 0;

// 	nRetCode = IsGetOne();
// 	if (nRetCode == -1)
// 		return -1;
// 
	nRetCode = GetOSType();
	if (nRetCode == -1)
		return -1;

	nRetCode |= GetOEMType();
	nRetCode |= GetBootInfo();
	nRetCode |= GetSystemDrive();

	return nRetCode;
}

//////////////////////////////////////////////////////////////////////////

int CKSysInfo::IsGetOne()
{
	HKEY hReg = NULL;
	LONG loRetCode;
	int nResult = 0;

	loRetCode = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"software\\ksafe", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hReg, NULL);
	if (loRetCode != ERROR_SUCCESS)
		return -1;

	DWORD dwType = 0;
	DWORD dwRetCode = sizeof(DWORD);
	DWORD dwValue = 0;

	loRetCode = RegQueryValueEx(hReg, REG_CHECK_NAME, NULL, &dwType, (LPBYTE)&dwValue, &dwRetCode);
	if (loRetCode != ERROR_SUCCESS)
		goto Exit0;

	if (dwType != REG_DWORD)
		goto Exit0;

	nResult = -1;

Exit0:
	if (nResult == 0)
	{
		dwType = REG_DWORD;
		dwValue = 1;

		RegSetValueEx(hReg, REG_CHECK_NAME, NULL, dwType, (const BYTE *)&dwValue, sizeof(dwValue));
	}

	RegCloseKey(hReg);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
//Windows XP Media Center Edition 
//Windows XP Tablet PC Edition 
//Windows XP Starter Edition 
int CKSysInfo::GetOSType()
{
	OSVERSIONINFOEX OSVer;

	BOOL bRetCode;

	int nResult = 0;

	OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bRetCode = GetVersionEx((OSVERSIONINFO *)&OSVer);
	if (bRetCode == FALSE)
		return -1;

	if (OSVer.dwMajorVersion == 5 && OSVer.dwMinorVersion == 1)
	{
		if (OSVer.wProductType != VER_NT_WORKSTATION)
			return -1;

		if ( (OSVer.wSuiteMask & VER_SUITE_PERSONAL) == VER_SUITE_PERSONAL)
		{
			nResult |= WIN_XP_HOME;
		}else
		{
			nResult |= WIN_XP_PRO;
		}

		if (OSVer.wServicePackMajor == 1)
			nResult |= WIN_XP_SP1;
		else if (OSVer.wServicePackMajor == 2)
			nResult |= WIN_XP_SP2;
		else if (OSVer.wServicePackMajor == 3)
			nResult |= WIN_XP_SP3;
	}else if (OSVer.dwMajorVersion == 6 && OSVer.dwMinorVersion == 1)
	{
		PFN_GetProductInfo pGetProductInfo = (PFN_GetProductInfo)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "GetProductInfo");
		if (pGetProductInfo == NULL)
			return -1;

		DWORD dwType = 0;

		pGetProductInfo(OSVer.dwMajorVersion, OSVer.dwMinorVersion, OSVer.wServicePackMajor, OSVer.wServicePackMinor, &dwType);

		switch (dwType)
		{
		case PRODUCT_ULTIMATE:
			nResult |= WIN7_ULTIMATE;
			break;

		case PRODUCT_HOME_PREMIUM:
			nResult |= WIN7_HOME_PREMIUM;
			break;

		case PRODUCT_HOME_BASIC:
			nResult |= WIN7_HOME_BASIC;
			break;

		case PRODUCT_ENTERPRISE:
			nResult |= WIN7_ENTERPRISE;
			break;

		case PRODUCT_BUSINESS:
			nResult |= WIN7_BUSINESS;
			break;

		case PRODUCT_STARTER:
			nResult |= WIN7_STARTER;
			break;

		default:
			nResult = -1;
		}
	}else
	{
		nResult = -1;
	}

	return nResult;
}

//////////////////////////////////////////////////////////////////////////

BYTE CKSysInfo::GetResKeyCode(
	HMODULE hModule,
	UINT unResID)
{
	BYTE byResult = 0;

	HRSRC hResource = ::FindResource(hModule, MAKEINTRESOURCE(unResID), L"BINK"); 
	if (hResource == NULL) 
		return 0;

	DWORD dwImageSize = ::SizeofResource(hModule, hResource); 
	if (dwImageSize == 0) 
		return 0;

	BYTE *lpRsrc = (BYTE *)::LoadResource(hModule, hResource); 
	if (lpRsrc != NULL) 
	{
		byResult = *lpRsrc;
		::FreeResource(lpRsrc); 

		return byResult;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CKSysInfo::GetOEMType()
{
	int nResult = 0;
	CString sPath;

	GetSystemDirectory(sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();

	sPath += L"\\pidgen.dll";

	HMODULE hPidGen = LoadLibrary(sPath);
	if (hPidGen == NULL)
		return 0;

	BYTE byKey0 = 0, byKey1 = 0;

	byKey0 = GetResKeyCode(hPidGen, 1);
	byKey1 = GetResKeyCode(hPidGen, 2);

	if (byKey0 == 0 || byKey1 == 0)
		goto Exit0;

	if (byKey0 == 0x2c && byKey1 == 0x2d)
		nResult = WIN_XP_OEM_FPP;
	else if (byKey0 == 0x2e && byKey1 == 0x2f)
		nResult = WIN_XP_OEM_VLK;
	else if (byKey0 == 0x32 && byKey1 == 0x33)
		nResult = WIN_XP_OEM_OEM;

Exit0:
	if (hPidGen != NULL)
		FreeLibrary(hPidGen);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////

int CKSysInfo::GetBootInfo()
{
	int nResult = 0;

	CString sPath;

	GetSystemDirectory(sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();

	int nTemp = sPath.Find(L'\\');
	if (nTemp >= 0)
		sPath.Delete(nTemp, sPath.GetLength() - nTemp);

	sPath += L"\\boot.ini";

	CIniFile BootIni(sPath);

	if (BootIni.IsSectionExist(BOOT_INI_OPERATION_SECTION) == FALSE)
		return 0;

	DWORD dwCount = BootIni.GetKeyNameCount(BOOT_INI_OPERATION_SECTION);
	if (dwCount > 1)
		nResult |= WIN_XP_MULTI_BOOT;

	CAtlArray<CString> Names;

	BootIni.GetKeyNames(BOOT_INI_OPERATION_SECTION, &Names);

	for (int nLoop = 0; nLoop < (int)Names.GetCount(); nLoop ++)
	{
		CString sTemp = Names.GetAt(nLoop);
		sTemp.MakeUpper();

		const WCHAR *psTemp = wcsstr(sTemp, L"C:\\GRLDR");
		if (psTemp != NULL)
		{
			nResult |= WIN_XP_GRUB_BOOT;
			break;
		}
	}

	return nResult;
}

//////////////////////////////////////////////////////////////////////////

int CKSysInfo::GetSystemDrive()
{
	CString sPath;

	GetSystemDirectory(sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();

	sPath.MakeLower();

	if (sPath[0] == L'c')
		return WIN_XP_C_DRIVE;

	return 0;
}

//////////////////////////////////////////////////////////////////////////