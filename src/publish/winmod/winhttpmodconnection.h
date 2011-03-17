/**
* @file    winhttpmodconnection.h
* @brief   ...
* @author  bbcallen
* @date    2010-05-04 15:49
*/

#ifndef WINHTTPMODCONNECTION_H
#define WINHTTPMODCONNECTION_H

#include <atlstr.h>
#include "winhttpmodhandle.h"
#include "winhttpmodfile.h"

NS_WINMOD_BEGIN

// 下载回调接口
class IWinHttpDownloadFile
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

class IWinHttpDownloadProgress
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnReceiveData(
        DWORD   dwTotalSize,
        DWORD   dwCurrentOffset,
        DWORD   dwCurrentSize,
        LPCVOID pvData) = 0;
};


class CWinHttpConnection: public CWinHttpHandle
{
public:
    CWinHttpConnection();
    virtual ~CWinHttpConnection();

    virtual void Close() {Interrupt(); CWinHttpHandle::Close();}


    HRESULT SetConnectTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetConnectTimeOut(DWORD& dwMilliSeconds);

    HRESULT SetReceiveTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetReceiveTimeOut(DWORD& dwMilliSeconds);

    HRESULT SetSendTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetSendTimeOut(DWORD& dwMilliSeconds);



    // pstrVerb could be one of:
    // "POST"
    // "GET"
    // "HEAD"
    // "PUT"
    // "LINK"
    // "DELETE"
    // "UNLINK"
    HINTERNET OpenRequest(
        LPCTSTR     pstrVerb,
        LPCTSTR     pstrObjectName,
        LPCTSTR     pstrVersion         = NULL,
        LPCTSTR     pstrReferer         = NULL,
        LPCTSTR*    ppstrAcceptTypes    = NULL,
        DWORD       dwFlags             = 0,
        DWORD_PTR   dwContext           = 1);

    // lpszContentType could be one of:
    // WINMOD_HTTP_CONTENT_TYPE__APPLICATION__X_WWW_FORM_URLENCODED
    // WINMDD_HTTP_CONTENT_TYPE__APPLICATION__X_OCTET_STREAM
    // ...
    HRESULT HttpRequest(
        /* [in ] */ LPCTSTR         lpObject,
        /* [in ] */ DWORD           dwTimeout,
        /* [in ] */ LPCTSTR         lpszContentType,
        /* [in ] */ const CStringA& strCommand,
        /* [out] */ CStringA*       pstrResponse        = NULL,
        /* [out] */ DWORD*          pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR         lpszSpecHostName    = NULL);

    HRESULT HttpDownload(
        /* [in ] */ IWinHttpDownloadFile*       piDownloadFile,
        /* [in ] */ IWinHttpDownloadProgress*   piCallback,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL);

    void    Interrupt();

private:
    // denied
    CWinHttpConnection(CWinHttpConnection& h);
    explicit CWinHttpConnection(HANDLE h);
    CWinHttpConnection& operator=(CWinHttpConnection& h);

private:

    // used by HttpRequest and HttpDownload
    CWinHttpFile m_hHttpFile;
};




inline CWinHttpConnection::CWinHttpConnection()
{
}

inline CWinHttpConnection::~CWinHttpConnection()
{
}


inline HRESULT CWinHttpConnection::SetConnectTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(WINHTTP_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CWinHttpConnection::GetConnectTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(WINHTTP_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CWinHttpConnection::SetReceiveTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(WINHTTP_OPTION_RECEIVE_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CWinHttpConnection::GetReceiveTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(WINHTTP_OPTION_RECEIVE_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CWinHttpConnection::SetSendTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(WINHTTP_OPTION_SEND_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CWinHttpConnection::GetSendTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(WINHTTP_OPTION_SEND_TIMEOUT, dwMilliSeconds);
}

inline HINTERNET CWinHttpConnection::OpenRequest(
    LPCTSTR     pstrVerb,
    LPCTSTR     pstrObjectName,
    LPCTSTR     pstrVersion,
    LPCTSTR     pstrReferer,
    LPCTSTR*    ppstrAcceptTypes,
    DWORD       dwFlags,
    DWORD_PTR   dwContext)
{
    assert(m_h);
    HINTERNET hFile = ::WinHttpOpenRequest(
        m_h,
        pstrVerb,
        pstrObjectName,
        pstrVersion,
        pstrReferer,
        ppstrAcceptTypes,
        dwFlags);
    if (!hFile)
        return NULL;

    return hFile;
}


inline void CWinHttpConnection::Interrupt()
{
    m_hHttpFile.Close();
}


NS_WINMOD_END

#endif//WINHTTPMODCONNECTION_H