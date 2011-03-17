#pragma once


#define BEGIN_MSG_MAP_EX_DECLARE(theClass) \
public: \
	BOOL m_bMsgHandled; \
	/* "handled" management for cracked handlers */ \
	BOOL IsMsgHandled() const \
	{ \
	return m_bMsgHandled; \
	} \
	void SetMsgHandled(BOOL bHandled) \
	{ \
	m_bMsgHandled = bHandled; \
	} \
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
	{ \
	BOOL bOldMsgHandled = m_bMsgHandled; \
	BOOL bRet = _ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	m_bMsgHandled = bOldMsgHandled; \
	return bRet; \
	} \
	BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);

#define BEGIN_MSG_MAP_EX_IMP(theClass) \
	BOOL theClass::_ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID) \
	{ \
	BOOL bHandled = TRUE; \
	hWnd; \
	uMsg; \
	wParam; \
	lParam; \
	lResult; \
	bHandled; \
	switch(dwMsgMapID) \
		{ \
	case 0:


#define END_MSG_MAP_IMP() \
	break; \
		default: \
		ATLTRACE(ATL::atlTraceWindowing, 0, _T("Invalid message map ID (%i)\n"), dwMsgMapID); \
		ATLASSERT(FALSE); \
		break; \
	} \
	return FALSE; \
	}