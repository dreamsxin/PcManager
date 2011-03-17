//////////////////////////////////////////////////////////////////////////
//   File Name: bkupclt.h
// Description: Update Module Interface Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.6.1 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <Unknwn.h>

#include "bkupclterr.h"

// 更新状态
enum Update_Status {
    BkUpd_Status_None               = 0,    // 占位符
    BkUpd_Status_Check_Version      = 1,    // 检查版本
    BkUpd_Status_Check_FileList     = 2,    // 分析文件列表
    BkUpd_Status_Download_Files     = 3,    // 下载文件
    BkUpd_Status_Update_Files       = 4,    // 更新文件
    BkUpd_Status_Finished           = 5,    // 完成更新
};

// 更新回调接口
class __declspec(uuid("F0790C63-75E1-4e88-A031-A909FCAF7730"))
    IBkUpdateCallback
{
public:
    /**
    * @brief    更新的回调
    * @param    dwStatus        当前的状态
    * @param    lpInformation   字符串信息,具体含义取决于dwStatus
    * @param    dwPercentage    完成的百分比,取值从0到100,不保证一定会返回100
    * @retval   TRUE            继续升级
    * @retval   FALSE           中断升级
    */
    virtual BOOL STDMETHODCALLTYPE OnProgress(
        DWORD   dwStatus,
        DWORD   dwPercentage) = 0;

    /**
    * @brief    更新错误的回调
    * @param    dwStatus        当前的状态
    * @param    hErrCode        错误码,定义见上面
    * @param    lpInformation   字符串信息,具体含义取决于dwStatus
    */
    virtual void STDMETHODCALLTYPE OnError(
        DWORD   dwStatus,
        HRESULT hErrCode, 
        LPCWSTR lpInformation) = 0;

    /**
    * @brief    查询模块所在目录的回调
    * @param    lpszModuleName      模块名
    * @param    lpszModulePathRet   回调返回模块所在目录
    * @param    dwRetBufferSize     lpszModulePathRet指向的buffer大小（by wchar）
    * @retval   TRUE                支持此模块升级
    * @retval   FALSE               不支持此模块升级
    */
    virtual BOOL STDMETHODCALLTYPE OnQueryModulePath(
        LPCWSTR lpszModuleName, 
        LPWSTR lpszModulePathRet, 
        DWORD dwRetBufferSize) = 0;

    /**
    * @brief    文件升级通知
    * @param    lpszModuleName      模块名
    * @param    lpszFileName        文件名
    */
    virtual void STDMETHODCALLTYPE OnFileUpdated(
        LPCWSTR lpszModuleName, 
        LPCWSTR lpszFileName) = 0;

    /**
    * @brief    文件升级通知
    * @param    lpszModuleName      模块名
    * @param    lpszFileName        文件名
    * @param    bBackup             TRUE : 备份原始文件, FALSE : 替换安装目录文件
    * @param    dwErrorCode         标准LastError错误码
    */
    virtual void STDMETHODCALLTYPE OnFileReplace(
        LPCWSTR lpszModuleName, 
        LPCWSTR lpszFileName,
        BOOL    bBackup,
        DWORD   dwErrorCode) = 0;
};

// 更新接口定义
class __declspec(uuid("F0790C63-75E1-4e88-A031-A909FCAF7730"))
    IBkUpdater: public IUnknown
{
public:

    virtual HRESULT STDMETHODCALLTYPE CheckVersion(
        LPCWSTR                 lpszVersionUrl,
        LPCWSTR                 lpszAppName,
        LPCWSTR                 lpszSelfFileName,
        IBkUpdateCallback*      piCallback
        ) = 0;

    virtual HRESULT STDMETHODCALLTYPE Update(
        IBkUpdateCallback*      piCallback
        ) = 0;

    // 是否需要重启当前进程
    virtual BOOL    STDMETHODCALLTYPE NeedRestart() = 0;
};

/// class Factory
EXTERN_C HRESULT BkCreateUpdater(IBkUpdater** ppiObj);

