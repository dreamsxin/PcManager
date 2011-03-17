#include <windows.h>
#include <stdio.h>
#include "globaldata.h"
#include "mouse.h"
#include "keyboard.h"
#include "helper.h"
//-----------------------------------------------------------------------------//
NTSTATUS
InitUdList (
	VOID
	)
{
	LONG lResult;

	memset(&UdHead, 0, sizeof(UDHEAD));

	lResult = ListKeyboardFilter();

	lResult = ListMouseFilter();

	return ERROR_SUCCESS;
}
//-----------------------------------------------------------------------------//
NTSTATUS
UninitUdList (
	VOID
	)
{
	ULONG i = 0;

	for (i = 0; i < UdHead.Count; i++)
	{
		if (NULL != UdHead.P[i])
		{
			free(UdHead.P[i]);
			UdHead.P[i] = NULL;
		}
	}

	UdHead.Count = 0;

	return ERROR_SUCCESS;
}
//-----------------------------------------------------------------------------//
NTSTATUS
AddPath (
	PWCHAR pwszPath
	)
{
	size_t Size;

	if (UdHead.Count >= UD_MAX_PATH_COUNT)
	{
		return -1;
	}

	Size = wcslen(pwszPath) * sizeof(WCHAR) + sizeof(UNICODE_NULL);

	UdHead.P[UdHead.Count] = malloc(Size);
	if (NULL == UdHead.P[UdHead.Count])
	{
		return -1;
	}

	wcscpy(UdHead.P[UdHead.Count], pwszPath);

	UdHead.Count += 1;

	return ERROR_SUCCESS;
}
//-----------------------------------------------------------------------------//
NTSTATUS
DebugDisplayUdList (
	VOID
	)
{
	ULONG i = 0;

	for (i = 0; i < UdHead.Count; i++)
	{
		printf("%S\n", UdHead.P[i]);
	}

	return ERROR_SUCCESS;
}
//-----------------------------------------------------------------------------//
BOOL
IsInUdList (
	PWCHAR pwszPath
	)
{
	ULONG i = 0;

	for (i = 0; i < UdHead.Count; i++)
	{
		if (TRUE == MatchPath(pwszPath, UdHead.P[i]))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//-----------------------------------------------------------------------------//