#pragma once
#include "winmod/winpath.h"
#include "skylark2/bkdb_mod.h"
#include "skylark2/bklevel.h"
#include "utility.h"
#include "comproxy/comdata_scan_def.h"

#define BKDB_DLL_NAME               L"ksafedb.dll"
#define RESCAN_DB_NAME              L"rsr.dat"
#define REPORT_DB_NAME              L"rfl2.dat"
#define RESCAN_RESULT_TABLENAME     L"rsr"
#define REPORT_FILE_TABLE           L"rfl"
#define RESCAN_RESULT_CREATE_TABLE  L"create table rsr ( file_md5 varchar not null, file_path varchar not null, sec_type integer not null, sec_level integer not null, op_status integer not null, create_time integer not null, show_state interger not null default 0, primary key ( file_md5, file_path ) )"
#define RESCAN_RESULT_ADD_RESULT    L"insert or replace into rsr ( file_md5, file_path, sec_type, sec_level, op_status, create_time ) values(?,?,?,?,?,?)"
#define RESCAN_RESULT_GET           L"select * from rsr order by create_time desc"
#define RESCAN_RESULT_GET_BY_TIME   L"select * from rsr where create_time<=%I64u and show_state=%d order by create_time desc"
#define RESCAN_RESULT_GET_BY_SHOW   L"select * from rsr where show_state=0 order by create_time desc"
#define RESCAN_RESULT_CLEAR         L"delete from rsr where sec_level<>100"
#define BACKUP_RESULT_CLEAR         L"delete from rfl where report_state=1 and file_track=8"
#define RESCAN_RESULT_UPDATE_SHOW   L"update rsr set show_state=1 where file_md5=?"
#define RESCAN_RESULT_COUNT         L"select count(*) from rsr"
#define REPORT_FILE_REPORTING       L"select * from rfl where report_state<>1 and ( file_track=4 or file_track=8 )order by commit_time desc"
#define REPORT_BACKUP_REPORTED      L"select * from rfl where report_state=1 and file_track=8"
#define REPORT_BACKUP_REPORTED_CNT  L"select count(*) from rfl where report_state=1 and file_track=8"
#define REPORT_FILE_NEVER_RESCAN    L"select * from rfl where report_state=1 and file_track=4 and (rescan_state>=0 and rescan_state<100) order by commit_time desc"
#define REPORT_FILE_REPORTING_CNT   L"select count(*) from rfl where report_state<>1 and ( file_track=4 or file_track=8 )"
#define REPORT_FILE_NEVER_RESCAN_CNT L"select count(*) from rfl where report_state=1 and file_track=4 and (rescan_state>=0 and rescan_state<100)"
#define REPORT_FILE_HAS_REPORTING   L"select * from rfl where report_state=0 and ( file_track=4 or file_track=8 ) limit 1"
#define REPORT_FILE_FIRST_REPORTING L"select * from rfl where report_state=0 and ( file_track=4 or file_track=8 )"
#define REPORT_FILE_STATE_REPORTING 0x00000001
#define REPORT_FILE_STATE_WAIT_SCAN 0x00000002

class CRescanResult
{
public:
    CRescanResult()
    {
        nSecType  = BkQueryInfoFileTypeNull;
        nSecLevel = SLPTL_LEVEL_MARK___NEVER_BEEN_SCANNED;
        nOpStatus = BkFileStateNull;
        ullCommitTime = 0;
        bRetired = FALSE;
        nTrack    = 4;
    }
public:
    CString     strMd5;
    CString     strFileName;
    int         nSecType;
    int         nSecLevel;
    int         nOpStatus;
    int         nTrack;
    BOOL        bRetired;
    ULONGLONG   ullCommitTime;
};

class   CRescanDB
{
public:
    CRescanDB(void) : 
      m_lInit( 0 ),
      m_piSlRsDB( NULL ),
      m_piSlRfDB( NULL )
      {

      }
      ~CRescanDB(void)
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

        m_modpath.Append( BKDB_DLL_NAME );
        m_dbpath.Append( RESCAN_DB_NAME );

