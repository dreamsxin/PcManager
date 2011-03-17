#pragma once


#pragma once

#include "interface.h"
#include "interfaceext.h"


struct KillInterface
{
	CString				strCLSID;
	IModuleKillClient*	pClient;
};

struct ModInfo 
{
public:
	CString				strModPath;
	HMODULE				hMod;
	PFNNewKillClient	pfnNew;
	PFNFreeKillClient	pfnDelete;

	CSimpleArray<KillInterface>	clientList;
};

class CModuleMgrNull : public IModuleKillService
{
public:
	CModuleMgrNull(){}
	virtual ~CModuleMgrNull(void)
	{
		UnLoad();
	}

	BOOL	Load()
	{
		return TRUE;
	}

	BOOL	UnLoad()
	{
		for ( INT nIndex = 0; nIndex < m_modArray.GetSize(); nIndex++)
		{
			ModInfo& modinfo = m_modArray[nIndex];
			for ( INT i = 0; i < modinfo.clientList.GetSize(); i++)
			{	
				modinfo.pfnDelete(modinfo.clientList[i].pClient);
			}
			::FreeLibrary(modinfo.hMod);
		}

		m_modArray.RemoveAll();
		return TRUE;
	}

	BOOL ScanDetail( CString & strModName, CString & strCLSID )
	{
		IModuleKillClient*	pClient = GetKillInterface(strModName,strCLSID);

		if ( pClient )
		{
			return pClient->ScanDetail();
		}
		return FALSE;
	}

	BOOL QuickScan( CString & strModName, CString & strCLSID )
	{
		IModuleKillClient*	pClient = GetKillInterface(strModName,strCLSID);

		if ( pClient )
		{
			return pClient->QuickScan();
		}
		return FALSE;
	}

	BOOL	Remove(CString & strModName, CString & strCLSID)
	{
		IModuleKillClient*	pClient = GetKillInterface(strModName,strCLSID);

		if ( pClient )
		{
			return pClient->Remove();
		}
		return FALSE;
	}
protected:
	IModuleKillClient* GetKillInterface( CString & strModName, CString & strCLSID )
	{
		IModuleKillClient*	pClient = NULL;
		CString				strModFullPath;
		BOOL				bFindMod = FALSE;

		strModFullPath = GetModuleDir();
		strModFullPath.Append(_T("\\") );
		strModFullPath.Append(strModName);

		for ( INT nIndex = 0; nIndex < m_modArray.GetSize(); nIndex++)
		{
			ModInfo& modinfo = m_modArray[nIndex];

			if ( strModFullPath.CompareNoCase(modinfo.strModPath) == 0 )
			{
				bFindMod = TRUE;
				for ( INT i = 0; i < modinfo.clientList.GetSize(); i++)
				{	
					if ( strCLSID.CompareNoCase( modinfo.clientList[i].strCLSID ) == 0 )
					{
						pClient = modinfo.clientList[i].pClient;
						break;
					}
				}
				if ( pClient == NULL )
				{
					pClient = modinfo.pfnNew(VER_EXTEND_MOD,strCLSID);

					if ( pClient != NULL )
					{
						KillInterface	interclinet;
						interclinet.pClient = pClient;
						interclinet.strCLSID= strCLSID;
						modinfo.clientList.Add(interclinet);
					}
				}
				break;
			}
		}

		if ( !bFindMod && pClient == NULL )
		{
			ModInfo	modinfo;
			modinfo.hMod = ::LoadLibrary( strModFullPath );
			modinfo.pfnDelete	= NULL;
			modinfo.pfnNew		= NULL;
			modinfo.strModPath	= strModFullPath;

			if ( modinfo.hMod != NULL )
			{
				modinfo.pfnNew		= (PFNNewKillClient)::GetProcAddress( modinfo.hMod, EXPORT_NEW_CLIENT );
				modinfo.pfnDelete	= (PFNFreeKillClient)::GetProcAddress( modinfo.hMod, EXPORT_DEL_CLIENT );
			}
			if ( modinfo.pfnDelete != NULL && modinfo.pfnNew != NULL )
			{
				pClient = modinfo.pfnNew(VER_EXTEND_MOD,strCLSID);

				if ( pClient )
				{
					KillInterface	interX;
					interX.pClient = pClient;
					interX.strCLSID= strCLSID;

					modinfo.clientList.Add(interX);
					m_modArray.Add(modinfo);
				}
			}
			if ( pClient == NULL && modinfo.hMod != NULL)
				::FreeLibrary(modinfo.hMod);
		}

		if ( pClient )
			pClient->Init(this);

		return pClient;
	}

	LPCTSTR GetModuleDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			GetModuleFileName( (HMODULE)&__ImageBase,szPath, MAX_PATH);
			PathRemoveFileSpec(szPath);
		}
		return szPath;
	}

protected:
	CSimpleArray<ModInfo>	m_modArray;
};
