// dummyz@126.com
// 需要 libudis86 支持
// lOffset raw

#pragma once

class IPEFile;
class CPEParser;
#include "libudis86/udis86.h"

class CPEDiasm
{
public:
	CPEDiasm(DWORD dwAddress = 32, DWORD dwBuffLen = 0x100);
	virtual ~CPEDiasm();

	BOOL	Attach(IPEFile* m_lpFile, ULONG lEip, ULONG lOffset/*raw*/);
	void	Detach();

	BOOL	SetDisameOffset(ULONG lEip, ULONG lOffset/*raw*/);
	ULONG	GetDisasmOffset(ULONG& lEip);
	BOOL	GotoEip(ULONG lEip);

	// 反汇编
	ud*		Disasm();

protected:
	static int UdInputHook(ud* u);
	int _UdInputHook();

protected:
	ULONG		m_nBufOffset;
	ULONG		m_nBufLen;

	ULONG		m_nDisasmBufLen;
	PBYTE		m_lpDisasmBuff;
	ud			m_ud;
	BOOL		m_bEof;

	ULONG		m_lDisasmEip;
	ULONG		m_lDisasmOffset;
	IPEFile*	m_lpFile;
};