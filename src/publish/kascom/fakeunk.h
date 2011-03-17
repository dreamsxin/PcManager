///////////////////////////////////////////////////////////////
//
//	Filename: 	fakeunk.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2009-6-26  15:18
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _fakeunk_h_
#define _fakeunk_h_

template <class T>
class KFakeUnknown : public T 
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		return E_NOTIMPL;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef( void)
	{
		return 1;
	}

	virtual ULONG STDMETHODCALLTYPE Release( void)
	{
		return 1;
	}
};

#endif // _fakeunk_h_