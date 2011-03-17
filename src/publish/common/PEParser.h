// dummyz@126.com

#pragma once

#include <list>

class CPEParser  
{
public:
	CPEParser();
	virtual ~CPEParser();

	BOOL	Load(LPCTSTR lpPath);
	void	Unload();
	BOOL	IsLoad() const {
		return m_lpMem != NULL;
	}

	PBYTE	GetData() {
		return (PBYTE)m_lpMem;
	}

	DWORD	GetSize() {
		return m_dwMemSize;
	}

	// 使用的 Offset 都是 rva 偏移
	PBYTE	ToPointer(DWORD dwOffset);
	ULONG	ToOffset(PVOID lpPointer);

	BOOL	FixRelocation(ULONG nBaseAddr);
	void	UnInlineHook(PBYTE lpDest, PBYTE lpSrc, DWORD nSize);

	// 查找导出的函数，返回地址
	ULONG	FindFunction(LPCSTR lpName);
	// 查找导出的函数, 返回地址
	ULONG	FindExpThunk(LPCSTR lpName);
	// 查找引入的函数, 返回找到的偏移和个数
	ULONG	FindImpThunk(LPCSTR lpDll, LPCSTR lpName, std::list<DWORD>& dwOffsetList);
	// 查找变量引用关系，返回找到的偏移和个数
	ULONG	FindDataRef(DWORD dwOffset, std::list<DWORD>& dwOffsetList);
	// 查找 call imm32 指令，返回找到的偏移和个数
//	ULONG	FindImmCall(DWORD dwBegin, DWORD dwDest, std::list<DWORD>& dwOffsetList);
	
	PIMAGE_SECTION_HEADER GetFirstSection() {
		return m_lpFirstSection;
	}

	DWORD GetSectionCount() {
		return m_lpNtHeader->FileHeader.NumberOfSections;
	}

	PIMAGE_NT_HEADERS GetNtHeader() {
		return m_lpNtHeader;
	}

	static ULONG AligUp(ULONG x, ULONG y)
	{
		ULONG r = x % y;
		return (r != 0) ? x + y - r : x;
	}

	static ULONG AligDown(ULONG x, ULONG y)
	{
		return (x < y) ? 0 : x - (x % y);
	}

protected:
	PVOID					m_lpMem;
	ULONG					m_dwMemSize;
	ULONG					m_dwBaseAddr;
	PIMAGE_DOS_HEADER		m_lpDosHeader;
	PIMAGE_NT_HEADERS		m_lpNtHeader;
	PIMAGE_SECTION_HEADER	m_lpFirstSection;
};


