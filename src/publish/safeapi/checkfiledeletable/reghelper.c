#include <Windows.h>
//-----------------------------------------------------------------------------//
//
// lpcbValue: in bytes
// 
// Return: ERROR_SUCCESS
//			ERROR_MORE_DATA
//			ERROR_DATATYPE_MISMATCH
//
LONG
RegHelperQueryStringValue (
	HKEY hMainKey,
	LPWSTR lpSubkey,
	LPWSTR lpValueName,
	LPWSTR lpValue,
	PLONG lpcbValue
	)
{
	LONG lResult;
	HKEY hSubKey = NULL;
	DWORD dwType;

	lResult = RegOpenKey(hMainKey, lpSubkey, &hSubKey);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}

	lResult = RegQueryValueEx(hSubKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue, (PDWORD)lpcbValue);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}

	if ((REG_SZ != dwType) && (REG_EXPAND_SZ != dwType))
	{
		lResult = ERROR_DATATYPE_MISMATCH;
		goto Exit0;
	}

Exit0:
	if (NULL != hSubKey)
	{
		RegCloseKey(hSubKey);
	}
	return lResult;
}
//-----------------------------------------------------------------------------//
LONG
RegHelperQueryMultiStringValue (
	HKEY hMainKey,
	LPWSTR lpSubkey,
	LPWSTR lpValueName,
	PVOID lpValue,
	PLONG lpcbValue
	)
{
	LONG lResult;
	HKEY hSubKey = NULL;
	DWORD dwType;

	lResult = RegOpenKey(hMainKey, lpSubkey, &hSubKey);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}

	lResult = RegQueryValueEx(hSubKey, lpValueName, NULL, &dwType, (LPBYTE)lpValue, (PDWORD)lpcbValue);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}

	if (REG_MULTI_SZ != dwType)
	{
		lResult = ERROR_DATATYPE_MISMATCH;
		goto Exit0;
	}

Exit0:
	if (NULL != hSubKey)
	{
		RegCloseKey(hSubKey);
	}
	return lResult;
}
//-----------------------------------------------------------------------------//
LONG
RegHelperQueryInfoKey (
	IN  HKEY hMainKey,
	OUT LPWSTR lpSubkey,
	IN OUT LPTSTR lpClass OPTIONAL,
	OUT LPDWORD lpcClass OPTIONAL,
	OUT LPDWORD lpReserved OPTIONAL,
	OUT LPDWORD lpcSubKeys OPTIONAL,
	OUT LPDWORD lpcMaxSubKeyLen OPTIONAL,
	OUT LPDWORD lpcMaxClassLen OPTIONAL,
	OUT LPDWORD lpcValues OPTIONAL,
	OUT LPDWORD lpcMaxValueNameLen OPTIONAL,
	OUT LPDWORD lpcMaxValueLen OPTIONAL,
	OUT LPDWORD lpcbSecurityDescriptor OPTIONAL,
	OUT PFILETIME lpftLastWriteTime OPTIONAL
	)
{
	LONG lResult;
	HKEY hSubKey = NULL;

	lResult = RegOpenKey(hMainKey, lpSubkey, &hSubKey);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}

	lResult = RegQueryInfoKeyW(hSubKey,
		lpClass,
		lpcClass,
		lpReserved,
		lpcSubKeys,
		lpcMaxSubKeyLen,
		lpcMaxClassLen,
		lpcValues,
		lpcMaxValueNameLen,
		lpcMaxValueLen,
		lpcbSecurityDescriptor,
		lpftLastWriteTime
		);

Exit0:
	if (NULL != hSubKey)
	{
		RegCloseKey(hSubKey);
	}
	return lResult;
}
//-----------------------------------------------------------------------------//
LONG
RegHelperEnumKeyEx (
	IN  HKEY hMainKey,
	OUT LPWSTR lpSubkey,
	IN  DWORD dwIndex,
	OUT LPTSTR lpName,
	IN OUT LPDWORD lpcName,
	IN  LPDWORD lpReserved,
	IN  LPTSTR lpClass,
	IN OUT LPDWORD lpcClass,
	IN  PFILETIME lpftLastWriteTime
	)
{
	LONG lResult;
	HKEY hSubKey = NULL;

	lResult = RegOpenKey(hMainKey, lpSubkey, &hSubKey);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}

	lResult = RegEnumKeyExW(hSubKey,
		dwIndex,
		lpName,
		lpcName,
		lpReserved, lpClass,
		lpcClass,
		lpftLastWriteTime
		);

Exit0:
	if (NULL != hSubKey)
	{
		RegCloseKey(hSubKey);
	}
	return lResult;
}
//-----------------------------------------------------------------------------//