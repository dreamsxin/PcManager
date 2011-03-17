/**
* @file    bkwhitelist.h
* @brief   ...
* @author  罗鹏
* @date    2010-7-12 14:42
*/

#ifndef BKWHITELIST_H
#define BKWHITELIST_H

#include <unknwn.h>
/**
* @brief 白名单文件匹配接口
*/
class __declspec(uuid("59602084-0E12-4218-96EE-2B49158B8655"))
IBKUserWhiteList
{
public:
	/**
	* @brief        查询文件是否存在
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Lookup(
		LPCTSTR lpszFileName,
		BOOL& bIsExist
		) = 0;
};

#endif // BKWHITELIST_H