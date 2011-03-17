//=================================================================
/**
* @file iksautoinstcallback.h
* @brief
* @author zhengyousheng <zhengyousheng@kingsoft.com>
* @data 2010-11-06 11:22
*/
//=================================================================

#pragma once

#include "ksautoinstdef.h"

class IKSAutoInstCallBack
{
public:

	/**
	* @brief 某一软件安装前的数据回调
	* @param[in] pstruSoftInfo 软件基本信息与插件信息
	* @return 0 成功，其他为失败错误码
	*/
	virtual HRESULT BeforeInstall(const S_KSAI_SOFT_INFO* pstruSoftInfo) = 0;

	/**
	* @brief 某一软件进度数据回调
	* @param[in] pstruStatus 软件基本信息与进度数据
	* @return 0 成功，其他为失败错误码
	*/
	virtual HRESULT OnProgress(const S_KSAI_STATUS* pstruStatus) = 0;

	/**
	* @brief 某一软件安装后的数据回调
	* @param[in] pstruInstResult 软件基本信息、插件信息与安装结果
	* @return 0 成功，其他为失败错误码
	*/
	virtual HRESULT AfterInstall(const S_KSAI_INST_RESULT* pstruInstResult) = 0;
};