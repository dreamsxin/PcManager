#include <Windows.h>
#include "reghelper.h"
//-----------------------------------------------------------------------------//
//
//  得到的是HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services下的服务
//
LONG
RegistryHelperGetServiceImagePath (
	IN PWCHAR pwstrServiceName,
	OUT PWCHAR pwstrImagePath,
	IN OUT PLONG plSizeInBytes
	)
{
	LONG lResult;
	WCHAR wstrServicePath[MAX_PATH] = {0};


	if ((wcslen(pwstrServiceName) + wcslen(L"SYSTEM\\CurrentControlSet\\Services\\")) > MAX_PATH)
	{
		lResult = -1;
		goto Exit0;
	}

	wcscpy(wstrServicePath, L"SYSTEM\\CurrentControlSet\\Services\\");
	wcscat(wstrServicePath, pwstrServiceName);

	lResult = RegHelperQueryStringValue(HKEY_LOCAL_MACHINE,
		                                wstrServicePath,
										L"ImagePath",
										pwstrImagePath,
										plSizeInBytes
										);
Exit0:
	return lResult;
}
//-----------------------------------------------------------------------------//