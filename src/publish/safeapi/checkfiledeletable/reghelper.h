#ifndef _REG_HELPER_H_
#define _REG_HELPER_H_
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
						   );


LONG
RegHelperQueryMultiStringValue (
								HKEY hMainKey,
								LPWSTR lpSubkey,
								LPWSTR lpValueName,
								PVOID lpValue,
								PLONG lpcbValue
								);

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
					   );

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
					);
//-----------------------------------------------------------------------------//
#endif