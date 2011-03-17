/**
* @file    winmodinethttpfile.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:12
*/

#ifndef WINMODINETHTTPFILE_H
#define WINMODINETHTTPFILE_H

#include "winmodinetfile.h"

NS_WINMOD_BEGIN

class CInetHttpFile: public CInetFile
{
public:
    CInetHttpFile();
    virtual ~CInetHttpFile();


    HRESULT AddRequestHeaders(
        LPCWSTR pstrHeaders,
        DWORD   dwHeadersLen    = -1,
        DWORD   dwFlags         = HTTP_ADDREQ_FLAG_ADD);

    HRESULT SendRequest(
        LPCTSTR pstrHeaders     = NULL,
        DWORD   dwHeadersLen    = -1,
        LPVOID  lpOptional      = NULL,
        DWORD   dwOptionalLen   = 0);

    HRESULT SendRequestEx(
        LPINTERNET_BUFFERS  lpBuffersIn = NULL,
        DWORD_PTR           dwContext   = 1);


    HRESULT EndRequest(
        LPINTERNET_BUFFERS  lpBuffIn    = NULL,
        DWORD_PTR           dwContext   = 1);


    HRESULT QueryInfoStatusCode(DWORD& dwStatusCode) const;

    HRESULT QueryInfoContentLength(DWORD& dwContentLength) const;


private:
    // denied
    CInetHttpFile(CInetHttpFile& h);
    explicit CInetHttpFile(HANDLE h);
    CInetHttpFile& operator=(CInetHttpFile& h);
};



inline CInetHttpFile::CInetHttpFile()
{
}


inline CInetHttpFile::~CInetHttpFile()
{
}

inline HRESULT CInetHttpFile::AddRequestHeaders(
    LPCWSTR pstrHeaders, 
    DWORD   dwHeadersLen,
    DWORD   dwFlags)
{
    assert(m_h);
    BOOL br = ::HttpAddRequestHeaders(m_h, pstrHeaders, dwHeadersLen, dwFlags);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetHttpFile::SendRequest(
    LPCTSTR pstrHeaders,
    DWORD   dwHeadersLen,
    LPVOID  lpOptional,
    DWORD   dwOptionalLen)
{
    assert(m_h);
    BOOL br = ::HttpSendRequest(m_h, pstrHeaders, dwHeadersLen, lpOptional, dwOptionalLen);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetHttpFile::SendRequestEx(
    LPINTERNET_BUFFERS  lpBuffersIn,
    DWORD_PTR           dwContext)
{
    assert(m_h);
    BOOL br = ::HttpSendRequestEx(m_h, lpBuffersIn, NULL, 0, dwContext);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetHttpFile::EndRequest(
    LPINTERNET_BUFFERS  lpBuffIn,
    DWORD_PTR           dwContext)
{
    assert(m_h);
    BOOL br = ::HttpEndRequest(m_h, lpBuffIn, 0, dwContext);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetHttpFile::QueryInfoStatusCode(DWORD& dwStatusCode) const
{
    assert(m_h);
    DWORD dwLen = sizeof(DWORD);

    BOOL br = ::HttpQueryInfo(
        m_h,
        HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER ,
        &dwStatusCode,
        &dwLen,
        NULL);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetHttpFile::QueryInfoContentLength(DWORD& dwContentLength) const
{
    assert(m_h);
    DWORD dwLen = sizeof(DWORD);

    BOOL br = ::HttpQueryInfo(m_h, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER , &dwContentLength, &dwLen, NULL);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK; 
}

NS_WINMOD_END

#endif//WINMODINETHTTPFILE_H