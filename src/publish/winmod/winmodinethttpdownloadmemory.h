/**
* @file    winmodinethttpdownloadmemory.h
* @brief   ...
* @author  gdier
* @date    2010-06-17 15:32
*/

#ifndef WINMODINETHTTPDOWNLOADMOMORY_H
#define WINMODINETHTTPDOWNLOADMOMORY_H

#include <atlcoll.h>
#include "winmodinetconnection.h"

NS_WINMOD_BEGIN

class CInetHttpDownloadMemory: public IInetHttpDownloadFile
{
public:
    CInetHttpDownloadMemory()
        : m_pbyBuffer(NULL)
        , m_lOffset(-1)
        , m_lBufferSize(0)
    {

    }

    ~CInetHttpDownloadMemory()
    {
        if (m_pbyBuffer)
            delete[] m_pbyBuffer;
    }

    virtual HRESULT STDMETHODCALLTYPE SetSize(DWORD dwSize = UNKNOWN_SIZE)
    {
        if (UNKNOWN_SIZE == dwSize)
            return E_INVALIDARG;

        if (m_pbyBuffer)
            delete[] m_pbyBuffer;

        m_pbyBuffer = new BYTE[dwSize];
        if (NULL == m_pbyBuffer)
        {
            m_lOffset = -1;
            m_lBufferSize = 0;

            return E_OUTOFMEMORY;
        }

        m_lOffset = 0;
        m_lBufferSize = (LONG)dwSize;

        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Close()
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Seek(LONG lOffset, int nOrigin)
    {
        switch (nOrigin)
        {
        case FILE_BEGIN:
            if (lOffset >= m_lBufferSize)
                return E_INVALIDARG;

            m_lOffset = lOffset;

        	break;

        case FILE_CURRENT:
            if ((lOffset + m_lOffset >= m_lBufferSize) || (lOffset + m_lOffset < 0))
                return E_INVALIDARG;

            m_lOffset += lOffset;

            break;

        default:
            return E_INVALIDARG;
        }
        
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Write(LPCVOID pvData, DWORD dwSize)
    {
        if (NULL == m_pbyBuffer)
            return E_FAIL;

        LONG lOffset = (LONG)dwSize;

        if (lOffset < 0 || lOffset + m_lOffset > m_lBufferSize)
            return E_INVALIDARG;

        memcpy(m_pbyBuffer + m_lOffset, pvData, dwSize);

        m_lOffset += lOffset;

        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE SetEndOfFile()
    {
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Flush()
    {
        return S_OK;
    }

public:
    LPBYTE  m_pbyBuffer;
    LONG    m_lOffset;
    LONG    m_lBufferSize;
};

NS_WINMOD_END

#endif//WINMODINETHTTPDOWNLOADMOMORY_H