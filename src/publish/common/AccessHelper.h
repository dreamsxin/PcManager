
#pragma once

#include <atlsecurity.h>
#include "procprivilege.h"



class CAccessHepler
{
public:
	CAccessHepler()
	{

	}

	BOOL AddFileAccess(LPCTSTR lpFileName, DWORD nFlags, BOOL bInherite)
	{
		BOOL bRet = FALSE;
		CDacl dacl;
		BYTE aceFlags = bInherite?(CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE) : 0;
		BOOL bSetOwner = FALSE;

		CProcPrivilege().SetPri(TRUE, SE_TAKE_OWNERSHIP_NAME);

_RETRY:
		bRet = AtlGetDacl(lpFileName, SE_FILE_OBJECT, &dacl);
		if (!bRet)
			goto Exit0;

		dacl.RemoveAllAces();
		bRet = dacl.AddAllowedAce(Sids::World(), nFlags, aceFlags );
		if (!bRet)
			goto Exit0;

		bRet = AtlSetDacl(lpFileName, SE_FILE_OBJECT, dacl);
		if (!bRet)
			goto Exit0;
Exit0:
		if (!bRet && !bSetOwner)
		{
			bSetOwner = TRUE;
			AtlSetOwnerSid(lpFileName,SE_FILE_OBJECT, Sids::Admins() );
			goto _RETRY;
		}

		return bRet;
	}

	BOOL AddRegAccess( HKEY hRoot, LPCTSTR lpKeyName, DWORD nFlags, BOOL bInherite=TRUE)
	{
		CString	strNameX = ConvertKeyString(hRoot, lpKeyName);
		BOOL	bRet = FALSE;
		CDacl	dacl;
		BYTE	aceFlags = bInherite?(CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE) : 0;
		BOOL	bSetOwner = FALSE;

		CProcPrivilege().SetPri(TRUE, SE_TAKE_OWNERSHIP_NAME);

		if ( strNameX.IsEmpty() )
			goto Exit0;
_RETRY:
		bRet = AtlGetDacl(strNameX, SE_REGISTRY_KEY, &dacl);
		if (!bRet)
			goto Exit0;

		dacl.RemoveAllAces();
		bRet = dacl.AddAllowedAce(Sids::World(), nFlags, aceFlags);
		if (!bRet)
			goto Exit0;

		bRet = AtlSetDacl(strNameX, SE_REGISTRY_KEY, dacl);

Exit0:
		if (!bRet && !bSetOwner)
		{
			bSetOwner = TRUE;
			AtlSetOwnerSid(strNameX,SE_REGISTRY_KEY, Sids::Admins() );
			goto _RETRY;
		}

		return bRet;
	}

protected:
	CString ConvertKeyString(HKEY hRootkey, LPCTSTR lpKeyName)
	{
		CString	strReturn;
		LPCTSTR lpRootkey = NULL;

		switch(PtrToLong(hRootkey))
		{
		case HKEY_CLASSES_ROOT:
			lpRootkey = _T("CLASSES_ROOT");
			break;

		case HKEY_CURRENT_USER:
			lpRootkey = _T("CURRENT_USER");
			break;

		case HKEY_LOCAL_MACHINE:
			lpRootkey = _T("MACHINE");
			break;

		case HKEY_USERS:
			lpRootkey = _T("USERS");
			break;

		default:
			break;
		}

		if ( lpRootkey == NULL )
			return strReturn;

		strReturn.Format( _T("%s\\%s"), lpRootkey, lpKeyName );
		return strReturn;
	}

};