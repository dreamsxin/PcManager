//////////////////////////////////////////////////////////////////////
///		@file		ikxeservicemgr.h
///		@author		luopeng
///		@date		2008-10-06 09:07:58
///
///		@brief		基础ServiceProvider
//////////////////////////////////////////////////////////////////////

#pragma once
#include "kxeservicemgr_def.h"
#include "idl/idl.h"

/**
 * @defgroup kxeservice_manager_service_provider_interface_group KXEngine Service Manager Service Provider Interface
 * @{
 */

/**
 * @brief 服务管理器的SP接口
 */
class IKxEServiceMgr
{
public:
	/**
	 * @brief 安装产品相关内容
	 * @param[in] productInfo 安装的产品信息内容
	 * @param[out] strFailedFile 如果是因为文件引起的安装错误，此变量有效
	 * @return 0 成功，其他为失败错误码
	 * @remark 当A,B两产品，A为B产品的子产品时，若先装了A，再装B，A的父产品项也不会置位，以标志是独立安装的
	 */
	virtual int __stdcall InstallProduct(
		const KXE_PRODUCT_INFO& productInfo,
		std::wstring& strFailedFile
		) = 0;

	/**
	 * @brief 再次安装产品相关内容，如果产品已经安装，也会再安装一次
	 * @param[in] productInfo 安装的产品信息内容
	 * @param[out] strFailedFile 如果是因为文件引起的安装错误，此变量有效
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall ReinstallProduct(
		const KXE_PRODUCT_INFO& productInfo,
		std::wstring& strFailedFile
		) = 0;

	/**
	 * @brief 卸载产品相关内容
	 * @param[in] strProductID 需要卸载的产品ID
	 * @return 0 成功，其他为失败错误码
	 * @remark 当A,B两产品，A为B产品的子产品时，卸掉B产品后，将把A产品的父产品去掉，变成独立产品
	 */
	virtual int __stdcall UninstallProduct(
		const std::wstring& strProductID,
		KXE_NULL_PARAEMETER&
		) = 0;

	/**
	 * @brief 判断服务框架是否还存在产品依赖，若不存在，则可以卸载服务框架
	 * @param[out] bExistDepend 返回告诉是否存在产品依赖服务框架
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall IsExistProductDependFramework(
		const KXE_NULL_PARAEMETER&,
		BOOL& bExistDepend
		) = 0;

	/**
	 * @brief 获取指定的产品信息
	 * @param[in] strProductID 指定的产品的id标志
	 * @param[out] productInfo 返回指定产品的产品信息
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall GetProductInfo(
		const std::wstring& strProductID,
		KXE_PRODUCT_INFO& productInfo
		) = 0;

	/**
	 * @brief 获取所有依赖服务框架的产品信息列表
	 * @param[out] vecProduct 返回服务框架当前存在的产品列表
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall GetProductInfoList(
		const KXE_NULL_PARAEMETER&,
		std::vector<KXE_PRODUCT_INFO>& vecProduct
		) = 0;

	/**
	 * @brief 判断指定的产品是否存在，用于在一个产品包含多个产品需要安装时使用
	 * @param[in] strProductID 需要判断的产品KSCLISD标志
	 * @param[out] bExist 返回告诉该产品是否存在
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall IsProductExist(
		const std::wstring& strProductID,
		BOOL& bExist
		) = 0;

	/**
	 * @brief 使用当前权限启动进程
	 * @param[in] strCommandLine 启动的命令行
	 * @param[out] dwProcessId 创建出来的进程ID
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall CreateProcessAsClient(
		const std::wstring& strCommandLine, 
		DWORD& dwProcessId
		) = 0;

	/**
	 * @brief 使用管理员权限启动进程
	 * @param[in] strCommandLine 启动的命令行
	 * @param[out] dwProcessId 创建出来的进程ID
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall CreateProcessAsAdmin(
		const std::wstring& strCommandLine, 
		DWORD& dwProcessId
		) = 0;

	/**
	 * @brief 使用服务权限启动进程
	 * @param[in] strCommandLine 启动的命令行
	 * @param[out] dwProcessId 返回目标进程的ID
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall CreateProcessAsServer(
		const std::wstring& strCommandLine, 
		DWORD& dwProcessId
		) = 0;

	/**
	 * @brief 启动服务
	 * @param[in] strServiceName 服务名
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall KxEStartService(
		const std::wstring& strServiceName,
		KXE_NULL_PARAEMETER&
		) = 0;

	/**
	 * @brief 查询进程ID是否存在
	 * @param[in] dwProcessId 需要查询的进程ID
	 * @param[out] bExist 返回是否存在
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall QueryProcessIdExist(
		const DWORD& dwProcessId,
		BOOL& bExist
		) = 0;

	/**
	 * @brief 通知指定的产品已经活跃
	 * @param[in] uProductId 指定活跃的产品ID
	 * @param[out] strProductVersion 活跃的产品版本
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall NotifyProductActivity(
		const unsigned int& uProductId,
		const std::wstring& strProductVersion,
		KXE_NULL_PARAEMETER&
		) = 0;

};

KXE_RCF_BEGIN(IKxEServiceMgr, "KxEServiceMgr")
	KXE_RCF_METHOD(InstallProduct, KXE_PRODUCT_INFO, std::wstring)
	KXE_RCF_METHOD(ReinstallProduct, KXE_PRODUCT_INFO, std::wstring)
	KXE_RCF_METHOD(UninstallProduct, std::wstring, KXE_NULL_PARAEMETER)
	KXE_RCF_METHOD(IsExistProductDependFramework, KXE_NULL_PARAEMETER, BOOL)
	KXE_RCF_METHOD(GetProductInfo, std::wstring, KXE_PRODUCT_INFO)
	KXE_RCF_METHOD(GetProductInfoList, KXE_NULL_PARAEMETER, std::vector<KXE_PRODUCT_INFO>)
	KXE_RCF_METHOD(IsProductExist, std::wstring, BOOL)
	KXE_RCF_METHOD(CreateProcessAsClient, std::wstring, DWORD)
	KXE_RCF_METHOD(CreateProcessAsAdmin, std::wstring, DWORD)
	KXE_RCF_METHOD(CreateProcessAsServer, std::wstring, DWORD)
	KXE_RCF_METHOD(KxEStartService, std::wstring, KXE_NULL_PARAEMETER)
	KXE_RCF_METHOD(QueryProcessIdExist, DWORD, BOOL)
	KXE_RCF_METHOD_2(NotifyProductActivity, unsigned int, std::wstring, KXE_NULL_PARAEMETER)
KXE_RCF_END()

/**
 * @}
 */
