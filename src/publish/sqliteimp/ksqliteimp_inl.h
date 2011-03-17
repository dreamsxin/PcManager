////////////////////////////////////////////////////////////////
//
//	Filename: 	ksqliteimp_inl.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-8-11  9: 47
//	Comment:	
//
///////////////////////////////////////////////////////////////

#include <atlstr.h>
#include "ksqliteimp.h"
#include "ksqliteresult.h"
#include "ksqlitestring.h"
#include <algorithm>
#pragma once

class KSQLiteSecurityAttribute
{
public:
	KSQLiteSecurityAttribute()
	{
		InitializeSecurityDescriptor(&m_sd, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&m_sd, TRUE, NULL, FALSE);
		m_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		m_sa.bInheritHandle = FALSE;
		m_sa.lpSecurityDescriptor = &m_sd;	
	}

	~KSQLiteSecurityAttribute()
	{		
	}

	PSECURITY_ATTRIBUTES operator&() 
	{
		return &m_sa;
	}
private:
	SECURITY_ATTRIBUTES		m_sa;
	SECURITY_DESCRIPTOR		m_sd;
};


typedef KSqliteResult<KSqliteString>  TSqliteResult;

#define LOCK_NAME_PREFIX	L"{B5A8A931-8118-40ee-9E16-CC6B335F433B}_SQLIETE_"


template<class T>
int sqliteBusyCallback(void *ptr,int count)
{
	KSqliteImp<T>* pThis =(KSqliteImp<T>*)ptr;

	if ( pThis->IsBusyTimeOut(count))
		return 0;
	else
		return 1;
}


template<class LOCK_TYPE>
KSqliteImp<LOCK_TYPE>::KSqliteImp(void):m_db(NULL),m_bIsInterrupt(false),m_uWaitTimeOut(-1)
{
	m_waitEvent.Init(true, false);
}

