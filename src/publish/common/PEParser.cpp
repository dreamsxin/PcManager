
// dummyz@126.com
//
#include "stdafx.h"
#include "PEParser.h"
#include <algorithm>
#include <assert.h>


using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPEParser::CPEParser()
{
	m_lpMem = NULL;
}

CPEParser::~CPEParser()
{
	Unload();
}

BOOL CPEParser::Load(LPCTSTR lpPath)
{
	assert(m_lpMem == NULL);
	BOOL bResult = FALSE;

	HANDLE hFile = CreateFile(lpPath,
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_EXISTING,
		0, 
		NULL
		);
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwFileSizeHig;
		DWORD dwFileSize = GetFileSize(hFile, &dwFileSizeHig);
		if ( dwFileSizeHig == 0 && dwFileSize < (1024 * 1024 * 50) )
		{
			PVOID lpMem = VirtualAlloc(NULL, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
			if ( lpMem != NULL )
			{
				DWORD dwReadBytes;

				if ( ReadFile(hFile, lpMem, dwFileSize, &dwReadBytes, NULL) &&
					dwReadBytes == dwFileSize
					)
				{
					__try
					{
						m_lpDosHeader = (PIMAGE_DOS_HEADER)lpMem;
						if ( m_lpDosHeader->e_magic == IMAGE_DOS_SIGNATURE && m_lpDosHeader->e_lfanew < 0x1000 )
						{
							m_lpNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)m_lpDosHeader + m_lpDosHeader->e_lfanew);
							if ( m_lpNtHeader->Signature == IMAGE_NT_SIGNATURE &&
								m_lpNtHeader->FileHeader.NumberOfSections != 0
								)
							{
								m_lpFirstSection = IMAGE_FIRST_SECTION(m_lpNtHeader);
								
								m_lpMem = lpMem;
								m_dwMemSize = dwFileSize;
								m_dwBaseAddr = m_lpNtHeader->OptionalHeader.ImageBase;
								
								bResult = TRUE;
							}
						}
					}
					__except ( EXCEPTION_EXECUTE_HANDLER )
					{
						m_lpMem = NULL;
					}
				}
				
				if ( !bResult )
				{
					VirtualFree(lpMem, dwFileSize, MEM_DECOMMIT);
					VirtualFree(lpMem, 0, MEM_RELEASE);

					lpMem = NULL;
				}
			}
		}

		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}

	return bResult;
}

void CPEParser::Unload()
{
	if ( m_lpMem != NULL )
	{
		VirtualFree(m_lpMem, m_dwMemSize, MEM_DECOMMIT);
		VirtualFree(m_lpMem, 0, MEM_RELEASE);
		m_lpMem = NULL;
	}
}

BOOL CPEParser::FixRelocation(DWORD nBaseAddr)
{
	assert(m_lpMem != NULL);
	BOOL bResult = TRUE;

	PIMAGE_DATA_DIRECTORY lpDir = &m_lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if ( lpDir->VirtualAddress != 0 && lpDir->Size > sizeof (IMAGE_BASE_RELOCATION) )
	{
		__try
		{
			PIMAGE_BASE_RELOCATION lpRel = (PIMAGE_BASE_RELOCATION)ToPointer(lpDir->VirtualAddress);
			DWORD dwSize = 0;
			DWORD dwDiff = nBaseAddr - m_dwBaseAddr;

			while ( dwSize < lpDir->Size )
			{
				DWORD dwCount = (lpRel->SizeOfBlock - sizeof (IMAGE_BASE_RELOCATION)) / sizeof (WORD);

				PWORD lpRelItem = (PWORD)(lpRel + 1);
				PBYTE lpRelAddr = ToPointer(lpRel->VirtualAddress);
				for ( DWORD j = 0; j < dwCount; j++ )
				{
					WORD w = lpRelItem[j];
					if ( (w >> 12) == 3 )
					{
						*(PDWORD)(lpRelAddr + (w & 0x0FFF)) += dwDiff;
					}
				}

				dwSize += lpRel->SizeOfBlock;
				lpRel = (PIMAGE_BASE_RELOCATION)UlongToPtr(PtrToUlong(lpRel) + lpRel->SizeOfBlock);
			}
		}
		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			bResult = FALSE;
		}
	}

	m_dwBaseAddr = nBaseAddr;
	return bResult;
}


