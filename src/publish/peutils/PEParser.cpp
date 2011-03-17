
// dummyz@126.com
//
#include "stdafx.h"
#include "PEParser.h"
#include "PEFile.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
#define MIN_STRING_LEN 4
const char* const __x_table = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()`-=\\[];',./_+|{}:\"<>?\t\r\n ";

static DWORD GetUnicodeStringLen(PBYTE lpStr, DWORD dwSize) // 只支持英文, 成功返回 字符个数
{
	DWORD i = 0;
	DWORD nLen = dwSize / 2;
	BOOL bBad = FALSE;

	while ( i < nLen )
	{
		BYTE c1 = *lpStr++;
		BYTE c2 = *lpStr++;
		if ( c1 == 0 && c2 == 0 )
		{
			break;
		}

		if ( (c1 & 0x80) || (c2 != 0) )
		{
			bBad = TRUE;
			break;
		}

		if ( strchr(__x_table, (CHAR)c1) == NULL )
		{
			bBad = TRUE;
			break;
		}

		i++;
	}

	if ( !bBad && i >= 4 )
	{
		return i;
	}

	return 0;
}

static DWORD GetAsciiStringLen(PBYTE lpStr, DWORD dwSize) // 只支持英文
{
	DWORD i = 0;
	DWORD nLen = dwSize;
	BOOL bBad = FALSE;

	while ( i < nLen )
	{
		BYTE c1 = *lpStr++;;
		if ( c1 == 0 )
		{
			break;
		}

		if ( c1 & 0x80 )
		{
			bBad = TRUE;
			break;
		}

		if ( strchr(__x_table, (CHAR)c1) == NULL )
		{
			bBad = TRUE;
			break;
		}

		i++;
	}

	if ( !bBad && i >= MIN_STRING_LEN )
	{
		return i;
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
CPEParser::CPEParser()
{
	m_lpFile = NULL;
	m_lpSectHeads = NULL;
}

CPEParser::~CPEParser()
{
	Detach();
}

BOOL CPEParser::Attach(IPEFile* lpFile)
{
	BOOL bResult = FALSE;

	if ( lpFile == NULL || !lpFile->IsOpen() )
	{
		return FALSE;
	}

	if ( m_lpFile != NULL )
	{
		return FALSE;
	}

	m_lpFile = lpFile;
	m_lpFile->Seek(0, FILE_BEGIN);
	if ( m_lpFile->ReadT(m_DosHead) && 
		m_DosHead.e_magic == IMAGE_DOS_SIGNATURE &&
		m_DosHead.e_lfanew < 0x1000
		)
	{
		m_lpFile->Seek(m_DosHead.e_lfanew, FILE_BEGIN);
		if ( m_lpFile->ReadT(m_NtHead64) &&
			m_NtHead64.Signature == IMAGE_NT_SIGNATURE 
			)
		{
			ULONG lSectionOffset = 0;

			if ( m_NtHead64.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC )
			{
				if ( m_NtHead64.FileHeader.SizeOfOptionalHeader >= sizeof (IMAGE_OPTIONAL_HEADER32) )
				{
					m_bPEPlus = FALSE;
					memcpy(&m_NtHead32, &m_NtHead64, sizeof (m_NtHead32));

					lSectionOffset = FIELD_OFFSET(IMAGE_NT_HEADERS32, OptionalHeader) + m_NtHead32.FileHeader.SizeOfOptionalHeader;
				}
			}
			else if ( m_NtHead64.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC )
			{
				if ( m_NtHead64.FileHeader.SizeOfOptionalHeader >= sizeof (IMAGE_OPTIONAL_HEADER64) )
				{
					m_bPEPlus = TRUE;
					lSectionOffset = FIELD_OFFSET(IMAGE_NT_HEADERS64, OptionalHeader) + m_NtHead64.FileHeader.SizeOfOptionalHeader;
				}
			}

			if ( lSectionOffset != 0 )
			{
				lSectionOffset += m_DosHead.e_lfanew;
				if ( m_lpFile->Seek(lSectionOffset, FILE_BEGIN) == lSectionOffset )
				{
					DWORD dwSectionHeadBytes = sizeof (IMAGE_SECTION_HEADER) * GetSectionCount();

					m_lpSectHeads = new IMAGE_SECTION_HEADER[GetSectionCount()];
					if ( m_lpFile->Read(m_lpSectHeads, dwSectionHeadBytes) == dwSectionHeadBytes )
					{
						bResult = TRUE;
					}
				}
			}
		}
	}

	if ( !bResult )
	{
		Detach();
	}

	return bResult;
}

void CPEParser::Detach()
{
	delete m_lpSectHeads;

	m_lpSectHeads = NULL;
	m_lpFile = NULL;
}

ULONG CPEParser::ToFilePointer(ULONG lOffset)
{
	assert(m_lpFile != NULL);
	ULONG lResult = EOF;

	if ( !IsVaildOffset(lOffset) )
	{
		return EOF;
	}

	if ( m_lpFile->IsRawPtr() )
	{
		if ( lOffset < m_lpSectHeads ->VirtualAddress )
		{
			lResult = lOffset;
		}
		else
		{
			PIMAGE_SECTION_HEADER lpSectHead = GetSectionHeadByOffset(lOffset);
			if ( lpSectHead != NULL )
			{
				lResult = lpSectHead->PointerToRawData + (lOffset - lpSectHead->VirtualAddress);
			}
		}
	}
	else
	{
		lResult = lOffset;
	}

	return lResult;
}

ULONG CPEParser::FilePointerTo(ULONG lOffset)
{
	assert(m_lpFile != NULL);
	ULONG lResult = EOF;

	if ( !IsVaildFilePointer(lOffset) )
	{
		return EOF;
	}


	if ( m_lpFile->IsRawPtr() )
	{
		if ( lOffset < m_lpSectHeads->PointerToRawData )
		{
			lResult = lOffset;
		}
		else
		{
			PIMAGE_SECTION_HEADER lpSectHead = &m_lpSectHeads[GetSectionCount() - 1];

			do
			{
				ULONG lBeginPointer = lpSectHead->PointerToRawData;
				ULONG lEndPointer = lBeginPointer + lpSectHead->SizeOfRawData;

				if ( lOffset >= lBeginPointer && lOffset < lEndPointer )
				{
					lResult = lOffset - lBeginPointer + lpSectHead->VirtualAddress;
					break;
				}

			} while ( --lpSectHead >= m_lpSectHeads );
		}
	}
	else
	{
		lResult = lOffset;
	}

	return lResult;
}

PIMAGE_SECTION_HEADER CPEParser::GetSectionHeadByOffset(ULONG lOffset)
{
	PIMAGE_SECTION_HEADER lpResult = NULL;

	ULONG VirtulAddressLimit = GetImageSize();
	PIMAGE_SECTION_HEADER lpSectHead = &m_lpSectHeads[GetSectionCount() - 1];
	
	do
	{
		if ( lOffset >= lpSectHead->VirtualAddress && lOffset < VirtulAddressLimit )
		{
			lpResult = lpSectHead;
			break;
		}

		VirtulAddressLimit = lpSectHead->VirtualAddress;
	} while ( --lpSectHead >= m_lpSectHeads );

	return lpResult;
}


ULONG CPEParser::GetOverlayOffset(LPDWORD lpdwSize)
{
	ULONG lResult = 0;

	DWORD NumberOfSections = GetNtFileHead()->NumberOfSections;
	for ( ULONG i = 0; i < NumberOfSections; i++ )
	{
		ULONG lOffset = m_lpSectHeads[i].PointerToRawData + m_lpSectHeads[i].SizeOfRawData;
		if ( lOffset > lResult )
		{
			lResult = lOffset;
		}
	}

	DWORD dwFileSize = m_lpFile->GetSize();
	if ( dwFileSize > lResult )
	{
		if ( lpdwSize != NULL )
			*lpdwSize = dwFileSize - lResult;
	}
	else
	{
		lResult = EOF;
		if ( lpdwSize != NULL )
			*lpdwSize = 0;
	}

	return lResult;
}

ULONG CPEParser::CalcChecksum(BOOL bHaveOverlay)
{
	ULONG Key = 0;
	ULONG lReadOffset = 0, lFileSize = 0;
	ULONG lChecksumOffset = 0;

	if ( m_bPEPlus )
	{
		lChecksumOffset = m_DosHead.e_lfanew + FIELD_OFFSET(IMAGE_NT_HEADERS64, OptionalHeader.CheckSum);
	}
	else
	{
		lChecksumOffset = m_DosHead.e_lfanew + FIELD_OFFSET(IMAGE_NT_HEADERS32, OptionalHeader.CheckSum);
	}

	if ( bHaveOverlay )
	{
		lFileSize = GetOverlayOffset(NULL);
		if ( lFileSize == EOF )
		{
			lFileSize = m_lpFile->GetSize();
		}
	}
	else
	{
		lFileSize = m_lpFile->GetSize();
	}

	PBYTE lpFileData = (PBYTE)m_lpFile->GetMapPtr();
	if ( lpFileData != NULL )
	{
		ULONG i = 0;
		ULONG Count = lChecksumOffset >> 1;
		while ( i < Count )
		{
			Key += *((PUSHORT)lpFileData + i);
			Key = (Key >> 16) + (USHORT)Key;

			i++;
		}

		if ( lChecksumOffset & 1 )
		{
			BYTE cBuff[3 * sizeof (USHORT)] = { 0 };

			cBuff[0] = lpFileData[i * sizeof (USHORT)];
			cBuff[5] = lpFileData[i * sizeof (USHORT) + 5];

			Key += *((PUSHORT)cBuff + 0);
			Key = (Key >> 16) + (USHORT)Key;

			Key += *((PUSHORT)cBuff + 1);
			Key = (Key >> 16) + (USHORT)Key;

			Key += *((PUSHORT)cBuff + 2);
			Key = (Key >> 16) + (USHORT)Key;

			i += 3;
		}
		else
		{
			Key += 0;
			Key = (Key >> 16) + (USHORT)Key;
			i++;

			Key += 0;
			Key = (Key >> 16) + (USHORT)Key;
			i++;
		}

		Count = (lFileSize + 1) >> 1;
		while ( i < Count )
		{
			Key += *((PUSHORT)lpFileData + i);
			Key = (Key >> 16) + (USHORT)Key;

			i++;
		}

		Key = (USHORT)((Key >> 16) + Key);
		Key += lFileSize;
	}

	return Key;
}

BOOL CPEParser::CheckDataVaild()
{
	DWORD dwCount = GetSectionCount();
	for ( DWORD i = 0; i < dwCount; i++ )
	{
		PIMAGE_SECTION_HEADER lpSectHead = &m_lpSectHeads[i];

		if ( !IsVaildOffset(lpSectHead->VirtualAddress - 1) ||
			!IsVaildOffset(lpSectHead->VirtualAddress + lpSectHead->Misc.VirtualSize - 1) 
			)
		{
			return FALSE;
		}

		if ( lpSectHead->PointerToRawData != 0 )
		{
			if ( !IsVaildFilePointer(lpSectHead->PointerToRawData - 1) ||
				!IsVaildFilePointer(lpSectHead->PointerToRawData + lpSectHead->SizeOfRawData - 1)
				)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}


LPWSTR CPEParser::ReadResDirName(ULONG lOffset)
{
	LPWSTR lpName = NULL;
	ULONG lFilePointer = ToFilePointer(lOffset);

	if ( lFilePointer != EOF &&
		lFilePointer == m_lpFile->Seek(lFilePointer, FILE_BEGIN)
		)
	{
		WORD nLength = 0;
		if ( m_lpFile->ReadT(nLength) )
		{
			lpName = new WCHAR[nLength + 1];
			if ( lpName != NULL )
			{
				DWORD dwReadBytes = nLength * sizeof (WCHAR);
				if ( m_lpFile->Read(lpName, dwReadBytes) == dwReadBytes )
				{
					lpName[nLength] = 0;
				}
				else
				{
					delete lpName;
					lpName = NULL;
				}
			}
		}
	}

	return lpName;
}

PIMAGE_RESOURCE_DIRECTORY_ENTRY	CPEParser::ReadResDirEntry(ULONG lOffset, IMAGE_RESOURCE_DIRECTORY* lpResDir)
{
	PIMAGE_RESOURCE_DIRECTORY_ENTRY lpResDirEntrys = NULL;

	ULONG lFilePointer = ToFilePointer(lOffset);
	if ( lFilePointer != EOF &&
		lFilePointer == m_lpFile->Seek(lFilePointer, FILE_BEGIN)
		)
	{
		if ( m_lpFile->Read(lpResDir, sizeof (IMAGE_RESOURCE_DIRECTORY)) == sizeof (IMAGE_RESOURCE_DIRECTORY) )
		{
			ULONG nEntryCount = lpResDir->NumberOfIdEntries + lpResDir->NumberOfNamedEntries;

			lpResDirEntrys = new IMAGE_RESOURCE_DIRECTORY_ENTRY[nEntryCount];
			if ( lpResDirEntrys != NULL )
			{
				ULONG nReadBytes = sizeof (IMAGE_RESOURCE_DIRECTORY_ENTRY) * nEntryCount;
				if ( m_lpFile->Read(lpResDirEntrys, nReadBytes) != nReadBytes )
				{
					delete lpResDirEntrys;
					lpResDirEntrys = NULL;
				}
			}
		}
	}

	return lpResDirEntrys;
}

ULONG CPEParser::FindResourceType(LPCWSTR lpType)
{
	class CResTypeEnumer
	{
	private:
		BOOL WINAPI _EnumFunc(LPCWSTR lpType, ULONG lOffset, LPARAM lParam)
		{
			BOOL bEquType = FALSE;

			if ( IS_INTRESOURCE(m_lpType) )
			{
				if ( IS_INTRESOURCE(lpType) )
				{
					bEquType = (lpType == m_lpType);
				}
			}
			else
			{
				if ( !IS_INTRESOURCE(lpType) )
				{
					bEquType = (_wcsicmp(m_lpType, lpType) == 0);
				}
			}

			if ( bEquType )
			{
				m_lOffset = lOffset;
				return FALSE;
			}

			return TRUE;
		}

	public:
		static BOOL WINAPI EnumFunc(PIMAGE_RESOURCE_DIRECTORY lpDir, LPCWSTR lpType, ULONG lOffset, LPARAM lParam)
		{
			CResTypeEnumer* _this = (CResTypeEnumer*)LongToPtr(lParam);
			return _this->_EnumFunc(lpType, lOffset, lParam);
		}

	public:
		LPCTSTR		m_lpType;
		CPEParser*	m_lpPEParser;
		ULONG		m_lOffset;
	};

	CResTypeEnumer enumer;

	enumer.m_lpType = lpType;
	enumer.m_lpPEParser = this;
	enumer.m_lOffset = EOF;

	EnumResourceTypes(CResTypeEnumer::EnumFunc, PtrToLong(&enumer));

	return enumer.m_lOffset;
}

ULONG CPEParser::FindResourceName(ULONG lOffset, LPCWSTR lpName, DWORD dwCodePage, DWORD* lpdwSize)
{
	assert(m_lpFile != NULL);

	class CResNameEnumer
	{
	private:
		BOOL WINAPI _EnumFunc(LPCWSTR lpName, ULONG lOffset, DWORD dwSize, DWORD dwCodePage, LPARAM lParam)
		{
			BOOL bEquName = FALSE;

			if ( IS_INTRESOURCE(m_lpName) )
			{
				if ( IS_INTRESOURCE(lpName) )
				{
					bEquName = (lpName == m_lpName);
				}
			}
			else
			{
				if ( !IS_INTRESOURCE(lpName) )
				{
					bEquName = (_wcsicmp(m_lpName, lpName) == 0);
				}
			}

			if ( bEquName )
			{
				if ( m_dwCodePage == 0 || m_dwCodePage == dwCodePage )
				{
					m_dwResDataSize = dwSize;
					m_lResDataOffset = lOffset;
					return FALSE;
				}
			}

			return TRUE;
		}

	public:
		static BOOL WINAPI EnumFunc(PIMAGE_RESOURCE_DIRECTORY lpDir, LPCTSTR lpName, ULONG lOffset, DWORD dwSize, DWORD dwCodePage, LPARAM lParam)
		{
			CResNameEnumer* _this = (CResNameEnumer*)LongToPtr(lParam);
			return _this->_EnumFunc(lpName, lOffset, dwSize, dwCodePage, lParam);
		}

	public:
		LPCTSTR		m_lpName;
		DWORD		m_dwCodePage;
		DWORD		m_dwResDataSize;
		ULONG		m_lResDataOffset;

		CPEParser*	m_lpPEParser;
	};

	CResNameEnumer enumer;

	enumer.m_lpName = lpName;
	enumer.m_dwCodePage = dwCodePage;
	enumer.m_dwResDataSize = 0;
	enumer.m_lResDataOffset = EOF;
	enumer.m_lpPEParser = this;

	EnumResourceNames(lOffset, CResNameEnumer::EnumFunc, PtrToLong(&enumer));
	
	if ( lpdwSize != NULL )
		*lpdwSize = enumer.m_dwResDataSize;

	return enumer.m_lResDataOffset;
}

ULONG CPEParser::FindResource(LPCWSTR lpName, LPCWSTR lpType, DWORD dwCodePage, DWORD* lpdwSize)
{
	assert(m_lpFile != NULL);
	ULONG lOffset;
	
	lOffset = FindResourceType(lpType);
	if ( lOffset != EOF )
	{
		lOffset = FindResourceName(lOffset, lpName, dwCodePage, lpdwSize);
	}

	return lOffset;
}

void CPEParser::EnumResourceTypes(PFN_ENUM_RESTYPE lpEnumFunc, LPARAM lParam)
{
	assert(m_lpFile != NULL);

	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_RESOURCE);
	if ( lpDir->VirtualAddress == 0 )
	{
		return ;
	}

	IMAGE_RESOURCE_DIRECTORY ResDir;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY lpDirEntrys;

	lpDirEntrys = ReadResDirEntry(lpDir->VirtualAddress, &ResDir);
	if ( lpDirEntrys == NULL )
	{
		return ;
	}

	BOOL bContinue = TRUE;
	const ULONG nDirCount = ResDir.NumberOfIdEntries + ResDir.NumberOfNamedEntries;
	for ( ULONG i = 0; bContinue && i < nDirCount; i++ )
	{
		if ( !lpDirEntrys[i].DataIsDirectory )
		{
			continue;
		}

		ULONG lChildDirOffset = lpDir->VirtualAddress + lpDirEntrys[i].OffsetToDirectory;
		if ( lpDirEntrys[i].NameIsString )
		{
			LPWSTR lpName = ReadResDirName(lpDir->VirtualAddress + lpDirEntrys[i].NameOffset);
			if ( lpName != NULL )
			{
				bContinue = lpEnumFunc(&ResDir, lpName, lChildDirOffset, lParam);

				delete lpName;
				lpName = NULL;
			}
		}
		else
		{
			bContinue = lpEnumFunc(&ResDir, MAKEINTRESOURCE(lpDirEntrys[i].Id), lChildDirOffset, lParam);
		}
	}

	delete lpDirEntrys;
	lpDirEntrys = NULL;
}

void CPEParser::EnumResourceNames(LPCWSTR lpType, PFN_ENUM_RESNAME lpEnumFunc, LPARAM lParam)
{
	assert(m_lpFile != NULL);
	
	const ULONG lOffset = FindResourceType(lpType);
	if ( lOffset == EOF )
		return ;

	EnumResourceNames(lOffset, lpEnumFunc, lParam);
}

void CPEParser::EnumResourceNames(ULONG lOffset, PFN_ENUM_RESNAME lpEnumFunc, LPARAM lParam)
{
	assert(m_lpFile != NULL);

	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_RESOURCE);
	if ( lpDir->VirtualAddress == 0 )
		return ;

	IMAGE_RESOURCE_DIRECTORY ResDir;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY lpDirEntrys;

	lpDirEntrys = ReadResDirEntry(lOffset, &ResDir);
	if ( lpDirEntrys == NULL )
		return ;

	BOOL bContinue = TRUE;
	const ULONG nDirCount = ResDir.NumberOfIdEntries + ResDir.NumberOfNamedEntries;
	for ( ULONG i = 0; bContinue && i < nDirCount; i++ )
	{
		if ( !lpDirEntrys[i].DataIsDirectory )
		{
			continue;
		}

		LPWSTR lpName = NULL;
		if ( lpDirEntrys[i].NameIsString )
		{
			lpName = ReadResDirName(lpDir->VirtualAddress + lpDirEntrys[i].NameOffset);
		}
		else
		{
			lpName = MAKEINTRESOURCE(lpDirEntrys[i].Id);
		}
		
		// 取出数据
		ULONG lChildDirOffset = lpDir->VirtualAddress + lpDirEntrys[i].OffsetToDirectory;
		IMAGE_RESOURCE_DIRECTORY ChildDir;
		PIMAGE_RESOURCE_DIRECTORY_ENTRY lpChildDirEntrys;

		lpChildDirEntrys = ReadResDirEntry(lChildDirOffset, &ChildDir);
		if ( lpChildDirEntrys != NULL )
		{
			DWORD dwChildDirCount = ChildDir.NumberOfIdEntries + ChildDir.NumberOfNamedEntries;
			for ( DWORD j = 0; bContinue && j < dwChildDirCount; j++ )
			{
				if ( lpChildDirEntrys[j].DataIsDirectory )
				{
					continue;
				}

				IMAGE_RESOURCE_DATA_ENTRY DataEntry;
				ULONG lDataEntryOffset = lpDir->VirtualAddress + lpChildDirEntrys[j].OffsetToData;
				ULONG lFilePoniter = ToFilePointer(lDataEntryOffset);

				if ( lFilePoniter != EOF && 
					m_lpFile->Seek(lFilePoniter, FILE_BEGIN) == lFilePoniter &&
					m_lpFile->ReadT(DataEntry)
					)
				{
					bContinue = lpEnumFunc(&ChildDir, lpName, DataEntry.OffsetToData, DataEntry.Size, DataEntry.CodePage, lParam);
				}
			}

			delete lpChildDirEntrys;
			lpChildDirEntrys = NULL;
		}

		if ( !IS_INTRESOURCE(lpName) )
		{
			delete lpName;
			lpName = NULL;
		}
	}

	delete lpDirEntrys;
	lpDirEntrys = NULL;
}

void CPEParser::EnumExpFunction(PFN_ENUM_EXPFUN pfnEnumFunc, LPARAM lParam)
{
	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if ( lpDir->VirtualAddress == 0 )
	{
		return ;
	}

	ULONG lExpDirPointer = ToFilePointer(lpDir->VirtualAddress);
	if ( lExpDirPointer == EOF )
	{
		return ;
	}

	IMAGE_EXPORT_DIRECTORY ExpDir;
	if ( lExpDirPointer != m_lpFile->Seek(lExpDirPointer, FILE_BEGIN) ||
		!m_lpFile->ReadT(ExpDir)
		)
	{
		return ;
	}

	ULONG lFunNamePointer = ToFilePointer(ExpDir.AddressOfNames);
	if ( lFunNamePointer == EOF )
	{
		return ;
	}

	ULONG lFunAddrPointer = ToFilePointer(ExpDir.AddressOfFunctions);
	if ( lFunAddrPointer == EOF )
	{
		return ;
	}

	ULONG lFunOrdsPointer = ToFilePointer(ExpDir.AddressOfNameOrdinals);
	if ( lFunOrdsPointer == EOF )
	{
		return ;
	}

	CHAR szDllName[MAX_PATH] = { 0 };
	ULONG lDllNamePointer = ToFilePointer(ExpDir.Name);
	if ( lDllNamePointer != EOF &&
		lDllNamePointer == m_lpFile->Seek(lDllNamePointer, FILE_BEGIN)
		)
	{
		m_lpFile->Read(szDllName, MAX_PATH);
		szDllName[MAX_PATH - 1] = 0;
	}
	
	if ( pfnEnumFunc(&ExpDir, szDllName, NULL, EOF, lParam) )
	{
		DWORD NumberOfNames = min(0xFFFF, ExpDir.NumberOfNames);
		PWORD lpwNameOrds = new WORD[NumberOfNames];
		NumberOfNames = m_lpFile->Read(lpwNameOrds, NumberOfNames * sizeof (WORD));
		NumberOfNames /= sizeof (WORD);

		for ( DWORD i = 0; i < ExpDir.NumberOfFunctions; i++ )
		{
			CHAR	szFunName[MAX_PATH] = { 0 };
			DWORD	dwFunAddr = EOF;
			WORD	nFunOrd = (WORD)(i + ExpDir.Base);

			ULONG	dwAddrPointer = lFunAddrPointer + i * sizeof (DWORD);
			if ( m_lpFile->Seek(dwAddrPointer, FILE_BEGIN) == dwAddrPointer )
			{
				m_lpFile->ReadT(dwFunAddr);
			}

			for ( DWORD j = 0; j < NumberOfNames; j++ )
			{
				if ( lpwNameOrds[j] == nFunOrd )
				{
					ULONG dwNamePointer = lFunNamePointer + j * sizeof (DWORD);
					if ( m_lpFile->Seek(dwNamePointer, FILE_BEGIN) != dwNamePointer ||
						!m_lpFile->ReadT(dwNamePointer)
						)
					{
						break;
					}

					dwNamePointer = ToFilePointer(dwNamePointer);
					if ( m_lpFile->Seek(dwNamePointer, FILE_BEGIN) != dwNamePointer ||
						m_lpFile->Read(szFunName, MAX_PATH) == 0
						)
					{
						break;
					}
					szFunName[MAX_PATH - 1] = 0;
					break;
				}
			}

			if ( szFunName[0] != 0 )
			{
				if ( !pfnEnumFunc(&ExpDir, szDllName, szFunName, dwFunAddr, lParam) )
				{
					break;
				}
			}
			else
			{
				if ( !pfnEnumFunc(&ExpDir, szDllName, MAKEINTRESOURCEA(nFunOrd), dwFunAddr, lParam) )
				{
					break;
				}
			}
		}

		delete lpwNameOrds;
		lpwNameOrds = NULL;
	}
}

void CPEParser::EnumImpFunction(PFN_ENUM_IMPFUN pfnEnumFunc, LPARAM lParam)
{
	if ( m_bPEPlus )
	{
		return ;
	}

	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);
	if ( lpDir->VirtualAddress == 0 )
	{
		return ;
	}

	BOOL bContinue = TRUE;
	IMAGE_IMPORT_DESCRIPTOR ImpDesc;
	ULONG lImpDescPointer = ToFilePointer(lpDir->VirtualAddress);
	while ( bContinue 
		&& m_lpFile->Seek(lImpDescPointer, FILE_BEGIN) == lImpDescPointer
		&& m_lpFile->ReadT(ImpDesc)
		)
	{
		if ( ImpDesc.Name == 0 )
		{
			break;
		}

		CHAR szDllName[MAX_PATH];
		ULONG lDllNamePointer = ToFilePointer(ImpDesc.Name);
		if ( m_lpFile->Seek(lDllNamePointer, FILE_BEGIN) != lDllNamePointer ||
			m_lpFile->Read(szDllName, MAX_PATH) == 0
			)
		{
			break;
		}
		szDllName[MAX_PATH - 1] = 0;

		IMAGE_THUNK_DATA32 ThunkData;
		ULONG lThunkDataOffset = ImpDesc.OriginalFirstThunk != 0 ? ImpDesc.OriginalFirstThunk : ImpDesc.FirstThunk;
		ULONG lThunkDataPointer = lThunkDataOffset;
		ULONG lFirstThunkDataOffset = ImpDesc.FirstThunk;

		lThunkDataPointer = ToFilePointer(lThunkDataPointer);
		while ( m_lpFile->Seek(lThunkDataPointer, FILE_BEGIN) == lThunkDataPointer &&
			m_lpFile->ReadT(ThunkData)
			)
		{
			if ( ThunkData.u1.AddressOfData == 0 )
			{
				break;
			}

			if ( IMAGE_SNAP_BY_ORDINAL(ThunkData.u1.AddressOfData) )
			{
				if ( !pfnEnumFunc(szDllName, MAKEINTRESOURCEA(ThunkData.u1.AddressOfData), lFirstThunkDataOffset, lParam) )
				{
					bContinue = FALSE;
					break;
				}
			}
			else
			{
				BYTE cName[MAX_PATH];

				ULONG lNamePointer = ToFilePointer(ThunkData.u1.AddressOfData);
				if ( m_lpFile->Seek(lNamePointer, FILE_BEGIN) != lNamePointer ||
					m_lpFile->Read(cName, MAX_PATH) == 0 
					)
				{
					break;
				}
				cName[MAX_PATH - 1] = 0;

				PIMAGE_IMPORT_BY_NAME lpName = (PIMAGE_IMPORT_BY_NAME)cName;
				if ( !pfnEnumFunc(szDllName, (CHAR*)lpName->Name, lFirstThunkDataOffset, lParam) )
				{
					bContinue = FALSE;
					break;
				}
			}

			lThunkDataOffset += sizeof (ThunkData);
			lThunkDataPointer += sizeof (ThunkData);
			lFirstThunkDataOffset += sizeof (ThunkData);
		}

		lImpDescPointer += sizeof (ImpDesc);
	}
}

void CPEParser::EnumBaseReloc(PFN_ENUM_BASERELOC pfnEnumFunc, LPARAM lParam)
{
	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if ( lpDir->VirtualAddress == 0 || lpDir->Size < sizeof (IMAGE_BASE_RELOCATION) )
	{
		return ;
	}

	ULONG lBaseRelocPointer = ToFilePointer(lpDir->VirtualAddress);
	ULONG lBaseRelocPointer_End = lBaseRelocPointer + lpDir->Size;
	BOOL bContinue = TRUE;

	while ( bContinue &&
		lBaseRelocPointer < lBaseRelocPointer_End &&
		m_lpFile->Seek(lBaseRelocPointer, FILE_BEGIN) == lBaseRelocPointer
		)
	{
		IMAGE_BASE_RELOCATION rel;
		if ( !m_lpFile->ReadT(rel) )
		{
			break;
		}

		if ( rel.SizeOfBlock < sizeof (IMAGE_BASE_RELOCATION) ||
			rel.SizeOfBlock > USHRT_MAX
			)
		{
			break;
		}

		DWORD dwItemSize = rel.SizeOfBlock - sizeof (IMAGE_BASE_RELOCATION);
		PWORD lpItem = (PWORD)new BYTE[dwItemSize];
		if ( lpItem == NULL )
		{
			break;
		}

		if ( m_lpFile->Read(lpItem, dwItemSize) != dwItemSize )
		{
			delete lpItem;
			break;
		}

		DWORD dwCount = dwItemSize / sizeof (WORD);
		for ( DWORD j = 0; j < dwCount; j++ )
		{
			WORD w = lpItem[j];
			DWORD type = w >> 12;
			if ( type == 3 )
			{
				if ( !pfnEnumFunc(type, rel.VirtualAddress + (w & 0xFFF), lParam) )
				{
					bContinue = FALSE;
					break;
				}
			}
		}

		delete lpItem;
		lBaseRelocPointer += rel.SizeOfBlock;
	}
}

ULONG CPEParser::FindExpFunction(LPCSTR lpName)
{
	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
	if ( lpDir->VirtualAddress == 0 )
	{
		return EOF;
	}

	ULONG lExpDirPointer = ToFilePointer(lpDir->VirtualAddress);
	if ( lExpDirPointer == EOF )
	{
		return EOF;
	}

	IMAGE_EXPORT_DIRECTORY ExpDir;
	if ( lExpDirPointer != m_lpFile->Seek(lExpDirPointer, FILE_BEGIN) ||
		!m_lpFile->ReadT(ExpDir)
		)
	{
		return EOF;
	}

	ULONG lFunNamePointer = ToFilePointer(ExpDir.AddressOfNames);
	if ( lFunNamePointer == EOF )
	{
		return EOF;
	}

	ULONG lFunAddrPointer = ToFilePointer(ExpDir.AddressOfFunctions);
	if ( lFunAddrPointer == EOF )
	{
		return EOF;
	}

	ULONG lFunOrdsPointer = ToFilePointer(ExpDir.AddressOfNameOrdinals);
	if ( lFunOrdsPointer == EOF )
	{
		return EOF;
	}

	WORD nFunOrd = -1;
	if ( IS_INTRESOURCE(lpName) )
	{
		nFunOrd = (WORD)lpName - (WORD)ExpDir.Base;
	}
	else
	{
		for ( DWORD i = 0; i < ExpDir.NumberOfNames; i++ )
		{
			CHAR szFunName[MAX_PATH];
			ULONG dwNamePointer;

			if ( m_lpFile->Seek(lFunNamePointer, FILE_BEGIN) != lFunNamePointer ||
				!m_lpFile->ReadT(dwNamePointer)
				)
			{
				break;
			}

			dwNamePointer = ToFilePointer(dwNamePointer);
			if ( m_lpFile->Seek(dwNamePointer, FILE_BEGIN) != dwNamePointer ||
				m_lpFile->Read(szFunName, MAX_PATH) == 0
				)
			{
				break;
			}
			szFunName[MAX_PATH - 1] = 0;

			if ( strcmp(szFunName, lpName) == 0 )
			{
				ULONG lOrdPointer = lFunOrdsPointer + i * sizeof (WORD);
				if ( m_lpFile->Seek(lOrdPointer, FILE_BEGIN) == lOrdPointer )
				{
					if ( !m_lpFile->ReadT(nFunOrd) )
					{
						nFunOrd = -1;
					}
				}

				break;
			}

			lFunNamePointer += sizeof (DWORD);
		}
	}

	ULONG nResult = EOF;
	if ( nFunOrd != (WORD)-1 )
	{
		ULONG lAddrPointer = lFunAddrPointer + nFunOrd * sizeof (DWORD);
		if ( m_lpFile->Seek(lAddrPointer, FILE_BEGIN) == lAddrPointer )
		{
			if ( !m_lpFile->ReadT(nResult) )
			{
				nResult = EOF;
			}
		}
	}

	return nResult;
}

ULONG CPEParser::FindImpFunction(LPCSTR lpDll, LPCSTR lpName)
{
	class CImpEnumer
	{
	public:
		static BOOL WINAPI Callback(LPCSTR lpDll, LPCSTR lpName, ULONG lOffset, LPARAM lParam)
		{
			CImpEnumer* _this = (CImpEnumer*)LongToPtr(lParam);
			return _this->_Callback(lpDll, lpName, lOffset);
		}

	private:
		BOOL _Callback(LPCSTR lpDll, LPCSTR lpName, ULONG lOffset)
		{
			if ( _stricmp(lpDll, m_lpDll) == 0 )
			{
				if ( IS_INTRESOURCE(lpName) )
				{
					if ( IS_INTRESOURCE(m_lpName) && m_lpName == lpName )
					{
						m_lOffset = lOffset;
						return FALSE;
					}
				}
				else
				{
					if ( !IS_INTRESOURCE(m_lpName) &&
						strcmp(lpName, m_lpName) == 0
						)
					{
						m_lOffset = lOffset;
						return FALSE;
					}
				}
			}

			return TRUE;
		}

	public:
		LPCSTR	m_lpDll;
		LPCSTR	m_lpName;
		ULONG	m_lOffset;
	} enumer;

	enumer.m_lpDll = lpDll;
	enumer.m_lpName = lpName;
	enumer.m_lOffset = EOF;

	EnumImpFunction(CImpEnumer::Callback, PtrToLong(&enumer));

	return enumer.m_lOffset;
}

void	CPEParser::EnumPointers(PIMAGE_SECTION_HEADER lpSectHead, PFN_ENUM_POINTER pfnEnumFunc, LPARAM lParam)
{
	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if ( lpDir->VirtualAddress != 0 && lpDir->Size > sizeof(IMAGE_BASE_RELOCATION) + 0x10 )
	{ 
		// 如果有重定位信息，依赖重定位信息获取指针
		ULONG lBaseRelocPointer = ToFilePointer(lpDir->VirtualAddress);
		const ULONG lBaseRelocPointer_End = lBaseRelocPointer + lpDir->Size;
		const ULONG nMaxVirtualAddress = lpSectHead->VirtualAddress + lpSectHead->Misc.VirtualSize;
		BOOL bContinue = TRUE;
		
		while ( bContinue && 
			lBaseRelocPointer < lBaseRelocPointer_End &&
			m_lpFile->Seek(lBaseRelocPointer, FILE_BEGIN) == lBaseRelocPointer
			)
		{
			IMAGE_BASE_RELOCATION rel;
			if ( !m_lpFile->ReadT(rel) )
			{
				break;
			}

			if ( rel.SizeOfBlock < sizeof (IMAGE_BASE_RELOCATION) ||
				rel.SizeOfBlock > USHRT_MAX
				)
			{
				break;
			}

			if ( rel.VirtualAddress >= lpSectHead->VirtualAddress &&
				rel.VirtualAddress < nMaxVirtualAddress
				)
			{
				DWORD dwItemSize = rel.SizeOfBlock - sizeof (IMAGE_BASE_RELOCATION);
				PWORD lpItem = (PWORD)new BYTE[dwItemSize];
				if ( lpItem == NULL )
				{
					break;
				}

				if ( m_lpFile->Read(lpItem, dwItemSize) != dwItemSize )
				{
					delete lpItem;
					break;
				}

				DWORD dwCount = dwItemSize / sizeof (WORD);
				for ( DWORD j = 0; j < dwCount; j++ )
				{
					WORD w = lpItem[j];
					if ( (w >> 12) == 3 )
					{
						ULONG lOffset = rel.VirtualAddress + (w & 0xFFF);
						if ( lOffset < nMaxVirtualAddress )
						{
							if ( !pfnEnumFunc(lOffset, lParam) )
							{
								bContinue = FALSE;
								break;
							}
						}
					}
				}

				delete lpItem;
			}

			lBaseRelocPointer += rel.SizeOfBlock;
		}
	}
	else
	{
		const ULONG ImageBase = (ULONG)GetImageBase();
		const DWORD dwSectCount = GetSectionCount();
		ULONG MinVA = ULONG_MAX,  MaxVA = 0;

		for ( DWORD i = 0; i < dwSectCount; i++ )
		{
			PIMAGE_SECTION_HEADER lpSectHead = &m_lpSectHeads[i];
			if ( lpSectHead->PointerToRawData != 0 )
			{
				if ( lpSectHead->VirtualAddress < MinVA )
				{
					MinVA = lpSectHead->VirtualAddress;
				}

				const ULONG t = lpSectHead->VirtualAddress + min(lpSectHead->SizeOfRawData, lpSectHead->Misc.VirtualSize);
				if ( t > MaxVA )
				{
					MaxVA = t;
				}
			}
		}
		MinVA += ImageBase;
		MaxVA += ImageBase;

		const DWORD dwBuffSize = 0x4000;
		PVOID lpBuff = new BYTE[dwBuffSize];

		ULONG lFilePointer = m_lpFile->IsRawPtr() ? lpSectHead->PointerToRawData : lpSectHead->VirtualAddress;
		ULONG lFilePointer_End = lFilePointer + lpSectHead->SizeOfRawData;
		ULONG lFixOffset = lpSectHead->VirtualAddress;
		BOOL bContinue = TRUE;

		while ( bContinue &&
			lFilePointer < lFilePointer_End &&
			lFilePointer == m_lpFile->Seek(lFilePointer, FILE_BEGIN) 
			)
		{
			DWORD dwReadBytes;
			if ( lFilePointer + dwBuffSize >= lFilePointer_End )
			{
				dwReadBytes = lFilePointer_End - lFilePointer;
			}
			else
			{
				dwReadBytes = dwBuffSize;
			}

			dwReadBytes = m_lpFile->Read(lpBuff, dwReadBytes);
			if ( dwReadBytes < 4 )
			{
				break;
			}

			PBYTE p = (PBYTE)lpBuff;
			PBYTE p_end = p + dwReadBytes - 3;
			while ( p < p_end )
			{
				DWORD t = *(PDWORD)p;
				if ( t >= MinVA && t < MaxVA  )
				{
					t -= ImageBase;
					if ( EOF != ToFilePointer(t) )
					{
						ULONG lOffset = (ULONG)(p - (PBYTE)lpBuff + lFixOffset);
						if ( !pfnEnumFunc(lOffset, lParam) )
						{
							bContinue = FALSE;
							break;
						}
					}
				}

				p++;
			}

			lFixOffset += dwReadBytes;
			lFilePointer += dwReadBytes;
		}

		delete lpBuff;
		lpBuff = NULL;
	}
}

void	CPEParser::EnumStrings(PIMAGE_SECTION_HEADER lpSectHead, PFN_ENUM_STRING pfnEnumFunc, LPARAM lParam)
{
	class CEnumPointer
	{
	public:
		CEnumPointer()
		{
			m_dwStrBuffSize = 200;
			m_lpStrBuff = new BYTE[m_dwStrBuffSize];
		}

		~CEnumPointer()
		{
			delete m_lpStrBuff;
		}

		static BOOL WINAPI EnumFunc(ULONG lOffset, LPARAM lParam)
		{
			CEnumPointer* _this = (CEnumPointer*)LongToPtr(lParam);

			return _this->_EnumFunc(lOffset);
		}
		
		BOOL WINAPI _EnumFunc(ULONG lOffset)
		{
			BOOL bResult = TRUE;

			ULONG lFilePointer = lOffset - m_lpSectHead->VirtualAddress + m_lpSectHead->PointerToRawData;
			if ( lFilePointer != m_lpFile->Seek(lFilePointer, FILE_BEGIN) )
			{
				return TRUE;
			}

			const DWORD k = 4;
			const DWORD dwBuffLen = 0x10;
			BYTE cBuff[dwBuffLen];

			const ULONG lRefOffset = lOffset;
			const ULONG lRefFilePointer = m_lpParser->ToFilePointer(lRefOffset);

			// 倒退 nFixBuffOffset 字节，读取最大的操作码
			if ( m_lpFile->Seek(lRefFilePointer - k, FILE_BEGIN) != lRefFilePointer - k ||
				m_lpFile->Read(cBuff, dwBuffLen) != dwBuffLen
				)
			{
				return TRUE;
			}

			// @1 常见字符串引用
			// push imm32
			// mov r32, imm32
			// lea r32, [imm32]
			// mov [ebp-xx], imm32
			// mov [esp-xx], imm32

			// @2 常见 import 引用
			// call [imm32]
			// mov	r32, [imm32]
			// jmp [imm32]
			DWORD dwImm32Type = 0;
			DWORD m = 0;

			if ( cBuff[k - 1] == 0x68 )
			{
				// push imm32
				m = 1;
				dwImm32Type = 1;
			}
			else if ( cBuff[k - 1] >= 0xB8 && cBuff[k - 1] <= 0xBF )
			{
				// mov r32, imm32
				m = 1;
				dwImm32Type = 1;
			}
		/*	else if ( cBuff[k - 1] == 0xA1 )
			{
				// mov eax, [imm32]
				m = 1;
				dwImm32Type = 2;
			} */
			else if ( cBuff[k - 2] == 0x8d )
			{
				if ( (cBuff[k - 1] & 0x03) == 5 && (cBuff[k - 1] & 0xC0) == 0xC0 )
				{
					// lea r32, [imm32]
					m = 2;
					dwImm32Type = 1;
				}
			}
	/*		else if ( cBuff[k - 2] == 0x8b )
			{
				if ( (cBuff[k - 1] & 0x03) == 5 && (cBuff[k - 1] & 0xC0) == 0xC0 )
				{
					// mov r32, [imm32]
					m = 2;
					dwImm32Type = 2;
				}
			} */
		/*	else if ( cBuff[k - 2] == 0xFF )
			{
				if ( cBuff[k - 1] == 0x15 )
				{
					// call [imm32]
					m = 2;
					dwImm32Type = 2;
				}
				else if ( cBuff[k - 1] == 0x25 )
				{
					// jmp [imm32]
					m = 2;
					dwImm32Type = 2;
				}
			} */
			else if ( cBuff[k - 3] == 0xC7 )
			{
				if ( cBuff[k - 2] == 0x45 )
				{
					// mov [ebp + imm8], imm32
					m = 3;
					dwImm32Type = 1;
				}
			}
			else if ( cBuff[k - 4] == 0xC7 )
			{
				if ( cBuff[k - 3] == 0x44 && cBuff[k - 2] == 0x24 )
				{
					// mov [esp + imm8], imm32
					m = 4;
					dwImm32Type = 1;
				}
			}

			if ( dwImm32Type == 0 )
			{
				return TRUE;
			}

			const DWORD dwImm32 = *(PDWORD)(cBuff + k);
			const ULONG lImm32Offset = dwImm32 - (ULONG)m_lpParser->GetImageBase();
			if ( lImm32Offset == 0 )
			{
				return TRUE;
			}

			if ( dwImm32Type == 1 )
			{
				ULONG lStringPointer = m_lpParser->ToFilePointer(lImm32Offset);
				if ( lStringPointer != EOF &&
					m_lpFile->Seek(lStringPointer, FILE_BEGIN) == lStringPointer
					)
				{
					// string
					DWORD dwReadBytes;

					dwReadBytes = m_lpFile->Read(m_lpStrBuff, m_dwStrBuffSize);
					if ( dwReadBytes != 0 )
					{
						DWORD dwStrLen = GetUnicodeStringLen(m_lpStrBuff, dwReadBytes);
						if ( dwStrLen != 0 )
						{
							if ( !m_pfnEnumString(TRUE, m_lpStrBuff, dwStrLen, m_lParam) )
							{
								return FALSE;
							}
						}
						else
						{
							dwStrLen = GetAsciiStringLen(m_lpStrBuff, dwReadBytes);
							if ( dwStrLen != 0 )
							{
								if ( !m_pfnEnumString(FALSE, m_lpStrBuff, dwStrLen, m_lParam) )
								{
									return FALSE;
								}
							}
						}
					}
				}
			}

			return TRUE;
		}

	public:
		PBYTE					m_lpStrBuff;
		DWORD					m_dwStrBuffSize;

		CPEParser*				m_lpParser;
		IPEFile*				m_lpFile;
		PIMAGE_SECTION_HEADER	m_lpSectHead;
		PFN_ENUM_STRING			m_pfnEnumString;
		LPARAM					m_lParam;
	} enumer;

	enumer.m_lpParser = this;
	enumer.m_lpFile = m_lpFile;
	enumer.m_lpSectHead = lpSectHead;
	enumer.m_pfnEnumString = pfnEnumFunc;
	enumer.m_lParam = lParam;

	EnumPointers(lpSectHead, CEnumPointer::EnumFunc, PtrToLong(&enumer));
}

ULONG	CPEParser::ReadImageData(ULONG lOffset, PVOID lpData, ULONG nSize)
{
	ULONG lReadBytes = 0;

	IPEFile* lpFile = GetFileObject();
	if ( lpFile != NULL )
	{
		ULONG lFilePointer = ToFilePointer(lOffset);
		if ( lFilePointer != EOF )
		{
			if ( lFilePointer == lpFile->Seek(lFilePointer, FILE_BEGIN) )
			{
				lReadBytes = lpFile->Read(lpData, nSize);
			}
		}
	}

	return lReadBytes;
}

BOOL CPEParser::FixBaseReloc(ULONG lNewBase, ULONG lOffset, PVOID lpData, DWORD nSize)
{
	BOOL bResult = FALSE;

	const ULONG lDiff = lNewBase - (ULONG)GetImageBase();
	if ( lDiff == 0 )
	{
		// 无需重定位
		return TRUE;
	}

	PIMAGE_DATA_DIRECTORY lpDir = GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
	if ( lpDir->VirtualAddress == 0 || lpDir->Size < sizeof (IMAGE_BASE_RELOCATION) )
	{
		// 无法重定位
		return FALSE;
	}

	bResult = TRUE;

	const ULONG lOffset_End = lOffset + nSize;
	ULONG lBaseRelocPointer = ToFilePointer(lpDir->VirtualAddress);
	const ULONG lBaseRelocPointer_End = lBaseRelocPointer + lpDir->Size;
	while (
		lBaseRelocPointer < lBaseRelocPointer_End &&
		m_lpFile->Seek(lBaseRelocPointer, FILE_BEGIN) == lBaseRelocPointer
		)
	{
		IMAGE_BASE_RELOCATION rel;
		if ( !m_lpFile->ReadT(rel) )
		{
			bResult = FALSE;
			break;
		}

		if ( rel.SizeOfBlock < sizeof (IMAGE_BASE_RELOCATION) ||
			rel.SizeOfBlock > USHRT_MAX
			)
		{
			bResult = FALSE;
			break;
		}

		if ( // 检查偏移在区间
			(lOffset >= rel.VirtualAddress && lOffset < rel.VirtualAddress + USHRT_MAX) ||
			(lOffset_End > rel.VirtualAddress && lOffset_End < rel.VirtualAddress + USHRT_MAX)
			)
		{
			DWORD dwItemSize = rel.SizeOfBlock - sizeof (IMAGE_BASE_RELOCATION);
			PWORD lpItem = (PWORD)new BYTE[dwItemSize];
			if ( lpItem == NULL )
			{
				bResult = FALSE;
				break;
			}

			if ( m_lpFile->Read(lpItem, dwItemSize) != dwItemSize )
			{
				delete lpItem;
				bResult = FALSE;
				break;
			}

			DWORD dwCount = dwItemSize / sizeof (WORD);
			for ( DWORD j = 0; j < dwCount; j++ )
			{
				WORD w = lpItem[j];
				DWORD type = w >> 12;
				if ( type == 3 )
				{
					const ULONG j = rel.VirtualAddress + (w & 0xffff);
					if ( j >= lOffset && j < lOffset_End )
					{
						*(PDWORD)((PBYTE)lpData + (j - lOffset)) += lDiff;
					}
				}
			}

			delete lpItem;
		}
		
		lBaseRelocPointer += rel.SizeOfBlock;
	}

	return bResult;
}

BOOL CPEParser::IsPEFile(LPCTSTR lpPath)
{
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
		IMAGE_DOS_HEADER DosHeader;
		IMAGE_NT_HEADERS NtHeader;
		DWORD dwReadBytes;

		if ( ReadFile(hFile, &DosHeader, sizeof (DosHeader), &dwReadBytes, NULL) &&
			dwReadBytes == sizeof (DosHeader) &&
			DosHeader.e_magic == IMAGE_DOS_SIGNATURE &&
			DosHeader.e_lfanew != 0 
			)
		{
			SetFilePointer(hFile, DosHeader.e_lfanew, NULL, FILE_BEGIN);
			if ( ReadFile(hFile, &NtHeader, sizeof (NtHeader), &dwReadBytes, NULL) &&
				dwReadBytes == sizeof (NtHeader) &&
				NtHeader.Signature == IMAGE_NT_SIGNATURE &&
				NtHeader.FileHeader.NumberOfSections != 0 
				)
			{
				bResult = TRUE;
			}
		}

		CloseHandle(hFile);
	}

	return bResult;
}

BOOL CPEParser::IsPackerFile(LPCTSTR lpPath)
{
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
		IMAGE_DOS_HEADER DosHeader;
		IMAGE_NT_HEADERS NtHeader;
		IMAGE_SECTION_HEADER SectHeader;
		DWORD dwReadBytes;

		if ( ReadFile(hFile, &DosHeader, sizeof (DosHeader), &dwReadBytes, NULL) &&
			dwReadBytes == sizeof (DosHeader) &&
			DosHeader.e_magic == IMAGE_DOS_SIGNATURE &&
			DosHeader.e_lfanew != 0
			)
		{
			SetFilePointer(hFile, DosHeader.e_lfanew, NULL, FILE_BEGIN);
			if ( ReadFile(hFile, &NtHeader, sizeof (NtHeader), &dwReadBytes, NULL) &&
				dwReadBytes == sizeof (NtHeader) &&
				NtHeader.Signature == IMAGE_NT_SIGNATURE &&
				NtHeader.FileHeader.NumberOfSections != 0 
				)
			{
				SetFilePointer(hFile, 
					NtHeader.FileHeader.SizeOfOptionalHeader - sizeof (NtHeader.OptionalHeader),
					NULL,
					FILE_CURRENT
					);

				if ( ReadFile(hFile, &SectHeader, sizeof (IMAGE_SECTION_HEADER), &dwReadBytes, NULL) &&
					dwReadBytes == sizeof (IMAGE_SECTION_HEADER)
					)
				{
					if ( memcmp(SectHeader.Name, ".textbss", 8) != 0 &&
						SectHeader.SizeOfRawData == 0
						)
					{
						if ( NtHeader.OptionalHeader.AddressOfEntryPoint < NtHeader.OptionalHeader.BaseOfCode ||
							NtHeader.OptionalHeader.AddressOfEntryPoint >= NtHeader.OptionalHeader.BaseOfCode + NtHeader.OptionalHeader.SizeOfCode
							)
						{
							bResult = TRUE;
							goto _Exit_Return;
						}

						if ( NtHeader.OptionalHeader.AddressOfEntryPoint > SectHeader.VirtualAddress + SectHeader.Misc.VirtualSize ||
							(memcmp(SectHeader.Name, ".text\x0\x0\x0", 8) != 0 && memcmp(SectHeader.Name, "CODE\x0\x0\x0\x0", 8) != 0)
							)
						{
							bResult = TRUE;
							goto _Exit_Return;
						}
					}
				}
			}
		}

_Exit_Return:
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}

	return bResult;
}