/**
* @file    winmodinethttpconnection.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:13
*/

#ifndef WINMODINETHTTPCONNECTION_H
#define WINMODINETHTTPCONNECTION_H

#include <atlstr.h>
#include "winmodinetconnection.h"
#include "winmodinethttpfile.h"
#include "winhttpdownloaderbase.h"

NS_WINMOD_BEGIN

class CInetHttpConnection: public CInetConnection
{
public:
    CInetHttpConnection();
    virtual ~CInetHttpConnection();

    virtual void Close() {Interrupt(); CInetConnection::Close();}

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
        DWORD       dwFlags             = INTERNET_FLAG_EXISTING_CONNECT,
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
        /* [in ] */ IInetHttpDownloadFile*      piDownloadFile,
        /* [in ] */ IInetHttpDownloadProgress*  piCallback,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL);

    void    Interrupt();
	
    void SetUseGZip(BOOL bUseGZip);

private:
    // denied
    CInetHttpConnection(CInetHttpConnection& h);
    explicit CInetHttpConnection(HANDLE h);
    CInetHttpConnection& operator=(CInetHttpConnection& h);

    BOOL m_bUseGZip;

    // used by HttpRequest and HttpDownload
    CInetHttpFile m_hHttpFile;
};




inline CInetHttpConnection::CInetHttpConnection()
    : m_bUseGZip(FALSE)
{
}

inline CInetHttpConnection::~CInetHttpConnection()
{
}



inline HINTERNET CInetHttpConnection::OpenRequest(
    LPCTSTR     pstrVerb,
    LPCTSTR     pstrObjectName,
    LPCTSTR     pstrVersion,
    LPCTSTR     pstrReferer,
    LPCTSTR*    ppstrAcceptTypes,
    DWORD       dwFlags,
    DWORD_PTR   dwContext)
{
    assert(m_h);
    HINTERNET hFile = ::HttpOpenRequest(
        m_h,
        pstrVerb,
        pstrObjectName,
        pstrVersion,
        pstrReferer,
        ppstrAcceptTypes,
        dwFlags,
        dwContext);
    if (!hFile)
        return NULL;

    return hFile;
}


inline void CInetHttpConnection::Interrupt()
{
    m_hHttpFile.Close();
}


NS_WINMOD_END

#endif//WINMODINETHTTPCONNECTION_H