#include "stdafx.h"
#include "kopksafereg.h"
#include <atlbase.h>
#include "kcalloem.h"
#include "kstringex.h"

std::wstring KOpKsafeReg::_GetSubKeyStr(void)
{
	std::wstring strSafeReg;

	if (KisPublic::Instance()->Init())
	{
		std::wstring strValueT;
		int nRet = KisPublic::Instance()->KQueryOEMLPCWSTR(KIS::KOemKsfe::oemc_SubKeyStr, strValueT);
		if (nRet != -1)
			strSafeReg = strValueT;
		else
			strSafeReg = _T("\\KSafe\\");
	}
	else
		strSafeReg = _T("\\KSafe\\");

	if (strSafeReg.size() == 0)
		strSafeReg = _T("\\KSafe\\");

	return strSafeReg;
}

BOOL KOpKsafeReg::CreateKsafeReg(LPCWSTR lpPath)
{
	std::wstring strSafeRegIndex = _T("SOFTWARE\\");
	strSafeRegIndex += _GetSubKeyStr();

	CRegKey reg;
	LONG lRet = reg.Create(HKEY_LOCAL_MACHINE, strSafeRegIndex.c_str());
	if (lRet == ERROR_SUCCESS)
		return TRUE;

	WCHAR szTemp[2];

	DWORD dwRet = 1;

	lRet = reg.QueryStringValue(L"Install Path", szTemp, &dwRet);
	if (ERROR_FILE_NOT_FOUND == lRet)
	{
		reg.SetStringValue(L"Install Path", lpPath);
	}
	
	return FALSE;
}

BOOL KOpKsafeReg::QueryKsafePath(std::wstring strKey, std::wstring strValueKey, OUT std::wstring& strValue)
{
	std::wstring strSafeRegIndex = _T("SOFTWARE\\");
	strSafeRegIndex += _GetSubKeyStr();
	strSafeRegIndex += _T("\\");
	strSafeRegIndex += strKey;

	CRegKey key;
	LONG lRet = key.Open(HKEY_LOCAL_MACHINE, strSafeRegIndex.c_str(), KEY_READ);
	if (lRet != ERROR_SUCCESS)
		return FALSE;

	wchar_t szPath[MAX_PATH] = {0};
	ULONG nSize = MAX_PATH;
	lRet = key.QueryStringValue(strValueKey.c_str(), szPath, &nSize);
	if (nSize > MAX_PATH)
	{
		wchar_t* pszPath = new wchar_t[nSize + 1];
		lRet = key.QueryStringValue(strValueKey.c_str(), pszPath, &nSize);
		if (lRet != ERROR_SUCCESS)
			return FALSE;

		strValue = pszPath;

		if (pszPath != NULL)
		{
			delete[] pszPath;
			pszPath = NULL;
		}
	}
	else
		strValue = szPath;

	return TRUE;
}

std::wstring KOpKsafeReg::ReplaceRegStr(IN LPCWSTR lpStr)
{
	KWString strTemp(lpStr);
	std::wstring strReturn;
	static std::wstring strValue;

	if (strValue.size() == 0)
		strValue = _GetSubKeyStr();

	BOOL bIsAdd = FALSE;
	if (strTemp.Right(1) != _T("\\"))
	{
		bIsAdd = TRUE;
		strTemp.append(_T("\\"));
	}
	
	strTemp.MakeLower();
	strTemp.Replace(_T("\\ksafe\\"), strValue.c_str());

	if (bIsAdd)
	{
		int nLen = strTemp.GetLength();
		strTemp = strTemp.Left(nLen - 1);
	}

	return strTemp.c_str();
}

BOOL KOpKsafeReg::QueryKsafePath(std::wstring strKey, std::wstring strValueKey, OUT DWORD& dwValue)
{
	std::wstring strSafeRegIndex = _T("SOFTWARE\\");
	strSafeRegIndex += _GetSubKeyStr();
	strSafeRegIndex += _T("\\");
	strSafeRegIndex += strKey;

	CRegKey key;
	LONG lRet = key.Open(HKEY_LOCAL_MACHINE, strSafeRegIndex.c_str(), KEY_READ);
	if (lRet != ERROR_SUCCESS)
		return FALSE;

	DWORD dwValueT = 0;
	lRet = key.QueryDWORDValue(strValueKey.c_str(), dwValueT);
	if (lRet != ERROR_SUCCESS)
		return FALSE;

	dwValue = dwValueT;

	return TRUE;
}
