///////////////////////////////////////////////////////////////
//
//	Filename: 	KGetClassObject.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2008-2-20  18:24
//	Comment:	
//
///////////////////////////////////////////////////////////////

#ifndef _KGetClassObject_h_
#define _KGetClassObject_h_
#include "dllcomobject.h"


/*
[创建组件宏:]
KAS_BEGIN_OBJECT_MAP()
	KAS_OBJECT_ENTRY( IXXX, CXXX )
	KAS_OBJECT_ENTRY( IYYY, CYYY )
KAS_END_OBJECT_MAP()


[宏展开]
HRESULT KDllModule_DllGetClassObject( REFIID riid, LPVOID* ppv )
{
	if( ppv == NULL )
		return E_INVALIDARG;


	if( riid == __uuidof( IXXX ) )
	{
		IXXX* pI = new KComObject<CXXX>;
		if( pI == NULL )
			return E_OUTOFMEMORY;

		pI->AddRef();
		*ppv = pI;
		return S_OK;
	}

	if( riid == __uuidof( IYYY ) )
	{
		IYYY* pI = new KComObject<CYYY>;
		if( pI == NULL )
			return E_OUTOFMEMORY;

		pI->AddRef();
		*ppv = pI;
		return S_OK;
	}


	*ppv = NULL;
	return E_NOINTERFACE;
}
*/




#define SCOM_BEGIN_OBJECT_MAP()   HRESULT KDllModule_DllGetClassObject( REFIID riid, LPVOID* ppv ){ \
	if( ppv == NULL ) return E_INVALIDARG;


#define SCOM_OBJECT_ENTRY( IXXX, CXXX )  if( riid == __uuidof( IXXX ) ){ \
	IXXX* pI = new KDllComObject<CXXX>; \
	if( pI == NULL ) return E_OUTOFMEMORY; \
	pI->AddRef(); \
	*ppv = pI; \
	return S_OK; }


#define SCOM_OBJECT_ENTRY_IID( IXXX, CXXX )  if( riid == IID_##IXXX ) ){ \
	IXXX* pI = new KDllComObject<CXXX>; \
	if( pI == NULL ) return E_OUTOFMEMORY; \
	pI->AddRef(); \
	*ppv = pI; \
	return S_OK; }


#define SCOM_END_OBJECT_MAP()  	*ppv = NULL; \
	return E_NOINTERFACE; }



#endif // _KGetClassObject_h_