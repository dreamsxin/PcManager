#pragma once

class __declspec(uuid("00140123-5CCE-4279-BD9B-11D66677BB76"))
IUpdateHelper : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Combine( LPCWSTR lpwszDifflib ) = 0;
    virtual HRESULT STDMETHODCALLTYPE Notify( LPCWSTR lpwszFileName ) = 0;
};

class __declspec(uuid("4BA95D1B-8A77-4920-824F-743B323CC095"))
IUpdateHelper2
{
public:
	virtual HRESULT STDMETHODCALLTYPE Combine2( LPCWSTR szBaselib, LPCWSTR lpwszDifflib ) = 0;
};


#define FN_CreateUpdateHelper			"CreateUpdateHelper"
typedef HRESULT (WINAPI *PFN_CreateUpdateHelper)( REFIID riid, void** ppvObj );
