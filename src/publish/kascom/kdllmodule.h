///////////////////////////////////////////////////////////////
//
//	Filename: 	KDllModule.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2007-8-25   12:44
//	Comment:	
//
///////////////////////////////////////////////////////////////

#ifndef _KDllModule_h_
#define _KDllModule_h_
#include <assert.h>
#include "KDllLock.h"
#include "KComObject.h"

//.-----------------------------------------------------------------------------
// KDllModule Decleration
//.-----------------------------------------------------------------------------

class KDllModule
{
public:
	KDllModule(void);
	~KDllModule(void);

	void Init( HINSTANCE hInstance );
	HINSTANCE GetInstance() const;

	HRESULT DllCanUnloadNow(void);
	HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

private:
	HINSTANCE m_hInstance;
};




//.-----------------------------------------------------------------------------
// KDllModule Implementation
//.-----------------------------------------------------------------------------

inline
KDllModule::KDllModule(void)
{
	m_hInstance = NULL;
}


inline
KDllModule::~KDllModule(void)
{
}


inline
void KDllModule::Init( HINSTANCE hInstance )
{
	m_hInstance = hInstance;
}


inline
HINSTANCE KDllModule::GetInstance() const
{
	return m_hInstance;
}


inline
HRESULT KDllModule::DllCanUnloadNow(void)
{
	return KDllLock::GetInstance()->GetLockCount() == 0 ? S_OK : E_FAIL;
}



HRESULT KDllModule_DllGetClassObject( REFIID riid, LPVOID* ppv );

inline
HRESULT KDllModule::DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID* ppv )
{
	if( rclsid == CLSID_NULL )
	{
		return KDllModule_DllGetClassObject( riid, ppv );
	}

	return E_FAIL;
}


#endif // _KDllModule_h_