PBYTE CPEParser::ToPointer(DWORD dwOffset)
{
	assert(m_lpMem != NULL);
	if ( dwOffset < m_lpFirstSection->VirtualAddress )
	{
		return (PBYTE)m_lpDosHeader + dwOffset;
	}

	for ( DWORD i = 0; i < m_lpNtHeader->FileHeader.NumberOfSections; i++ )
	{
		DWORD nBeginRva = m_lpFirstSection[i].VirtualAddress;
		DWORD nEndRva = nBeginRva + max(m_lpFirstSection[i].Misc.VirtualSize, m_lpFirstSection[i].SizeOfRawData);

		if ( dwOffset >= nBeginRva && dwOffset < nEndRva )
		{
			return (PBYTE)m_lpDosHeader + m_lpFirstSection[i].PointerToRawData + (dwOffset - nBeginRva);
		}
	}

	return NULL;
}

DWORD CPEParser::ToOffset(PVOID lpPointer)
{
	assert(m_lpMem != NULL);

	PBYTE lp = (PBYTE)lpPointer;
	if ( lp > (PBYTE)m_lpMem && 
		lp < (PBYTE)m_lpMem + m_dwMemSize
		)
	{
		DWORD dwOffset = PtrToUlong(lp) - PtrToUlong(m_lpMem);
		if ( dwOffset < m_lpFirstSection->PointerToRawData )
		{
			return dwOffset;
		}
		
		for ( DWORD i = 0; i < m_lpNtHeader->FileHeader.NumberOfSections; i++ )
		{
			PBYTE a = (PBYTE)m_lpMem + m_lpFirstSection[i].PointerToRawData;
			PBYTE b = a + m_lpFirstSection[i].SizeOfRawData;

			if ( lp >= a && lp < b )
			{
				return (PtrToUlong(lp) - PtrToUlong(a)) + m_lpFirstSection[i].VirtualAddress;
			}
		}
	}

	return 0;
}

void CPEParser::UnInlineHook(PBYTE lpDest, PBYTE lpSrc, DWORD dwSize)
{
	assert(m_lpMem != NULL);
}


