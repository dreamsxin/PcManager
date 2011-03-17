/**
* @file    winhttpmodsession.h
* @brief   ...
* @author  bbcallen
* @date    2010-05-04 16:12
*/

#ifndef WINHTTPMODSESSION_H
#define WINHTTPMODSESSION_H

#include <assert.h>
#include <atlstr.h>
#include "winhttpmodhandle.h"
#include "winhttpmoddownloadfile.h"
#include "winhttpmodconnection.h"

NS_WINMOD_BEGIN

class CWinHttpSession: public CWinHttpHandle
{
public:
    CWinHttpSession();
    virtual ~CWinHttpSession();

    virtual void Close() {Interrupt(); CWinHttpHandle::Close();}

    HRESULT Open(
        LPCTSTR lpszAgent       = NULL,
        DWORD   dwAccessType    = WINHTTP_ACCESS_TYPE_NO_PROXY, 
        LPCTSTR lpszProxy       = NULL, 
        LPCTSTR lpszProxyBypass = NULL, 
        DWORD   dwFlags         = 0);

    HINTERNET HttpConnect(
        LPCTSTR         lpszServerName,
        INTERNET_PORT   nServerPort     = INTERNET_DEFAULT_HTTP_PORT);

    HRESULT HttpRequest(
        /* [in ] */ LPCTSTR         lpszServerName,
        /* [in ] */ INTERNET_PORT   nServerPort,
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
        /* [in ] */ LPCTSTR                     lpszServerName,
        /* [in ] */ INTERNET_PORT               nServerPort,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL);

    HRESULT HttpDownload(
        /* [in ] */ LPCWSTR                     lpszLocalFilePath,
        /* [in ] */ IWinHttpDownloadProgress*   piCallback,
        /* [in ] */ LPCTSTR                     lpszServerName,
        /* [in ] */ INTERNET_PORT               nServerPort,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL);



    HRESULT SetConnectTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetConnectTimeOut(DWORD& dwMilliSeconds);

    void    Interrupt();

private:
    // denied
    CWinHttpSession(CWinHttpSession& h);
    explicit CWinHttpSession(HANDLE h);
    CWinHttpSession& operator=(CWinHttpSession& h);


private:

    // used by HttpRequest and HttpDownload
    CWinHttpConnection m_hHttpConnection;
};




inline CWinHttpSession::CWinHttpSession()
{
}

inline CWinHttpSession::~CWinHttpSession()
{
}

inline HRESULT CWinHttpSession::Open(
    LPCTSTR lpszAgent,
    DWORD   dwAccessType, 
    LPCTSTR lpszProxy, 
    LPCTSTR lpszProxyBypass, 
    DWORD   dwFlags)
{
    Close();
    HINTERNET hSession = ::WinHttpOpen(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
    if (!hSession)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    Attach(hSession);
    return S_OK;
}

inline HINTERNET CWinHttpSession::HttpConnect(
    LPCTSTR         lpszServerName,
    INTERNET_PORT   nServerPort)
{
    assert(m_h);
    HINTERNET hConn = ::WinHttpConnect(m_h, lpszServerName, nServerPort, 0);
    if (!hConn)
        return NULL;

    return hConn;
}

inline HRESULT CWinHttpSession::HttpRequest(
    /* [in ] */ LPCTSTR         lpszServerName,
    /* [in ] */ INTERNET_PORT   nServerPort,
    /* [in ] */ LPCTSTR         lpObject,
    /* [in ] */ DWORD           dwTimeout,
    /* [in ] */ LPCTSTR         lpszContentType,
    /* [in ] */ const CStringA& strCommand,
    /* [out] */ CStringA*       pstrResponse,
    /* [out] */ DWORD*          pdwStatusCode,
    /* [in ] */ LPCTSTR         lpszSpecHostName)
{
    if (!m_h)
        return E_HANDLE;

    if (!lpszServerName || !lpObject)
        return E_POINTER;


    m_hHttpConnection.Close();
    m_hHttpConnection.Attach(HttpConnect(lpszServerName, nServerPort));
    if (!m_hHttpConnection)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    return m_hHttpConnection.HttpRequest(
        lpObject,
        dwTimeout,
        lpszContentType,
        strCommand,
        pstrResponse,
        pdwStatusCode,
        lpszSpecHostName);
}

inline HRESULT CWinHttpSession::HttpDownload(
    /* [in ] */ IWinHttpDownloadFile*       piDownloadFile,
    /* [in ] */ IWinHttpDownloadProgress*   piCallback,
    /* [in ] */ LPCTSTR                     lpszServerName,
    /* [in ] */ INTERNET_PORT               nServerPort,
    /* [in ] */ LPCTSTR                     lpObject,
    /* [in ] */ DWORD                       dwTimeout,
    /* [out] */ DWORD*                      pdwStatusCode,
    /* [in ] */ LPCTSTR                     lpszSpecHostName)
{
    if (!m_h)
        return E_HANDLE;

    if (!lpszServerName || !lpObject)
        return E_POINTER;


    m_hHttpConnection.Close();
    m_hHttpConnection.Attach(HttpConnect(lpszServerName, nServerPort));
    if (!m_hHttpConnection)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    return m_hHttpConnection.HttpDownload(
        piDownloadFile,
        piCallback,
        lpObject,
        dwTimeout,
        pdwStatusCode,
        lpszSpecHostName);  
}

inline HRESULT CWinHttpSession::HttpDownload(
    /* [in ] */ LPCWSTR                     lpszLocalFilePath,
    /* [in ] */ IWinHttpDownloadProgress*   piCallback,
    /* [in ] */ LPCTSTR                     lpszServerName,
    /* [in ] */ INTERNET_PORT               nServerPort,
    /* [in ] */ LPCTSTR                     lpObject,
    /* [in ] */ DWORD                       dwTimeout,
    /* [out] */ DWORD*                      pdwStatusCode,
    /* [in ] */ LPCTSTR                     lpszSpecHostName)
{
    assert(lpszLocalFilePath);

    CWinHttpDownloadFile hDownloadFile;
    HRESULT hr = hDownloadFile.m_hFile.Create(
        lpszLocalFilePath,
        GENERIC_WRITE,
        FILE_SHARE_DELETE,
        CREATE_ALWAYS);
    if (FAILED(hr))
        return hr;

    return HttpDownload(
        &hDownloadFile,
        piCallback,
        lpszServerName,
        nServerPort,
        lpObject,
        dwTimeout,
        pdwStatusCode,
        lpszSpecHostName);
}

inline HRESULT CWinHttpSession::SetConnectTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(WINHTTP_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CWinHttpSession::GetConnectTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(WINHTTP_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}


inline void CWinHttpSession::Interrupt()
{
    m_hHttpConnection.Close();
}

NS_WINMOD_END

#endif//WINHTTPMODSESSION_H