template<class LOCK_TYPE>
KSqliteImp<LOCK_TYPE>::~KSqliteImp(void)
{
	Uninit();
	m_waitEvent.UnInit();
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::Init(const char *szDbName, unsigned int uWaitTimeout /* = -1 */, const char *szDbKey /* = NULL  */)
{
	int	nRetCode = 0;
	int nRet = -1;
	std::wstring strLockMutexName;

	if ( szDbName == NULL)
		return E_INVALIDARG;

	Uninit();

	nRetCode = sqlite3_open(szDbName, &m_db);
	if (nRetCode) 
		goto Exit0;

	nRet = -1;
	if (m_db == NULL)
		goto Exit0;

	nRet = 0;
	if (szDbKey)
	{
		nRetCode = sqlite3_key(m_db, szDbKey, (int)strlen(szDbKey));
		if (nRetCode) 
			goto Exit0;
	}

	//如果需要设置超时
	//sqlite3_busy_timeout(m_db, 100000);
	if (uWaitTimeout)
	{
		nRet = sqlite3_busy_handler(m_db, sqliteBusyCallback<LOCK_TYPE>, this);
		if (nRet)
			goto Exit0;
	}

	m_uWaitTimeOut = uWaitTimeout;

	GetDbMutexName(strLockMutexName, szDbName);
	{
		KSQLiteSecurityAttribute sa;
		nRetCode = m_lockMutex.Init(strLockMutexName.c_str(), &sa);
	}


Exit0:
	if (nRetCode)
		Uninit();

	return nRetCode;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::Uninit()
{
	if ( m_db)
	{
		sqlite3_close(m_db);
		m_db = NULL;
	}

	m_lockMutex.UnInit();

	return 0;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::Execute(const char* sql, char **ppszError /* = NULL */)
{
	if (!TryBeginTransac())
		return SQLITE_BUSY;

	int nRet = ExecuteInTransac(sql, ppszError);

	m_lockMutex.UnLock();

	return nRet;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::Query(const char* sql, IKSqliteResult** ppv)
{
	if (!TryBeginTransac())
		return SQLITE_BUSY;

	int nRet = QueryInTransac(sql, ppv);

	m_lockMutex.UnLock();

	return nRet;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::BeginTransac(char **ppszError /* = NULL */)
{
	if (!TryBeginTransac())
		return SQLITE_BUSY;

	int nRet =  ExecuteInTransac("begin transaction;", ppszError);
	if (nRet)
		m_lockMutex.UnLock();

	return nRet;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::CommitTransac(char **ppszError /* = NULL */)
{
	int nRet = ExecuteInTransac("commit transaction;",ppszError);
	
	m_lockMutex.UnLock();

	return nRet;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::RollbackTransac(char **ppszError /* = NULL */)
{
	int nRet = ExecuteInTransac("rollback transaction;",ppszError);

	m_lockMutex.UnLock();

	return nRet;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::ExecuteInTransac(const char* sql, char **ppszError /* = NULL */)
{
	if(m_bIsInterrupt)
	{
		*ppszError = sqlite3_mprintf("db is closed");
		return SQLITE_INTERRUPT;
	}

	if (m_db == NULL || sql == NULL)
		return E_POINTER;

	int nRet = sqlite3_exec(m_db, sql, 0 , 0 , ppszError);

	if (nRet == SQLITE_OK && ppszError && *ppszError)
		FreeData(*ppszError);

	if (nRet != SQLITE_OK && ppszError && *ppszError == NULL)
		*ppszError = sqlite3_mprintf(errorCodeAsString(nRet));

	return nRet;
}

template<class LOCK_TYPE>
int KSqliteImp<LOCK_TYPE>::QueryInTransac(const char* sql, IKSqliteResult** ppv)
{
	int	nRetCode = -1;
	sqlite3_stmt *stmt	= NULL;
	TSqliteResult *pRes	= NULL;

	if(m_bIsInterrupt)
		return SQLITE_INTERRUPT;

	if ( ppv == NULL || m_db == NULL || sql == NULL)
		return E_POINTER;

	nRetCode = sqlite3_prepare( m_db,sql,-1,&stmt, NULL);
	if ( nRetCode )
		goto Exit0;

	pRes = new TSqliteResult;
	nRetCode = pRes->Init( stmt);
	if ( nRetCode != 0)
	{
		delete pRes;
		goto Exit0;
	}

	*ppv = pRes;

Exit0:
	if ( stmt)
		sqlite3_finalize( stmt);

	return nRetCode;
}

template<class LOCK_TYPE>
const char* KSqliteImp<LOCK_TYPE>::EscapeSqlString(const char *sql,...)
{
	va_list arglist;
	va_start(arglist, sql); 
	return sqlite3_vmprintf(sql, arglist);	
}

template<class LOCK_TYPE>
const char* KSqliteImp<LOCK_TYPE>::VEscapeSqlString(const char *sql, va_list arglist)
{
	return sqlite3_vmprintf(sql, arglist);	
}

template<class LOCK_TYPE>
void KSqliteImp<LOCK_TYPE>::FreeData(const void *pData)
{
	if(pData)
		sqlite3_free((void *)pData);
}

template<class LOCK_TYPE>
void KSqliteImp<LOCK_TYPE>::NotifyInterrupt()
{
	m_waitEvent.Signal();
	if (m_db)
	{
		sqlite3_interrupt(m_db);
		m_bIsInterrupt = true;
	}
}

template<class LOCK_TYPE>
void KSqliteImp<LOCK_TYPE>::NotifyResume()
{
	m_waitEvent.Reset();
	m_bIsInterrupt = false;
}

template<class LOCK_TYPE>
bool KSqliteImp<LOCK_TYPE>::IsBusyTimeOut(int nCount)
{
	static const unsigned int delays[] =
	{ 1, 2, 5, 10, 15, 20, 25, 25,  25,  50,  50, 100 };
	static const unsigned int totals[] =
	{ 0, 1, 3,  8, 18, 33, 53, 78, 103, 128, 178, 228 };

	const int nSize = (sizeof(delays)/sizeof(delays[0]));
	unsigned int delay, total;

	if( nCount < nSize )
	{
		delay = delays[nCount];
		total = totals[nCount];
	}
	else
	{
		delay = delays[nSize-1];
		total = totals[nSize-1] + delay*(nCount-(nSize-1));
	}

	if(m_uWaitTimeOut != -1 && total + delay > m_uWaitTimeOut)
	{
		delay = m_uWaitTimeOut - total;
		if(delay <= 0) 
			return true;
	}

	if ( m_waitEvent.Wait(delay) == WAIT_OBJECT_0)
		return true;
	else
		return false;
}

template<class LOCK_TYPE>
bool KSqliteImp<LOCK_TYPE>::TryBeginTransac()
{
	int i = 0;

	int nCount = 0;
	while (!IsBusyTimeOut(nCount++))
	{
		if (!m_lockMutex.TryLock())
			continue;

		return true;
	}

	return false;
}

template<class LOCK_TYPE>
const char* KSqliteImp<LOCK_TYPE>::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
	case SQLITE_OK          : return "SQLITE_OK";
	case SQLITE_ERROR       : return "SQLITE_ERROR";
	case SQLITE_INTERNAL    : return "SQLITE_INTERNAL";
	case SQLITE_PERM        : return "SQLITE_PERM";
	case SQLITE_ABORT       : return "SQLITE_ABORT";
	case SQLITE_BUSY        : return "SQLITE_BUSY";
	case SQLITE_LOCKED      : return "SQLITE_LOCKED";
	case SQLITE_NOMEM       : return "SQLITE_NOMEM";
	case SQLITE_READONLY    : return "SQLITE_READONLY";
	case SQLITE_INTERRUPT   : return "SQLITE_INTERRUPT";
	case SQLITE_IOERR       : return "SQLITE_IOERR";
	case SQLITE_CORRUPT     : return "SQLITE_CORRUPT";
	case SQLITE_NOTFOUND    : return "SQLITE_NOTFOUND";
	case SQLITE_FULL        : return "SQLITE_FULL";
	case SQLITE_CANTOPEN    : return "SQLITE_CANTOPEN";
	case SQLITE_PROTOCOL    : return "SQLITE_PROTOCOL";
	case SQLITE_EMPTY       : return "SQLITE_EMPTY";
	case SQLITE_SCHEMA      : return "SQLITE_SCHEMA";
	case SQLITE_TOOBIG      : return "SQLITE_TOOBIG";
	case SQLITE_CONSTRAINT  : return "SQLITE_CONSTRAINT";
	case SQLITE_MISMATCH    : return "SQLITE_MISMATCH";
	case SQLITE_MISUSE      : return "SQLITE_MISUSE";
	case SQLITE_NOLFS       : return "SQLITE_NOLFS";
	case SQLITE_AUTH        : return "SQLITE_AUTH";
	case SQLITE_FORMAT      : return "SQLITE_FORMAT";
	case SQLITE_RANGE       : return "SQLITE_RANGE";
	case SQLITE_ROW         : return "SQLITE_ROW";
	case SQLITE_DONE        : return "SQLITE_DONE";
	default: return "UNKNOWN_ERROR";
	}
}

template<class LOCK_TYPE>
void KSqliteImp<LOCK_TYPE>::GetDbMutexName(std::wstring & strMutexName, const char* pszDbFileName)
{
	std::wstring strDbName =  CA2W(pszDbFileName, CP_UTF8);
	std::replace(strDbName.begin(), strDbName.end(), '\\', '_');
	GetKernelMutexName(strMutexName, LOCK_NAME_PREFIX);
	strMutexName += strDbName;
}
