//////////////////////////////////////////////////////////////////////
///		@file		kxeservicemgr_def.h
///		@author		luopeng
///		@date		2008-10-06 09:07:58
///
///		@brief		服务管理器的通信数据结构定义
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../include/kxe_serialize.h"
#include <vector>

/**
 * @defgroup kxeservice_manager_data_struct_group KXEngine Service Manager Data Struct Define
 * @{
 */

/// 停止服务时的回复
typedef struct _KXE_STOP_SERVICE_RSP
{
	DWORD dwProcessId;  ///< 该服务当前的进程ID
} KXE_STOP_SERVICE_RSP;

KXE_JSON_DDX_BEGIN(KXE_STOP_SERVICE_RSP)
	KDX_OBJ_NM("ProcessId", dwProcessId);
KXE_JSON_DDX_END()

/// 当前产品的信息
typedef struct _KXE_PRODUCT_INFO
{
	std::wstring strProductID;                     ///< 该产品的产品标志
	std::wstring strParentProductID;               ///< 该产品的父产品标志，若存在，则非独立产品
	std::wstring strRecommendProductID;            ///< 推荐该产品的产品标志
#if _MSC_VER > 1200
	__int64 nInstallTime;                          ///< 该产品安装时间，若此时间为0，则服务会获取该值
#else
	int nInstallTime;                          ///< 该产品安装时间，若此时间为0，则服务会获取该值
#endif
	std::wstring strProductPath;                   ///< 产品安装的目录
	std::wstring strPluginPath;                    ///< Plugin目录
	std::wstring strWebDirectoryPath;              ///< 网页路径
	std::vector<KSCLSID> vecChildProduct;          ///< 该产品的子产品标志
	std::vector<std::wstring> vecComponent;        ///< 该产品所有的组件路径
	std::vector<std::wstring> vecServiceProvider;  ///< 该产品所有的服务提供者路径，该文件必须存在于vecComponent中
} KXE_PRODUCT_INFO;

KXE_JSON_DDX_BEGIN(KXE_PRODUCT_INFO)
	KDX_OBJ_NM("ProductID", strProductID);
	KDX_OBJ_NM("ParentProductID", strParentProductID);
	KDX_OBJ_NM("RecommendProductID", strRecommendProductID);
	KDX_OBJ_NM("InstallTime", nInstallTime);
	KDX_OBJ_NM("ProductPath", strProductPath);
	KDX_OBJ_NM("PluginPath", strPluginPath);
	KDX_OBJ_NM("WebPath", strWebDirectoryPath);
	KDX_OBJ_NM("ChildProductID", vecChildProduct);
	KDX_OBJ_NM("ComponentArray", vecComponent);
	KDX_OBJ_NM("ServiceProviderArray", vecServiceProvider);
KXE_JSON_DDX_END()

/**
 * @}
 */