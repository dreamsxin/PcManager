/**
* @file    bkunknownreport.h
* @brief   ...
* @author  罗鹏
* @date    2010-7-12 14:42
*/

#ifndef BKUNKNOWNREPORT_H
#define BKUNKNOWNREPORT_H

#include <unknwn.h>
/**
* @brief 未知文件上传接口
*/
class __declspec(uuid("663D4B88-5CD3-4801-BBEA-5E7B4875464A"))
IBKUnknownFileReport
{
public:
	/**
	* @brief        添加未知文件至上传列表
	* @param[in]    lpszFilePath    原始文件路径
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE AddUnknownFile(
		LPCWSTR             lpszFilePath
		) = 0;

	/**
	* @brief        将文件上传至服务端
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Report() = 0;

	/**
	* @brief        是否为自动上传至服务端
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE IsAutoReport(
		BOOL& bIsAutoReport
		) = 0;
};

#endif // BKUNKNOWNREPORT_H
