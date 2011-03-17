//////////////////////////////////////////////////////////////////////////
//   File Name: bkurl.h
// Description: Url Cracker
//     Creator: Zhang Xiaoxuan
//     Version: 2009.6.2 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <WinInet.h>

LPCTSTR g_wszFilterHost[] = 
{
    L".beike.cn",
    L".ijinshan.com",
    L".duba.net"
};

#define FILETER_HOST_NUMBER ( int )( sizeof( g_wszFilterHost ) / sizeof( LPCTSTR ) )

class CBkUrl
{
public:
    CBkUrl()
        : m_bIsValidUrl(FALSE)
    {

    }
    ~CBkUrl()
    {

    }

    BOOL SetUrl(LPCTSTR lpszUrl)
    {
        URL_COMPONENTS urlcomp = {sizeof(URL_COMPONENTS)};
        BOOL bRet = FALSE;

        urlcomp.lpszHostName = m_strHostName.GetBuffer(MAX_PATH + 1);
        urlcomp.dwHostNameLength = MAX_PATH;
        urlcomp.lpszUrlPath = m_strUrlPath.GetBuffer(MAX_PATH + 1);
        urlcomp.dwUrlPathLength = MAX_PATH;

        bRet = ::InternetCrackUrl(lpszUrl, 0, ICU_ESCAPE, &urlcomp);
        if ( bRet )
        {
            m_strHostName.ReleaseBuffer();
            m_strUrlPath.ReleaseBuffer();
            m_usPort = urlcomp.nPort;

            bRet = FALSE;
            for ( int i = 0; i < FILETER_HOST_NUMBER; i++ )
            {
                if ( m_strHostName.GetLength() > _tcslen( g_wszFilterHost[ i ] ) )
                {
                    CString strRight = m_strHostName.Right( 
                        _tcslen( g_wszFilterHost[ i ] ) 
                        );
                    if ( 0 == strRight.CompareNoCase( g_wszFilterHost[ i ] ) )
                    {
                        bRet = TRUE;
                        break;
                    }
                }
            }
        }

        m_bIsValidUrl = bRet;

        m_strFullUrl = lpszUrl;

        return m_bIsValidUrl;
    }

    LPCTSTR GetHostName(void)
    {
        if (!m_bIsValidUrl)
            return NULL;

        return m_strHostName;
    }

    LPCTSTR GetUrlPath(void)
    {
        if (!m_bIsValidUrl)
            return NULL;

        return m_strUrlPath;
    }

    INTERNET_PORT GetPort(void)
    {
        if (!m_bIsValidUrl)
            return NULL;

        return m_usPort;
    }

    LPCTSTR GetFullUrl(void)
    {
        if (!m_bIsValidUrl)
            return NULL;

        return m_strFullUrl;
    }

private:
    BOOL m_bIsValidUrl;
    CString m_strFullUrl;
    CString m_strHostName;
    CString m_strUrlPath;
    INTERNET_PORT m_usPort;
};
