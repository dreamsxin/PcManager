/**
* @file    bkdb.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-15 16:51
*/

#ifndef BKDB_H
#define BKDB_H

#include <unknwn.h>
#include "bkdbdef.h"

NS_SKYLARK_BEGIN

namespace BKDb
{
    /// 事务的锁方式
    enum EM_TRANSLOCK
    {
        em_Deferred     = 1,    ///< 延迟锁, 按需锁表
        em_Immediate    = 2,    ///< 意向锁, 锁数据库, 其他连接可以读数据库但不可写数据库, 也不能加意向锁
        em_Exclusive    = 3,    ///< 排他锁, 锁数据库, 其他连接无法操作数据库
    };
}

/// 数据库查询结果集
class __declspec(uuid("288C088E-461A-4212-B6C5-73A69AE5EE24"))
ISQLiteComResultSet3: public IUnknown
{
public:
    /// 获取列的数目
    virtual int         STDMETHODCALLTYPE GetColumnCount() = 0;

    /// 获取列索引
    virtual int         STDMETHODCALLTYPE FindColumnIndex(LPCWSTR columnName) = 0;

    /// 获取列名
    virtual LPCWSTR     STDMETHODCALLTYPE GetColumnName(int columnIndex) = 0;


    /// 获取列声明的类型
    virtual LPCWSTR     STDMETHODCALLTYPE GetDeclaredColumnType(int columnIndex) = 0;

    /// 获取列类型的值
    virtual int         STDMETHODCALLTYPE GetColumnType(int columnIndex) = 0;




    virtual LPCWSTR     STDMETHODCALLTYPE GetAsString(int columnIndex, LPCWSTR nullValue = L"") = 0;    ///< 获取指定列的值(字符串)
    virtual LPCWSTR     STDMETHODCALLTYPE GetAsString(LPCWSTR columnName, LPCWSTR nullValue = L"") = 0; ///< 获取指定列的值(字符串)

    virtual int         STDMETHODCALLTYPE GetInt(int columnIndex, int nullValue = 0) = 0;               ///< 获取指定列的值(int)
    virtual int         STDMETHODCALLTYPE GetInt(LPCWSTR columnName, int nullValue = 0) = 0;            ///< 获取列类型的值(int)

    virtual ULONGLONG   STDMETHODCALLTYPE GetInt64(int columnIndex, ULONGLONG nullValue = 0) = 0;       ///< 获取指定列的值(int64)
    virtual ULONGLONG   STDMETHODCALLTYPE GetInt64(LPCWSTR columnName, ULONGLONG nullValue = 0) = 0;    ///< 获取指定列的值(int64)

    virtual int         STDMETHODCALLTYPE GetBlob(int columnIndex, char* pBlog, int nBlogBytes) = 0;    ///< 获取指定列的值(blob)
    virtual int         STDMETHODCALLTYPE GetBlob(LPCWSTR columnName, char* pBlog, int nBlogBytes) = 0; ///< 获取指定列的值(blob)

    virtual int         STDMETHODCALLTYPE GetBlobBytes(int columnIndex) = 0;                            ///< 获取指定列的blob长度(字节数)
    virtual int         STDMETHODCALLTYPE GetBlobBytes(LPCWSTR columnName) = 0;                         ///< 获取指定列的blob长度(字节数)

    virtual BOOL        STDMETHODCALLTYPE IsNull(int columnIndex) = 0;      ///< 判断指定列是否为NULL
    virtual BOOL        STDMETHODCALLTYPE IsNull(LPCWSTR columnName) = 0;   ///< 判断指定列是否为NULL




    /// 判断结果集的指针是否已到末尾(最后一条记录的后一个位置)
    virtual BOOL        STDMETHODCALLTYPE IsEof() = 0;



    /// 取结果集中下一条记录
    virtual BOOL        STDMETHODCALLTYPE NextRow() = 0;
};



