/**
* @file    winmodinetfile.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:10
*/

#ifndef WINMODINETFILE_H
#define WINMODINETFILE_H

#include "winmodinethandle.h"

NS_WINMOD_BEGIN

class CInetFile: public CInetHandle
{
public:
    CInetFile();
    virtual ~CInetFile();

    HRESULT Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

    HRESULT Write(LPCVOID pBuffer, DWORD nBufSize, DWORD& nBytesWritten);

    HRESULT Seek(LONGLONG nOffset, DWORD dwFrom = FILE_CURRENT);

private:
    // denied
    CInetFile(CInetFile& h);
    CInetFile(HINTERNET h);
    CInetFile& operator=(CInetFile& h);
};




inline CInetFile::CInetFile()
{
}

inline CInetFile::~CInetFile()
{
}



inline HRESULT CInetFile::Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
{
    assert(m_h);
    BOOL br = ::InternetReadFile(m_h, pBuffer, nBufSize, &nBytesRead);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetFile::Write(LPCVOID pBuffer, DWORD nBufSize, DWORD& nBytesWritten)
{
    assert(m_h);
    BOOL br = ::InternetWriteFile(m_h, pBuffer, nBufSize, &nBytesWritten);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

inline HRESULT CInetFile::Seek(LONGLONG nOffset, DWORD dwFrom)
{
    assert(m_h);
    BOOL br = ::InternetSetFilePointer(m_h, (LONG)nOffset, NULL, dwFrom, 0);
    if (!br)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

    return S_OK;
}

NS_WINMOD_END

#endif//WINMODINETFILE_H