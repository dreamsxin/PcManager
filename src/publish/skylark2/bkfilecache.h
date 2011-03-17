/**
* @file    bkfilecache.h
* @brief   ...
* @author  罗鹏
* @date    2010-7-12 14:42
*/

#ifndef BKFILECACHE_H
#define BKFILECACHE_H

#include <unknwn.h>
#include "skylarkbase.h"
#include "bkengdef.h"
#include "bklevel.h"

/**
* @brief 文件Cache缓冲接口
*/
class __declspec(uuid("18E73B75-01C8-408c-AC81-A935113E6DBA"))
IBKFileCache : public IUnknown
{
public:
	/**
	* @brief        初始化文件Cache缓冲
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Initialize() = 0;


	/**
	* @brief        反初始化文件Cache缓冲
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;

	/**
	* @brief        将查询的文件结果进行更新
	* @param[in]    lpszFilePath    原始文件路径
	* @param[in]    dwFileLevel     文件的级别，针对金山云查杀发现的病毒，使用SLPTL_LEVEL_MARK___BLACK_AUTO(50)
	* @param[in]    lpszVirusName   病毒名
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE UpdateCloudPEFile(
		LPCWSTR             lpszFilePath,
		DWORD               dwFileLevel,
		LPCWSTR             lpszVirusName
		) = 0;

	/**
	* @brief        将结果更新至数据库
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE FlushToDB() = 0;
};


#endif // BKFILECACHE_H