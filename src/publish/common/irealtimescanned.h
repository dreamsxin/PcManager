/** 
* @file irealtimescanned.h
* @brief 最新扫描过的回扫文件查询接口
* @author BAO<baoderong@kingsoft.com>
* @date November 11,2010
*/

#ifndef IREALTIMESCANNED_H
#define IREALTIMESCANNED_H

#include <Unknwn.h>

#include "bkrescanbasedata.h"

/** 
* @interface IBKRSRTScanned
* @brief 实时交互文件查询
*/
interface __declspec(uuid("132FABB8-0D2B-495b-B274-1FF524392F7A"))
IBKRSRTScanned : public IUnknown
{
public:
	/** 
	* @brief 查询一类回扫文件
	* @details 为了加快速度，这里uMaxCount为0时获取的记录数未对文件来源过虑，获取的记录为大于等于
	*	当时的（数据库是不断变化的，所以用当时）记录数
	*
	* @param[in] const BKRS_RTScannedQueryType enumQurerType 要查询的文件类型
	* @param[in] const BKRS_FileFromType enumFromType 查询的文件来源类型
	* @param[in] const uint32_t uMaxCount 最大返回文件个数，为0时由puGetCount返回所有满足条件的文件个数
	* @param[in] const uint32_t uWaitSeconds 等待数据库锁时间，由于数据库是被多进程访问的
	* @param[out] BKRS_QueryFileInfo *pstQueryResult 保存查询结果接口	
	* @param[out] uint32_t *puGetCount 返回获取的文件记录数
	* @return 标准的HRESULT值
	*
	* @attention 由于数据库是不断的更新的，所有在首次获取记录个数后，第二次结空间获取记录时，最好多给些
	*	记录空间，以便能获取新增的满足条件的记录
	*/
	virtual HRESULT __stdcall QueryFiles(
		const BKRS_RTScannedQueryType enumQurerType, 
		const BKRS_FileFromType enumFromType, 
		const uint32_t uMaxCount, 
		const uint32_t uWaitSeconds, 
		BKRS_QueryFileInfo *pstQueryResult, 
		uint32_t *puGetCount) = 0;

	/** 
	* @brief 删除某些扫描过的文件
	* @details 由于数据库在不断在更新，不能提供删除某类所有文件，以免删除了不应该删除的文件
	*
	* @param[in] const BKRS_QueryFileInfo *pstQueryResult 回要删除的文件记录
	* @param[in] const uint32_t uFileCount 要删除的文件记录个数
	* @param[in] const BKRS_FileFromType enumFromType 文件来源类型
	* @param[in] const uint32_t uWaitSeconds 等待数据库锁时间，由于数据库是被多进程访问的
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall RemoveFiles(
		const BKRS_QueryFileInfo *pstQueryResult, 
		const uint32_t uFileCount, 
		const BKRS_FileFromType enumFromType, 
		const uint32_t uWaitSeconds) = 0;	

	/** 
	* @brief 更新某些扫描过的文件状态
	* @details 目前只更新处理状态
	*
	* @param[in] const BKRS_QueryFileInfo *pstQueryResult 要更新的文件记录
	* @param[in] const uint32_t uFileCount 要更新的文件记录个数
	* @param[in] const uint32_t uWaitSeconds 等待数据库锁时间，由于数据库是被多进程访问的
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall UpdateFiles(
		const BKRS_QueryFileInfo *pstQueryResult, 
		const uint32_t uFileCount, 
		const uint32_t uWaitSeconds) = 0;

	/** 
	* @brief 最新结果查询初始化函数
	*
	* @param 无
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall Initialize() = 0;

	/** 
	* @brief 最新结果反初始化函数
	*
	* @param 无
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall UnInitialize() = 0;
};

#endif

