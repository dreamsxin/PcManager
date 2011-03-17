#ifndef	_I_CONTROLLER_H_ 
#define	_I_CONTROLLER_H_ 
#include <Unknwn.h>


#if defined(_MSC_VER) && (_MSC_VER >= 1300) 
[
	uuid("DA735010-8DDF-4ef7-8C18-72321F4014C7")
]
#else //_MSC_VER
	interface __declspec(uuid("DA735010-8DDF-4ef7-8C18-72321F4014C7")) IController;
#endif//_MSC_VER

interface IController : IUnknown
{
	virtual BOOL STDMETHODCALLTYPE IsKwsEnable(BOOL bRefresh = TRUE) = 0;
	virtual BOOL STDMETHODCALLTYPE SetKwsEnable(BOOL bEnable) = 0;
	virtual BOOL STDMETHODCALLTYPE IsSafeDonwEnable(BOOL bRefresh = TRUE) = 0;
	virtual BOOL STDMETHODCALLTYPE SetSafeDonwEnable(BOOL bEnable) = 0;
};


#endif	// #ifndef _I_CONTROLLER_H_ 
