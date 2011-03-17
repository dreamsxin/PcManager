/////////////////////////////////////////////////////////////////
//
//	Filename: 	winhttpdownloaderbase.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2010-11-06  11:13
//	Comment:	
//
///////////////////////////////////////////////////////////////

#ifndef WINHTTPDOWNLOADERBASE_H
#define WINHTTPDOWNLOADERBASE_H


#include "winmodbase.h"

NS_WINMOD_BEGIN

// 下载回调接口
class IInetHttpDownloadFile
{
public:

    enum {
        UNKNOWN_SIZE = 0, 
    };

    virtual HRESULT STDMETHODCALLTYPE SetSize(DWORD dwSize = UNKNOWN_SIZE) = 0;
    virtual HRESULT STDMETHODCALLTYPE Close() = 0;
    virtual HRESULT STDMETHODCALLTYPE Seek(LONG lOffset, int nOrigin) = 0;
    virtual HRESULT STDMETHODCALLTYPE Write(LPCVOID pvData, DWORD dwSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetEndOfFile() = 0;
    virtual HRESULT STDMETHODCALLTYPE Flush() = 0;
};

class IInetHttpDownloadProgress
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnReceiveData(
        DWORD   dwTotalSize,
        DWORD   dwCurrentOffset,
        DWORD   dwCurrentSize,
        LPCVOID pvData) = 0;
};

NS_WINMOD_END

#endif //WINHTTPDOWNLOADERBASE_H