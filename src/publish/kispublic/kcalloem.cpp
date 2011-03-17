#include "stdafx.h"
#include "kcalloem.h"
#include "koemksfe.h"
#include "kstringex.h"


typedef BOOL ( *DllQueryOEMInt)(int, int&);
typedef BOOL ( *DllQueryOEMLPCWSTR)(int, LPWSTR, int&);

KisPublic KisPublic::m_pKisPublic;

int KisPublic::Init( LPCWSTR lpFileName )
{
	if (m_hDll != NULL)
		return TRUE;

	KWString strFilePath;
	DWORD dwRet = 0;
	WCHAR szFilePath[MAX_PATH] = {0};
	if (lpFileName == NULL)
		dwRet = ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	else
		dwRet = ::GetModuleFileName(GetModuleHandle(lpFileName), szFilePath, MAX_PATH);

	if (0 != dwRet)
	{
		strFilePath = szFilePath;
		strFilePath.PathRemoveFileSpec();
	}

	strFilePath.append(_T("kispublic.dll"));

	if (!::PathFileExists(strFilePath.c_str()))
		return FALSE;

	m_hDll = ::LoadLibrary(strFilePath.c_str());
	if (m_hDll == NULL)
		return FALSE;

	return TRUE;
}

int KisPublic::Unit( void )
{
	if (m_hDll != NULL)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

	return TRUE;
}

int KisPublic::KQueryOEMIntA( IN int nKey, IN int& nValue )
{
	KLocker locker(m_pLockerCS);

	DllQueryOEMInt pQueryOemInt = (DllQueryOEMInt)GetProcAddress(m_hDll, "QueryOEMInt");
	if (pQueryOemInt != NULL)
		return pQueryOemInt(nKey, nValue);

	return FALSE;
}

KisPublic::KisPublic()
{
	m_pLockerCS = new KLockerEvent(_T("{E5D0EC25-CD0B-4aab-BF40-581E279D0E65}oem")); 
	m_hDll = NULL;
}

int KisPublic::KQueryOEMLPCWSTR( IN int nKey, IN std::wstring& strValue )
{
	KLocker locker(m_pLockerCS);

	WCHAR szValue[MAX_PATH] = {0};
	int nSize = MAX_PATH;
	DllQueryOEMLPCWSTR pQueryOemLpcwstr = (DllQueryOEMLPCWSTR)GetProcAddress(m_hDll, "QueryOEMStrW");

	BOOL bRet = CallFunc<int, int, LPWSTR, size_t&>(pQueryOemLpcwstr, nKey, strValue);

	return bRet;		
}

KisPublic* KisPublic::Instance( void )
{
	return &m_pKisPublic;
}