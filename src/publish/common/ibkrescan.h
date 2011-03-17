/** 
* @file ibkrescan.h
* @brief 回扫文件接口
* @author BAO<baoderong@kingsoft.com>
* @date November 05,2010
*/

#ifndef IBKRESCAN_H
#define IBKRESCAN_H

#include "irescancallback.h"

#define BKRS_INSERTFROM_FASTSCAN 0x01			/// 文件来自快速扫描
#define BKRS_INSERTFROM_FILEMONITOR 0x02		/// 文件来自文件监控
#define BKRS_INSERTFROM_FULLDISKSCAN 0x04		/// 文件来自全盘扫描
#define BKRS_INSERTFROM_RSS_KXESCANSP 0x08		/// 文件来自毒霸查杀sp#define BKRS_INSERTFROM_RSS_USBMONITOR 0x10		/// 文件来自毒霸U盘监控

#define BKRS_INSERTFROM_MASKCODE 0x1F			/// 文件来源掩码
#define BKRS_INSERTFROM_COUNT 5					/// 文件来源个数

/** 
* @interface IBKReScan
* @brief 回扫文件接口
*/
interface __declspec(uuid("57F612D4-138D-4bae-A477-B3142E8DDC8D"))
IBKReScan : public IUnknown
{
public:
	/** 
	* @brief 增加一个回扫文件
	* @details 文件以文件名为关键字，添加一个文件执行如下三种操作之一
	*	1、当回扫文件列表中无此文件名的文件，向数据库添加
	*	2、当列表中有此文件，且MD5相同，不执行任何操作，丢弃此文件
	*	3、当列表中有此文件，且MD5不同，替换原有同名文件，并重设回扫信息
	*	当只是文件来源不同时，更新文件来源，加入新来源
	*
	* @param[in] const wchar_t *pszFileName 要添加的回扫文件名
	* @param[in] const BYTE pszMD5[16] 回扫文件的MD5，传NULL时我会自己计算MD5
	* @param[in] const wchar_t *pszFileChannel 回扫文件的通道号
	* @param[in] unsigned int uFlag 保存文件来源（如全盘，快扫，监控），可以按位或，见上面宏定义
	* @param[in|out] void *pvData 暂时未用
	* @return 标准的HRESULT值
	*
	* @attention 此接口只将文件放入缓存中，返回是可能数据还没有插入到数据库中
	*/
	virtual HRESULT __stdcall InsertOneFile(
		const wchar_t *pszFileName,
		const BYTE pszMD5[16],
		const wchar_t *pszFileChannel, 
		unsigned int uFlag, 
		void *pvData = NULL) = 0;

	/** 
	* @brief 回扫初始化函数
	*
	* @param[in] IReScanCallback *piReScan 得知未知文件结果时的回调接口
	* @param[in] const wchar_t *pszChannnelName 回扫的通道号
	* @param[in] const wchar_t *pszChannnelKey 回扫的通道key
	* @param[in|out] void *pvData 暂时未用
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall Initialize(
		IReScanCallback *piReScan, 
		const wchar_t *pszChannnelName,
		const wchar_t *pszChannnelKey) = 0;

	/** 
	* @brief 反初始化函数
	*
	* @param[in|out] void *pvData 暂时未用
	* @return 标准的HRESULT值
	*/
	virtual HRESULT __stdcall UnInitialize() = 0;
};

#endif

