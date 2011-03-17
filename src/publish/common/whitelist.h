#pragma once
#include "winmod/winpath.h"
#include "skylark2/bkdb_mod.h"
#include "skylark2/bklevel.h"
#include "utility.h"
#include "atltime.h"

typedef enum
{
    enumItemTypeInvalid = -1,
    enumItemTypeManual,
    enumItemTypeFalse,
    enumItemTypeCount,
};

#define WHITELIST_DB_NAME           L"wl.dat"
#define WHITELIST_DB_TABLENAME      L"wl"
#define WHITELIST_DB_CREATE_TABLE   L"create table if not exists wl(path varchar primary key not null,timestamp integer not null, type integer not null) ;"
#define WHITELIST_DB_INSERT         L"insert or replace into wl ( path, timestamp, type ) values(?,?,?)"
#define WHITELIST_DB_GET            L"select * from wl order by timestamp desc"
#define WHITELIST_DB_DEL            L"delete from wl where path=?"
#define WHITELIST_DB_CLEAR          L"delete from wl"
#define WHITELIST_DB_COUNT          L"select count(*) from wl"

class CWhiteListItem
{
public:
    CWhiteListItem()
    {
        m_ullCommitTime = 0;
        m_nType = enumItemTypeManual;
    }

    void SetCommitTime( const FILETIME& ft )
    {
        ULARGE_INTEGER  ui;

        ui.HighPart = ft.dwHighDateTime;
        ui.LowPart  = ft.dwLowDateTime;

        m_ullCommitTime = ui.QuadPart;
    }

public:
    CString     m_filepath;
    ULONGLONG   m_ullCommitTime;
    int         m_nType;
};

class   CWhiteList
{
public:
    CWhiteList(void) : 
      m_lInit( 0 ),
      m_piWlDB( NULL )
    {

    }
    ~CWhiteList(void)
    {
      Uninitialize();
    }

public:
    HRESULT  Initialize()
    {
        HRESULT hr = S_OK;

        if ( InterlockedCompareExchange( &m_lInit, 1, 0 ) )
        {
            hr = S_FALSE;
            goto Exit0;
        }

        hr = CAppPath::Instance().GetLeidianAppPath( m_modpath.m_strPath );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        hr = CAppPath::Instance().GetLeidianLogPath( m_dbpath.m_strPath );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        m_modpath.Append( BKMOD_NAME_BKDB );
        m_dbpath.Append( WHITELIST_DB_NAME );

        hr = m_dbmod.LoadLib( m_modpath );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        hr = m_dbmod.BKDbCreateObject( 
            __uuidof( Skylark::ISQLiteComDatabase3 ), 
            ( void** )&m_piWlDB 
            );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        hr = m_piWlDB->Open( m_dbpath );
        if ( FAILED( hr ) )
        {
            ::SetFileAttributes( m_dbpath, 0 );
            ::DeleteFile( m_dbpath );
            hr = m_piWlDB->Open( m_dbpath );
        }

        if ( FAILED( hr ) )
        {
            goto Exit1;
        }

        if ( !m_piWlDB->TableExists( WHITELIST_DB_TABLENAME ) )
        {
            hr = m_piWlDB->ExecuteUpdate( WHITELIST_DB_CREATE_TABLE );
            if ( FAILED( hr ) )
            {
                m_piWlDB->GetErrorMsg();
                m_piWlDB->Close();
                goto Exit1;
            }
        }

        goto Exit0;
Exit1:
        if ( m_piWlDB )
        {
            m_piWlDB->Release();
            m_piWlDB = NULL;
        }
Exit0:
        if ( FAILED( hr ) )
        {
            InterlockedExchange( &m_lInit, 0 );
        }
        return hr;
    }

