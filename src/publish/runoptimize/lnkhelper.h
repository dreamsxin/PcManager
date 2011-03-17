#pragma once

#include <shlobj.h>
#include <Msi.h>

#pragma comment(lib,"msi.lib")

class CLnkDecode
{
public:
	static BOOL IsLnkPostFile(LPCTSTR lpFileName)
	{
		LPCTSTR		lpPostFix = GetFilePostFix(lpFileName);

		if (lpPostFix==NULL)
			return FALSE;

		return (_tcsicmp(lpPostFix,_T("lnk"))==0);
	}
	static BOOL QueryLnk(LPCTSTR lpLinkPath,CString & strExePath,CString &strExeParam)
	{
		IShellLink*	pShellLink;
		LPCTSTR		lpPostFix = GetFilePostFix(lpLinkPath);
		BOOL bReturn = FALSE;
		TCHAR szProductCode[MAX_GUID_CHARS + 1];
		TCHAR szFeatureId[MAX_FEATURE_CHARS + 1];
		TCHAR szComponentCode[MAX_GUID_CHARS + 1];
		TCHAR szPath[MAX_PATH + 1];
		DWORD dwLen = MAX_PATH;

		if (ERROR_SUCCESS == MsiGetShortcutTarget(lpLinkPath, szProductCode, szFeatureId, szComponentCode))
		{
			MsiGetComponentPath(szProductCode, szComponentCode, szPath, &dwLen);
			strExePath = szPath;
			bReturn = TRUE;
		}
		else
		{
			bReturn = (::CoInitialize(NULL) == S_OK);
			if(TRUE || bReturn)
			{
				bReturn = ::CoCreateInstance (CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
					IID_IShellLink, (void **)&pShellLink) >= 0;
				if(bReturn)
				{
					IPersistFile *ppf;
					bReturn = pShellLink->QueryInterface(IID_IPersistFile, (void **)&ppf) >= 0;
					if(bReturn && ppf)
					{
						bReturn = ppf->Load(lpLinkPath, TRUE) >= 0;
						if(bReturn)
						{
							CString		strIePath;
							CString		strParam;
							pShellLink->GetPath( strIePath.GetBuffer(MAX_PATH+1), MAX_PATH, NULL, SLGP_UNCPRIORITY );
							pShellLink->GetArguments( strParam.GetBuffer(MAX_PATH + 1),MAX_PATH);
							strIePath.ReleaseBuffer();
							strParam.ReleaseBuffer();

							if (!strIePath.IsEmpty())
							{
								strExePath = (LPCTSTR)strIePath;

								if (!strParam.IsEmpty())
									strExeParam = strParam;

								bReturn = TRUE;
							}
							else
								bReturn = FALSE;
						}
						ppf->Release();
					}
					pShellLink->Release();
				}
				::CoUninitialize();
			}
		}

		return bReturn;
	}

protected:
	static LPCTSTR GetFilePostFix(LPCTSTR lpFile)
	{
		LPCTSTR lpPostFix = _tcsrchr(lpFile, _T('.'));
		if ( lpPostFix != NULL )
			lpPostFix++;
		return lpPostFix;
	}
};