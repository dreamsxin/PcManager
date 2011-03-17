////////////////////////////////////////////////////////////////
//
//	Filename: 	ksqlite_loader.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-8-13  10: 46
//	Comment:	
//
///////////////////////////////////////////////////////////////
#pragma once
#include "ksqlite_export_fun_def.h"
#include "kthreadsyn.h"
#include "singleton.h"
#include <tchar.h>
#include <Shlwapi.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#define KSQLITE_DLL_NAME _T("sqlite.dll")

class KSqliteDynamicLoader
{
public:
	KSqliteDynamicLoader()
	{
		m_hModule = NULL;
		m_bSelfLoaded = false;
		ClearFunction();

		m_hModule = GetModuleHandle(KSQLITE_DLL_NAME);
		if (m_hModule != NULL)
		{
			int nRet = GetFunction();
			if (nRet != 0)
			{
				ClearFunction();
				m_hModule = NULL;
			}
		}
	}


	int ksqlite3_open(
		const char *filename,
		sqlite3 **ppDb
		)
	{
		int nRet = -1;

		if (m_hModule == NULL)
		{
			nRet = Load();
			if (nRet != 0)
				return nRet;
		}

		nRet = m_pfn_ksqlite3_open(filename, ppDb);
		if (nRet != 0 && m_bSelfLoaded)
			Unload();

		return nRet;
	}

	int ksqlite3_open_v2(const char *filename,
		sqlite3 **ppDb, 
		int flags,
		const char *zVfs
		)
	{
		int nRet = -1;

		if (m_hModule == NULL)
		{
			nRet = Load();
			if (nRet != 0)
				return nRet;
		}

		nRet = m_pfn_ksqlite3_open_v2(filename, ppDb, flags, zVfs);
		if (nRet != 0 && m_bSelfLoaded)
			Unload();

		return nRet;
	}

