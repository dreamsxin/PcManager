#include <Windows.h>
#include <stdio.h>
#include <sfc.h>
#include "udlist.h"
#include "export.h"

#pragma comment(lib, "sfc.lib")
//-----------------------------------------------------------------------------//
int main(void)
{
	LONG Status;

	Status = InitUdList();


	//bResult = SfcIsFileProtected(NULL, L"C:\\WINDOWS\\system32\\xmllite1.dll");


	Status = DebugDisplayUdList();


	Status = CheckFileDeletable(L"system32\\DRIVERS\\mouclass.sys");
	printf("%d\n", Status);

	Status = CheckFileDeletable(L"windows\\system32\\DRIVERS\\mouclass.sys");
	printf("%d\n", Status);

	Status = CheckFileDeletable(L"c:\\windows\\system32\\DRIVERS\\mouclass.sys");
	printf("%d\n", Status);

	Status = CheckFileDeletable(L"\\??\\c:\\windows\\system32\\DRIVERS\\mouclass.sys");
	printf("%d\n", Status);

	Status = CheckFileDeletable(L"\\??\\c:\\windows\\system32\\DRIVERS\\mouclass.sys");
	printf("%d\n", Status);

	
	Status = CheckFileDeletable(L"\\??\\C:\\WINDOWS\\system32\\drivers\\VMkbd.sys");
	printf("%d\n", Status);

	Status = CheckFileDeletable(L"system32\\drivers\\VMkbd.sys");
	printf("%d\n", Status);


	Status = CheckFileDeletable(L"c:\\windows\\system32\\drivers\\tcpip.sys");
	printf("%d\n", Status);

	return 0;
}
//-----------------------------------------------------------------------------//