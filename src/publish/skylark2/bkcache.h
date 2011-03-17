/**
* @file    bkcache.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-16 17:37
*/

#ifndef BKCACHE_H
#define BKCACHE_H

#include <unknwn.h>
#include "skylarkbase.h"
#include "bkdb.h"
#include "bkcachedef.h"

NS_SKYLARK_BEGIN



/**
* @brief    缓存 path -> hash
* @remark   nidx(<path>, md5, crc32, timestamp, tag)
*/
class __declspec(uuid("FEB9A481-88A5-4eef-82C9-3B16202F97C5"))
IBKCachePathToHash: public IUnknown
{
public:

    /** 
    * @brief    打开标准的缓存
    * @param    dwStdCache  要打开的缓存id,取值为
    *           BKCACHE_STD_PATH_TO_HASH
    */
    virtual HRESULT STDMETHODCALLTYPE OpenStandard(DWORD dwStdCache) = 0;


    /** 
    * @brief    打开自定义的缓存
    * @param    lpCacheFile 缓存的路径,如果为NULL,则取默认位置的缓存
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(LPCWSTR lpCacheFile = NULL) = 0;


    /**
    * @brief        缓存查找
    * @param[in]    lpszFilePath    文件路径
    * @param[out]   pRecord          返回的cache信息
    */ 
    virtual HRESULT STDMETHODCALLTYPE Lookup(
        LPCWSTR                 lpszFilePath,
        BKCACHE_PATH_TO_HASH*   pRecord) = 0;


    /**
    * @brief        更新缓存
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    pRecord         返回的cache信息
    * @remark       如果新记录的时间戳较老,原有记录不会被覆盖
    */ 
    virtual HRESULT STDMETHODCALLTYPE Update(
        LPCWSTR                     lpszFilePath,
        const BKCACHE_PATH_TO_HASH* pRecord) = 0;


    /**
    * @brief        删除缓存
    * @param[in]    lpszFilePath    文件路径
    */
    virtual HRESULT STDMETHODCALLTYPE Delete(LPCWSTR lpszFilePath) = 0;


    /**
    * @brief        删除过期的缓存(保留,暂未实现)
    */
    virtual HRESULT STDMETHODCALLTYPE DeleteExpired() = 0;


    /**
    * @brief        将内存中记录同步到数据库文件,耗时的操作
    */
    virtual HRESULT STDMETHODCALLTYPE Flush() = 0;




    /// 获取设置
    virtual HRESULT STDMETHODCALLTYPE GetSettings(BKCACHE_SETTINGS_PATH_TO_HASH* pSettings) = 0;

    /// 修改设置,如果为NULL,会恢复默认设置
    virtual HRESULT STDMETHODCALLTYPE SetSettings(const BKCACHE_SETTINGS_PATH_TO_HASH* pSettings) = 0;



    /// 开启事务
    virtual HRESULT STDMETHODCALLTYPE BeginTransaction(BKDb::EM_TRANSLOCK transLock = BKDb::em_Deferred) = 0;

    /// 提交事务
    virtual HRESULT STDMETHODCALLTYPE CommitTransaction() = 0;

    /// 回滚事务
    virtual HRESULT STDMETHODCALLTYPE RollbackTransaction() = 0;

    /// 检查当前对象是否在事务中
    virtual BOOL    STDMETHODCALLTYPE IsInTransaction() = 0;

    /// 获取当前时间
    virtual ULONGLONG STDMETHODCALLTYPE GetCurrentTimeAsTimeStamp() = 0;

};







