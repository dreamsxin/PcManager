#include <windows.h>
#include <Sfc.h>
#include "udlist.h"
#include "export.h"
//-----------------------------------------------------------------------------//
#pragma comment(lib, "sfc.lib")
//-----------------------------------------------------------------------------//
NTSTATUS
Init (
	 VOID
	 )
{
	LONG Status;

	Status = InitUdList();

	return Status;
}
//-----------------------------------------------------------------------------//
NTSTATUS
Uninit (
	VOID
	)
{
	LONG Status;

	Status = UninitUdList();

	return Status;
}
//-----------------------------------------------------------------------------//
NTSTATUS
CheckFileDeletable (
	PWCHAR pwszFilePath
	)
{
	BOOL bResult;

	//bResult = IsInIgList(pwszFilePath);
	//if (TRUE == bResult)
	//{
	//	return UD_RESULT_UNDELETABLE_FILE;
	//}

	bResult = IsInUdList(pwszFilePath);
	if (TRUE == bResult)
	{
		return UD_RESULT_UNDELETABLE_FILE;
	}

	bResult = SfcIsFileProtected(NULL, pwszFilePath);
	if (TRUE == bResult)
	{
		return UD_RESULT_SYSTEM_FILE;
	}


	return UD_RESULT_NORMAL_FILE;

}
//-----------------------------------------------------------------------------//