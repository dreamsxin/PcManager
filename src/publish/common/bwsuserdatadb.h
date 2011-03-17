#pragma once

#include "winmod/winpath.h"
#include "skylark2/bkdb_mod.h"
#include "utility.h"

#define BK_BWSUSERDATA_DB_DLL_NAME               L"ksafedb.dll"
#define BK_BWSUSERDATA_DB_NAME                   L"bud.dat"


#define BK_BWSUSERDATA_OPERATION_ALLOW           1
#define BK_BWSUSERDATA_OPERATION_BLOCK           2

////////////////////////
///> 浏览器保护进程操作 ++
#define BK_BWSUSERDATA_PROCESS_TABLENAME         L"budprocess"

#define BK_BWSUSERDATA_PROCESS_CREATE_TABLE      L"create table budprocess ( file_path varchar not null, op_status integer not null, primary key ( file_path ) )"
#define BK_BWSUSERDATA_PROCESS_ADD_RECORD        L"insert or replace into budprocess ( file_path, op_status ) values(?,?)"
#define BK_BWSUSERDATA_PROCESS_GET_RECORDS       L"select * from budprocess"
#define BK_BWSUSERDATA_PROCESS_CLEAR_RECORD      L"delete from budprocess where file_path=?"
#define BK_BWSUSERDATA_PROCESS_CLEAR_ALL         L"delete from budprocess"

typedef struct _BWS_USER_DATA_PROCESS 
{
	CString m_strFilePath;
	CString m_strFileName;
	DWORD   m_dwOperation;
} BWS_USER_DATA_PROCESS, *PBWS_USER_DATA_PROCESS;

///> 浏览器保护进程操作 --

////////////////////////
///> 浏览器保护网址操作 ++
#define BK_BWSUSERDATA_URL_TABLENAME             L"budurl"

#define BK_BWSUSERDATA_URL_CREATE_TABLE          L"create table budurl ( url varchar not null, op_status integer not null, primary key ( url ) )"
#define BK_BWSUSERDATA_URL_ADD_RECORD            L"insert or replace into budurl ( url, op_status ) values(?,?)"
#define BK_BWSUSERDATA_URL_GET_RECORDS           L"select * from budurl"
#define BK_BWSUSERDATA_URL_CLEAR_RECORD          L"delete from budurl where url=?"
#define BK_BWSUSERDATA_URL_CLEAR_ALL             L"delete from budurl"

typedef struct _BWS_USER_DATA_URL 
{
	CString m_strUrl;
	DWORD   m_dwOperation;
} BWS_USER_DATA_URL, *PBWS_USER_DATA_URL;

///> 浏览器保护网址操作 --

////////////////////////
///> 浏览器保护操作历史 ++
#define BK_BWSUSERDATA_HISTORY_TABLENAME         L"budhistory"

#define BK_BWSUSERDATA_HISTORY_CREATE_TABLE      L"create table budhistory ( caption varchar not null, op_status integer not null, reason integer not null, create_time integer not null )"
#define BK_BWSUSERDATA_HISTORY_ADD_RECORD        L"insert or replace into budhistory ( caption, op_status, reason, create_time ) values(?,?,?,?)"
#define BK_BWSUSERDATA_HISTORY_GET_RECORDS       L"select * from budhistory"
#define BK_BWSUSERDATA_HISTORY_CLEAR_ALL         L"delete from budhistory"

#define BK_BWSUSERDATA_OPERATION_HISTORY_REASON_PROCESS       1 // 木马执行
#define BK_BWSUSERDATA_OPERATION_HISTORY_REASON_SCRIPT        2 // 恶意脚本
#define BK_BWSUSERDATA_OPERATION_HISTORY_REASON_URL           3 // 恶意网址

typedef struct _BWS_USER_DATA_HISTORY 
{
	CString   m_strTimeStr;
	CString   m_strCaption;
	DWORD     m_dwOperation;
	DWORD     m_dwReason;
	ULONGLONG m_ullCommitTime;
} BWS_USER_DATA_HISTORY, *PBWS_USER_DATA_HISTORY;

///> 浏览器保护操作历史 --


class   CBwsUserDataDB
{
public:
	CBwsUserDataDB(void) : 
	  m_lInit( 0 ),
		  m_piSlRsDB( NULL ),
		  m_piSlRfDB( NULL )
	  {

	  }
	  ~CBwsUserDataDB(void)
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

		m_modpath.Append( BK_BWSUSERDATA_DB_DLL_NAME );
		m_dbpath.Append(  BK_BWSUSERDATA_DB_NAME );

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

		if ( !m_piSlRsDB->TableExists( BK_BWSUSERDATA_PROCESS_TABLENAME ) )
		{
			hr = m_piSlRsDB->ExecuteUpdate( BK_BWSUSERDATA_PROCESS_CREATE_TABLE );
			if ( FAILED( hr ) )
			{
				m_piSlRsDB->GetErrorMsg();
				m_piSlRsDB->Close();
				goto Exit1;
			}
		}

