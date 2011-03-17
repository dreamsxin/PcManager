//////////////////////////////////////////////////////////////////////
///		@file		ikxesystemservice.h
///		@author		luopeng
///		@date		2008-10-06 09:07:58
///
///		@brief		系统服务的ServiceProvider
//////////////////////////////////////////////////////////////////////

#pragma once
#include "kxesystemservice_def.h"
#include "../include/idl.h"

/**
* @defgroup kxebase_system_service_provider KXEngine Architecture System Service Provider
* @{
*/

/**
* @brief 由平台提供的系统相关服务
*/
class IKxESystemService
{
public:
    /**
    * @brief 通知重启计算机
    * @return 0 成功,其他为失败错误码
    */
    virtual int __stdcall Reboot(
        const KXE_NULL_PARAEMETER&, 
        KXE_NULL_PARAEMETER&
        ) = 0;

    /**
    * @brief 通过nFolder的值获取对应的路径
    * @param[in] nFolder 指定的Folder选项,参见MSDN的CSLIDL
    * @param[out] strFolderPath 获取到指定的FolderPath
    * @return 0 成功, 其他为失败错误码
    */
    virtual int __stdcall GetFolderPath(
        const int& nFolder,
        std::wstring& strFolderPath
        ) = 0;

    /**
    * @brief 获取所有驱动器名称
    * @param[out] listDriverInfo 得到驱动器名称及其类型
    * @return 0 成功, 其他为失败错误码
    */
    virtual int __stdcall GetLogicalDrives(
        const KXE_NULL_PARAEMETER&, 
        std::vector<KXE_LOGIC_DRIVER_INFO>& listDriverInfo
        ) = 0;

    /**
    * @brief 根据指定的目录获取其子目录
    * @param[in] strParentPath 需要获取子目录的路径
    * @param[out] vecChildPath 得到的子目录名字,非全路径
    * @return 0 成功,其他为失败错误码
    */
    virtual int __stdcall GetChildPath(
        const std::wstring& strParentPath,
        std::vector<std::wstring>& vecChildPath
        ) = 0;

    /**
    * @brief 根据桌面用户权限创建进程
    * @param[in] strCommandLine 需要启动的进程全路径，包含参数
    * @return 0 成功, 其他为失败错误码
    */
    virtual int __stdcall CreateProcess(
        const std::wstring& strCommandLine,
        KXE_NULL_PARAEMETER&
        ) = 0;

    /**
    * @brief 关闭计算机
    * @return 0 成功,其他为失败错误码
    */
    virtual int __stdcall ShutDown(
        const KXE_NULL_PARAEMETER&, 
        KXE_NULL_PARAEMETER&
        ) = 0;
};

KXE_RCF_BEGIN(IKxESystemService, "KxESystemService")
    KXE_RCF_METHOD(Reboot, KXE_NULL_PARAEMETER, KXE_NULL_PARAEMETER)
    KXE_RCF_METHOD(GetFolderPath, int, std::wstring)
    KXE_RCF_METHOD(GetLogicalDrives, KXE_NULL_PARAEMETER, std::vector<KXE_LOGIC_DRIVER_INFO> )
	KXE_RCF_METHOD(GetChildPath, std::wstring, std::vector<std::wstring>)
	KXE_RCF_METHOD(CreateProcess, std::wstring, KXE_NULL_PARAEMETER)
    KXE_RCF_METHOD(ShutDown, KXE_NULL_PARAEMETER, KXE_NULL_PARAEMETER)
KXE_RCF_END()

/**
 * @}
 */