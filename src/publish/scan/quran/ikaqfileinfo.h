#ifndef _IKAQ_FILEINFO_H_
#define _IKAQ_FILEINFO_H_


#include "unknwn.h"

// {7DC323FF-7266-4bca-B157-69D2D4BDE093}
extern const GUID IID_KAQFILEINFO;


#define E_BUFFER_TOO_SMALL    0x84000321


/** \class IKAQFileInfo
* \brief 枚举文件信息接口
*/
class IKAQFileInfo : public IUnknown
{
public:
	/** \brief  隔离文件的生成时间
	*   \param  index  文件索引 (从0到总数减1)
	*   \param  pTime  返回创建隔离文件时间
	*   \return 成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE getCreateTime(
		unsigned int index, SYSTEMTIME* pTime) = 0;

	/** \brief  隔离文件名
	*   \param  index         文件索引 (从0到总数减1)
	*   \param  pQurantFile   接收隔离文件名缓冲区
	*   \param  ulSizeOfName  接收缓冲区大小
	*   \return 如果接收缓冲区小于文件名长度E_BUFFER_TOO_SMALL。
	*           成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE getQurantFileName(
		unsigned int index, wchar_t* pQurantFile, unsigned long ulSizeOfName) = 0;

	/** \brief  隔离病毒名
	*   \param  index        文件索引 (从0到总数减1)
	*   \param  pVirusName   接收病毒名缓冲区
	*   \param  ulSizeOfName 接收缓冲区大小
	*   \return 如果接收缓冲区小于文件名长度E_BUFFER_TOO_SMALL。
	*           成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE getVirusName(
		unsigned int index, wchar_t* pVirusName, unsigned long ulSizeOfName) = 0;

	/** \brief  隔离原始文件名
	*   \param  index        文件索引 (从0到总数减1)
	*   \param  pVirusName   接收原始文件名缓冲区
	*   \param  ulSizeOfName 接收缓冲区大小
	*   \return 如果接收缓冲区小于文件名长度E_BUFFER_TOO_SMALL。
	*           成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE getOrignFileName(
		unsigned int index, wchar_t* pFileName, unsigned long ulSizeOfName) = 0;

	/** \brief  得到隔离文件总数
	*   \param  pCount  返回文件总数
	*   \return 成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE getQurantFileCount(unsigned long* pCount) = 0;
};


#endif