#pragma once
#include <atlbase.h>
#include "netmon_comindex.h"

#define NETMON_MAIN         L"netmon.exe"

#define REG_ROOT            L"SOFTWARE\\kingsoft"
#define PRI_KEY             L"netmonpri"
#define PRI_VALUE           L"pri"
#define PRI_MAIN            L"main"

class CNetmonPri
{
public:
    CNetmonPri():m_nPri(NETMON_INVALID), m_strNetmonPath(L""), m_nHighPri(NETMON_INVALID), m_strHighPriPath(L""){};
public:
    void    Initialize( int nPri, LPCWSTR  lpwszNetmonPath )
    {
        m_nPri = nPri;
        if ( lpwszNetmonPath )
        {
            m_strNetmonPath = lpwszNetmonPath;
        }

        InitPriInfo();
    }

    BOOL    HashHighPriVersion()
    {
        if ( ( m_nHighPri != NETMON_INVALID ) && ( m_nHighPri < m_nPri ) )
        {
            return TRUE;
        }

        return FALSE;
    }

    CString GetHighPriVersion()
    {
        if ( ( m_nHighPri != NETMON_INVALID ) && ( m_nHighPri < m_nPri ) )
        {
            return  m_strHighPriPath;
        }
        else
        {
            return L"";
        }
    }

    HRESULT SetPriInfomation()
    {
        LONG    lRet = ERROR_SUCCESS;
        CRegKey reg;

        lRet = reg.Create( HKEY_LOCAL_MACHINE, REG_ROOT );
        if ( 0 == lRet )
        {
            lRet = reg.Create( reg.m_hKey, PRI_KEY );
            if ( 0 == lRet )
            {
                CString path;

                path = m_strNetmonPath;
                path += NETMON_MAIN;

                lRet = reg.SetDWORDValue( PRI_VALUE, ( DWORD )m_nPri );
                lRet = reg.SetStringValue( PRI_MAIN, m_strNetmonPath );
            }

            reg.Close();
        }

        return HRESULT_FROM_WIN32( lRet );
    }
private:
    void    InitPriInfo()
    {
        LONG    lRet = ERROR_SUCCESS;
        CRegKey reg;

        lRet = reg.Open( HKEY_LOCAL_MACHINE, REG_ROOT );
        if ( 0 == lRet )
        {
            lRet = reg.Open( reg.m_hKey, PRI_KEY );
            if ( 0 == lRet )
            {
                int   nPri = NETMON_INVALID;

                lRet = reg.QueryDWORDValue( PRI_VALUE, *(DWORD *)&nPri );
                if ( 0 == lRet )
                {
                    if ( nPri < m_nPri )
                    {
                        // 有高优先级的存在
                        DWORD   dwReadSize = MAX_PATH;
                        TCHAR   wszHighPriPath[ MAX_PATH + 1 ] = { 0 };

                        m_nHighPri = ( int )nPri;

                        lRet = reg.QueryStringValue( PRI_MAIN, wszHighPriPath, &dwReadSize );
                        if ( 0 == lRet )
                        {
                            m_strHighPriPath = wszHighPriPath;
                            m_strHighPriPath += NETMON_MAIN;
                        }
                    }
                }
            }
            reg.Close();
        }
    }
protected:
private:
    CString m_strNetmonPath;
    CString m_strHighPriPath;
    int   m_nPri;
    int   m_nHighPri;
};