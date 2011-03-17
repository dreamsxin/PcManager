////////////////////////////////////////////////////////////////
//
//	Filename: 	ksqlite3_import.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-8-15  17: 12
//	Comment:	
//
///////////////////////////////////////////////////////////////

#pragma once
#include "sqlite3.h"
#include "ksqlite_loader.h"

inline int sqlite3_open(
				  const char *filename,
				  sqlite3 **ppDb
				  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_open(
		filename, 
		ppDb
		);
}

inline int sqlite3_open_v2(const char *filename, 
						   sqlite3 **ppDb, 
						   int flags,
						   const char *zVfs
						   )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_open_v2(
		filename, 
		ppDb,
		flags,
		zVfs
		);
}

inline int sqlite3_key(
				 sqlite3 *pdb,
				 const void *pKey, 
				 int nKey
				 )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_key(
		pdb, 
		pKey, 
		nKey
		);
}

inline int sqlite3_busy_handler(
						  sqlite3* pDb, 
						  int(*pbusy_callback_fun)(void*,int),
						  void* pParam
						  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_busy_handler(
		pDb,
		pbusy_callback_fun,
		pParam
		);
}

inline int sqlite3_close(
				   sqlite3 *pDb
				   )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_close(
		pDb
		);
}

inline int sqlite3_exec(
				  sqlite3* pDb,
				  const char *sql,
				  int (*callback)(void*,int,char**,char**),
				  void *pParam,
				  char **errmsg
				  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_exec(
		pDb,
		sql,
		callback,
		pParam,
		errmsg
		);
}

inline int sqlite3_prepare(
					 sqlite3 *pdb,
					 const char *zSql,
					 int nByte,
					 sqlite3_stmt **ppStmt,
					 const char **pzTail
					 )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_prepare(
		pdb,
		zSql,
		nByte,
		ppStmt,
		pzTail
		);
}

inline int sqlite3_prepare_v2(sqlite3 *pdb,
							  const char *zSql,
							  int nByte,
							  sqlite3_stmt **ppStmt,
							  const char **pzTail
							  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_prepare_v2(
		pdb,
		zSql,
		nByte,
		ppStmt,
		pzTail
		);
}


inline int sqlite3_column_count(
						  sqlite3_stmt *pStmt
						  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_count(
		pStmt
		);
}

inline int sqlite3_step(
				  sqlite3_stmt*pStmt
				  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_step(
		pStmt
		);
}


inline const unsigned char* sqlite3_column_text(
	sqlite3_stmt* pstmt,
	int iCol
	)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_text(
		pstmt,
		iCol
		);
}

inline int sqlite3_column_int(
    sqlite3_stmt* pstmt, 
	int iCol
	)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_int(
		pstmt,
		iCol
		);
}

inline sqlite3_int64 sqlite3_column_int64(
    sqlite3_stmt* pstmt, 
	int iCol
	)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_int64(
		pstmt,
		iCol
		);
}


inline int sqlite3_column_bytes(
    sqlite3_stmt* pstmt, 
    int iCol
	)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_bytes(
		pstmt,
		iCol
		);
}

inline const void* sqlite3_column_blob(
    sqlite3_stmt* pstmt,
    int iCol
	)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_blob(
		pstmt,
		iCol
		);
}

inline int sqlite3_finalize(
					  sqlite3_stmt *pStmt
					  )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_finalize(
		pStmt
		);
}

inline char* sqlite3_mprintf(
					   const char* szFormat,
					   ...
					   )
{
	va_list arglist;
	va_start(arglist, szFormat); 

	return  KSqliteDynamicLoaderSingleton::Instance().ksqlite3_vmprintf(
		szFormat,
		arglist
		);
}

inline char* sqlite3_vmprintf(
					   const char* szFormat,
					   va_list arglist
					   )
{
	return  KSqliteDynamicLoaderSingleton::Instance().ksqlite3_vmprintf(
		szFormat,
		arglist
		);
}

inline void sqlite3_free(
				   void* pData
				   )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_free(
		pData
		);
}

inline void sqlite3_interrupt(
						sqlite3* pDb
						)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_interrupt(
		pDb
		);
}

