#ifndef _URL_WHITE_LIST_H
#define _URL_WHITE_LIST_H

class IURLWhiteList
{
public:
    virtual HRESULT STDMETHODCALLTYPE Add(LPCWSTR lpwzUrl) = 0;
    virtual HRESULT STDMETHODCALLTYPE Delete(LPCWSTR lpwzUrl) = 0;
    virtual HRESULT STDMETHODCALLTYPE Search(LPCWSTR lpwzUrl) = 0;
    virtual HRESULT STDMETHODCALLTYPE Flush() = 0;

    // return number of URLs in white list.
    virtual HRESULT STDMETHODCALLTYPE Length(long *p) = 0;

    // call LocalFree to release memory returned in lppUrl.
    virtual HRESULT STDMETHODCALLTYPE First(LPCWSTR *lppUrl) = 0;

    // call LocalFree to release memory returned in lppUrl.
    virtual HRESULT STDMETHODCALLTYPE Next(LPCWSTR *lppUrl) = 0;

    virtual void    STDMETHODCALLTYPE RollbackChanges() = 0;
    virtual void    STDMETHODCALLTYPE ApplyChanges() = 0;
    
    // delete all URLs in white list.
    virtual void    STDMETHODCALLTYPE Clear() = 0;
};

#endif