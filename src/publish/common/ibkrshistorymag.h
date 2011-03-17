/** 
* @file ibkrshistorymag.h
* @brief 回扫文件历史管理接口
* @author BAO<baoderong@kingsoft.com>
* @date November 10,2010
*/

#ifndef IBKRSHISTORYMAG_H
#define IBKRSHISTORYMAG_H

#include <Unknwn.h>

#include "bkrescanbasedata.h"

/** 
* @interface
*/
interface __declspec(uuid("0C593FE8-DCF3-4b3d-AC64-2BB0C227DE47"))
IBKRSHistoryMag : public IUnknown
{
public:
	/** 
	* @brief 查询一类回扫文件
	* @details 为了加快速度，这里uMaxCount为0时获取的记录数未对文件来源过虑，获取的记录为大于等于
	*	当时的（数据库是不断变化的，所以用当时）记录数
	*
	* @param[in] const BKRS_HistoryQueryType enumQurerType 要查询的文件类型，参见BKRS_HistoryQueryType
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
		const BKRS_HistoryQueryType enumQurerType, 
		const BKRS_FileFromType enumFromType, 
		const uint32_t uMaxCount, 
		const uint32_t uWaitSeconds, 
		BKRS_QueryFileInfo *pstQueryResult, 
		uint32_t *puGetCount) = 0;

	/** 
	* @brief 删除某些历史文件记录
	* @details 由于数据库在不断在更新，不能提供删除某类所有文件，以免删除了不应该删除的文件
	*
	* @param[in] const BKRS_QueryFileInfo *pstQueryResult 回要删除的文件记录
	* @param[in] const uint32_t uFileCount 要删除的文件记录个数
	* @param[in] const BKRS_FileFromType enumFromType 文件来源类型
	* @param[in] const uint32_t uWaitSeconds 等待数据库锁时间，由于数据库是被多进程访问的
	* @return 标准的HRESULT值
	*
	* @attention 不能删除未知的回扫文件，遇到则路过
	*/
	virtual HRESULT __stdcall RemoveFiles(
		const BKRS_QueryFileInfo *pstQueryResult, 
		const uint32_t uFileCount, 
		const BKRS_FileFromType enumFromType, 
		const uint32_t uWaitSeconds) = 0;	

	/** 
	* @brief 历史查询初始化函数
	*
	* @param 无
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall Initialize() = 0;

	/** 
	* @brief 历史反初始化函数
	*
	* @param 无
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall UnInitialize() = 0;
};


#endif

