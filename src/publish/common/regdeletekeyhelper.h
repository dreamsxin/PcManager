
#pragma once

#include "AccessHelper.h"
#include "RegKeyHelper.h"

class CRecuDeleteKey
{
public:
	CRecuDeleteKey(){}
	~CRecuDeleteKey(){}

	
	DWORD DeleteKey(HKEY hRoot, LPCTSTR lpKeyName)
	{
		if (::SHDeleteKey(hRoot,lpKeyName) == ERROR_SUCCESS )
			return ERROR_SUCCESS;

		CAccessHepler().AddRegAccess(hRoot, lpKeyName, KEY_ALL_ACCESS);
		CRegKeyHepler	key;
		BOOL			bFailed = FALSE;
		DWORD			nRet = ERROR_SUCCESS;

		nRet = key.Open(hRoot,  lpKeyName, KEY_ALL_ACCESS);

		if ( nRet == ERROR_SUCCESS )
		{
			DWORD	iIndex = 0;
			do 
			{
				CString		strSubKey;
				DWORD		nRet = key.EnumKey(iIndex++, strSubKey);
				
				if ( nRet == ERROR_SUCCESS )
				{
					CString	strSubFull;
					strSubFull.Format( _T("%s\\%s"), lpKeyName, strSubKey);	
					this->DeleteKey(hRoot, strSubFull);
				}
				else if (nRet == ERROR_NO_MORE_ITEMS )
				{
					break;
				}
			} while (TRUE);
			key.Close();
			nRet = ::SHDeleteKey(hRoot, lpKeyName);
		}
		return nRet;
	}

};