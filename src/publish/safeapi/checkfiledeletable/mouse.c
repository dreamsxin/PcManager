#include <Windows.h>
#include <stdio.h>
#include "reghelper.h"
#include "helper.h"
#include "helper2.h"
#include "udlist.h"
//-----------------------------------------------------------------------------//
LONG
ListMouseFilter (
	VOID
	)
{
	LONG lResult;

	CHAR pMultiStringData[1024] = {0};
	LONG lDataSize = 1024;
	PWCHAR aString[10] = {0};
	LONG lStringCount = 10;
	LONG i;

	WCHAR wstrImagePath[MAX_PATH + 1];
	LONG lImagePathBufferSize = 0;


	lResult = RegHelperQueryMultiStringValue(HKEY_LOCAL_MACHINE,
		L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}",
		L"UpperFilters",
		pMultiStringData, &lDataSize
		);
	if (ERROR_SUCCESS != lResult)
	{
		goto Exit0;
	}


	lResult = ParseSpecialBuffer2_MultiString(pMultiStringData, lDataSize, aString, &lStringCount);
	if (ERROR_SUCCESS != lResult)
	{
		//
		// not handled for now
		//
		goto Exit0;
	}

	for (i = 0; i < lStringCount; i++)
	{
		lImagePathBufferSize = MAX_PATH * sizeof(WCHAR);
		lResult = RegistryHelperGetServiceImagePath(aString[i], wstrImagePath, &lImagePathBufferSize);
		if (ERROR_SUCCESS == lResult)
		{
			printf("%d: %S\n", i, wstrImagePath);
			AddPath(wstrImagePath);
		}
	}


Exit0:
	return lResult;
}
//-----------------------------------------------------------------------------//