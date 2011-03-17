#ifndef _IKAV_RECYCLE_H_
#define _IKAV_RECYCLE_H_


#include "unknwn.h"
#include "ikaqfileinfo.h"


// {DD0240C9-F3FB-43c0-B74E-2E3070100AD6}
extern const GUID IID_KAVRECYCLE; 

#define E_FILE_EXISTS             0x04000001
#define E_DISK_FULL               0x84000001
#define E_GENERATE_FILE_FAILED    0x84000002
#define E_DELETE_FILE_FAILED      0x84000003
#define E_GET_FILE_INFO_FAILED    0x84000004
#define E_SET_FILE_ATTRIB_FAILED  0x84000005
#define E_FILE_NOT_FOUND          0x80070006
#define E_INITIALIZE_FAIL         0x84000007


typedef struct _KAVQUARN_SETTING
{
	DWORD	dwQuaranSpaceQuota;			// 隔离目录大小的限制以兆为单位。
	wchar_t	szQuaranPath[MAX_PATH];		// 隔离目录的路径
} KAVQUARN_SETTING;


/** \class IKAVRecycle
* \brief 隔离文件接口
*/
class IKAVRecycle : public IUnknown
{
public:
	/**	\brief 隔离文件
    * \param   pszQuarnFile	    指定要隔离的文件名
	* \param   pszVirusName		病毒名
	* \param   nOption		    = 0 隔离后删除;  = 2 隔离后不删除
	* \return  成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE QuarantineFile(
		const wchar_t* pszQuarnFile, 
		const wchar_t* pszVirusName, 
		int nOption) = 0;

	/** \brief 恢复隔离文件
	* \param   pszQuarantFile     隔离文件名 (隔离后文件名)
	* \param   pszFileName        恢复文件名
	* \return  成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE RestoreQuarantineFile(
		const wchar_t* pszQuarantFile, 
		const wchar_t* pszRestoreFile) = 0;

	/** \brief 删除隔离文件
	* \param   pszFileName   隔离文件名 (隔离后文件名)
	* \return  成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE DeleteQuarantineFile(
		const wchar_t* pszFileName) = 0;

	/** \brief 得到已隔离文件信息
	* \param   ppInfos  返回IKAQFileInfo接口
	* \return  成功返回S_OK, 如果隔离目录为空返回E_FILE_NOT_FOUND
	*/
	virtual HRESULT STDMETHODCALLTYPE GetQuarantineInfo(
		IKAQFileInfo** ppInfos) = 0;

	/** \brief  从外部计数文件读取被隔离文件总数 
	* \param    pCount  返回文件数
	* \return   成功返回S_OK
	*/
	virtual HRESULT STDMETHODCALLTYPE GetQuarantineFileCount(
		unsigned long* pCount) = 0;

	/** \brief 未使用
	*/
	virtual HRESULT STDMETHODCALLTYPE SetSetting(
		KAVQUARN_SETTING setting) = 0;

	/** \brief 未使用
	*/
	virtual HRESULT STDMETHODCALLTYPE GetSetting(
		KAVQUARN_SETTING* pSetting) = 0;
};


#endif