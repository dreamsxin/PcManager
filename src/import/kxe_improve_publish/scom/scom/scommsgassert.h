/********************************************************************
* CreatedOn: 2005-11-29   11:32
* FileName: SCOMMsgAssert.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef SCOMMSGASSERT_H
#define SCOMMSGASSERT_H

#include <assert.h>
#include <string>

#if     _MSC_VER >= 1400
#define _SCOM_ASSERT_T(x) L ## x
typedef wchar_t SCOM_ASSERT_CHAR;
typedef std::wstring SCOM_ASSERT_STR;
#else
#define _SCOM_ASSERT_T(x) x
typedef char SCOM_ASSERT_CHAR;
typedef std::string SCOM_ASSERT_STR;
#endif

#define SCOM_ASSERT_TEXT(x) _SCOM_ASSERT_T(x)

#ifdef _DEBUG
#define SCOM_ASSERT_MESSAGE(message, e) ((e) ? (void)0 : SCOM_Asserter(message, SCOM_ASSERT_TEXT(#e), SCOM_ASSERT_TEXT(__FILE__), __LINE__))


#if _MSC_VER >= 1400
inline void SCOM_Asserter(const wchar_t *pszMessage, const wchar_t* pszExp, const wchar_t* pszFileName, int nFileLine)
{
	wchar_t szMsg[512] = { 0 };
	size_t count = 511;
	wchar_t* pos = szMsg;
	const wchar_t* pszMsgTitle = SCOM_ASSERT_TEXT("\nMessage: ");
	if (pszExp)
	{
		while (count && (*pos++ = *pszExp++)) 
			count--;
	}
	if (0 == *pos)
		--pos;
	while (count && (*pos++ = *pszMsgTitle++)) 
		count--;	

	if (pszMessage)
	{
		if (0 == *pos)
			--pos;

		while (count && (*pos++ = *pszMessage++)) 
			count--;	
	}
	_wassert(szMsg, pszFileName, nFileLine);
}

#elif _MSC_VER == 1200

inline void SCOM_Asserter(const char *pszMessage, const char *pszExp, const char *pszFileName, int nFileLine)
{
	char szMsg[512] = { 0 };
	size_t count = 511;
	char* pos = szMsg;
	const char* pszMsgTitle = SCOM_ASSERT_TEXT("\nMessage: ");
	if (pszExp)
	{
		while (count && (*pos++ = *pszExp++)) 
			count--;
	}
	
	while (count && (*pos++ = *pszMsgTitle++)) 
		count--;	
	
	if (pszMessage)
	{
		while (count && (*pos++ = *pszMessage++)) 
			count--;	
	}	
	_assert(szMsg, const_cast<char*>(pszFileName), nFileLine);	
}

#else
inline void SCOM_Asserter(const char *pszMessage, const char *pszExp, const char *pszFileName, int nFileLine)
{
	char szMsg[512] = { 0 };
	size_t count = 511;
	char* pos = szMsg;
	const char* pszMsgTitle = SCOM_ASSERT_TEXT("\nMessage: ");
	if (pszExp)
	{
		while (count && (*pos++ = *pszExp++)) 
			count--;
	}

	while (count && (*pos++ = *pszMsgTitle++)) 
		count--;	

	if (pszMessage)
	{
		while (count && (*pos++ = *pszMessage++)) 
			count--;	
	}	
	_assert(szMsg, pszFileName, nFileLine);	
}
#endif

#else

#define SCOM_ASSERT_MESSAGE(message, e) ((void)0)

#endif

#endif //SCOMMSGASSERT_H