    HRESULT  Uninitialize()
    {
        HRESULT hr = S_OK;

        if ( !InterlockedCompareExchange( &m_lInit, 0, 1 ) )
        {
            hr = S_FALSE;
            goto Exit0;
        }

        if ( m_piWlDB )
        {
            m_piWlDB->Close();
            m_piWlDB->Release();
            m_piWlDB = NULL;
        }

        m_dbmod.FreeLib();
Exit0:
        return hr;
    }

    HRESULT AddWhiteItem( CWhiteListItem& wi )
    {
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComStatement3>  piStm;

        if ( NULL == m_piWlDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

		wi.SetCommitTime( ATL::CFileTime::GetCurrentTime() );

        hr = m_piWlDB->PrepareStatement( WHITELIST_DB_INSERT, &piStm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        piStm->Bind( 1, wi.m_filepath );
        piStm->Bind( 2, wi.m_ullCommitTime );
        piStm->Bind( 3, wi.m_nType );

        hr = piStm->ExecuteUpdate();
Exit0:
        return hr;
    }

    HRESULT AddWhiteItem( CAtlArray<CWhiteListItem>& wl )
    {
        HRESULT hr = S_OK;

        if ( NULL == m_piWlDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        m_piWlDB->BeginTransaction();

        for ( size_t i = 0; i < wl.GetCount(); i++ )
        {
            hr = AddWhiteItem( wl[ i] );
            if ( FAILED( hr ) )
            {
                break;
            }
        }

        m_piWlDB->CommitTransaction();
Exit0:
        return hr;
    }

    HRESULT DeleteWhiteItem( const CWhiteListItem& wi )
    {
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComStatement3>  piStm;

        if ( NULL == m_piWlDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        hr = m_piWlDB->PrepareStatement( WHITELIST_DB_DEL, &piStm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        piStm->Bind( 1, wi.m_filepath );

        hr = piStm->ExecuteUpdate();
Exit0:
        return hr;
    }

    HRESULT DeleteWhiteItem( const CAtlArray<CWhiteListItem>& wl )
    {
        HRESULT hr = S_OK;

        if ( NULL == m_piWlDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        m_piWlDB->BeginTransaction();

        for ( size_t i = 0; i < wl.GetCount(); i++ )
        {
            hr = DeleteWhiteItem( wl[ i] );
            if ( FAILED( hr ) )
            {
                break;
            }
        }

        m_piWlDB->CommitTransaction();
Exit0:
        return hr;
    }

    HRESULT LoadWhiteList( CAtlArray<CWhiteListItem>& wl )
    {
        HRESULT hr = S_OK;
        CWhiteListItem  wi;
        CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;
        DWORD   dwCount = 0;

        if ( NULL == m_piWlDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        hr = GetWhiteItemCount( dwCount );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        wl.SetCount( dwCount );

        hr = m_piWlDB->ExecuteQuery( WHITELIST_DB_GET, &piRtm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        for ( DWORD i = 0; i < dwCount; i++ )
        {
            if ( piRtm->IsEof() )
            {
                break;
            }

            wl[ i ].m_filepath = piRtm->GetAsString( 0 );
            wl[ i ].m_ullCommitTime = ( ULONGLONG )piRtm->GetInt64( 1 );
            wl[ i ].m_nType = piRtm->GetInt( 2 );
            piRtm->NextRow();
        }
Exit0:
        return hr;
    }

    HRESULT GetWhiteItemCount( DWORD& dwCount )
    {
        HRESULT hr = S_OK;
        CWhiteListItem  wi;
        CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;

        if ( NULL == m_piWlDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        dwCount = 0;

        hr = m_piWlDB->ExecuteQuery( WHITELIST_DB_COUNT, &piRtm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        if ( !piRtm->IsEof() )
        {
            dwCount = piRtm->GetInt( 0 );
        }
Exit0:
        return hr;
    }
private:
    LONG                m_lInit;
    WinMod::CWinPath    m_modpath;
    WinMod::CWinPath    m_dbpath;
    Skylark::CWinModule_bkdb        m_dbmod;
    Skylark::ISQLiteComDatabase3*   m_piWlDB;
};