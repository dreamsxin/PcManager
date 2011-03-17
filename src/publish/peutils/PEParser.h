// dummyz@126.com
// Offset 都是 rva 偏移

#pragma once

#include <list>

#include "PEFile.h"

class CPEExport;
class CPEImport;
class CPEResource;

typedef BOOL (WINAPI *PFN_ENUM_RESTYPE)(PIMAGE_RESOURCE_DIRECTORY lpDir, LPCTSTR lpType, ULONG lOffset, LPARAM lParam);
typedef BOOL (WINAPI *PFN_ENUM_RESNAME)(PIMAGE_RESOURCE_DIRECTORY lpDir, LPCTSTR lpName, ULONG lOffset, DWORD dwSize, DWORD dwCodePage, LPARAM lParam);
typedef BOOL (WINAPI *PFN_ENUM_EXPFUN)(PIMAGE_EXPORT_DIRECTORY lpDir, LPCSTR lpDll, LPCSTR lpName, ULONG lOffset, LPARAM lParam);
typedef BOOL (WINAPI *PFN_ENUM_IMPFUN)(LPCSTR lpDll, LPCSTR lpName, ULONG lOffset, LPARAM);
typedef BOOL (WINAPI *PFN_ENUM_BASERELOC)(ULONG nType, ULONG lOffset, LPARAM lParam);
typedef BOOL (WINAPI *PFN_ENUM_STRING)(BOOL bUnicode, LPCVOID lpStr, DWORD dwStrLen, LPARAM lParam);
typedef BOOL (WINAPI *PFN_ENUM_POINTER)(ULONG lOffset, LPARAM lParam);

class CPEParser
{
public:
	CPEParser();
	virtual ~CPEParser();

	BOOL		Attach(IPEFile* lpFile);
	void		Detach();
	IPEFile*	GetFileObject() {
		return m_lpFile;
	}

	BOOL		IsAttached() const {
		return (m_lpFile != NULL);
	}

public:
	static BOOL IsPEFile(LPCTSTR lpPath);
	static BOOL IsPackerFile(LPCTSTR lpPath);

public:
	BOOL IsPEPlus() const {
		assert(m_lpFile != NULL);
		return m_bPEPlus;
	}

	BOOL IsDll() {
		WORD wSubsysem = GetSubsystem();
		return ((wSubsysem == IMAGE_SUBSYSTEM_WINDOWS_GUI || wSubsysem == IMAGE_SUBSYSTEM_WINDOWS_CUI) &&
			(GetNtFileHead()->Characteristics & IMAGE_FILE_DLL) != 0
			);
	}

	BOOL IsSys() {
		WORD wSubsystem = GetSubsystem();
		return (wSubsystem == IMAGE_SUBSYSTEM_NATIVE);
	}

	BOOL IsExe() {
		WORD wSubsysem = GetSubsystem();
		return ((wSubsysem == IMAGE_SUBSYSTEM_WINDOWS_GUI || wSubsysem == IMAGE_SUBSYSTEM_WINDOWS_CUI) &&
			(GetNtFileHead()->Characteristics & IMAGE_FILE_DLL) == 0
			);
	}

	PIMAGE_DOS_HEADER GetDosHead() {
		assert(m_lpFile != NULL);
		return &m_DosHead;
	}

	PIMAGE_FILE_HEADER GetNtFileHead() {
		assert(m_lpFile != NULL);
		return &m_NtHead64.FileHeader;
	}

	ULONG GetEntryPoint() const {
		return m_bPEPlus ? m_NtHead64.OptionalHeader.AddressOfEntryPoint : m_NtHead32.OptionalHeader.AddressOfEntryPoint;
	}

	ULONGLONG GetImageBase() const {
		return m_bPEPlus ? m_NtHead64.OptionalHeader.ImageBase : m_NtHead32.OptionalHeader.ImageBase;
	}

	ULONG GetImageSize() const {
		return m_bPEPlus ? m_NtHead64.OptionalHeader.SizeOfImage : m_NtHead32.OptionalHeader.SizeOfImage;
	}

	WORD GetSubsystem() const {
		return m_bPEPlus ? m_NtHead64.OptionalHeader.Subsystem : m_NtHead32.OptionalHeader.Subsystem;
	}
	
	PIMAGE_NT_HEADERS GetNtHead() {
		assert(m_lpFile != NULL);
		return m_bPEPlus ? (PIMAGE_NT_HEADERS)&m_NtHead64 : (PIMAGE_NT_HEADERS)&m_NtHead32;
	}

