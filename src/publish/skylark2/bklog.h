/**
* @file    bklog.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-04 17:54
*/

#ifndef BKLOG_H
#define BKLOG_H

#include "bklogdef.h"
#include "bkcache.h"

NS_SKYLARK_BEGIN

/**
* @brief    扫描日志写入接口
*/
class __declspec(uuid("D4595D08-B6D1-42f4-816C-91A930720794"))
IBKEngScanLogWriter: public IUnknown
{
public:

    /** 
    * @brief    打开标准的日志
    * @param    dwStdCache  要打开的日志id,取值为
    *                       BKCACHE_STD_SCAN_LOG
    */
    virtual HRESULT STDMETHODCALLTYPE OpenStandard(DWORD dwStdCache) = 0;


    /** 
    * @brief    打开自定义缓存
    * @param    lpCacheFile 缓存的路径,如果为NULL,则取默认位置的缓存
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(LPCWSTR lpCacheFile = NULL) = 0;




    /// 写入日志
    virtual HRESULT STDMETHODCALLTYPE WriteLog(const BKENG_SCAN_LOG* pLog) = 0;

    /// 写入日志
    virtual HRESULT STDMETHODCALLTYPE WriteResultAsLog(LPCWSTR lpszFilePath, const BKENG_RESULT* pResult) = 0;


    /// 删除过期日志
    virtual HRESULT STDMETHODCALLTYPE DeleteExpiredLogs() = 0;

    /// 删除所有日志
    virtual HRESULT STDMETHODCALLTYPE DeleteAllLogs() = 0;



    /// 将日志保存到文件
    virtual HRESULT STDMETHODCALLTYPE SaveToFile() = 0;
};



/**
* @brief    日志读取接口
*/
class __declspec(uuid("F0A73B6F-EA9D-445a-B80E-B30F34E990CE"))
IBKEngScanLogReader: public IUnknown
{
public:


    /** 
    * @brief    打开标准的日志
    * @param    dwStdCache  要打开的日志id,取值为
    *                       BKCACHE_STD_SCAN_LOG
    */
    virtual HRESULT STDMETHODCALLTYPE OpenStandard(DWORD dwStdCache) = 0;


    /** 
    * @brief    打开自定义缓存
    * @param    lpCacheFile 缓存的路径,如果为NULL,则取默认位置的缓存
    */
    virtual HRESULT STDMETHODCALLTYPE OpenCustom(LPCWSTR lpCacheFile = NULL) = 0;



    /**
    * @brief        获取日志条数
    */ 
    virtual DWORD STDMETHODCALLTYPE GetLogCount() = 0;

    /**
    * @brief        读取日志
    * @param[out]   logArray    保存读取的日志数组, 需要能够容纳dwCount个元素
    * @param[out]   pdwLogCount 返回读取的日志数目, 不大于dwCount
    * @param[in]    dwOffset    读取的偏移(从0开始计算)
    * @param[in]    dwCount     要读取的日志的个数
    */ 
    virtual HRESULT STDMETHODCALLTYPE ReadLog(
        BKENG_SCAN_LOG  logArray[],
        DWORD*          pdwLogCount,
        DWORD           dwOffset = 0,
        DWORD           dwCount  = 1) = 0;
};

NS_SKYLARK_END

#endif//BKLOG_H