/// 数据库预编译语句
class __declspec(uuid("2A32BD36-8C49-478e-85D0-00386DB0C81D"))
ISQLiteComStatement3: public IUnknown
{
public:
    /**
    * @brief    执行预编译语句
    * @param    pnRows  返回影响的行数目
    */ 
    virtual HRESULT     STDMETHODCALLTYPE ExecuteUpdate(int* pnRows = NULL) = 0;

    /// 执行预编译查询
    virtual HRESULT     STDMETHODCALLTYPE ExecuteQuery(ISQLiteComResultSet3** ppiResultSet) = 0;

    /// 执行预编译标量查询(select count(*) from ...)
    virtual int         STDMETHODCALLTYPE ExecuteScalar() = 0;



    virtual HRESULT     STDMETHODCALLTYPE Bind(int paramIndex, LPCWSTR szValue) = 0;        ///< 绑定参数(字符串)
    virtual HRESULT     STDMETHODCALLTYPE Bind(int paramIndex, int intValue) = 0;           ///< 绑定参数(int)
    virtual HRESULT     STDMETHODCALLTYPE Bind(int paramIndex, ULONGLONG int64Value) = 0;   ///< 绑定参数(int64)
    /// 绑定参数(ANSI或ANSI兼容的字符串,约定为UTF-8编码)
    virtual HRESULT     STDMETHODCALLTYPE Bind(int paramIndex, LPCSTR szValue) = 0;         
    virtual HRESULT     STDMETHODCALLTYPE BindBlob(int paramIndex, const char* blobValue, int blobLen) = 0; ///< 绑定参数(Blob
    virtual HRESULT     STDMETHODCALLTYPE BindNull(int paramIndex) = 0;                     ///< 绑定参数(NULL)
    virtual HRESULT     STDMETHODCALLTYPE BindZeroBlob(int paramIndex, int blobSize) = 0;   ///< 绑定全0的blob

    /// 获取参数个数
    /// 获取参数个数
    virtual int         STDMETHODCALLTYPE GetParamCount() = 0;

    /// 根据参数名获取参数索引,如果没有找到,会返回-1
    virtual int         STDMETHODCALLTYPE GetParamIndex(LPCWSTR szParam) = 0;

    /// 重置预编译语句,以便再次执行
    virtual void        STDMETHODCALLTYPE Reset() = 0;
};