	int ksqlite3_key(
		sqlite3 *pdb,
		const void *pKey, 
		int nKey
		)
	{
		if(m_pfn_ksqlite3_key == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_key(pdb, pKey, nKey);
	}

	int ksqlite3_busy_handler(
		sqlite3* pDb, 
		int(*pbusy_callback_fun)(void*,int),
		void* pParam
		)
	{
		if(m_pfn_ksqlite3_busy_handler == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_busy_handler(pDb, pbusy_callback_fun, pParam);
	}

	int ksqlite3_close(
		sqlite3 *pDb
		)
	{
		if(m_pfn_ksqlite3_close == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_close(pDb);
	}

	int ksqlite3_exec(
		sqlite3* pDb,
		const char *sql,
		int (*callback)(void*,int,char**,char**),
		void *pParam,
		char **errmsg
		)
	{
		if(m_pfn_ksqlite3_exec== NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_exec(pDb, sql, callback, pParam,errmsg);
	}

	int ksqlite3_prepare(
		sqlite3 *pdb,
		const char *zSql,
		int nByte,
		sqlite3_stmt **ppStmt,
		const char **pzTail
		)
	{
		if(m_pfn_ksqlite3_prepare == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_prepare(pdb, zSql, nByte, ppStmt, pzTail);
	}

	int ksqlite3_prepare_v2(
		sqlite3 *pdb,
		const char *zSql,
		int nByte,
		sqlite3_stmt **ppStmt,
		const char **pzTail
		)
	{
		if(m_pfn_ksqlite3_prepare == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_prepare_v2(pdb, zSql, nByte, ppStmt, pzTail);
	}


	int ksqlite3_column_count(
		sqlite3_stmt *pStmt
		)
	{
		if(m_pfn_ksqlite3_column_count== NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_column_count(pStmt);
	}

	int ksqlite3_step(
		sqlite3_stmt*pStmt
		)
	{
		if(m_pfn_ksqlite3_step == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_step(pStmt);
	}


	const unsigned char* ksqlite3_column_text(
		sqlite3_stmt* pstmt,
		int iCol
		)
	{
		if(m_pfn_ksqlite3_column_text == NULL)
			return NULL;

		return m_pfn_ksqlite3_column_text(pstmt, iCol);
	}

	int ksqlite3_column_int(
		sqlite3_stmt* pstmt,
		int iCol
		)
	{
		if(m_pfn_ksqlite3_column_int == NULL)
			return 0;

		return m_pfn_ksqlite3_column_int(pstmt, iCol);
	}

	int ksqlite3_column_bytes(
		sqlite3_stmt* pstmt,
		int iCol
		)
	{
		if(m_pfn_ksqlite3_column_bytes == NULL)
			return 0;

		return m_pfn_ksqlite3_column_bytes(pstmt, iCol);
	}

	const void* ksqlite3_column_blob(
		sqlite3_stmt* pstmt,
		int iCol
		)
	{
		if(m_pfn_ksqlite3_column_blob == NULL)
			return NULL;

		return m_pfn_ksqlite3_column_blob(pstmt, iCol);
	}

	int ksqlite3_finalize(
		sqlite3_stmt *pStmt
		)
	{
		if(m_pfn_ksqlite3_finalize == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_finalize(pStmt);
	}

	char* ksqlite3_vmprintf(
		const char* szFormat,
		va_list valist
		)
	{
		if(m_pfn_ksqlite3_vmprintf == NULL)
			return NULL;

		return m_pfn_ksqlite3_vmprintf(szFormat, valist);
	}

	void ksqlite3_free(
		void* pData
		)
	{
		if(m_pfn_ksqlite3_free != NULL)
			m_pfn_ksqlite3_free(pData);
		
	}

	void ksqlite3_interrupt(
		sqlite3* pDb
		)
	{
		if(m_pfn_ksqlite3_interrupt != NULL)
		{
			m_pfn_ksqlite3_interrupt(pDb);
		}
	}

	int ksqlite3_bind_int64(sqlite3_stmt* pStmt, int nCol, sqlite3_int64 nValue)
	{
		if(m_pfn_ksqlite3_bind_int64 == NULL)
			return E_NOTIMPL;
		
		return m_pfn_ksqlite3_bind_int64(pStmt, nCol, nValue);
	}

	int ksqlite3_bind_int(sqlite3_stmt* pStmt, int nCol, int nValue)
	{
		if(m_pfn_ksqlite3_bind_int == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_bind_int(pStmt, nCol, nValue);
	}

	int ksqlite3_bind_blob(sqlite3_stmt* pStmt, int nCol, const void* pv, int nlen, void(*pFun)(void*))
	{
		if(m_pfn_ksqlite3_bind_blob == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_bind_blob(pStmt, nCol, pv, nlen, pFun);
	}

	int ksqlite3_reset(sqlite3_stmt *pStmt)
	{
		if(m_pfn_ksqlite3_reset == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_reset(pStmt);
	}

	sqlite3_int64 ksqlite3_column_int64(
		sqlite3_stmt* pstmt,
		int iCol
		)
	{
		if(m_pfn_ksqlite3_column_int64 == NULL)
			return 0;

		return m_pfn_ksqlite3_column_int64(pstmt, iCol);
	}


	int ksqlite3_bind_null(sqlite3_stmt* pStmt, int nCol)
	{
		if(m_pfn_ksqlite3_bind_null == NULL)
			return E_NOTIMPL;

		return m_pfn_ksqlite3_bind_null(pStmt, nCol);
	}
	
	int ksqlite3_column_type(sqlite3_stmt* stmt, int iCol)
	{
		if(m_pfn_ksqlite3_column_type == NULL)
			return 0;

		return m_pfn_ksqlite3_column_type(stmt, iCol);
	}

	int ksqlite3_open16(const void* filename, sqlite3 **ppDb)
	{
		/*
		if ( m_pfn_ksqlite3_open16 == NULL )
			return E_NOTIMPL;

		return m_pfn_ksqlite3_open16(filename, ppDb);
		*/

		int nRet = -1;

		if (m_hModule == NULL)
		{
			nRet = Load();

			if (nRet != 0)
			{
				return nRet;
			}
		}

		nRet = m_pfn_ksqlite3_open16(filename, ppDb);

		if (nRet != 0 && m_bSelfLoaded)
		{
			Unload();
		}

		return nRet;
	}

	int ksqlite3_rekey(sqlite3 *pdb, const void *pKey, int nKey)
	{
		if ( m_pfn_ksqlite3_rekey == NULL )
			return E_NOTIMPL;

		return m_pfn_ksqlite3_rekey(pdb, pKey, nKey);
	}

	/// 2011.01.24
	int ksqlite3_extended_result_codes(
		sqlite3* pdb, 
		int onoff)
	{
		if (NULL == m_pfn_ksqlite3_extended_result_codes)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_extended_result_codes(pdb, onoff);
	}

	int ksqlite3_get_autocommit(
		sqlite3* pdb)
	{
		if (NULL == m_pfn_ksqlite3_get_autocommit)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_get_autocommit(pdb);
	}

	sqlite3_int64 ksqlite3_last_insert_rowid(
		sqlite3* pdb)
	{
		if (NULL == m_pfn_ksqlite3_last_insert_rowid)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_last_insert_rowid(pdb);
	}

	int ksqlite3_busy_timeout(
		sqlite3*pdb
		, int ms)
	{
		if (NULL == m_pfn_ksqlite3_busy_timeout)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_busy_timeout(pdb, ms);
	}

	const void *ksqlite3_errmsg16(
		sqlite3*pdb)
	{
		if (NULL == m_pfn_ksqlite3_errmsg16)
		{
			return NULL;
		}

		return m_pfn_ksqlite3_errmsg16(pdb);
	}

	int ksqlite3_errcode(
		sqlite3*pdb)
	{
		if (NULL == m_pfn_ksqlite3_errcode)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_errcode(pdb);
	}

	int ksqlite3_prepare16_v2(
		sqlite3 *pdb 
		, const void *zSql
		, int nByte 
		, sqlite3_stmt **ppStmt 
		, const void **pzTail)
	{
		if (NULL == m_pfn_ksqlite3_prepare16_v2)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_prepare16_v2(pdb, zSql, nByte, ppStmt, pzTail);
	}

	int ksqlite3_changes(
		sqlite3 *pdb)
	{
		if (NULL == m_pfn_ksqlite3_changes)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_changes(pdb);
	}

	int ksqlite3_bind_text16(
		sqlite3_stmt *ppStmt 
		, int nInt1 
		, const void *pVoid
		, int nint2 
		, void(* pFunc)(void*) )
	{
		if (NULL == m_pfn_ksqlite3_bind_text16)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_bind_text16(ppStmt, nInt1 , pVoid, nint2 , pFunc);
	}

	int ksqlite3_bind_text(
		sqlite3_stmt *ppStmt 
		, int nInt1 
		, const char *pChar
		, int nint2 
		, void(* pFunc)(void*) )
	{
		if (NULL == m_pfn_ksqlite3_bind_text)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_bind_text(ppStmt, nInt1 , pChar, nint2 , pFunc);
	}

	int ksqlite3_bind_zeroblob(
		sqlite3_stmt *ppStmt 
		, int nlen
		, int n)
	{
		if (NULL == m_pfn_ksqlite3_bind_zeroblob)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_bind_zeroblob(ppStmt, nlen, n);
	}

	int ksqlite3_bind_parameter_count(
		sqlite3_stmt *ppStmt)
	{
		if (NULL == m_pfn_ksqlite3_bind_parameter_count)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_bind_parameter_count(ppStmt);
	}

	int ksqlite3_bind_parameter_index(
		sqlite3_stmt *ppStmt
		, const char *zName)
	{
		if (NULL == m_pfn_ksqlite3_bind_parameter_index)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_bind_parameter_index(ppStmt, zName);
	}

	const char *ksqlite3_column_name(
		sqlite3_stmt *ppStmt
		, int N)
	{
		if (NULL == m_pfn_ksqlite3_column_name)
		{
			return NULL;
		}

		return m_pfn_ksqlite3_column_name(ppStmt, N);
	}

	const void *ksqlite3_column_name16(
		sqlite3_stmt *ppStmt
		, int N)
	{
		if (NULL == m_pfn_ksqlite3_column_name16)
		{
			return NULL;
		}

		return m_pfn_ksqlite3_column_name16(ppStmt, N);
	}

	const void *ksqlite3_column_decltype16(
		sqlite3_stmt *ppStmt
		, int N)
	{
		if (NULL == m_pfn_ksqlite3_column_decltype16)
		{
			return NULL;
		}

		return m_pfn_ksqlite3_column_decltype16(ppStmt, N);
	}

	int ksqlite3_column_bytes16(
		sqlite3_stmt *ppStmt
		, int iCol)
	{
		if (NULL == m_pfn_ksqlite3_column_bytes16)
		{
			return E_NOTIMPL;
		}

		return m_pfn_ksqlite3_column_bytes16(ppStmt, iCol);
	}

	const void *ksqlite3_column_text16(
		sqlite3_stmt *ppStmt
		, int iCol)
	{
		if (NULL == m_pfn_ksqlite3_column_text16)
		{
			return NULL;
		}

		return m_pfn_ksqlite3_column_text16(ppStmt, iCol);
	}


private:
	int Load()
	{
		KSYNGuard guard(m_mutex);
		if (m_hModule != NULL)
		{
			return 0;
		}

		m_hModule = GetModuleHandle(KSQLITE_DLL_NAME);
		if (m_hModule == NULL)
		{
			TCHAR szPath[MAX_PATH] = { 0 };
			::GetModuleFileName((HINSTANCE)&__ImageBase, szPath, MAX_PATH);
			PathRemoveFileSpec(szPath);
			PathAppend(szPath, KSQLITE_DLL_NAME);
			m_hModule = LoadLibrary(szPath);
			if (m_hModule != NULL)
			{
				m_bSelfLoaded = true;
			}
			else
			{
				/// 在EXE目录下尝试装载
				::GetModuleFileName(NULL, szPath, MAX_PATH);
				PathRemoveFileSpec(szPath);
				PathAppend(szPath, KSQLITE_DLL_NAME);

				m_hModule = LoadLibrary(szPath);

				if (m_hModule != NULL)
				{
					m_bSelfLoaded = true;
				}
			}
		}
		else
		{
			m_bSelfLoaded = false;
		}

		int nRet = -1;
		if (m_hModule)
		{
			nRet = GetFunction();
			if (nRet != 0)
			{
				ClearFunction();
				if (m_bSelfLoaded)
				{
					FreeLibrary(m_hModule);
					m_hModule = NULL;
					m_bSelfLoaded = false;
				}				
			}
		}
		return nRet;
	}

	void Unload()
	{
		KSYNGuard guard(m_mutex);

		ClearFunction();
		if (m_bSelfLoaded && m_hModule != NULL)
		{
			FreeLibrary(m_hModule);
		}

		m_bSelfLoaded = false;
		m_hModule = NULL;
	}

	int GetFunction()
	{
		m_pfn_ksqlite3_open = (type_ksqlite3_open)GetProcAddress(m_hModule, "sqlite3_open");
		if (m_pfn_ksqlite3_open == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_key = (type_ksqlite3_key)GetProcAddress(m_hModule, "sqlite3_key");
		if (m_pfn_ksqlite3_key == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_busy_handler = (type_ksqlite3_busy_handler)GetProcAddress(m_hModule, "sqlite3_busy_handler");
		if (m_pfn_ksqlite3_busy_handler == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_close = (type_ksqlite3_close)GetProcAddress(m_hModule, "sqlite3_close");
		if (m_pfn_ksqlite3_close == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_exec = (type_ksqlite3_exec)GetProcAddress(m_hModule, "sqlite3_exec");
		if (m_pfn_ksqlite3_exec == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_prepare = (type_ksqlite3_prepare)GetProcAddress(m_hModule, "sqlite3_prepare");
		if (m_pfn_ksqlite3_prepare == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_count = (type_ksqlite3_column_count)GetProcAddress(m_hModule, "sqlite3_column_count");
		if (m_pfn_ksqlite3_column_count == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_step = (type_ksqlite3_step)GetProcAddress(m_hModule, "sqlite3_step");
		if (m_pfn_ksqlite3_step == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_text = (type_ksqlite3_column_text)GetProcAddress(m_hModule, "sqlite3_column_text");
		if (m_pfn_ksqlite3_column_text == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_finalize = (type_ksqlite3_finalize)GetProcAddress(m_hModule, "sqlite3_finalize");
		if (m_pfn_ksqlite3_finalize == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_vmprintf = (type_ksqlite3_vmprintf)GetProcAddress(m_hModule, "sqlite3_vmprintf");
		if (m_pfn_ksqlite3_vmprintf == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_free = (type_ksqlite3_free)GetProcAddress(m_hModule, "sqlite3_free");
		if (m_pfn_ksqlite3_free == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_interrupt = (type_ksqlite3_interrupt)GetProcAddress(m_hModule, "sqlite3_interrupt");
		if (m_pfn_ksqlite3_interrupt == NULL)
			return E_NOTIMPL;

		//////////////////////////////////////////////////////////////////////////
		m_pfn_ksqlite3_open_v2 = (type_ksqlite3_open_v2)GetProcAddress(m_hModule, "sqlite3_open_v2");
		if (m_pfn_ksqlite3_open_v2 == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_prepare_v2 = (type_ksqlite3_prepare_v2)GetProcAddress(m_hModule, "sqlite3_prepare_v2");
		if (m_pfn_ksqlite3_prepare_v2 == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_int = (type_ksqlite3_column_int)GetProcAddress(m_hModule, "sqlite3_column_int");
		if (m_pfn_ksqlite3_column_int == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_bytes = (type_ksqlite3_column_bytes)GetProcAddress(m_hModule, "sqlite3_column_bytes");
		if (m_pfn_ksqlite3_column_bytes == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_blob = (type_ksqlite3_column_blob)GetProcAddress(m_hModule, "sqlite3_column_blob");
		if (m_pfn_ksqlite3_column_blob == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_int64 = (type_ksqlite3_bind_int64)GetProcAddress(m_hModule, "sqlite3_bind_int64");
		if (m_pfn_ksqlite3_bind_int64 == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_int = (type_ksqlite3_bind_int)GetProcAddress(m_hModule, "sqlite3_bind_int");
		if (m_pfn_ksqlite3_bind_int == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_blob = (type_ksqlite3_bind_blob)GetProcAddress(m_hModule, "sqlite3_bind_blob");
		if (m_pfn_ksqlite3_bind_blob == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_reset = (type_ksqlite3_reset)GetProcAddress(m_hModule, "sqlite3_reset");
		if (m_pfn_ksqlite3_reset == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_int64 = (type_ksqlite3_column_int64)GetProcAddress(m_hModule, "sqlite3_column_int64");
		if (m_pfn_ksqlite3_column_int64 == NULL)
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_null = (type_ksqlite3_bind_null)GetProcAddress(m_hModule, "sqlite3_bind_null");
		if ( m_pfn_ksqlite3_bind_null == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_type = (type_ksqlite3_column_type)GetProcAddress(m_hModule, "sqlite3_column_type");
		if ( m_pfn_ksqlite3_column_type == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_open16 = ( type_ksqlite3_open16)GetProcAddress(m_hModule, "sqlite3_open16");
		if ( m_pfn_ksqlite3_open16 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_rekey = (type_ksqlite3_rekey)GetProcAddress(m_hModule, "sqlite3_rekey");
		if ( m_pfn_ksqlite3_rekey == NULL )
			return E_NOTIMPL;

		/// 2011.01.24
		m_pfn_ksqlite3_extended_result_codes = (type_ksqlite3_extended_result_codes)GetProcAddress(m_hModule, "sqlite3_extended_result_codes");
		if ( m_pfn_ksqlite3_extended_result_codes == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_get_autocommit = (type_ksqlite3_get_autocommit)GetProcAddress(m_hModule, "sqlite3_get_autocommit");
		if ( m_pfn_ksqlite3_get_autocommit == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_last_insert_rowid = (type_ksqlite3_last_insert_rowid)GetProcAddress(m_hModule, "sqlite3_last_insert_rowid");
		if ( m_pfn_ksqlite3_last_insert_rowid == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_busy_timeout = (type_ksqlite3_busy_timeout)GetProcAddress(m_hModule, "sqlite3_busy_timeout");
		if ( m_pfn_ksqlite3_busy_timeout == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_errmsg16 = (type_ksqlite3_errmsg16)GetProcAddress(m_hModule, "sqlite3_errmsg16");
		if ( m_pfn_ksqlite3_errmsg16 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_errcode = (type_ksqlite3_errcode)GetProcAddress(m_hModule, "sqlite3_errcode");
		if ( m_pfn_ksqlite3_errcode == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_prepare16_v2 = (type_ksqlite3_prepare16_v2)GetProcAddress(m_hModule, "sqlite3_prepare16_v2");
		if ( m_pfn_ksqlite3_prepare16_v2 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_changes = (type_ksqlite3_changes)GetProcAddress(m_hModule, "sqlite3_changes");
		if ( m_pfn_ksqlite3_changes == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_text16 = (type_ksqlite3_bind_text16)GetProcAddress(m_hModule, "sqlite3_bind_text16");
		if ( m_pfn_ksqlite3_bind_text16 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_text = (type_ksqlite3_bind_text)GetProcAddress(m_hModule, "sqlite3_bind_text");
		if ( m_pfn_ksqlite3_bind_text == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_zeroblob = (type_ksqlite3_bind_zeroblob)GetProcAddress(m_hModule, "sqlite3_bind_zeroblob");
		if ( m_pfn_ksqlite3_bind_zeroblob == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_parameter_count = (type_ksqlite3_bind_parameter_count)GetProcAddress(m_hModule, "sqlite3_bind_parameter_count");
		if ( m_pfn_ksqlite3_bind_parameter_count == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_bind_parameter_index = (type_ksqlite3_bind_parameter_index)GetProcAddress(m_hModule, "sqlite3_bind_parameter_index");
		if ( m_pfn_ksqlite3_bind_parameter_index == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_name = (type_ksqlite3_column_name)GetProcAddress(m_hModule, "sqlite3_column_name");
		if ( m_pfn_ksqlite3_column_name == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_name16 = (type_ksqlite3_column_name16)GetProcAddress(m_hModule, "sqlite3_column_name16");
		if ( m_pfn_ksqlite3_column_name16 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_decltype16 = (type_ksqlite3_column_decltype16)GetProcAddress(m_hModule, "sqlite3_column_decltype16");
		if ( m_pfn_ksqlite3_column_decltype16 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_bytes16 = (type_ksqlite3_column_bytes16)GetProcAddress(m_hModule, "sqlite3_column_bytes16");
		if ( m_pfn_ksqlite3_column_bytes16 == NULL )
			return E_NOTIMPL;

		m_pfn_ksqlite3_column_text16 = (type_ksqlite3_column_text16)GetProcAddress(m_hModule, "sqlite3_column_text16");
		if ( m_pfn_ksqlite3_column_text16 == NULL )
			return E_NOTIMPL;


		return 0;
	}

	void ClearFunction()
	{
		m_pfn_ksqlite3_open = NULL;
		m_pfn_ksqlite3_key = NULL;
		m_pfn_ksqlite3_busy_handler = NULL;
		m_pfn_ksqlite3_close = NULL;
		m_pfn_ksqlite3_exec = NULL;
		m_pfn_ksqlite3_prepare = NULL;
		m_pfn_ksqlite3_column_count = NULL;
		m_pfn_ksqlite3_step = NULL;
		m_pfn_ksqlite3_column_text = NULL;
		m_pfn_ksqlite3_finalize = NULL;
		m_pfn_ksqlite3_vmprintf = NULL;
		m_pfn_ksqlite3_free = NULL;
		m_pfn_ksqlite3_interrupt = NULL;

		m_pfn_ksqlite3_open_v2 = NULL;
		m_pfn_ksqlite3_prepare_v2 = NULL;
		m_pfn_ksqlite3_column_int = NULL;
		m_pfn_ksqlite3_column_bytes = NULL;
		m_pfn_ksqlite3_column_blob = NULL;
		m_pfn_ksqlite3_bind_int64 = NULL;
		m_pfn_ksqlite3_bind_int = NULL;
		m_pfn_ksqlite3_bind_blob = NULL;
		m_pfn_ksqlite3_reset = NULL;

		m_pfn_ksqlite3_column_int64 = NULL;

		m_pfn_ksqlite3_bind_null = NULL;
		m_pfn_ksqlite3_column_type = NULL;
		m_pfn_ksqlite3_open16 = NULL;
		m_pfn_ksqlite3_rekey = NULL;

		// 2011.01.24
		m_pfn_ksqlite3_extended_result_codes = NULL;
		m_pfn_ksqlite3_get_autocommit = NULL;
		m_pfn_ksqlite3_last_insert_rowid = NULL;
		m_pfn_ksqlite3_busy_timeout = NULL;
		m_pfn_ksqlite3_errmsg16 = NULL;
		m_pfn_ksqlite3_errcode = NULL;
		m_pfn_ksqlite3_prepare16_v2 = NULL;
		m_pfn_ksqlite3_changes = NULL;
		m_pfn_ksqlite3_bind_text16 = NULL;
		m_pfn_ksqlite3_bind_text = NULL;
		m_pfn_ksqlite3_bind_zeroblob = NULL;
		m_pfn_ksqlite3_bind_parameter_count = NULL;
		m_pfn_ksqlite3_bind_parameter_index = NULL;
		m_pfn_ksqlite3_column_name = NULL;
		m_pfn_ksqlite3_column_name16 = NULL;
		m_pfn_ksqlite3_column_decltype16 = NULL;
		m_pfn_ksqlite3_column_bytes16 = NULL;
		m_pfn_ksqlite3_column_text16 = NULL;
	}


private:
	HMODULE m_hModule;
	bool m_bSelfLoaded;
	KSYNMutex m_mutex;

	type_ksqlite3_open	m_pfn_ksqlite3_open;
	type_ksqlite3_key	m_pfn_ksqlite3_key;
	type_ksqlite3_busy_handler	m_pfn_ksqlite3_busy_handler;
	type_ksqlite3_close	m_pfn_ksqlite3_close;
	type_ksqlite3_exec	m_pfn_ksqlite3_exec;
	type_ksqlite3_prepare	m_pfn_ksqlite3_prepare;
	type_ksqlite3_column_count	m_pfn_ksqlite3_column_count;
	type_ksqlite3_step m_pfn_ksqlite3_step;
	type_ksqlite3_column_text	m_pfn_ksqlite3_column_text;
	type_ksqlite3_finalize	m_pfn_ksqlite3_finalize;
	type_ksqlite3_vmprintf	m_pfn_ksqlite3_vmprintf;
	type_ksqlite3_free m_pfn_ksqlite3_free;
	type_ksqlite3_interrupt m_pfn_ksqlite3_interrupt;

	//////////////////////////////////////////////////////////////////////////
	//后续2010.5.11补充如下导出函数
	type_ksqlite3_open_v2 m_pfn_ksqlite3_open_v2;
	type_ksqlite3_prepare_v2 m_pfn_ksqlite3_prepare_v2;
	type_ksqlite3_column_int m_pfn_ksqlite3_column_int;
	type_ksqlite3_column_bytes	m_pfn_ksqlite3_column_bytes;
	type_ksqlite3_column_blob m_pfn_ksqlite3_column_blob;
	type_ksqlite3_bind_int64  m_pfn_ksqlite3_bind_int64;
	type_ksqlite3_bind_int  m_pfn_ksqlite3_bind_int;
	type_ksqlite3_bind_blob m_pfn_ksqlite3_bind_blob;
	type_ksqlite3_reset m_pfn_ksqlite3_reset;

	//////////////////////////////////////////////////////////////////////////
	//后续2010.6.02 补充如下导出函数
	type_ksqlite3_column_int64 m_pfn_ksqlite3_column_int64;

	//////////////////////////////////////////////////////////////////////////
	// 2010.09.07
	type_ksqlite3_bind_null m_pfn_ksqlite3_bind_null;
	type_ksqlite3_column_type	m_pfn_ksqlite3_column_type;
	type_ksqlite3_open16		m_pfn_ksqlite3_open16;
	type_ksqlite3_rekey			m_pfn_ksqlite3_rekey;

	//////////////////////////////////////////////////////////////////////////
	// 2011.01.24
	type_ksqlite3_extended_result_codes m_pfn_ksqlite3_extended_result_codes;
	type_ksqlite3_get_autocommit m_pfn_ksqlite3_get_autocommit;
	type_ksqlite3_last_insert_rowid m_pfn_ksqlite3_last_insert_rowid;
	type_ksqlite3_busy_timeout m_pfn_ksqlite3_busy_timeout;
	type_ksqlite3_errmsg16 m_pfn_ksqlite3_errmsg16;
	type_ksqlite3_errcode m_pfn_ksqlite3_errcode;
	type_ksqlite3_prepare16_v2 m_pfn_ksqlite3_prepare16_v2;
	type_ksqlite3_changes m_pfn_ksqlite3_changes;
	type_ksqlite3_bind_text16 m_pfn_ksqlite3_bind_text16;
	type_ksqlite3_bind_text m_pfn_ksqlite3_bind_text;
	type_ksqlite3_bind_zeroblob m_pfn_ksqlite3_bind_zeroblob;
	type_ksqlite3_bind_parameter_count m_pfn_ksqlite3_bind_parameter_count;
	type_ksqlite3_bind_parameter_index m_pfn_ksqlite3_bind_parameter_index;
	type_ksqlite3_column_name m_pfn_ksqlite3_column_name;
	type_ksqlite3_column_name16 m_pfn_ksqlite3_column_name16;
	type_ksqlite3_column_decltype16 m_pfn_ksqlite3_column_decltype16;
	type_ksqlite3_column_bytes16 m_pfn_ksqlite3_column_bytes16;
	type_ksqlite3_column_text16 m_pfn_ksqlite3_column_text16;

};

typedef SingletonHolder<KSqliteDynamicLoader> KSqliteDynamicLoaderSingleton;
