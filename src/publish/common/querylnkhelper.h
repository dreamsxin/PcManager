
#pragma once

#include "autoinitcom.h"
#include <Shlwapi.h>


class CQueryUrl
{
public:
	CQueryUrl(){}
	virtual ~CQueryUrl(){}

	BOOL Load(LPCTSTR lpFilePath)
	{
		m_strUrl.Empty();
		::GetPrivateProfileString( _T("InternetShortcut"), _T("URL"),_T(""), m_strUrl.GetBuffer(MAX_PATH), MAX_PATH, lpFilePath);
		m_strUrl.ReleaseBuffer();

		return !m_strUrl.IsEmpty();
	}

	CString& GetUrl()
	{
		return m_strUrl;
	}

protected:
	CString	m_strUrl;
};

class CQueryLnk
{
public:
	CQueryLnk(){}
	virtual ~CQueryLnk(){}

	BOOL Load(LPCTSTR lpLinkPath)
	{
		CAutoCoInit			initCom;
		BOOL				bReturn = FALSE;
		CComPtr<IShellLink>	pShellLink;
		
		if(initCom.IsSucceed())
		{
			::CoCreateInstance (CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
				IID_IShellLink, (void **)&pShellLink);
			if(pShellLink)
			{
				CComPtr<IPersistFile> ppf;
				pShellLink->QueryInterface(IID_IPersistFile, (void **)&ppf);
				if(ppf)
				{
					if(SUCCEEDED(ppf->Load(lpLinkPath,0)))
					{
						CString			strIePath;
						LPITEMIDLIST	idlist;
						HRESULT			hrIDList = S_OK;

						hrIDList = pShellLink->GetIDList(&idlist);
						if ( SUCCEEDED(hrIDList) )
						{
							SHGetPathFromIDList(idlist,strIePath.GetBuffer(MAX_PATH+1));
							strIePath.ReleaseBuffer();
						}
						if (strIePath.IsEmpty())
						{
							pShellLink->GetPath( strIePath.GetBuffer(MAX_PATH+1), MAX_PATH, NULL, SLGP_UNCPRIORITY );
							strIePath.ReleaseBuffer();
						}

						if (strIePath.IsEmpty()&&SUCCEEDED(hrIDList))
						{
							LPITEMIDLIST p = idlist;
							while ( p != NULL )
							{
								TCHAR					szName[MAX_PATH] = {0};
								CComPtr<IShellFolder>	spShellFolder;
								LPCITEMIDLIST			pItemChild;

								HRESULT hr = ::SHBindToParent(p, IID_IShellFolder, (LPVOID*)&spShellFolder, &pItemChild);
								if ( S_OK == hr || NULL != spShellFolder )
								{
									STRRET strDispName;

									hr = spShellFolder->GetDisplayNameOf(pItemChild, SHGDN_FORPARSING, &strDispName);
									if ( S_OK == hr )
									{
										ZeroMemory(szName, sizeof(szName));
										StrRetToBufW(&strDispName, p, szName, MAX_PATH);
										strIePath = szName;
									}
								}

								if (!strIePath.IsEmpty())
									break;

								p = ILGetNext(p);
							}
						}

						m_strExePath = strIePath;

						if ( TRUE )
						{
							pShellLink->GetArguments( m_strParam.GetBuffer(MAX_PATH+1), MAX_PATH );
							m_strParam.ReleaseBuffer();
						}

						bReturn = TRUE;
					}
				}
			}
		}

		return bReturn;
	}

	CString& GetExePath()
	{
		return m_strExePath;
	}

	CString& GetParam()
	{
		return m_strParam;
	}

protected:
	CString	m_strExePath;
	CString	m_strParam;
};