/// 数据库连接对象
class __declspec(uuid("DAD0CB8C-D2A8-4e1b-8C3D-54AC236172FD"))
ISQLiteComDatabase3: public IUnknown
{
public:

    /**
    * @brief    以加密方式打开数据库
    * @param    pszFile         数据库文件的绝对路径
    * @param    pPassword       口令(二进制数据)
    * @param    dwPasswordBytes 口令长度(字节数)
    */ 
    virtual HRESULT     STDMETHODCALLTYPE OpenWithKey(
        LPCWSTR pszFile,
        void*   pPassword,
        DWORD   dwPasswordBytes) = 0;

    /**
    * @brief    以普通方式打开数据库
    * @param    pszFile         数据库文件的绝对路径
    */ 
    virtual HRESULT     STDMETHODCALLTYPE Open(LPCWSTR pszFile) = 0;

    /// 判断数据库是否是打开的状态
    virtual BOOL        STDMETHODCALLTYPE IsOpen() throw() = 0;

    /// 关闭数据库
    virtual void        STDMETHODCALLTYPE Close() throw() = 0;

    /**
    * @brief    开启一个事务,事务不能嵌套
    * @param    transLock       事务的锁方式
    */ 
    virtual HRESULT     STDMETHODCALLTYPE BeginTransaction(BKDb::EM_TRANSLOCK transLock = BKDb::em_Deferred) = 0;

    /// 提交一个事务,事务不能嵌套
    virtual HRESULT     STDMETHODCALLTYPE CommitTransaction() = 0;

    /// 回滚一个事务,事务不能嵌套
    virtual HRESULT     STDMETHODCALLTYPE RollbackTransaction() throw() = 0;

    /// 判断是否是自动提交状态
    virtual BOOL        STDMETHODCALLTYPE GetAutoCommit() = 0;




    /**
    * @brief    创建一个保存点
    * @param    lpszPointName   保存点的名字在事务栈中应该是唯一的
    */ 
    virtual HRESULT     STDMETHODCALLTYPE CreateSavePoint(LPCWSTR lpszPointName) = 0;

    /**
    * @brief    释放(提交)一个保存点
    * @param    lpszPointName   保存点的名字在事务栈中应该是唯一的
    */ 
    virtual HRESULT     STDMETHODCALLTYPE ReleaseSavePoint(LPCWSTR lpszPointName) throw() = 0;

    /**
    * @brief    回滚一个保存点
    * @param    lpszPointName   保存点的名字在事务栈中应该是唯一的
    */ 
    virtual HRESULT     STDMETHODCALLTYPE RollbackToSavePoint(LPCWSTR lpszPointName) = 0;




    /**
    * @brief    以普通方式打开数据库
    * @param    szTable     表名
    */ 
    virtual BOOL        STDMETHODCALLTYPE TableExists(LPCWSTR szTable) = 0;


    /**
    * @brief    创建预编译语句
    * @param    szSQL           sql语句
    * @param    ppiStmt         返回创建的预编译语句接口
    */ 
    virtual HRESULT     STDMETHODCALLTYPE PrepareStatement(LPCWSTR szSQL, ISQLiteComStatement3** ppiStmt) = 0;

    /**
    * @brief    执行sql语句
    * @param    szSQL           sql语句
    * @param    pnRows          返回影响的行数目
    */ 
    virtual HRESULT     STDMETHODCALLTYPE ExecuteUpdate(LPCWSTR szSQL, int* pnRows = NULL) = 0;

    /**
    * @brief    执行sql查询
    * @param    szSQL           sql语句
    * @param    ppiResultSet    返回查询的结果集
    */ 
    virtual HRESULT     STDMETHODCALLTYPE ExecuteQuery(LPCWSTR szSQL, ISQLiteComResultSet3** ppiResultSet) = 0;

    /**
    * @brief    执行标量的sql查询(select count(*) from ...)
    * @param    szSQL           sql语句
    */ 
    virtual int         STDMETHODCALLTYPE ExecuteScalar(LPCWSTR szSQL) = 0;


    /// 返回最后插入的行id
    virtual ULONGLONG   STDMETHODCALLTYPE LastRowId() = 0;

    /// 中断正在执行的操作
    virtual void        STDMETHODCALLTYPE Interrupt() = 0;

    /// 设置查询超时的时间
    virtual void        STDMETHODCALLTYPE SetBusyTimeout(int nMillisecs) = 0;

    /// 返回sqlite的版本
    virtual LPCSTR      STDMETHODCALLTYPE SQLiteVersion() throw() = 0;

    /// 获取错误码
    virtual int         STDMETHODCALLTYPE GetErrorCode() = 0;

    /// 获取错误信息
    virtual LPCWSTR     STDMETHODCALLTYPE GetErrorMsg() = 0;
};

NS_SKYLARK_END

#define FN_BKDbInitialize       "BKDbInitialize"        ///< export BKDbInitialize
#define FN_BKDbUninitialize     "BKDbUninitialize"      ///< export BKDbUninitialize
#define FN_BKDbCreateObject     "BKDbCreateObject"      ///< export BKDbCreateObject


/**
* @brief    初始化BKDb模块
* @param    dwFlag  保留,未使用
*/
EXTERN_C HRESULT WINAPI BKDbInitialize(Skylark::BKDB_PLATFORM_INIT* pInit);
/// typedef for BKCacheInitialize
typedef HRESULT (WINAPI *PFN_BKDbInitialize)(Skylark::BKDB_PLATFORM_INIT* pInit);



/**
* @brief    反初始化BKDb模块
* @remark   调用反初始化前必须确保该模块导出的所有对象都被释放
*/
EXTERN_C HRESULT WINAPI BKDbUninitialize();
/// typedef for BKCacheUninitialize
typedef HRESULT (WINAPI *PFN_BKDbUninitialize)();



/// class Factory
EXTERN_C HRESULT WINAPI BKDbCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKDbCreateObject
typedef HRESULT (WINAPI *PFN_BKDbCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);

#endif//BKDB_H