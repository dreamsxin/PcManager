//////////////////////////////////////////////////////////////////////
///		@file		ikxerequestdispatch.h
///		@author		luopeng
///		@date		2008-8-27 14:07:58
///
///		@brief		IPC转换的接口
//////////////////////////////////////////////////////////////////////

#pragma once

/**
 * @brief 用于IKxERequestDispatch存储时返回的数据的接口
 * @see IKxERequestDispatch
 */
class IKxERequestDispatchReturnBlock
{
public:
	virtual int __stdcall SetBuffer(
		const void* pBuffer, 
		size_t bufSize
		) = 0;

};

typedef enum _ENUM_KXE_COMMUNICATION_TYPE
{
	enum_EKCT_Socket,
	enum_EKCT_Pipe
}ENUM_KXE_COMMUNICATION_TYPE;

/**
 * @brief 用于控制当前IPC连接的接口
 */
class IKxERequestControl
{
public:
	/**
	 * @brief 模拟当前客户端进程的运行环境
	 * @return 0 成功, 其他为失败错误码
	 */
	virtual int __stdcall KxEImpersonateClient() = 0;

	/**
	 * @brief 还原当前进程的环境
	 * @return 0 成功, 其他为失败错误码
	 */
	virtual int __stdcall KxERevertSelf() = 0;

	/**
	 * @brief 获取连接属性
	 * @param[out] pEnumType 返回的通信类型
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall KxEGetCommunicatieType(
		ENUM_KXE_COMMUNICATION_TYPE* pEnumType
		) = 0;

	/**
	 * @brief 获取连接属性
	 * @param[out] pdwProcessId 源进程ID
	 * @return 0 成功，其他为失败错误码
	 */
	virtual int __stdcall KxEGetClientProcessId(
		DWORD* pdwProcessId
		) = 0;
};

/**
* @brief 向IPC服务注册目录处理重定向的接口
* @see IKxERequestDispatchReturnBlock
*/
class IKxERequestDispatch
{
public:
	/**
	* @brief IPC获取到/SP/pszMethod上的请求时,就会回调到此接口上
	* @param[in] pszMethod 为注册时重定向的路径名
	* @param[in] pszRequest 完整的IPC Request
	* @param[in, out] pReturnBlock 在用户程序处理完成后,返回时的数据存储的结构
	* @param[in] pControl 用于当前连接控制的参数
	*/
	virtual int __stdcall Dispatch(
		const char* pszMethod, 
		const char* pszRequest, 
		IKxERequestDispatchReturnBlock* pReturnBlock,
		IKxERequestControl* pControl
		) = 0;
};

