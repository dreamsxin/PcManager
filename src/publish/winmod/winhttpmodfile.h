/**
* @file    winhttpmodfile.h
* @brief   ...
* @author  bbcallen
* @date    2010-05-04 16:44
*/

#ifndef WINHTTPMODFILE_H
#define WINHTTPMODFILE_H

#include "winhttpmodhandle.h"

NS_WINMOD_BEGIN

class CWinHttpFile: public CWinHttpHandle
{
public:
    CWinHttpFile();
    virtual ~CWinHttpFile();

    HRESULT Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

    HRESULT Write(LPCVOID pBuffer, DWORD nBufSize, DWORD& nBytesWritten);

    //HRESULT Seek(LONGLONG nOffset, DWORD dwFrom = FILE_CURRENT);

    HRESULT AddRequestHeaders(
        LPCWSTR pstrHeaders,
        DWORD   dwHeadersLen    = -1,
        DWORD   dwFlags         = WINHTTP_ADDREQ_FLAG_ADD);

    HRESULT SendRequest(
        LPCTSTR pstrHeaders     = NULL,
        DWORD   dwHeadersLen    = -1,
        LPVOID  lpOptional      = NULL,
        DWORD   dwOptionalLen   = 0);

    HRESULT SendRequestEx(DWORD dwContentLength);


    HRESULT EndRequest();


    HRESULT QueryInfoStatusCode(DWORD& dwStatusCode) const;

    HRESULT QueryInfoContentLength(DWORD& dwContentLength) const;

private:
    // denied
    CWinHttpFile(CWinHttpFile& h);
    CWinHttpFile(HINTERNET h);
    CWinHttpFile& operator=(CWinHttpFile& h);
};




inline CWinHttpFile::CWinHttpFile()
{
}

inline CWinHttpFile::~CWinHttpFile()
{
}



inline HRESULT CWinHttpFile::Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
{
    assert(m_h);
    BOOL br = ::WinHttpReadData(m_h, pBuffer, nBufSize, &nBytesRead);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CWinHttpFile::Write(LPCVOID pBuffer, DWORD nBufSize, DWORD& nBytesWritten)
{
    assert(m_h);
    BOOL br = ::WinHttpWriteData(m_h, pBuffer, nBufSize, &nBytesWritten);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}





inline HRESULT CWinHttpFile::AddRequestHeaders(
    LPCWSTR pstrHeaders, 
    DWORD   dwHeadersLen,
    DWORD   dwFlags)
{
    assert(m_h);
    BOOL br = ::WinHttpAddRequestHeaders(m_h, pstrHeaders, dwHeadersLen, dwFlags);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CWinHttpFile::SendRequest(
    LPCTSTR pstrHeaders,
    DWORD   dwHeadersLen,
    LPVOID  lpOptional,
    DWORD   dwOptionalLen)
{
    assert(m_h);
    BOOL br = ::WinHttpSendRequest(m_h, pstrHeaders, dwHeadersLen, lpOptional, dwOptionalLen, dwOptionalLen, 0);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return EndRequest();
}

inline HRESULT CWinHttpFile::SendRequestEx(DWORD dwContentLength)
{
    assert(m_h);
    BOOL br = ::WinHttpSendRequest(m_h, NULL, 0, NULL, 0, dwContentLength, 0);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CWinHttpFile::EndRequest()
{
    assert(m_h);
    BOOL br = ::WinHttpReceiveResponse(m_h, NULL);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CWinHttpFile::QueryInfoStatusCode(DWORD& dwStatusCode) const
{
    assert(m_h);
    DWORD dwLen = sizeof(DWORD);

    BOOL br = ::WinHttpQueryHeaders(
        m_h,
        WINHTTP_QUERY_STATUS_CODE  | WINHTTP_QUERY_FLAG_NUMBER ,
        WINHTTP_HEADER_NAME_BY_INDEX,
        &dwStatusCode,
        &dwLen,
        NULL);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CWinHttpFile::QueryInfoContentLength(DWORD& dwContentLength) const
{
    assert(m_h);
    DWORD dwLen = sizeof(DWORD);

    BOOL br = ::WinHttpQueryHeaders(
        m_h,
        WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, 
        WINHTTP_HEADER_NAME_BY_INDEX,
        &dwContentLength,
        &dwLen,
        NULL);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK; 
}

NS_WINMOD_END

#endif//WINHTTPMODFILE_H