#pragma once

#include <atltime.h>
#include <common/utility.h>

enum T_ProxyMode{
	PROXY_MODE_NONE,
	PROXY_MODE_IE,
	PROXY_MODE_USER,
};

class CPubConfig
{
public:
	CPubConfig()
	{
		//CShandianPath::Instance().GetProductCfgPath(m_strIniPath, TRUE);
		CAppPath::Instance().GetLeidianCfgPath(m_strIniPath);
		m_strIniPath += _T("\\pubconfig.ini");
	}

	~CPubConfig(void)
	{

	}

	BOOL GetAutoUploadUnknownFiles()
	{
		return GetValue(_T("common"), _T("autoupload"), FALSE);
	}

	VOID SetAutoUploadUnknownFiles(BOOL bValue)
	{
		return SetValue(_T("common"), _T("autoupload"), bValue);
	}

	BOOL GetAutoReportError()
	{
		return GetValue(_T("common"), _T("autoreport"), TRUE);
	}

	VOID SetAutoReportError(BOOL bValue)
	{
		return SetValue(_T("common"), _T("autoreport"), bValue);
	}

	BOOL GetInstallDriverFailed()
	{
		return GetValue(_T("common"), _T("instdrvfail"), FALSE);
	}

	VOID SetInstallDriverFailed(BOOL bFailed=TRUE)
	{
		return SetValue(_T("common"), _T("instdrvfail"), bFailed);
	}

	BOOL GetTransScanIgnoreSafeFile()
	{
		return GetValue(_T("common"), _T("transscaningnore"), TRUE);
	}

	void SetTransScanIgnoreSafeFile(BOOL bValue)
	{
		return SetValue(_T("common"), _T("transscaningnore"), bValue);
	}

	BOOL GetScanAllDownloadProduct()
	{
		return GetValue(_T("common"), _T("scanalldownloadproduct"), TRUE);
	}

	void SetScanAllDownloadProduct(BOOL bValue)
	{
		return SetValue(_T("common"), _T("scanalldownloadproduct"), bValue);
	}

	BOOL GetScanAllIMProduct()
	{
		return GetValue(_T("common"), _T("scanallimproduct"), TRUE);
	}
	
	void SetScanAllIMProduct(BOOL bValue)
	{
		return SetValue(_T("common"), _T("scanallimproduct"), bValue);
	}

	// AVP PROXY SETTING
#define _DEFINE_DWORD_PAIR_(s, k, d)\
	DWORD Get##s##k() { return GetValue(_T(#s), _T(#k), d);};\
	void Set##s##k(DWORD v) { return SetValue(_T(#s), _T(#k), v);};
	
#define _DEFINE_STRING_PAIR_(s, k)\
	LPCTSTR Get##s##k(CString &v) { GetValue(_T(#s), _T(#k), v); return v; };\
	void Set##s##k(LPCTSTR v) { return SetValue(_T(#s), _T(#k), v);};
	
	// 
	_DEFINE_DWORD_PAIR_(Proxy, Mode, PROXY_MODE_IE);
	_DEFINE_DWORD_PAIR_(Proxy, Needauth, FALSE);
	_DEFINE_DWORD_PAIR_(Proxy, Bypassproxyserverforlocaladdress, TRUE);
	_DEFINE_DWORD_PAIR_(Proxy, Port, 8080);

	_DEFINE_STRING_PAIR_(Proxy, Host);
	_DEFINE_STRING_PAIR_(Proxy, Login);
	_DEFINE_STRING_PAIR_(Proxy, Password);
	

private:

	void SetValue(LPCTSTR lpKey, LPCTSTR lpName, LPCTSTR lpValue)
	{
		WritePrivateProfileString(lpKey, lpName, lpValue, m_strIniPath);
	}
	
	void SetValue(LPCTSTR lpKey, LPCTSTR lpName, DWORD dwValue)
	{
		TCHAR szValue[20];

		_sntprintf(szValue, 20, _T("%d"), dwValue);
		WritePrivateProfileString(lpKey, lpName, szValue, m_strIniPath);
	}

	LPCTSTR GetValue(LPCTSTR lpKey, LPCTSTR lpName, LPTSTR lpValue, DWORD dwLen, LPCTSTR lpDefault) const
	{
		GetPrivateProfileString(lpKey, lpName, lpDefault, lpValue, dwLen, m_strIniPath);
		return lpValue;
	}
	
	void GetValue(LPCTSTR lpKey, LPCTSTR lpName, CString &str, LPCTSTR lpDefault=NULL) const
	{
		TCHAR buffer[MAX_PATH] = {0};
		GetPrivateProfileString(lpKey, lpName, lpDefault, buffer, MAX_PATH, m_strIniPath);
		str = buffer;
	}
	
	DWORD GetValue(LPCTSTR lpKey, LPCTSTR lpName, DWORD dwDefault) const
	{
		return (DWORD)GetPrivateProfileInt(lpKey, lpName, dwDefault, m_strIniPath);
	}

private:
	CString		m_strIniPath;

public:

	struct T_ProxySetting
	{
		T_ProxySetting() 
			: nProxyMode(PROXY_MODE_IE)
			, nPort(0)
			, bNeedAuthorization(FALSE)
			, bBypassProxyServerForLocalAddresses(TRUE)
		{		
		}

		T_ProxySetting &operator = (const T_ProxySetting &o)
		{
			nProxyMode			= o.nProxyMode;
			strHost				= o.strHost;
			nPort				= o.nPort;

			bNeedAuthorization = o.bNeedAuthorization;
			strLogin			= o.strLogin;
			strPassword			= o.strPassword;

			bBypassProxyServerForLocalAddresses = o.bBypassProxyServerForLocalAddresses;
			return *this;
		}

		INT		nProxyMode;
		CString strHost;
		INT		nPort;

		// 使用验证
		BOOL	bNeedAuthorization;
		CString strLogin, strPassword;	

		// 本地不使用代理
		BOOL bBypassProxyServerForLocalAddresses;
	};

	void LoadProxyConfig( T_ProxySetting &proxy )
	{
		proxy.nProxyMode = GetProxyMode();
		if(proxy.nProxyMode!=PROXY_MODE_NONE)
		{
			if(proxy.nProxyMode==PROXY_MODE_USER)
			{
				GetProxyHost(proxy.strHost);
				proxy.nPort = GetProxyPort();
			}
			
			proxy.bNeedAuthorization = GetProxyNeedauth();
			if(proxy.bNeedAuthorization)
			{
				GetProxyLogin(proxy.strLogin);
				GetProxyPassword(proxy.strPassword);
			}
			proxy.bBypassProxyServerForLocalAddresses = GetProxyBypassproxyserverforlocaladdress();			
		}
	}

public:
	
};
