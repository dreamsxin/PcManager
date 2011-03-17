/**
* @file    winmodinethttpdownloadfile.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 17:32
*/

#ifndef WINMODINETHTTPDOWNLOADFILE_H
#define WINMODINETHTTPDOWNLOADFILE_H

#include <atlfile.h>
#include "winmodinetconnection.h"

NS_WINMOD_BEGIN

class CInetHttpDownloadFile: public IInetHttpDownloadFile
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

#endif//WINMODINETHTTPDOWNLOADFILE_H