		if ( !m_piSlRsDB->TableExists( BK_BWSUSERDATA_URL_TABLENAME ) )
		{
			hr = m_piSlRsDB->ExecuteUpdate( BK_BWSUSERDATA_URL_CREATE_TABLE );
			if ( FAILED( hr ) )
			{
				m_piSlRsDB->GetErrorMsg();
				m_piSlRsDB->Close();
				goto Exit1;
			}
		}

		//if ( !m_piSlRsDB->TableExists( BK_BWSUSERDATA_HISTORY_TABLENAME ) )
		//{
		//	hr = m_piSlRsDB->ExecuteUpdate( BK_BWSUSERDATA_HISTORY_CREATE_TABLE );
		//	if ( FAILED( hr ) )
		//	{
		//		m_piSlRsDB->GetErrorMsg();
		//		m_piSlRsDB->Close();
		//		goto Exit1;
		//	}
		//}

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

	HRESULT AddProcessRecord( const BWS_USER_DATA_PROCESS& bwsUdProcess )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;
		WinMod::CWinPath pathNormalized;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_PROCESS_ADD_RECORD, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		pathNormalized.m_strPath = bwsUdProcess.m_strFilePath;
		pathNormalized.ExpandNormalizedPathName();

		piStm->Bind( 1, pathNormalized.m_strPath );
		piStm->Bind( 2, (int)bwsUdProcess.m_dwOperation );

		hr = piStm->ExecuteUpdate();
Exit0:
		return hr;
	}

	HRESULT GetProcessRecordArray( CAtlArray<BWS_USER_DATA_PROCESS>& arrayBwsUdProcess, BOOL bShow = TRUE )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;
		BWS_USER_DATA_PROCESS udProcess;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		arrayBwsUdProcess.RemoveAll();

		hr = m_piSlRsDB->ExecuteQuery( BK_BWSUSERDATA_PROCESS_GET_RECORDS, &piRtm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		while( !piRtm->IsEof() )
		{
			udProcess.m_strFilePath = piRtm->GetAsString( 0 );
			udProcess.m_dwOperation = piRtm->GetInt( 1 );

			if ( bShow )
			{
				udProcess.m_strFilePath = WinMod::CWinPathApi::FindAfterAnyPrefix( udProcess.m_strFilePath );
			}

			udProcess.m_strFileName = WinMod::CWinPathApi::FindFileName( udProcess.m_strFilePath );
			
			//rfile.ullCommitTime = piRtm->GetInt64( 5 );

			arrayBwsUdProcess.Add( udProcess );

			piRtm->NextRow();
		}
Exit0:
		return hr;
	}

	HRESULT ClearProcessRecordArray( CAtlArray<CString>& arrayProcessPath )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;
		WinMod::CWinPath pathNormalized;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_PROCESS_CLEAR_RECORD, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		size_t stCount = arrayProcessPath.GetCount();
		for ( size_t i = 0 ; i < stCount ; i ++ )
		{
			pathNormalized.m_strPath = arrayProcessPath[i];
			pathNormalized.ExpandNormalizedPathName();
			piStm->Bind( 1, pathNormalized.m_strPath );
			hr = piStm->ExecuteUpdate();
			if ( FAILED (hr) )
			{
				goto Exit0;
			}

			piStm->Reset();
		}
Exit0:
		return hr;
	}

	HRESULT ClearProcessRecordAll( )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_PROCESS_CLEAR_ALL, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		hr = piStm->ExecuteUpdate();
Exit0:
		return hr;
	}

	HRESULT AddHistoryRecord( const BWS_USER_DATA_HISTORY& bwsUdHistory )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_HISTORY_ADD_RECORD, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		piStm->Bind( 1, bwsUdHistory.m_strCaption );
		piStm->Bind( 2, (int)bwsUdHistory.m_dwOperation );
		piStm->Bind( 3, (int)bwsUdHistory.m_dwReason );
		piStm->Bind( 4, bwsUdHistory.m_ullCommitTime );

		hr = piStm->ExecuteUpdate();
Exit0:
		return hr;
	}

	HRESULT GetHistoryRecordArray( CAtlArray<BWS_USER_DATA_HISTORY>& arrayBwsUdHistory, BOOL bShow = TRUE )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;
		BWS_USER_DATA_HISTORY udHistory;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		arrayBwsUdHistory.RemoveAll();

		hr = m_piSlRsDB->ExecuteQuery( BK_BWSUSERDATA_HISTORY_GET_RECORDS, &piRtm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		while( !piRtm->IsEof() )
		{
			udHistory.m_strCaption    = piRtm->GetAsString( 0 );
			udHistory.m_dwOperation   = piRtm->GetInt( 1 );
			udHistory.m_dwReason      = piRtm->GetInt( 2 );
			udHistory.m_ullCommitTime = piRtm->GetInt64( 3 );

			if ( bShow )
			{
				TimeULONGLONG2CString( udHistory.m_ullCommitTime, udHistory.m_strTimeStr );
			}

			arrayBwsUdHistory.Add( udHistory );

			piRtm->NextRow();
		}
Exit0:
		return hr;
	}

	HRESULT ClearHistoryRecordAll( )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_HISTORY_CLEAR_ALL, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		hr = piStm->ExecuteUpdate();