/**
* @brief    缓存 hash -> level
* @remark   nidx(<path>, md5, crc32, timestamp, tag)
*/
class __declspec(uuid("9AF3DB0D-A098-47e1-A1D8-7FF4AD675A48"))
IBKCacheHashToLevel: public IUnknown
{
public:

    /** 
    * @brief    打开标准的缓存
    * @param    dwStdCache  要打开的缓存id,取值为
    *           BKCACHE_STD_HASH_TO_LEVEL
    */
    virtual HRESULT STDMETHODCALLTYPE OpenStandard(DWORD dwStdCache) = 0;


    /** 
    * @brief    打开自定义缓存
    * @param    lpCacheFile 缓存的路径,如果为NULL,则取默认位置的缓存
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(LPCWSTR lpCacheFile = NULL) = 0;


    /**
    * @brief        缓存查找
    * @param[in]    byMD5       md5
    * @param[in]    uCRC32      crc32
    * @param[out]   pRecord     返回的cache信息
    */ 
    virtual HRESULT STDMETHODCALLTYPE Lookup(
        const BYTE              byMD5[SKYLARK_MD5_BYTES],
        UINT32                  uCRC32,
        BKCACHE_HASH_TO_LEVEL*  pRecord) = 0;


    /**
    * @brief        更新缓存
    * @param[in]    byMD5       md5
    * @param[in]    uCRC32      crc32
    * @param[in]    pRecord     返回的cache信息
    * @remark       如果新记录的时间戳较老,原有记录不会被覆盖
    */ 
    virtual HRESULT STDMETHODCALLTYPE Update(
        const BYTE                      byMD5[SKYLARK_MD5_BYTES],
        UINT32                          uCRC32,
        const BKCACHE_HASH_TO_LEVEL*    pRecord) = 0;


    /**
    * @brief        删除缓存
    * @param[in]    byMD5       md5
    * @param[in]    uCRC32      crc32
    */
    virtual HRESULT STDMETHODCALLTYPE Delete(const BYTE byMD5[SKYLARK_MD5_BYTES], UINT32 uCRC32) = 0;


    /**
    * @brief        删除过期的缓存(保留,暂未实现)
    */
    virtual HRESULT STDMETHODCALLTYPE DeleteExpired() = 0;


    /**
    * @brief        将内存中记录同步到数据库文件,耗时的操作
    */
    virtual HRESULT STDMETHODCALLTYPE Flush() = 0;




    /// 获取设置
    virtual HRESULT STDMETHODCALLTYPE GetSettings(BKCACHE_SETTINGS_HASH_TO_LEVEL* pSettings) = 0;

    /// 修改设置,如果为NULL,会恢复默认设置
    virtual HRESULT STDMETHODCALLTYPE SetSettings(const BKCACHE_SETTINGS_HASH_TO_LEVEL* pSettings) = 0;



    /// 开启事务
    virtual HRESULT STDMETHODCALLTYPE BeginTransaction(BKDb::EM_TRANSLOCK transLock = BKDb::em_Deferred) = 0;

    /// 提交事务
    virtual HRESULT STDMETHODCALLTYPE CommitTransaction() = 0;

    /// 回滚事务
    virtual HRESULT STDMETHODCALLTYPE RollbackTransaction() = 0;

    /// 检查当前对象是否在事务中
    virtual BOOL    STDMETHODCALLTYPE IsInTransaction() = 0;

    /// 获取当前时间
    virtual ULONGLONG STDMETHODCALLTYPE GetCurrentTimeAsTimeStamp() = 0;

};




