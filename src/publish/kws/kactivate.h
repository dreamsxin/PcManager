#ifndef _K_ACTIVATE_H_
#define _K_ACTIVATE_H_
#include <windows.h>
#include <atlstr.h>

class KActivate
{
public:
	BOOL	ActivateKWS(LPTSTR lpParam = NULL, BOOL bShow = TRUE, int nBy = 0);
	static	void SetAppVersion(CString& strVersion);

protected:
private:
	static CString m_strAppVersion;
};

#endif	// _K_ACTIVATE_H_
