//=============================================================================
/**
* @file kserviceutil.h
* @brief 
* @author qiuruifeng <qiuruifeng@kingsoft.com>
* @date 2009-8-6   21:31
*/
//=============================================================================
#pragma once 

enum enumKSERVICE_FIX_ACTION
{
	enumSERVICE_FIX_NONE		= 0,
	enumSERVICE_FIX_START		= 1, 
	enumSERVICE_FIX_INSTALL_AND_START	= 2 
};

/**
 * @brief 检查服务状态
 * @remark 如果注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\下的服务键值被删,依然检测出服务存在,状态是SERVICE_STOPPED
 * @param[in] pszServiceName 服务名
 * @param[out] pbServiceExist 服务是否存在
 * @param[out] pdwStatus 服务状态(SERVICE_STOPPED SERVICE_START_PENDING SERVICE_RUNNING .....)
 * @param[out] pdwError api执行的错误码
 * @return TRUE成功,FALSE失败
 */
BOOL KCheckServiceStatus(const wchar_t* pszServiceName, 
						 BOOL* pbServiceExist, 
						 DWORD* pdwStatus, 
						 DWORD* pdwError);

/**
* @brief 启动服务状态
* @remark 如果注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\下的服务键值被删,错误码是ERROR_PATH_NOT_FOUND,
*			如果服务被禁用错误码是ERROR_SERVICE_DISABLED
* @param[in] pszServiceName 服务名
* @param[out] pdwError api执行的错误码
* @return TRUE成功,FALSE失败
*/
BOOL KStartService(const wchar_t* pszServiceName, DWORD* pdwError);

/**
 * @brief 更改服务设置
 * @remark 如果注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\下的服务键值被删,错误码是ERROR_PATH_NOT_FOUND,
 * @param[in] pszServiceName 服务名
 * @param[in] pszImagePath	文件路径可以传NULL
 * @param[in] pszSvcOrderGroup 组名 可以传NULL
 * @param[in] pszDisplayName 服务友好名称 可以传NULL
 * @param[in] pszDescription 服务描述  可以传NULL
 * @param[in] dwStartType 启动类型,如果SERVICE_DEMAND_START SERVICE_AUTO_START，SERVICE_NO_CHANGE是不更改
 * @param[out] pdwError api执行的错误码
 * @return TRUE成功,FALSE失败
 */
BOOL KChangeServiceConfig(const wchar_t* pszServiceName, 
						  const wchar_t* pszImagePath,  
						  const wchar_t* pszSvcOrderGroup, 
						  const wchar_t* pszDisplayName,
						  const wchar_t* pszDescription,
						  DWORD dwStartType,
						  DWORD* pdwError);

/**
 * @brief 创建服务
 * @remark 如果注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\下的服务键值被删,错误码是ERROR_FILE_NOT_FOUND,
 * @param[in] pszServiceName 服务名
 * @param[in] pszImagePath 文件路径
 * @param[in] pszSvcOrderGroup 组名
 * @param[in] pszDisplayName 服务友好名称 可以传NULL
 * @param[in] pszDescription 服务描述  可以传NULL
 * @param[in] dwStartType 启动类型,如果SERVICE_DEMAND_START SERVICE_AUTO_START
 * @param[out] pdwError  api执行的错误码
 * @return TRUE成功,FALSE失败
 */
BOOL KCreateService(const wchar_t* pszServiceName, 
					const wchar_t* pszImagePath,  
					const wchar_t* pszSvcOrderGroup, 
					const wchar_t* pszDisplayName,
					const wchar_t* pszDescription,
					DWORD dwStartType,
					DWORD* pdwError,
					const wchar_t* pDepend = NULL,
					DWORD nServicesType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS);

/**
* @brief		停止指定服务
* @remark		
* @param[in]	pszServiceName 服务名
* @param[out]	pdwError api执行的错误码
* @return		0 成功，其他为错误码
**/
BOOL KStopService(const wchar_t* pszServiceName, DWORD* pdwError);

/**
 * @brief 删除服务
 * @remark 如果注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\下的服务键值被删,可以正常的删除服务
 * @param[in] pszServiceName 服务名
 * @param[out] pdwError api执行的错误码
 * @return TRUE成功,FALSE失败
 */
BOOL KDeleteService(const wchar_t* pszServiceName, DWORD* pdwError);



/**
 * @brief 检查服务状态并试图修复
 * @remark 
 * @param[in] pszServiceName 服务名
 * @param[in] pszImagePath 文件路径
 * @param[in] pszSvcOrderGroup 组名
 * @param[in] pszDisplayName 服务描述
 * @param[out] pnFixAction 修复的动作 
 * @return TRUE成功,FALSE失败
 */
BOOL CheckAndFixService(const wchar_t* pszServiceName, 
						const wchar_t* pszImagePath,  
						const wchar_t* pszSvcOrderGroup, 
						const wchar_t* pszDisplayName,
						const wchar_t* pszDescription,
						int* pnFixAction,
						DWORD* pdwError);


/**
 * @brief 修复cryptsvc服务
 * @remark 
 * @return 0成功, 非0失败
 */
int FixCryptService();

BOOL IsNeededReboot(void);
