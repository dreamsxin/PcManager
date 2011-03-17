#include <Windows.h>
//-----------------------------------------------------------------------------//
ULONGLONG*
GetFileClusters(
				PWCHAR lpFileName,
				ULONG ClusterSize, 
				ULONG *ClCount,
				ULONG *FileSize 
				)
{
	HANDLE  hFile;
	ULONG   OutSize;
	ULONG   Bytes, Cls, CnCount, r;
	ULONGLONG *Clusters = NULL;
	BOOLEAN Result = FALSE;
	LARGE_INTEGER PrevVCN, Lcn;
	STARTING_VCN_INPUT_BUFFER  InBuf;
	PRETRIEVAL_POINTERS_BUFFER OutBuf;

	hFile = CreateFile(lpFileName, FILE_READ_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, 0, 0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		*FileSize = GetFileSize(hFile, NULL);

		OutSize = sizeof(RETRIEVAL_POINTERS_BUFFER) + (*FileSize / ClusterSize) * sizeof(OutBuf->Extents);

		OutBuf = (PRETRIEVAL_POINTERS_BUFFER)malloc(OutSize);

		InBuf.StartingVcn.QuadPart = 0;

		if (DeviceIoControl(hFile, FSCTL_GET_RETRIEVAL_POINTERS, &InBuf, 
			sizeof(InBuf), OutBuf, OutSize, &Bytes, NULL))
		{
			*ClCount = (*FileSize + ClusterSize - 1) / ClusterSize;

			Clusters = (PULONGLONG)malloc(*ClCount * sizeof(ULONGLONG));

			PrevVCN = OutBuf->StartingVcn;

			for (r = 0, Cls = 0; r < OutBuf->ExtentCount; r++)
			{
				Lcn = OutBuf->Extents[r].Lcn;

				for (CnCount = OutBuf->Extents[r].NextVcn.QuadPart - PrevVCN.QuadPart;
					CnCount; CnCount--, Cls++, Lcn.QuadPart++) Clusters[Cls] = Lcn.QuadPart;

					PrevVCN = OutBuf->Extents[r].NextVcn;
			}
		}

		free(OutBuf);	

		CloseHandle(hFile);
	}
	return Clusters;
}
//-----------------------------------------------------------------------------//
//
//Note:
//    Caller release buffer
//
//Parameters:
//
// pwstrFilePath  
//     Pointer to a null-terminated string that names the file to be opened
//
// ppWholeFileBuffer
//     晕，提供个指针，接收读出的文件buffer，调用者负责释放
//
// PULONG pulBufferLength
//     接收buffer长度
//
LONG
DISKRawReadWholeFile(
					 PWCHAR pwstrFilePath,
					 PVOID* ppWholeFileBuffer,
					 PULONG pulBufferLength
					 )
{
	LONG          lResult = -1;

	ULONG         ClusterSize = 0, BlockSize = 0;
	ULONGLONG    *Clusters = NULL;
	ULONG         ClCount = 0, FileSize = 0, Bytes = 0;
	HANDLE        hDrive = INVALID_HANDLE_VALUE;
	ULONG         SecPerCl = 0, BtPerSec = 0, r = 0;
	PVOID         Buff = NULL;
	LARGE_INTEGER Offset = {0};
	WCHAR         Name[7] = {0};
	PUCHAR        pucPointer = NULL;

	BOOL bResult = FALSE;


	Name[0] = pwstrFilePath[0];
	Name[1] = L':';
	Name[2] = 0;

	GetDiskFreeSpace(Name, &SecPerCl, &BtPerSec, NULL, NULL);

	ClusterSize = SecPerCl * BtPerSec;

	Clusters = GetFileClusters(pwstrFilePath, ClusterSize, &ClCount, &FileSize);

	if (NULL == Clusters)
	{
		lResult = -1;
		goto Exit0;
	}
	Name[0] = L'\\';
	Name[1] = L'\\';
	Name[2] = L'.';
	Name[3] = L'\\';
	Name[4] = pwstrFilePath[0];
	Name[5] = L':';
	Name[6] = 0;

	hDrive = CreateFile(Name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	if (INVALID_HANDLE_VALUE == hDrive)
	{
		lResult = -1;
		goto Exit0;
	}

	Buff = malloc(ClusterSize);
	if (NULL == Buff)
	{
		lResult = -1;
		goto Exit0;
	}

	*ppWholeFileBuffer = NULL;
	*pulBufferLength = 0;

	*ppWholeFileBuffer = malloc(FileSize);
	if (NULL == *ppWholeFileBuffer)
	{
		lResult = -1;
		goto Exit0;
	}
	*pulBufferLength = FileSize;

	pucPointer = *ppWholeFileBuffer;

	for (r = 0; r < ClCount; r++, FileSize -= BlockSize)
	{
		Offset.QuadPart = ClusterSize * Clusters[r];

		SetFilePointer(hDrive, Offset.LowPart, &Offset.HighPart, FILE_BEGIN);
		if (NO_ERROR != GetLastError())
		{
			lResult = -1;
			goto Exit0;
		}

		bResult = ReadFile(hDrive, Buff, ClusterSize, &Bytes, NULL);
		if (FALSE == bResult)
		{
			lResult = -1;
			goto Exit0;
		}

		BlockSize = FileSize < ClusterSize ? FileSize : ClusterSize;

		memcpy(pucPointer, Buff, BlockSize);
		pucPointer += BlockSize;
	}

	lResult = ERROR_SUCCESS;
Exit0:
	if (NULL != Buff)
	{
		free(Buff);
	}
	if (INVALID_HANDLE_VALUE != hDrive)
	{
		CloseHandle(hDrive);
	}
	if (NULL != Clusters)
	{
		free(Clusters);
	}
	return lResult;
}
//-----------------------------------------------------------------------------//