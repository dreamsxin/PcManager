////////////////////////////////////////////////////////////////
//
//	Filename: 	ksqlite_export_fun_def.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-8-15  14: 37
//	Comment:	
//
///////////////////////////////////////////////////////////////

#pragma once

typedef int (*type_ksqlite3_open)(
	const char *filename,
	sqlite3 **ppDb
	);

typedef int (*type_ksqlite3_open_v2)(const char *filename,
		sqlite3 **ppDb, 
		int flags,
		const char *zVfs
	);

typedef int (*type_ksqlite3_key)(
	sqlite3 *db,
	const void *pKey, 
	int nKey
	);

typedef int (*type_ksqlite3_busy_handler)(
	sqlite3* pDb, 
	int(*pbusy_callback_fun)(void*,int), 
	void* pParam
	);

typedef int (*type_ksqlite3_close)(
	sqlite3 *pDb
	);

typedef int (*type_ksqlite3_exec)(
	sqlite3* pDb,
	const char *sql,
	int (*callback)(void*,int,char**,char**),
	void *pParam,
	char **errmsg
	);

typedef int (*type_ksqlite3_prepare)(
	sqlite3 *db,
	const char *zSql,
	int nByte,
	sqlite3_stmt **ppStmt,
	const char **pzTail
	);

typedef int (*type_ksqlite3_prepare_v2)(
	sqlite3 *db,
	const char *zSql,
	int nByte,
	sqlite3_stmt **ppStmt,
	const char **pzTail
	);

typedef int (*type_ksqlite3_column_count)(
	sqlite3_stmt *pStmt
	);

typedef int (*type_ksqlite3_step)(
	sqlite3_stmt*pStmt
	);


typedef const unsigned char* (*type_ksqlite3_column_text)(
	sqlite3_stmt*,
	int iCol
	);

typedef int (*type_ksqlite3_column_int)(
	sqlite3_stmt*,
	int iCol
	);

typedef sqlite3_int64 (*type_ksqlite3_column_int64)(
	sqlite3_stmt*,
	int iCol
	);

typedef int (*type_ksqlite3_column_bytes)(
	sqlite3_stmt*,
	int iCol
	);

typedef const void* (*type_ksqlite3_column_blob)(
	sqlite3_stmt*,
	int iCol
	);

typedef int (*type_ksqlite3_finalize)(
	sqlite3_stmt *pStmt
	);

typedef char* (*type_ksqlite3_vmprintf)(
	const char* szFormat,
	va_list valist
	);

typedef void (*type_ksqlite3_free)(
	void* pData
	);

typedef void (*type_ksqlite3_interrupt)(
	sqlite3* pDb
	);


typedef int (*type_ksqlite3_bind_int64)(
    sqlite3_stmt*, 
	int, 
	sqlite3_int64
	);

typedef int (*type_ksqlite3_bind_int)(
    sqlite3_stmt*, 
	int,
	int n
	);

typedef int (*type_ksqlite3_bind_blob)(
    sqlite3_stmt*, 
	int, 
	const void*, 
	int n, 
	void(*)(void*)
	);

typedef int (*type_ksqlite3_reset)(
    sqlite3_stmt*
	);

typedef int (*type_ksqlite3_bind_null)(
	sqlite3_stmt*,
	int
	);

typedef int (*type_ksqlite3_column_type)(
	sqlite3_stmt*,
	int
	);

typedef int (*type_ksqlite3_open16)(
	const void *filename, 
	sqlite3 **ppDb
	);

typedef int (*type_ksqlite3_rekey)(
	sqlite3 *pdb, 
	const void *pKey, 
	int nKey
	);

/// 2011.01.24
typedef int (*type_ksqlite3_extended_result_codes)(
	sqlite3*, 
	int);

typedef int (*type_ksqlite3_get_autocommit)(
	sqlite3*);

typedef sqlite3_int64 (*type_ksqlite3_last_insert_rowid)(
	sqlite3*);

typedef int (*type_ksqlite3_busy_timeout)(
	sqlite3*, 
	int);

typedef const void * (*type_ksqlite3_errmsg16)(
	sqlite3*);

typedef int (*type_ksqlite3_errcode)(
	sqlite3*);

typedef int (*type_ksqlite3_prepare16_v2)(
	sqlite3 *, 
	const void *,
	int ,
	sqlite3_stmt **, 
	const void **);

typedef int (*type_ksqlite3_changes)(
	sqlite3*);

typedef int (*type_ksqlite3_bind_text16)(
	sqlite3_stmt*, 
	int, 
	const void*, 
	int, 
	void(*)(void*));

typedef int (*type_ksqlite3_bind_text)(
	sqlite3_stmt*, 
	int, 
	const char*, 
	int, 
	void(*)(void*));

typedef int (*type_ksqlite3_bind_zeroblob)(
	sqlite3_stmt*, 
	int, 
	int);

typedef int (*type_ksqlite3_bind_parameter_count)(
	sqlite3_stmt*);

typedef int (*type_ksqlite3_bind_parameter_index)(
	sqlite3_stmt*, 
	const char *);

typedef const char * (*type_ksqlite3_column_name)(
	sqlite3_stmt*, 
	int);

typedef const void * (*type_ksqlite3_column_name16)(
	sqlite3_stmt*, 
	int);

typedef const void * (*type_ksqlite3_column_decltype16)(
	sqlite3_stmt*,
	int);

typedef int (*type_ksqlite3_column_bytes16)(
	sqlite3_stmt*, 
	int);

typedef const void * (*type_ksqlite3_column_text16)(
	sqlite3_stmt*, 
	int);