	PIMAGE_SECTION_HEADER GetSectionHead(DWORD dwIndex) {
		assert(m_lpFile != NULL);

		if ( m_lpSectHeads != NULL && dwIndex >= 0 && dwIndex < GetNtFileHead()->NumberOfSections )
			return m_lpSectHeads + dwIndex;

		return NULL;
	}

	PIMAGE_DATA_DIRECTORY GetDataDirectory(DWORD dwDirIndex) {
		if ( dwDirIndex >= 0 && dwDirIndex < IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
		{
			return m_bPEPlus ? &m_NtHead64.OptionalHeader.DataDirectory[dwDirIndex] :
				&m_NtHead32.OptionalHeader.DataDirectory[dwDirIndex];
		}

		return NULL;
	}
	
	DWORD GetSectionCount() {
		return GetNtFileHead()->NumberOfSections;
	}

	PIMAGE_SECTION_HEADER GetSectionHeadByOffset(ULONG lOffset);
	ULONG	GetOverlayOffset(LPDWORD lpdwSize); // Raw

	ULONG	CalcChecksum(BOOL bHaveOverlay);
	BOOL	CheckDataVaild();

	// 读取指定的数据, 返回读取个数
	ULONG	ReadImageData(ULONG lOffset, PVOID lpData, ULONG nSize);

	// 重定位指定的数据段
	BOOL	FixBaseReloc(ULONG lNewBase, ULONG lOffset, PVOID lpData, DWORD nSize);

public:
	static ULONG	AligUP(ULONG x, ULONG y)
	{
		if ( y != 0 )
		{
			ULONG r = x % y;
			return (r != 0) ? x + y - r : x;
		}
		
		return 0;
	}
	
	static ULONG	AligDOWN(ULONG x, ULONG y)
	{
		return (x < y) ? 0 : x - (x % y);
	}

public:
	ULONG	ToFilePointer(ULONG lOffset); // to raw
	ULONG	FilePointerTo(ULONG lOffset); // to rva

	BOOL	IsVaildOffset(ULONG lOffset)
	{
		assert(m_lpFile != NULL);

		return (lOffset >= 0) &&
			(m_bPEPlus ? (lOffset < m_NtHead32.OptionalHeader.SizeOfImage) : (lOffset < m_NtHead64.OptionalHeader.SizeOfImage));
	}

	BOOL	IsVaildFilePointer(ULONG lOffset) 
	{
		assert(m_lpFile != NULL);

		return (lOffset >= 0) && (lOffset < m_lpFile->GetSize());
	}

public:
	// 查找资源
	ULONG	FindResourceType(LPCWSTR lpType);
	ULONG	FindResourceName(ULONG lOffset, LPCWSTR lpName, DWORD dwCodePage, DWORD* lpdwSize);
	ULONG	FindResource(LPCWSTR lpName, LPCWSTR lpType, DWORD dwCodePage, DWORD* lpdwSize);

	void	EnumResourceTypes(PFN_ENUM_RESTYPE lpEnumFunc, LPARAM lParam);
	void	EnumResourceNames(LPCWSTR lpType, PFN_ENUM_RESNAME lpEnumFunc, LPARAM lParam);
	void	EnumResourceNames(ULONG lOffset, PFN_ENUM_RESNAME lpEnumFunc, LPARAM lParam);

	void	EnumExpFunction(PFN_ENUM_EXPFUN pfnEnumFunc, LPARAM lParam);
	void	EnumImpFunction(PFN_ENUM_IMPFUN pfnEnumFunc, LPARAM lParam);
	void	EnumBaseReloc(PFN_ENUM_BASERELOC pfnEnumFunc, LPARAM lParam);

	void	EnumPointers(PIMAGE_SECTION_HEADER lpSectHead, PFN_ENUM_POINTER pfnEnumFunc, LPARAM lParam);
	void	EnumStrings(PIMAGE_SECTION_HEADER lpSectHead, PFN_ENUM_STRING pfnEnumFunc, LPARAM lParam);

	// 查找导出的函数，返回地址
	ULONG	FindExpFunction(LPCSTR lpName);
	ULONG	FindImpFunction(LPCSTR lpDll, LPCSTR lpName);

protected:
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	ReadResDirEntry(ULONG lOffset, IMAGE_RESOURCE_DIRECTORY* lpResDir);
	LPWSTR	ReadResDirName(ULONG lOffset);

protected:
	IPEFile*				m_lpFile;

	BOOL					m_bPEPlus;
	IMAGE_DOS_HEADER		m_DosHead;
	IMAGE_NT_HEADERS32		m_NtHead32;
	IMAGE_NT_HEADERS64		m_NtHead64;
	PIMAGE_SECTION_HEADER	m_lpSectHeads;
};