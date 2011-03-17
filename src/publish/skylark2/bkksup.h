/**
* @file    bkksup.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-22 17:42
*/

#ifndef BKKSUP_H
#define BKKSUP_H

#include "bkksupdef.h"

NS_SKYLARK_BEGIN

class IBKEngKsgUpdateCallback;

/**
* @brief    毒霸病毒库升级接口
*/
class __declspec(uuid("EF9CC682-9035-4697-B271-59BC9565F20B"))
IBKEngKsgUpdate: public IUnknown
{
public:

    /**
    * @brief        初始化升级
    * @param[in]    pInit           初始化参数
    */
    virtual HRESULT STDMETHODCALLTYPE Initialize(BKKSUP_INIT* pInit) = 0;


    /**
    * @brief        反初始化扫描引擎
    */
    virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;


    /**
    * @brief        检查是否需要升级
    * @param[in]    pUpdateParam        升级参数
    * @param[out]   pUpdateInstruction  升级指示
    */ 
    virtual HRESULT STDMETHODCALLTYPE CheckUpdate(
        const BKKSUP_UPDATE_PARAM* pUpdateParam,
        BKKSUP_UPDATE_INSTRUCTION* pUpdateInstruction) = 0;


    /**
    * @brief        开始升级
    * @param[in]    pUpdateParam        升级参数
    * @param[out]   pUpdateInstruction  升级指示
    * @remark       Update 会调用 CheckUpdate, 所以不需要先调用 CheckUpdate
    */ 
    virtual HRESULT STDMETHODCALLTYPE Update(
        const BKKSUP_UPDATE_PARAM* pUpdateParam,
        BKKSUP_UPDATE_INSTRUCTION* pUpdateInstruction) = 0;


    /**
    * @brief        通知升级停止
    * @remark       本方法是非阻塞调用
    */ 
    virtual HRESULT STDMETHODCALLTYPE NotifyStop() = 0;


    /**
    * @brief        获取本地病毒库版本
    * @param[in]    pLocalSignVersion   本地病毒库版本
    */ 
    virtual HRESULT STDMETHODCALLTYPE GetLocalSignVersion(
        BKAVE_SIGN_VERSION* pLocalSignVersion) = 0;
};








/**
* @brief    毒霸病毒库升级回调接口
*/
class __declspec(uuid("234FC5F4-5815-4f2f-BF58-11DC2ED69C42"))
IBKEngKsgUpdateCallback
{
public:

   /**
    * @brief        下载文件前回调
    * @param[in]    lpszFileName            下载的文件名
    * @param[in]    dwFileSize              文件大小(压缩前)
    */ 
    virtual HRESULT STDMETHODCALLTYPE OnPrepareFile(
        LPCWSTR     lpszFileName,
        DWORD       dwFileSize) = 0;

   /**
    * @brief        下载文件时回调
    * @param[in]    lpszFileName            下载的文件名
    * @param[in]    dwTotalSize             文件总长度(压缩后)
    * @param[in]    dwReceiveSize           已经接收的大小(压缩后)
    */ 
    virtual HRESULT STDMETHODCALLTYPE OnDownloadFile(
        LPCWSTR     lpszFileName,
        DWORD       dwTotalSize,
        DWORD       dwDownloadedSize) = 0;

    /**
    * @brief        结束文件的下载
    * @param[in]    lpszFileName            下载的文件名
    * @param[in]    dwFileSize              文件大小(压缩前)
    * @param[in]    hrDownload              错误码(仅用来调试)
    * @remark       如果下载失败导致重试,那么在最后一次重试完成后,才会发生该回调
    */ 
    virtual HRESULT STDMETHODCALLTYPE FinishDownloadFile(
        LPCWSTR     lpszFileName,
        DWORD       dwFileSize,
        HRESULT     hrDownload) = 0;

    /**
    * @brief        替换文件时回调
    * @param[in]    lpszFileName            下载的文件名
    * @param[in]    lpszDstPath             被替换的文件位置
    * @param[in]    hrReplace               错误码(仅用来调试)
    */ 
    virtual HRESULT STDMETHODCALLTYPE OnReplaceFile(
        LPCWSTR     lpszFileName,
        LPCWSTR     lpszDstPath,
        HRESULT     hrReplace) = 0;
};


NS_SKYLARK_END






#define FN_BKKsupCreateObject			"BKKsupCreateObject"



/**
* @brief    创建实例
* @param    dwBKEngVer  BKEng的版本号,默认使用BKENG_CURRENT_VERSION,传入0表示不作版本检查
* @param    riid        要创建的对象的iid
* @param    ppvObj      返回创建的对象
*/
EXTERN_C HRESULT WINAPI BKKsupCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKKsupCreateObject
typedef HRESULT (WINAPI *PFN_BKKsupCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);



#endif//BKKSUP_H