/**
* @brief    缓存 hash -> level
* @remark   nidx(<path>, md5, crc32, timestamp, tag)
*/
class __declspec(uuid("9942D1EC-7B54-4e1c-8F58-5F4F38E95FF6"))
IBKCacheHashToLevel2: public IBKCacheHashToLevel
{
public:
    /** 
    * @brief    打开自定义缓存
    * @param    lpCacheFile     缓存的路径,如果为NULL,则取默认位置的缓存
    * @param    pPassword       密钥
    * @param    dwPasswordBytes 密钥长度
    * @param    bCreateNewIfFailedToOpen 如果打开失败,是否尝试创建新数据库
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(
        LPCWSTR lpCacheFile,
        void*   pPassword,
        DWORD   dwPasswordBytes,
        BOOL    bCreateNewIfFailedToOpen = TRUE) = 0;
};


/**
* @brief    缓存 path -> tag
* @remark   nidx(<path>, md5, crc32, timestamp, tag)
*/
class __declspec(uuid("EA7CA7BA-9350-4894-9099-8D5E4B01A79F"))
IBKCachePathToHRESULT: public IUnknown
{
public:

    /** 
    * @brief    打开标准的缓存
    * @param    dwStdCache  要打开的缓存id,取值为
    *           BKCACHE_STD_PATH_TO_WINTRUST,
    *           BKCACHE_STD_PATH_TO_NONPE,
    *           BKCACHE_STD_PATH_TO_AVEFT,
    *           BKCACHE_STD_PATH_TO_AVEBWM,
    *           BKCACHE_STD_PATH_TO_DELETE_TRACE,
    */
    virtual HRESULT STDMETHODCALLTYPE OpenStandard(DWORD dwStdCache) = 0;


    /** 
    * @brief    打开缓存
    * @param    lpCacheFile 缓存的路径,如果为NULL,则取默认位置的缓存
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(LPCWSTR lpCacheFile = NULL) = 0;


    /**
    * @brief        缓存查找
    * @param[in]    lpszFilePath    文件路径
    * @param[out]   pRecord         返回的cache信息
    */ 
    virtual HRESULT STDMETHODCALLTYPE Lookup(
        LPCWSTR                     lpszFilePath,
        BKCACHE_PATH_TO_HRESULT*    pRecord) = 0;


    /**
    * @brief        更新缓存
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    pRecord         返回的cache信息
    * @remark       如果新记录的时间戳较老,原有记录不会被覆盖
    */ 
    virtual HRESULT STDMETHODCALLTYPE Update(
        LPCWSTR                         lpszFilePath,
        const BKCACHE_PATH_TO_HRESULT*  pRecord) = 0;


    /**
    * @brief        删除缓存
    * @param[in]    lpszFilePath    文件路径
    */
    virtual HRESULT STDMETHODCALLTYPE Delete(LPCWSTR lpszFilePath) = 0;


    /**
    * @brief        删除过期的缓存(保留,暂未实现)
    */
    virtual HRESULT STDMETHODCALLTYPE DeleteExpired() = 0;


    /**
    * @brief        将内存中记录同步到数据库文件,耗时的操作
    */
    virtual HRESULT STDMETHODCALLTYPE Flush() = 0;




    /// 获取设置
    virtual HRESULT STDMETHODCALLTYPE GetSettings(BKCACHE_SETTINGS_PATH_TO_HRESULT* pSettings) = 0;

    /// 修改设置,如果为NULL,会恢复默认设置
    virtual HRESULT STDMETHODCALLTYPE SetSettings(const BKCACHE_SETTINGS_PATH_TO_HRESULT* pSettings) = 0;



    /// 开启事务
    virtual HRESULT STDMETHODCALLTYPE BeginTransaction(BKDb::EM_TRANSLOCK transLock = BKDb::em_Deferred) = 0;

    /// 提交事务
    virtual HRESULT STDMETHODCALLTYPE CommitTransaction() = 0;

    /// 回滚事务
    virtual HRESULT STDMETHODCALLTYPE RollbackTransaction() = 0;

    /// 检查当前对象是否在事务中
    virtual BOOL    STDMETHODCALLTYPE IsInTransaction() = 0;

    /// 获取当前时间
    virtual ULONGLONG STDMETHODCALLTYPE GetCurrentTimeAsTimeStamp() = 0;

};









/**
* @brief    路径列表
* @remark   plidx(<path>, flag, timestamp)
*/
class __declspec(uuid("58AF1B9D-8DDD-4f2c-83BA-337CEDD44605"))
IBKCachePathList: public IUnknown
{
public:

    /** 
    * @brief    打开标准的缓存
    * @param    dwStdCache  要打开的缓存id,取值为
    *           BKCACHE_STD_EXCLUDE_FILES
    */
    virtual HRESULT STDMETHODCALLTYPE OpenStandard(DWORD dwStdCache) = 0;


    /** 
    * @brief    打开自定义的缓存
    * @param    lpCacheFile 缓存的路径,如果为NULL,则取默认位置的缓存
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(LPCWSTR lpCacheFile = NULL) = 0;


    /** 
    * @brief    查找第一条记录
    */
    virtual BOOL    STDMETHODCALLTYPE FindFirstItem() = 0;


    /** 
    * @brief    查找下一条记录
    */
    virtual BOOL    STDMETHODCALLTYPE FindNextItem() = 0;



    /** 
    * @brief    获取当前记录的路径
    */
    virtual LPCWSTR STDMETHODCALLTYPE Get_FilePath() = 0;

    /** 
    * @brief    查找当前记录的标志位
    */
    virtual DWORD   STDMETHODCALLTYPE Get_Flag() = 0;

    /** 
    * @brief    查找当前记录的时间戳
    */
    virtual ULONGLONG STDMETHODCALLTYPE Get_TimeStamp() = 0;



    /**
    * @brief        添加记录
    * @param[in]    lpszFilePath    文件路径
    */
    virtual HRESULT STDMETHODCALLTYPE Add(
        LPCWSTR     lpszFilePath,
        DWORD       dwFlag = 0,
        ULONGLONG   uTimeStamp = 0) = 0;


    /**
    * @brief        删除记录
    * @param[in]    lpszFilePath    文件路径
    */
    virtual HRESULT STDMETHODCALLTYPE Delete(LPCWSTR lpszFilePath) = 0;




    /// 开启事务
    virtual HRESULT STDMETHODCALLTYPE BeginTransaction(BKDb::EM_TRANSLOCK transLock = BKDb::em_Deferred) = 0;

    /// 提交事务
    virtual HRESULT STDMETHODCALLTYPE CommitTransaction() = 0;

    /// 回滚事务
    virtual HRESULT STDMETHODCALLTYPE RollbackTransaction() = 0;

    /// 获取当前时间
    virtual ULONGLONG STDMETHODCALLTYPE GetCurrentTimeAsTimeStamp() = 0;
};












NS_SKYLARK_END




#define FN_BKCacheInitialize    "BKCacheInitialize"     ///< export BKCacheInitialize
#define FN_BKCacheCreateObject  "BKCacheCreateObject"   ///< export BKCacheCreateObject


/// 初始化bkcache模块,可以多次调用
EXTERN_C HRESULT WINAPI BKCacheInitialize();
/// typedef for BKCacheCreateObject
typedef HRESULT (WINAPI *PFN_BKCacheInitialize)();


/**
* @brief    创建BKCache的模块
* @param    dwBKEngVer      BKEng的版本号,默认使用BKENG_CURRENT_VERSION,传入0表示不作版本检查
* @param    riid            要创建的对象的iid
* @param    ppvObj          返回创建的对象
*/
EXTERN_C HRESULT WINAPI BKCacheCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKCacheCreateObject
typedef HRESULT (WINAPI *PFN_BKCacheCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);



#endif//BKCACHE_H