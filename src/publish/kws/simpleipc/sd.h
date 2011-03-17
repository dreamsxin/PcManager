///////////////////////////////////////////////////////////////
//
//	Filename: 	sd.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-4-6  19:57
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _sd_h_
#define _sd_h_
#include <AccCtrl.h>
#include <Sddl.h>
#include <Aclapi.h>

#define LOW_INTEGRITY_SDDL_SACL     TEXT("S:(ML;;NW;;;LW)")

#ifndef LABEL_SECURITY_INFORMATION
#define LABEL_SECURITY_INFORMATION   (0x00000010L)
#endif


inline bool SetObjectToLowIntegrity( HANDLE hObject, SE_OBJECT_TYPE type = SE_KERNEL_OBJECT )
{
	bool  bRet           = false;
	DWORD dwErr          = ERROR_SUCCESS;
	PACL  pSacl          = NULL;
	BOOL  fSaclPresent   = FALSE;
	BOOL  fSaclDefaulted = FALSE;
	PSECURITY_DESCRIPTOR pSD = NULL;

	if ( ConvertStringSecurityDescriptorToSecurityDescriptor(
		LOW_INTEGRITY_SDDL_SACL, SDDL_REVISION_1, &pSD, NULL ) )
	{
		if ( GetSecurityDescriptorSacl (
			pSD, &fSaclPresent, &pSacl, &fSaclDefaulted ) )
		{
			dwErr = SetSecurityInfo( 
				hObject, type, LABEL_SECURITY_INFORMATION,
				NULL, NULL, NULL, pSacl );

			bRet = (ERROR_SUCCESS == dwErr);
		}

		LocalFree ( pSD );
	}

	return bRet;
}

#endif // _sd_h_