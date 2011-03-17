//////////////////////////////////////////////////////////////////////
///		@file		ikxeauthorizeserviceprovider.h
///		@author		luopeng
///		@date		2008-11-11 09:07:58
///
///		@brief		HTTP的连接授权服务提供者
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../include/idl.h"

class IKxEAuthorizeServiceProvider
{
public:
	/**
	 * @brief 客户端提出需要进行授权
	 * @param[in] strClientAuthorizeInfo 客户端发出的授权信息,用于服务器识别客户端
	 * @param[in] strServerReplyInfo 服务端返回的信息,用于后续连接及客户端识别服务器
	 * @return 0 成功, 其他为失败错误码
	 */
	virtual int __stdcall Authorize(
		const std::string& strClientAuthorizeInfo,
		std::string& strServerReplyInfo
		) = 0;
};

KXE_RCF_BEGIN(IKxEAuthorizeServiceProvider, "KxEAuthorizeService")
	KXE_RCF_METHOD(Authorize, std::string, std::string)
KXE_RCF_END()