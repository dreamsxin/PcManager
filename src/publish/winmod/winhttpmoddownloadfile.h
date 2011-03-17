/**
* @file    winhttpmoddownloadfile.h
* @brief   ...
* @author  bbcallen
* @date    2010-05-04 16:15
*/

#ifndef WINHTTPMODDOWNLOADFILE_H
#define WINHTTPMODDOWNLOADFILE_H

#include <atlfile.h>
#include "winhttpmodconnection.h"

NS_WINMOD_BEGIN

class CWinHttpDownloadFile: public IWinHttpDownloadFile
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetSize(DWORD dwSize = UNKNOWN_SIZE)
    {
        return m_hFile.SetSize((ULONGLONG)dwSize);
    }

    virtual HRESULT STDMETHODCALLTYPE Close()
    {
        m_hFile.Close();
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Seek(LONG lOffset, int nOrigin)
    {
        return m_hFile.Seek(lOffset, nOrigin);
    }

    virtual HRESULT STDMETHODCALLTYPE Write(LPCVOID pvData, DWORD dwSize)
    {
        return m_hFile.Write(pvData, dwSize);
    }

    virtual HRESULT STDMETHODCALLTYPE SetEndOfFile()
    {
        BOOL br = ::SetEndOfFile(m_hFile);
        if (!br)
            return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Flush()
    {
        return m_hFile.Flush();
    }

public:
    CAtlFile m_hFile;
};

NS_WINMOD_END

#endif//WINHTTPMODDOWNLOADFILE_H