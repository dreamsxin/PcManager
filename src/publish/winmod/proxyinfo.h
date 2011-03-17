
#pragma once


// 目前支持的代理类型 [1/11/2011 zhangbaoliang]
namespace PROXY_TYPE
{
	enum {
		PROXY_TYPE_UNKNOWN		= 0,
		PROXY_TYPE_HTTP			= 1,
		PROXY_TYPE_SOCKS5		= 2,
		PROXY_TYPE_ALL			= 0x1000
	};
}

namespace BKSAFE_PROXY_INFO
{
	typedef struct _UPLIVE_PROXY_INFO
	{
		BOOL		bProxyValid;
		int			nProxyType;
		int			nVerify;
		CString		m_strProxyUserName;
		CString		m_strProxyPassWord;
		CString		m_strProxyHost;
		CString		m_strProxyPort;

		_UPLIVE_PROXY_INFO()
		{
			m_strProxyUserName.Empty();
			m_strProxyPort.Empty();
			m_strProxyHost.Empty();
			m_strProxyPassWord.Empty();
			bProxyValid = FALSE;
			nProxyType = -1;
			nVerify = -1;
		}

		_UPLIVE_PROXY_INFO(CString strHost, CString strPort, CString strUser, CString strPwd, int nProxyType, int nVerify)
		{
			bProxyValid					= TRUE;
			nProxyType					= nProxyType;
			nVerify						= nVerify;
			m_strProxyHost				= strHost;
			m_strProxyPassWord			= strPwd;
			m_strProxyPort				= strPort;
			m_strProxyUserName			= strUser;
		}

	}UPLIVE_PROXY_INFO, *LPUPLIVE_PROXY_INFO;
}
