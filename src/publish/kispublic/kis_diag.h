// ----------------------------------------------------------------------------- 
//	FileName	: framework/kis_diag.h
//	Author		: Zqy
//	Create On	: 2007-9-11 17:59:22
//	Description	: 
//
// ----------------------------------------------------------------------------- 
#ifndef _FRAMEWORK_KIS_DIAG_H_
#define _FRAMEWORK_KIS_DIAG_H_

#ifdef _DEBUG	//ASSERT IMPLEMENT

#ifndef _INC_TCHAR
	#include <tchar.h>
#endif

#ifndef __wtypes_h__
	#include <wtypes.h>
#endif

#ifndef _WINBASE_
	#include <winbase.h>
#endif

#ifndef _WINUSER_
	#include <winuser.h>
#endif

#ifdef __cplusplus
	inline
#else
	__inline
#endif
int __KSASSERT_REPORTER (LPCTSTR lpszAct,
						 LPCTSTR lpszMsg, 
						 LPCTSTR lpszFile,
						 int	 nLine)
{
	TCHAR szMsg  [4096]     = { 0 };
	TCHAR szPath [MAX_PATH] = { 0 };

	GetModuleFileName(NULL, szPath, MAX_PATH);

	_sntprintf_s(szMsg, 4096, 4096,
		_T("%s: %s\n\n")
		_T("Program: %s\n")
		_T("File   : %s\n")
		_T("Line   : %d\n\n")
		_T("(Press Retry to debug the application)\n")
		_T("\nFor information on how your program can cause an assertion")
		_T("\nfailure, see the Visual C++ documentation on asserts.")
		, 
		lpszAct,
		lpszMsg, 
		szPath,
		lpszFile, 
		nLine,
		_TRUNCATE);

	return MessageBox(GetForegroundWindow(),
		szMsg,
		_T("ASSERT Failed"),
		MB_ABORTRETRYIGNORE |
		MB_DEFBUTTON2		 |
		MB_ICONSTOP);
}

#define _KASSERT(act, x, info) \
	do{\
	if (!(x))\
{\
	MSG msg = {0};\
	BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);\
	switch (__KSASSERT_REPORTER(act, info, _T(__FILE__), __LINE__))\
{\
	case IDCANCEL:\
	exit (3);\
	break;\
			case IDRETRY:\
			_asm { int 3 };\
			break;\
			case IDIGNORE:\
			break;\
}\
	if (bQuit)\
	PostQuitMessage((int)msg.wParam);\
}\
	}while (0)


#define _KASSERT_ONCE(act, x, info)\
	do{\
	static bool __assert_once = false;\
	if (!__assert_once && !(x))\
			{\
			__assert_once = true;\
			_KASSERT(act, x, info);\
			}\
	}while (0)

#else
	#define _KASSERT(act, x, info) NULL
	#define _KASSERT_ONCE(act, x, info) NULL
#endif	//ASSERT IMPLEMENT


#ifndef ASSERT
	#define ASSERT(x) _KASSERT(_T("ASSERT"), (x), _T(#x))
#endif

#undef assert
#define assert ASSERT

#ifndef ASSERT_ONCE
	#define ASSERT_ONCE(x) _KASSERT_ONCE(_T("ASSERT_ONCE"), (x), _T(#x))
#endif


#ifndef KS_CHECK_BOOL
#define KS_CHECK_BOOL(result)												\
	if (!(result))															\
	{																		\
		ASSERT_ONCE(result);												\
		goto KS_EXIT;														\
	}
#endif


#ifndef REPORT
	#define REPORT(x)	_KASSERT(_T("REPORT"), 0, _T(x))
#endif

#ifndef REPORT_ONCE
	#define REPORT_ONCE(x) _KASSERT_ONCE(_T("REPORT_ONCE"), 0, _T(x))
#endif



// ----------------------------------------------------------------------------- 
#endif //_FRAMEWORK_KIS_DIAG_H_