inline int sqlite3_bind_int64(sqlite3_stmt* pStmt, int nCol, sqlite3_int64 nValue)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_int64(pStmt,
		nCol,
		nValue
		);
}

inline int sqlite3_bind_int(sqlite3_stmt* pStmt, int nCol, int nValue)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_int(pStmt,
		nCol,
		nValue
		);
}

inline int sqlite3_bind_blob(sqlite3_stmt* pStmt, int nCol, const void* pv, int nlen, void(*pFun)(void*))
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_blob(pStmt,
		nCol,
		pv,
		nlen,
		pFun
		);
}

inline int sqlite3_reset(sqlite3_stmt *pStmt)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_reset(pStmt);
}


inline int sqlite3_bind_null(sqlite3_stmt *pStmt, int nIndex)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_null(pStmt, nIndex);
}


inline int sqlite3_column_type(sqlite3_stmt* stmt, int iCol)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_type(stmt, iCol);
}

inline int sqlite3_open16(const void *filename, sqlite3 **ppDb)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_open16(filename, ppDb);
}

inline int sqlite3_rekey(sqlite3 *pdb, const void *pKey, int nKey)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_rekey(pdb, pKey, nKey);
}

/// 2011.01.24
inline int sqlite3_extended_result_codes(
	sqlite3 *pdb
	, int onoff)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_extended_result_codes(pdb, onoff);
}

inline int sqlite3_get_autocommit(
	sqlite3 *pdb)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_get_autocommit(pdb);
}

inline sqlite3_int64 sqlite3_last_insert_rowid(
	sqlite3 *pdb)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_last_insert_rowid(pdb);
}

inline int sqlite3_busy_timeout(
	sqlite3 *pdb 
	, int ms)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_busy_timeout(pdb, ms);
}

inline const void *sqlite3_errmsg16(
	sqlite3 *pdb)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_errmsg16(pdb);
}

inline int sqlite3_errcode(
	sqlite3 *pdb)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_errcode(pdb);
}

inline int sqlite3_prepare16_v2(
	sqlite3 *pdb 
	, const void *zSql
	, int nByte 
	, sqlite3_stmt **ppStmt 
	, const void **pzTail)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_prepare16_v2(pdb, zSql, nByte, ppStmt, pzTail);
}

inline int sqlite3_changes(
	sqlite3 *pdb)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_changes(pdb);
}

inline int sqlite3_bind_text16(
	sqlite3_stmt *ppStmt 
	, int nInt1 
	, const void* pVoid
	, int nint2 
	, void(* pFunc)(void*) )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_text16(ppStmt, nInt1 , pVoid, nint2 , pFunc);
}

inline int sqlite3_bind_text(
	sqlite3_stmt *ppStmt 
	, int nInt1 
	, const char *pChar
	, int nint2 
	, void(* pFunc)(void*) )
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_text(ppStmt, nInt1 , pChar, nint2 , pFunc);
}

inline int sqlite3_bind_zeroblob(
	sqlite3_stmt *ppStmt 
	, int nlen
	, int n)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_zeroblob(ppStmt, nlen, n);
}

inline int sqlite3_bind_parameter_count(
	sqlite3_stmt *ppStmt)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_parameter_count(ppStmt);
}

inline int sqlite3_bind_parameter_index(
	sqlite3_stmt *ppStmt
	, const char *zName)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_bind_parameter_index(ppStmt, zName);
}

inline const char *sqlite3_column_name(
	sqlite3_stmt *ppStmt
	, int N)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_name(ppStmt, N);
}

inline const void *sqlite3_column_name16(
	sqlite3_stmt *ppStmt
	, int N)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_name16(ppStmt, N);
}

inline const void *sqlite3_column_decltype16(
	sqlite3_stmt *ppStmt
	, int N)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_decltype16(ppStmt, N);
}

inline int sqlite3_column_bytes16(
	sqlite3_stmt *ppStmt
	, int iCol)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_bytes16(ppStmt, iCol);
}

inline const void *sqlite3_column_text16(
	sqlite3_stmt *ppStmt
	, int iCol)
{
	return KSqliteDynamicLoaderSingleton::Instance().ksqlite3_column_text16(ppStmt, iCol);
}