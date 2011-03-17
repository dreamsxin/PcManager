//IKwsPopObtainer.h

#ifndef __I_KWS_POP_OBTAINER_H__
#define __I_KWS_POP_OBTAINER_H__

#include <Unknwn.h>
#include <string>


enum E_KWS_POP_TYPE_ID
{
	enum_KPTI_Fishing		= 1,	//钓鱼泡泡
	enum_KPTI_Trojan		= 2,		//挂马泡泡
	enum_KPTI_LockHomePage	= 21,		//
	enum_KPTI_RemindLock	= 26,
	enum_KPTI_DllHijack		= 200,   //dll劫持拦截泡泡
	enum_KPTI_AdvRuleChange		= 500,   //广告过滤规则改变
    enum_KPTI_DownsafePopo = 600, //下载前拦截 提示泡泡
};

enum E_SAFE_DOWN_POP_TYPE
{
	enum_Safe_Down_Pop_Safe = 1,		// 安全泡泡
	enum_Safe_Down_Pop_Danger = 2,		// 危险泡泡
	enum_Safe_Down_Pop_Unkown = 3,		// 未知泡泡
};

enum E_URL_SEC_TYPE
{
	enum_Url_Sec_Type_Safe = 1,
	enum_Url_Sec_Type_Danger = 2,
	enum_Url_Sec_Type_Unknown = 3
};

enum E_Proc_Sec_Type
{

};

interface IKSafePopParam
{
	virtual	LPCWSTR	__stdcall GetFilePath() = 0;
	virtual	LPCWSTR	__stdcall GetDownloader() = 0;
	virtual	LPCWSTR	__stdcall GetVirus() = 0;
	virtual	int		__stdcall GetPopID() = 0;
	virtual	int		__stdcall GetStatus() = 0;
	virtual	int		__stdcall GetClear() = 0;
	virtual	int		__stdcall GetFileType() = 0;
	virtual	LPCWSTR	__stdcall GetDownloadTool() = 0;
	virtual	BOOL	__stdcall CanShow() = 0;
    virtual BOOL	__stdcall GetAllVirusFileName(BSTR& bstrFilesName) = 0;
    virtual BOOL	__stdcall IsArcEncrypt() = 0;
	virtual DWORD	__stdcall GetBindSetupType() = 0;
	virtual DWORD	__stdcall GetDownloadToolID() = 0;
};

interface IKSafeDownPopInfo
{
	virtual unsigned int __stdcall GetPopInfoCount() = 0;
	virtual IKSafePopParam*	__stdcall GetPopParam(unsigned int nIndex) = 0;
	virtual void		__stdcall ReleasePopInfo() = 0;
	virtual int			__stdcall GetPopType()		= 0;
	virtual int			__stdcall SetShowNext(BOOL bShow)	= 0;
	virtual int			__stdcall OpenFileDir(INT nIndex)		= 0;
	virtual int			__stdcall ClearVirus()		= 0;
	virtual int			__stdcall TrustFile()		= 0;
};

interface IKUrlMonPopInfo
{
	virtual void		__stdcall ReleasePopInfo()		= 0;
	virtual int			__stdcall GetPopType()			= 0;
	virtual	BOOL		__stdcall GetShowPop()			= 0;
	virtual	LPCWSTR		__stdcall GetUrl()				= 0;
	virtual	DWORD		__stdcall GetUrlSecurity()		= 0;
	virtual	DWORD		__stdcall GetUrlProperty()		= 0;
	virtual	LPCWSTR		__stdcall GetProcPath()			= 0;
	virtual	DWORD		__stdcall GetProcSecurity()		= 0;
	virtual	DWORD		__stdcall OnUpdateWhiteUrl()		= 0;
};

struct IPopObtainerCallback
{
	virtual BOOL __stdcall OnObtained(DWORD dwType, LPCWSTR szParam, DWORD nReason, LPCWSTR strUrl, LPCWSTR strBrowser) = 0;
	virtual BOOL __stdcall OnObtained(IKSafeDownPopInfo* pPopInfo) = 0;
	virtual BOOL __stdcall OnObtained(IKUrlMonPopInfo* pPopInfo) = 0;
};


struct IPopUserController
{
	virtual int  __stdcall GetPopType() = 0;
	// 调用者负责释放 bstrValue
	virtual BOOL __stdcall GetControllerValue(const wchar_t * wszKey, BSTR& bstrValue) = 0;
};



#if defined(_MSC_VER) && (_MSC_VER >= 1300) 
[
	uuid("A05599FF-E946-457e-95C6-D752263A330E")
]
#else //_MSC_VER
interface __declspec(uuid("A05599FF-E946-457e-95C6-D752263A330E")) IKwsPopObtainer;
#endif//_MSC_VER

interface IKwsPopObtainer : IUnknown
{
	virtual BOOL STDMETHODCALLTYPE Init(IPopObtainerCallback* lpCallback = 0) = 0;
	virtual BOOL STDMETHODCALLTYPE Start()	= 0;
	virtual BOOL STDMETHODCALLTYPE Stop()	= 0;
	virtual BOOL STDMETHODCALLTYPE Uninit() = 0;

	virtual BOOL STDMETHODCALLTYPE DoUserOperOnPop(IPopUserController* lpController = 0) = 0;
};

#endif
