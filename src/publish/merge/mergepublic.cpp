#include "stdafx.h"
#include "mergepublic.h"
//#include "kdebuglog.h"

wchar_t* pszLogFileName = L"patch_log.txt";
bool	 blogfile		= false;

void logtofileW(
	const wchar_t* szLogMsg
	)
{
	if (!blogfile ||
		!szLogMsg)
		return;
	/*FILE* fp = NULL;
	 _wfopen_s(&fp, pszLogFileName, L"a+");
	if (fp)
		fwprintf(fp, L"%s",	szLogMsg);
	if (fp)
		fclose(fp);*/
	//KAV_LOG2(_T("path"), szLogMsg);
}

void enablelog(bool benable)
{
	blogfile = benable;
}