        hr = m_dbmod.LoadLib( m_modpath );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        hr = m_dbmod.BKDbCreateObject( 
            __uuidof( Skylark::ISQLiteComDatabase3 ), 
            ( void** )&m_piSlRsDB 
            );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        hr = m_dbmod.BKDbCreateObject(
            __uuidof( Skylark::ISQLiteComDatabase3 ), 
            ( void** )&m_piSlRfDB 
            );
        if ( FAILED( hr ) )
        {
            goto Exit1;
        }

        hr = m_piSlRsDB->Open( m_dbpath );
        if ( FAILED( hr ) )
        {
            ::SetFileAttributes( m_dbpath, 0 );
            ::DeleteFile( m_dbpath );
            hr = m_piSlRsDB->Open( m_dbpath );
        }

        if ( FAILED( hr ) )
        {
            goto Exit1;
        }

        if ( !m_piSlRsDB->TableExists( RESCAN_RESULT_TABLENAME ) )
        {
            hr = m_piSlRsDB->ExecuteUpdate( RESCAN_RESULT_CREATE_TABLE );
            if ( FAILED( hr ) )
            {
                m_piSlRsDB->GetErrorMsg();
                m_piSlRsDB->Close();
                goto Exit1;
            }
        }

        m_dbpath.RemoveFileSpec();
        m_dbpath.Append( REPORT_DB_NAME );

        goto Exit0;
Exit1:
        if ( m_piSlRfDB )
        {
            m_piSlRfDB->Release();
            m_piSlRfDB = NULL;
        }

        if ( m_piSlRsDB )
        {
            m_piSlRsDB->Release();
            m_piSlRsDB = NULL;
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

        if ( m_piSlRfDB )
        {
            m_piSlRfDB->Close();
            m_piSlRfDB->Release();
            m_piSlRfDB = NULL;
        }

        if ( m_piSlRsDB )
        {
            m_piSlRsDB->Close();
            m_piSlRsDB->Release();
            m_piSlRsDB = NULL;
        }

        m_dbmod.FreeLib();
Exit0:
        return hr;
    }

