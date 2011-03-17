////////////////////////////////////////////////////////////////
//
//	Filename: 	ksqliteimp.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-7-29  19: 47
//	Comment:	
//
///////////////////////////////////////////////////////////////
#pragma once

#if defined(_USE_SQLITE_DLL)
#include "ksqlite3_import.h"
#else
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")
#endif

#include <string>
#include "kthreadsyn.h"

class IKSqliteResult
{
public:
	virtual size_t GetRowCount() = 0;
	virtual size_t GetColCount() = 0;

	virtual void   Release() = 0;

	virtual __int64			 GetInt64Value(size_t nRow, size_t nCol) = 0;
	virtual unsigned __int64 GetUnsignedInt64Value(size_t nRow, size_t nCol) = 0;
	virtual const char*		 GetStringValue(size_t nRow, size_t nCol) = 0;
};

template<class LOCK_TYPE>
class KSqliteImp
{
public:
	KSqliteImp(void);
	~KSqliteImp(void);

	/**
	* @brief	初始化数据库　
	* @param	[in]	szDbName 数据库名称
	* @param	[in]	szDbKey			数据库的密码
	* @return	0成功，其它为失败
	*/
	int Init(const char *szDbName,
		/*[in]*/ unsigned int	uWaitTimeout = -1,
		/*[in]*/ const char	*szDbKey = NULL
		);
	
	int Uninit();

	/**
	* @brief	执行一条SQL语句
	* @param	[in]	sql		 要执行的SQL语句
	* @param	[in]	pszError 发生错误时pszError指向错误信息的字符串,传NULL则忽略错误信息
	* @return	0成功，其它为失败
	* @remark	如果*ppszError不为空,需要调用FreeData来释放*ppszError指向的字符串
	*/
	int Execute(/*[in]*/ const char* sql,/*[in]*/ char **ppszError = NULL);
	
	/**
	* @brief	执行查询语句
	* @param	[in]	sql	要扫描的SQL语句
	* @param	[out]	ppv	返回的结束集接口
	* @return	0成功，其它为失败
	* @remark	如果成功,外部调用IKSqliteResult的Release来释放接口
	*/
	int Query(/*[in]*/ const char* sql,/*[in]*/ IKSqliteResult** ppv);

	/**
	* @brief	开始一个事务
	* @param	[in]	pszError 发生错误时pszError指向错误信息的字符串,传NULL则忽略错误信息
	* @return	0成功，其它为失败
	* @remark	如果*ppszError不为空,需要调用FreeData来释放*ppszError指向的字符串
	*			注意如果此时有其它线程也在
	*/
	int BeginTransac(/*[in]*/ char **ppszError = NULL);

	/**
	* @brief	执行一条事务中的SQL语句
	* @param	[in]	sql		 要执行的SQL语句
	* @param	[in]	pszError 发生错误时pszError指向错误信息的字符串,传NULL则忽略错误信息
	* @return	0成功，其它为失败
	* @remark	如果*ppszError不为空,需要调用FreeData来释放*ppszError指向的字符串
				
	*/
	int ExecuteInTransac(/*[in]*/ const char* sql, /*[in]*/ char **ppszError = NULL);

	/**
	* @brief	执行查询语句
	* @param	[in]	sql	要扫描的SQL语句
	* @param	[out]	ppv	返回的结束集接口
	* @return	0成功，其它为失败
	* @remark	如果成功,外部调用IKSqliteResult的Release来释放接口
				同时开始多个事务并且LOCK_TYPE没有实现互斥锁可能会返回数据库被锁定的错误信息
	*/
	int QueryInTransac(/*[in]*/ const char* sql,/*[in]*/ IKSqliteResult** ppv);

	/**
	* @brief	提交一个事务
	* @param	[in]	pszError 发生错误时pszError指向错误信息的字符串,传NULL则忽略错误信息
	* @return	0成功，其它为失败
	* @remark	如果*ppszError不为空,需要调用FreeData来释放*ppszError指向的字符串
	*			同时开始多个事务并且LOCK_TYPE没有实现互斥锁可能会返回数据库被锁定的错误信息
	*/
	int CommitTransac(/*[in]*/ char **ppszError = NULL);

	/**
	* @brief	回滚一个事务
	* @param	[in]	pszError 发生错误时pszError指向错误信息的字符串,传NULL则忽略错误信息
	* @return	0成功，其它为失败
	* @remark	如果*ppszError不为空,需要调用FreeData来释放*ppszError指向的字符串
	*			同时开始多个事务并且LOCK_TYPE没有实现互斥锁可能会返回数据库被锁定的错误信息
	*/
	int RollbackTransac(/*[in]*/ char **ppszError = NULL);

	/**
	* @brief	格式化一条Sql语句
	* @param	[in]	sql 要格式化的Sql格式化控制语句
	* @return	返回格式化后的Sql语句
	* @remark	返回的Sql语句要使用FreeData来释放
				同时开始多个事务并且LOCK_TYPE没有实现互斥锁可能会返回数据库被锁定的错误信息
	*/
	const char* EscapeSqlString(/*[in]*/ const char* sql,...);

	/**
	* @brief	格式化一条Sql语句
	* @param	[in]	sql		要格式化的Sql格式化控制语句
	* @param	[in]	arglist	要格式化的Sql语句的参数
	* @return	返回格式化后的Sql语句
	* @remark	返回的Sql语句要使用FreeData来释放
	*/
	const char* VEscapeSqlString(/*[in]*/ const char *sql,/*[in]*/ va_list arglist);

	/**
	* @brief	释放内部返回的内存
	* @param	[in] pData 要释放的内存
	* @remark	可以传NULL
	*/
	void FreeData(/*[in]*/ const void *pData);

	/**
	* @brief	中断正在执行的操作
	* @remark	由于多线程时执行顺序的不确定性
	*/
	void NotifyInterrupt();

	/**
	* @brief	恢复中断的状态
	* @remark	由于多线程时执行顺序的不确定性,必须显式来清除中断的状态,不能在反初始化时的另一个线程中调用此函数
	*/
	void NotifyResume();

private:
	template<class T>
	friend int sqliteBusyCallback(void *ptr,int count);

	bool IsBusyTimeOut(int nCount);

	bool TryBeginTransac();

	const char *errorCodeAsString(int nErrCode);

	void GetDbMutexName(std::wstring & strMutexName, const char* pszDbFileName);



protected:
	sqlite3*		m_db;
	bool			m_bIsInterrupt;
	LOCK_TYPE		m_lockMutex;
	KSYNEvent		m_waitEvent;
	unsigned int	m_uWaitTimeOut;
};

class KFakedMutex
{
public:
	int Init(const TCHAR* pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
	{return 0;}

	int UnInit()
	{return 0;}

	bool TryLock()
	{return true;}

	int UnLock()
	{return true;}
};

class _KSYNMutex: public KSYNMutex
{
public:
	int Init(const TCHAR* pszName = NULL, LPSECURITY_ATTRIBUTES psa = NULL)
	{
		return 0;
	}
};

typedef KSqliteImp<KFakedMutex>		KSqliteDbNoSyn;			///	不需要同步的SQLITE操作对象,当数据库被其它进程或线程占用时返回SQLITE_BUSY
typedef KSqliteImp<_KSYNMutex>		KSqliteDbThreadSyn;		///	仅仅需要线程同步的SQLITE操作对象
typedef KSqliteImp<KSYNKernelMutex>	KSqliteDbProcessSyn;	///	进程或跨线程多实例同步的SQLITE操作对象

#include "ksqliteimp_inl.h"