Exit0:
		return hr;
	}

	HRESULT AddUrlRecord( const BWS_USER_DATA_URL& bwsUdUrl )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			return hr;
		}

		hr = m_piSlRsDB->BeginTransaction();
		if ( FAILED( hr ) )
		{
			return hr;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_URL_ADD_RECORD, &piStm );
		if ( FAILED( hr ) )
		{
			m_piSlRsDB->CommitTransaction();
			return hr;
		}

		piStm->Bind( 1, bwsUdUrl.m_strUrl );
		piStm->Bind( 2, (int)bwsUdUrl.m_dwOperation );

		hr = piStm->ExecuteUpdate();

		m_piSlRsDB->CommitTransaction();

		return hr;
	}

	HRESULT AddUrlRecordArray( const CAtlArray<BWS_USER_DATA_URL>& arrayBwsUdUrl )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			return hr;
		}

		hr = m_piSlRsDB->BeginTransaction();
		if ( FAILED( hr ) )
		{
			return hr;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_URL_ADD_RECORD, &piStm );
		if ( FAILED( hr ) )
		{
			m_piSlRsDB->CommitTransaction();
			return hr;
		}

		size_t stCount = arrayBwsUdUrl.GetCount();
		for ( size_t i = 0 ; i < stCount ; i ++ )
		{
			piStm->Bind( 1, arrayBwsUdUrl[i].m_strUrl );
			piStm->Bind( 2, (int)arrayBwsUdUrl[i].m_dwOperation );
			hr = piStm->ExecuteUpdate();
			if ( FAILED(hr) )
			{
				break;
			}

			piStm->Reset();
		}

		m_piSlRsDB->CommitTransaction();

		return hr;
	}

	HRESULT GetUrlRecordArray( CAtlArray<BWS_USER_DATA_URL>& arrayBwsUdUrl )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComResultSet3>  piRtm;
		BWS_USER_DATA_URL udUrl;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			return hr;
		}

		arrayBwsUdUrl.RemoveAll();

		hr = m_piSlRsDB->BeginTransaction();
		if ( FAILED( hr ) )
		{
			return hr;
		}

		hr = m_piSlRsDB->ExecuteQuery( BK_BWSUSERDATA_URL_GET_RECORDS, &piRtm );
		if ( FAILED( hr ) )
		{
			m_piSlRsDB->CommitTransaction();
			return hr;
		}

		while( !piRtm->IsEof() )
		{
			udUrl.m_strUrl      = piRtm->GetAsString( 0 );
			udUrl.m_dwOperation = piRtm->GetInt( 1 );

			arrayBwsUdUrl.Add( udUrl );

			piRtm->NextRow();
		}

		m_piSlRsDB->CommitTransaction();		
		return hr;
	}

	HRESULT ClearUrlRecordArray( CAtlArray<CString>& arrayUrlPath )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_URL_CLEAR_RECORD, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		size_t stCount = arrayUrlPath.GetCount();
		for ( size_t i = 0 ; i < stCount ; i ++ )
		{
			piStm->Bind( 1, arrayUrlPath[i] );
			hr = piStm->ExecuteUpdate();
			if ( FAILED (hr) )
			{
				goto Exit0;
			}

			piStm->Reset();
		}
Exit0:
		return hr;
	}

	HRESULT ClearUrlRecordAll( )
	{
		HRESULT hr = S_OK;
		CComPtr<Skylark::ISQLiteComStatement3>  piStm;

		if ( NULL == m_piSlRsDB )
		{
			hr = E_NOINTERFACE;
			goto Exit0;
		}

		hr = m_piSlRsDB->PrepareStatement( BK_BWSUSERDATA_URL_CLEAR_ALL, &piStm );
		if ( FAILED( hr ) )
		{
			goto Exit0;
		}

		hr = piStm->ExecuteUpdate();
Exit0:
		return hr;
	}

private:
	void TimeULONGLONG2CString( ULONGLONG ullCommitTime, CString& strTime )
	{
		ULARGE_INTEGER ullTime;
		ullTime.QuadPart = ullCommitTime;

		FILETIME ftTime;
		ftTime.dwHighDateTime = ullTime.HighPart;
		ftTime.dwLowDateTime  = ullTime.LowPart;

		SYSTEMTIME systTime;
		if ( ::FileTimeToSystemTime( &ftTime, &systTime ) )
		{
			strTime.Format(L"%d-%02d-%02d", systTime.wYear, systTime.wMonth, systTime.wDay );
		}
		else
		{
			strTime = L"";
		}
	}

private:
	LONG                m_lInit;
	WinMod::CWinPath    m_modpath;
	WinMod::CWinPath    m_dbpath;
	Skylark::CWinModule_bkdb        m_dbmod;
	Skylark::ISQLiteComDatabase3*   m_piSlRsDB;
	Skylark::ISQLiteComDatabase3*   m_piSlRfDB;
};