    HRESULT AddRescanResult( const BK_FILE_RESULT& fr, ULONGLONG ullCommitTime )
    {
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComStatement3>  piStm;

        if ( NULL == m_piSlRsDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        hr = m_piSlRsDB->PrepareStatement( RESCAN_RESULT_ADD_RESULT, &piStm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        piStm->Bind( 1, fr.MD5 );
        piStm->Bind( 2, fr.FileName );
        piStm->Bind( 3, ( int )fr.Type );
        piStm->Bind( 4, ( int )fr.SecLevel);
        piStm->Bind( 5, ( int )fr.Status );
        piStm->Bind( 6, ullCommitTime );

        hr = piStm->ExecuteUpdate();
Exit0:
        return hr;
    }

    HRESULT GetReportedBackupFile( CAtlArray<CRescanResult>& rescanResArray )
    {
        HRESULT hr = S_OK;
        CRescanResult   rfile;

        if ( NULL == m_piSlRfDB )
        {
            return E_NOINTERFACE;
        }

        hr = m_piSlRfDB->Open( m_dbpath );
        if ( SUCCEEDED( hr ) )
        {
            CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

            hr = m_piSlRfDB->ExecuteQuery( REPORT_BACKUP_REPORTED, &piRfRtm );
            if ( SUCCEEDED( hr ) )
            {
                while( !piRfRtm->IsEof() )
                {
                    rfile.strMd5 = L"";
                    rfile.strFileName = piRfRtm->GetAsString( 0 );
                    rfile.nOpStatus = BkFileStateNull;
                    rfile.nSecType = BkQueryInfoFileTypeVirus;
                    rfile.nSecLevel = SLPTL_LEVEL_MARK___LOCAL_AVENGINE;
                    rfile.nTrack = piRfRtm->GetInt( 3 );
                    rfile.ullCommitTime = piRfRtm->GetInt64( 5 );
                    rfile.bRetired = 0;

                    rescanResArray.Add( rfile );
                    piRfRtm->NextRow();
                }
            }

            m_piSlRfDB->Close();
        }

        return hr;
    }

    HRESULT ClearBackupInfo()
    {
        HRESULT hr = S_OK;
        CRescanResult   rfile;

        if ( NULL == m_piSlRfDB )
        {
            return E_NOINTERFACE;
        }

        hr = m_piSlRfDB->Open( m_dbpath );
        if ( SUCCEEDED( hr ) )
        {
            CComPtr<Skylark::ISQLiteComStatement3>  piRfRtm;

            hr = m_piSlRfDB->PrepareStatement( BACKUP_RESULT_CLEAR, &piRfRtm );
            if ( SUCCEEDED( hr ) )
            {
                hr = piRfRtm->ExecuteUpdate();
            }

            m_piSlRfDB->Close();
        }

        return hr;
    }

    HRESULT GetReportingResult( 
        CAtlArray<CRescanResult>& rescanResArray, 
        int nType = REPORT_FILE_STATE_REPORTING | REPORT_FILE_STATE_WAIT_SCAN
        )
    {
        HRESULT hr = S_OK;
        CRescanResult   rfile;

        if ( NULL == m_piSlRfDB )
        {
            return E_NOINTERFACE;
        }

        hr = m_piSlRfDB->Open( m_dbpath );
        if ( SUCCEEDED( hr ) )
        {
            if ( ( nType & REPORT_FILE_STATE_REPORTING ) == REPORT_FILE_STATE_REPORTING )
            {
                CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

                hr = m_piSlRfDB->ExecuteQuery( REPORT_FILE_REPORTING, &piRfRtm );
                if ( SUCCEEDED( hr ) )
                {
                    while( !piRfRtm->IsEof() )
                    {
                        rfile.strMd5 = L"";
                        rfile.strFileName = piRfRtm->GetAsString( 0 );
                        rfile.nTrack = piRfRtm->GetInt( 3 );
                        rfile.nOpStatus = BkFileStateNull;
                        rfile.nSecType = BkQueryInfoFileTypeUnknown;
                        rfile.nSecLevel = SLPTL_LEVEL_MARK___UNRECOGNISED_FILE;
                        rfile.ullCommitTime = piRfRtm->GetInt64( 5 );
                        rfile.bRetired = ( piRfRtm->GetInt( 2 ) == 2 ); 

                        rescanResArray.Add( rfile );
                        piRfRtm->NextRow();
                    }
                }
            }

            if ( ( nType & REPORT_FILE_STATE_WAIT_SCAN ) == REPORT_FILE_STATE_WAIT_SCAN )
            {
                CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

                hr = m_piSlRfDB->ExecuteQuery( REPORT_FILE_NEVER_RESCAN, &piRfRtm );
                if ( SUCCEEDED( hr ) )
                {
                    while( !piRfRtm->IsEof() )
                    {
                        rfile.strMd5 = L"";
                        rfile.strFileName = piRfRtm->GetAsString( 0 );
                        rfile.nOpStatus = BkFileStateNull;
                        rfile.nSecType = BkQueryInfoFileTypeSafe;
                        rfile.nSecLevel = SLPTL_LEVEL_MARK___NEVER_BEEN_SCANNED;
                        rfile.nTrack = piRfRtm->GetInt( 3 );
                        rfile.ullCommitTime = piRfRtm->GetInt64( 5 );

                        rescanResArray.Add( rfile );
                        piRfRtm->NextRow();
                    }
                }
            }

            m_piSlRfDB->Close();
        }

        return hr;
    }

    HRESULT GetRescanResult( 
        CAtlArray<CRescanResult>& rescanResArray, 
        ULONGLONG ullTimePoint,
        BOOL bShowAll
        )
    {
        CString strSql;
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;
        CRescanResult rfile;

        if ( NULL == m_piSlRsDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        rescanResArray.RemoveAll();

        if ( bShowAll )
        {
            GetReportingResult( rescanResArray );
            GetReportedBackupFile( rescanResArray );
        }
        else
        {
            GetReportingResult( rescanResArray, REPORT_FILE_STATE_WAIT_SCAN );
        }

        if ( bShowAll )
        {
            strSql = RESCAN_RESULT_GET;
        }
        else
        {
            if ( 0 != ullTimePoint )
            {
                strSql.Format( RESCAN_RESULT_GET_BY_TIME, ullTimePoint, 0 );
            }
            else
            {
                strSql = RESCAN_RESULT_GET_BY_SHOW;
            }
        }

        hr = m_piSlRsDB->ExecuteQuery( strSql, &piRtm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        while( !piRtm->IsEof() )
        {
            rfile.strMd5 = piRtm->GetAsString( 0 );
            rfile.strFileName = piRtm->GetAsString( 1 );
            rfile.nSecType = piRtm->GetInt( 2 );
            rfile.nSecLevel = piRtm->GetInt( 3 );
            rfile.nOpStatus = piRtm->GetInt( 4 );
            rfile.ullCommitTime = piRtm->GetInt64( 5 );

            if ( BkQueryInfoFileTypeVirus == rfile.nSecType )
            {
                if ( ( WinMod::CWinPathApi::IsDirectory( rfile.strFileName ) ) 
                    || ( !WinMod::CWinPathApi::IsFileExisting( rfile.strFileName ) ) )
                {
                    rfile.nOpStatus = BkFileStateDeleteSuccess;
                }
            }

            rescanResArray.Add( rfile );

            piRtm->NextRow();
        }
Exit0:
        return hr;
    }

    HRESULT GetRescanResult2( 
        CAtlArray<CRescanResult>& rescanResArray, 
        ULONGLONG ullTimePoint,
        int nType = REPORT_FILE_STATE_REPORTING | REPORT_FILE_STATE_WAIT_SCAN
        )
    {
        CString strSql;
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;
        CRescanResult rfile;

        if ( NULL == m_piSlRsDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        rescanResArray.RemoveAll();

        GetReportingResult( rescanResArray, nType );

        if ( 0 != ullTimePoint )
        {
            strSql.Format( RESCAN_RESULT_GET_BY_TIME, ullTimePoint, 0 );
        }
        else
        {
            strSql = RESCAN_RESULT_GET_BY_SHOW;
        }

        hr = m_piSlRsDB->ExecuteQuery( strSql, &piRtm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        while( !piRtm->IsEof() )
        {
            rfile.strMd5 = piRtm->GetAsString( 0 );
            rfile.strFileName = piRtm->GetAsString( 1 );
            rfile.nSecType = piRtm->GetInt( 2 );
            rfile.nSecLevel = piRtm->GetInt( 3 );
            rfile.nOpStatus = piRtm->GetInt( 4 );
            rfile.ullCommitTime = piRtm->GetInt64( 5 );

            if ( BkQueryInfoFileTypeVirus == rfile.nSecType )
            {
                if ( ( WinMod::CWinPathApi::IsDirectory( rfile.strFileName ) ) 
                    || ( !WinMod::CWinPathApi::IsFileExisting( rfile.strFileName ) ) )
                {
                    rfile.nOpStatus = BkFileStateDeleteSuccess;
                }
            }

            rescanResArray.Add( rfile );

            piRtm->NextRow();
        }
Exit0:
        return hr;
    }

    HRESULT GetFirstTimeReportFile( CAtlArray<CRescanResult>& rescanResArray )
    {
        HRESULT hr = S_OK;
        CRescanResult   rfile;

        if ( NULL == m_piSlRfDB )
        {
            return E_NOINTERFACE;
        }

        hr = m_piSlRfDB->Open( m_dbpath );
        if ( SUCCEEDED( hr ) )
        {
            CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

            hr = m_piSlRfDB->ExecuteQuery( REPORT_FILE_FIRST_REPORTING, &piRfRtm );
            if ( SUCCEEDED( hr ) )
            {
                while( !piRfRtm->IsEof() )
                {
                    rfile.strMd5 = L"";
                    rfile.strFileName = piRfRtm->GetAsString( 0 );
                    rfile.nOpStatus = BkFileStateNull;
                    rfile.nSecType = BkQueryInfoFileTypeUnknown;
                    rfile.nSecLevel = SLPTL_LEVEL_MARK___UNRECOGNISED_FILE;
                    rfile.ullCommitTime = piRfRtm->GetInt64( 5 );
                    rfile.bRetired = FALSE;

                    rescanResArray.Add( rfile );
                    piRfRtm->NextRow();
                }
            }

            m_piSlRfDB->Close();
        }

        return hr;
    }

    BOOL    IsReportingFile()
    {
        HRESULT hr = S_OK;
        BOOL    bRet = FALSE;

        if ( NULL == m_piSlRfDB )
        {
            return E_NOINTERFACE;
        }

        hr = m_piSlRfDB->Open( m_dbpath );
        if ( SUCCEEDED( hr ) )
        {
            CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

            hr = m_piSlRfDB->ExecuteQuery( REPORT_FILE_HAS_REPORTING, &piRfRtm );
            if ( SUCCEEDED( hr ) )
            {
                if( !piRfRtm->IsEof() )
                {
                    bRet = TRUE;
                }
            }
            m_piSlRfDB->Close();
        }

        return bRet;
    }

    HRESULT ClearResult()
    {
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComStatement3>  piStm;

        if ( NULL == m_piSlRsDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        hr = m_piSlRsDB->PrepareStatement( RESCAN_RESULT_CLEAR, &piStm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        hr = piStm->ExecuteUpdate();

        ClearBackupInfo();
Exit0:
        return hr;
    }

    HRESULT UpdateShowState( const CAtlArray<CRescanResult>& resultList )
    {
        HRESULT     hr = S_OK;
        POSITION    pos = NULL;

        if ( NULL == m_piSlRsDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        if ( 0 == resultList.GetCount() )
        {
            goto Exit0;
        }

        hr = m_piSlRsDB->BeginTransaction();
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        for ( size_t i = 0; i < resultList.GetCount(); i++ )
        {
            CComPtr<Skylark::ISQLiteComStatement3>  piStm;
            CRescanResult   rs;
            CString         strSql;

            if ( resultList[ i ].strMd5 != L"" )
            {
                hr = m_piSlRsDB->PrepareStatement( RESCAN_RESULT_UPDATE_SHOW, &piStm );
                if ( FAILED( hr ) )
                {
                    goto Exit1;
                }

                piStm->Bind( 1, resultList[ i ].strMd5 );

                piStm->ExecuteUpdate();
            }
        }
Exit1:
        m_piSlRsDB->CommitTransaction();
Exit0:
        return hr;
    }

    HRESULT GetReportingCount( int& nCount )
    {
        HRESULT hr = S_OK;

        if ( NULL == m_piSlRfDB )
        {
            return E_NOINTERFACE;
        }

        nCount = 0;

        hr = m_piSlRfDB->Open( m_dbpath );
        if ( SUCCEEDED( hr ) )
        {
            {
                CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

                hr = m_piSlRfDB->ExecuteQuery( REPORT_FILE_REPORTING_CNT, &piRfRtm );
                if ( SUCCEEDED( hr ) )
                {
                    nCount += piRfRtm->GetInt( 0 );
                }
            }

            {
                CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

                hr = m_piSlRfDB->ExecuteQuery( REPORT_FILE_NEVER_RESCAN_CNT, &piRfRtm );
                if ( SUCCEEDED( hr ) )
                {
                    nCount += piRfRtm->GetInt( 0 );
                }
            }

            {
                CComPtr<Skylark::ISQLiteComResultSet3>  piRfRtm;

                hr = m_piSlRfDB->ExecuteQuery( REPORT_BACKUP_REPORTED_CNT, &piRfRtm );
                if ( SUCCEEDED( hr ) )
                {
                    nCount += piRfRtm->GetInt( 0 );
                }
            }

            m_piSlRfDB->Close();
        }

        return hr;
    }

    HRESULT GetTotalCount( int& nCount )
    {
        HRESULT hr = S_OK;
        CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;

        if ( NULL == m_piSlRsDB )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        nCount = 0;

        GetReportingCount( nCount );

        hr = m_piSlRsDB->ExecuteQuery( RESCAN_RESULT_COUNT, &piRtm );
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        if ( piRtm->IsEof() )
        {
            hr = E_FAIL;
            goto Exit0;
        }

        nCount += piRtm->GetInt( 0 );
Exit0:
        return hr;
    }
private:
    LONG                m_lInit;
    WinMod::CWinPath    m_modpath;
    WinMod::CWinPath    m_dbpath;
    Skylark::CWinModule_bkdb        m_dbmod;
    Skylark::ISQLiteComDatabase3*   m_piSlRsDB;
    Skylark::ISQLiteComDatabase3*   m_piSlRfDB;
};