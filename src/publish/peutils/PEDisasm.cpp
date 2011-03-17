// dummyz@126.com

#include "stdafx.h"
#include "PEDisasm.h"
#include "PEFile.h"
#include <algorithm>

CPEDiasm::CPEDiasm(DWORD dwAddress, DWORD dwBufLen)
{
	m_lpFile = NULL;
	m_lDisasmEip = 0;
	m_lDisasmOffset = 0;

	m_bEof = FALSE;
	m_nBufOffset = 0;
	m_nBufLen = 0;

	m_nDisasmBufLen = max(0x20, dwBufLen);
	m_lpDisasmBuff = new BYTE[m_nDisasmBufLen];

	ud_init(&m_ud);
	ud_set_mode(&m_ud, (uint8_t)dwAddress);
	ud_set_user_data(&m_ud, this);
	ud_set_input_hook(&m_ud, UdInputHook);
}

CPEDiasm::~CPEDiasm()
{
	Detach();

	delete m_lpDisasmBuff;
	m_lpDisasmBuff = NULL;
}

BOOL CPEDiasm::Attach(IPEFile* lpFile, ULONG lEip, ULONG lOffset)
{
	if ( m_lpFile != NULL )
		return FALSE;

	if ( lpFile == NULL || !lpFile->IsOpen() )
		return FALSE;

	m_nBufOffset = 0;
	m_nBufLen = 0;
	m_lpFile = lpFile;

	if ( !SetDisameOffset(lEip, lOffset) )
	{
		Detach();
		return FALSE;
	}

	return TRUE;
}

void CPEDiasm::Detach()
{
	m_lpFile = NULL;
}

BOOL CPEDiasm::SetDisameOffset(ULONG lEip, ULONG lOffset)
{
	assert(m_lpFile != NULL);

	m_bEof = FALSE;
	m_lDisasmEip = lEip;
	m_lDisasmOffset = lOffset;
	m_nBufOffset = 0;
	m_nBufLen = 0;
	ud_set_pc(&m_ud, m_lDisasmEip);
	
	return (lOffset == m_lpFile->Seek(lOffset, FILE_BEGIN));
}

BOOL CPEDiasm::GotoEip(ULONG lEip)
{
	ULONG lOffset = m_lDisasmOffset + (lEip - m_lDisasmEip);
	return SetDisameOffset(lEip, lOffset);
}

ULONG CPEDiasm::GetDisasmOffset(ULONG& lEip)
{
	lEip = m_lDisasmEip;
	return m_lDisasmOffset;
}

ud* CPEDiasm::Disasm()
{
	if ( m_lpFile == NULL || m_bEof )
		return NULL;

	if ( ud_disassemble(&m_ud) != 0 )
	{
#if 0
		ud_translate_intel(&m_ud);
		OutputDebugStringA(ud_insn_asm(&m_ud));
#endif
		return &m_ud;
	}

	return NULL;
}

int CPEDiasm::UdInputHook(ud* u)
{
	CPEDiasm* _this = (CPEDiasm*)u->user_data;
	return _this->_UdInputHook();
}

int CPEDiasm::_UdInputHook()
{
	if ( !m_bEof )
	{
		if ( m_nBufOffset < m_nBufLen )
		{
			return m_lpDisasmBuff[m_nBufOffset++];
		}

		m_nBufOffset = 0;
		m_nBufLen = m_lpFile->Read(m_lpDisasmBuff, m_nDisasmBufLen);

		if ( m_nBufOffset < m_nBufLen )
		{
			return m_lpDisasmBuff[m_nBufOffset++];
		}

		m_bEof = TRUE;
	}
	
	return -1;
}