DWORD CPEParser::FindFunction(LPCSTR lpName)
{
	assert(m_lpMem != NULL);
	DWORD nResult = 0;
	
	PIMAGE_DATA_DIRECTORY lpDir = &m_lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if ( lpDir->VirtualAddress == 0 )
	{
		return 0;
	}

	__try
	{
		PIMAGE_EXPORT_DIRECTORY lpExpDir = (PIMAGE_EXPORT_DIRECTORY)ToPointer(lpDir->VirtualAddress);
		if ( lpExpDir != NULL )
		{
			PDWORD lpFunNames = (PDWORD)ToPointer(lpExpDir->AddressOfNames);
			PDWORD lpFunAddrs = (PDWORD)ToPointer(lpExpDir->AddressOfFunctions);
			PWORD lpFunOrds = (PWORD)ToPointer(lpExpDir->AddressOfNameOrdinals);
			
			if ( IS_INTRESOURCE(lpName) )
			{
				nResult = lpFunAddrs[PtrToUlong(lpName) - lpExpDir->Base];
			}
			else
			{
				for ( DWORD i = 0; i < lpExpDir->NumberOfNames; i++ )
				{
					LPCSTR lpFunName = (LPCSTR)ToPointer(lpFunNames[i]);
					if ( lpFunName != NULL && strcmp(lpName, lpFunName) == 0 )
					{
						DWORD dwFunOrd = lpFunOrds[i];
						
						nResult = lpFunAddrs[dwFunOrd - lpExpDir->Base];
						break;
					}
				}
			}
			
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		nResult = 0;
	}

	return nResult;
}

DWORD CPEParser::FindExpThunk(LPCSTR lpName)
{
	assert(m_lpMem != NULL);
	DWORD nResult = 0;
	
	PIMAGE_DATA_DIRECTORY lpDir = &m_lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if ( lpDir->VirtualAddress == 0 )
	{
		return 0;
	}
	
	__try
	{
		PIMAGE_EXPORT_DIRECTORY lpExpDir = (PIMAGE_EXPORT_DIRECTORY)ToPointer(lpDir->VirtualAddress);
		if ( lpExpDir != NULL )
		{
			PDWORD lpFunNames = (PDWORD)ToPointer(lpExpDir->AddressOfNames);
			PDWORD lpFunAddrs = (PDWORD)ToPointer(lpExpDir->AddressOfFunctions);
			PWORD lpFunOrds = (PWORD)ToPointer(lpExpDir->AddressOfNameOrdinals);
			
			if ( IS_INTRESOURCE(lpName) )
			{
				nResult = ToOffset(&lpFunAddrs[PtrToUlong(lpName) - lpExpDir->Base]);
			}
			else
			{
				for ( DWORD i = 0; i < lpExpDir->NumberOfNames; i++ )
				{
					LPCSTR lpFunName = (LPCSTR)ToPointer(lpFunNames[i]);
					if ( lpFunName != NULL && strcmp(lpName, lpFunName) == 0 )
					{
						DWORD dwFunOrd = lpFunOrds[i];
						
						nResult = ToOffset(&lpFunAddrs[dwFunOrd]);
						break;
					}
				}
			}
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		nResult = 0;
	}
	
	return nResult;
}

DWORD CPEParser::FindDataRef(DWORD dwRva, std::list<DWORD>& dwOffsetList)
{
	assert(m_lpMem != NULL);

	dwOffsetList.clear();

	PIMAGE_DATA_DIRECTORY lpDir = &m_lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	if ( lpDir->VirtualAddress == 0 || lpDir->Size < sizeof (IMAGE_BASE_RELOCATION) )
	{
		return 0;
	}

	__try
	{
		PIMAGE_BASE_RELOCATION lpRel = (PIMAGE_BASE_RELOCATION)ToPointer(lpDir->VirtualAddress);
		DWORD dwSize = 0;
		DWORD dwObjAddr = dwRva + m_dwBaseAddr;

		while ( dwSize < lpDir->Size )
		{
			DWORD dwCount = (lpRel->SizeOfBlock - sizeof (IMAGE_BASE_RELOCATION)) / sizeof (WORD);
			PWORD lpRelItem = (PWORD)(lpRel + 1);
			PBYTE lpRelAddr = ToPointer(lpRel->VirtualAddress);
			for ( DWORD j = 0; j < dwCount; j++ )
			{
				WORD w = lpRelItem[j];
				if ( (w >> 12) == 3 )
				{
					PDWORD lpdw = (PDWORD)(lpRelAddr + (w & 0x0FFF));
					if ( *lpdw == dwObjAddr )
					{
						DWORD a = lpRel->VirtualAddress + (w & 0x0FFF);
						dwOffsetList.push_back(a);
					}
				}
			}
			
			dwSize += lpRel->SizeOfBlock;
			lpRel = (PIMAGE_BASE_RELOCATION)UlongToPtr(PtrToUlong(lpRel) + lpRel->SizeOfBlock);
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
	}
	
	return (DWORD)dwOffsetList.size();
}

DWORD CPEParser::FindImpThunk(LPCSTR lpDll, LPCSTR lpName, std::list<DWORD>& dwOffsetList)
{
	assert(m_lpMem != NULL);
	
	dwOffsetList.clear();
	
	PIMAGE_DATA_DIRECTORY lpDir = &m_lpNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	if ( lpDir->VirtualAddress == 0 )
	{
		return 0;	
	}

	__try
	{
		PIMAGE_IMPORT_DESCRIPTOR lpImpDesc = (PIMAGE_IMPORT_DESCRIPTOR)ToPointer(lpDir->VirtualAddress);
		while ( lpImpDesc->Name != 0 )
		{
			LPCSTR lpDllName = (LPCSTR)ToPointer(lpImpDesc->Name);
			if ( stricmp(lpDllName, lpDll) == 0 )
			{
				PIMAGE_THUNK_DATA lpThunk = (PIMAGE_THUNK_DATA)ToPointer(lpImpDesc->FirstThunk);
				PIMAGE_THUNK_DATA lpOrgThunk = lpThunk;
				if ( lpImpDesc->OriginalFirstThunk != 0 )
				{
					lpOrgThunk = (PIMAGE_THUNK_DATA)ToPointer(lpImpDesc->OriginalFirstThunk);
				}
				
				while ( lpOrgThunk->u1.Function != 0 )
				{
					if ( IMAGE_SNAP_BY_ORDINAL(lpOrgThunk->u1.Function) )
					{
						if ( IS_INTRESOURCE(lpName) &&
							IMAGE_ORDINAL(lpOrgThunk->u1.Function) == PtrToUlong(lpName) 
							)
						{
							dwOffsetList.push_back(ToOffset(&lpThunk->u1.Function));
						}
					}
					else
					{
						PIMAGE_IMPORT_BY_NAME lpFunName = (PIMAGE_IMPORT_BY_NAME)ToPointer(lpOrgThunk->u1.Function);
						if ( strcmp((CHAR*)lpFunName->Name, lpName) == 0 )
						{
							dwOffsetList.push_back(ToOffset(&lpThunk->u1.Function));
						}
					}
					
					lpThunk++;
					lpOrgThunk++;
				}
			}
			
			lpImpDesc++;
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
	}

	return (DWORD)dwOffsetList.size();
}