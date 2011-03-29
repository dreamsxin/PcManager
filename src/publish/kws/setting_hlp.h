////////////////////////////////////////////////////////////////////////////////
//      
//      File for kws 3.5 
//      
//      File      : setting_hlp.h
//      Version   : 1.0
//      Comment   : 设置辅助类
//      
//      Create at : 2010-03-02
//		Create by : chenguicheng  
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "KwsSetting.h"
#include "ctrls/kcombobox.h"

#define KB_1000			(1000)
#define	KB_BYTES		(1024)
#define MB_BYTES		(1024*1024)
#define GB_BYTES		(1024*1024*1024)

/**
*   广告拦截规则包项
*/
typedef struct tagAdRulePackItem
{
	DWORD     nTotal;      /* 包含规则数 */
	WORD      nPackID;     /* 包ID */
	CString   strName;     /* 包名 */
	CString   strTime;     /* 导入时间 */

} ADRULEPACKITEM;


typedef std::vector<ADRULEPACKITEM> AdRulePackArray;
typedef std::vector<int>            AdRuleTotalMap;
/**
*   广告拦截规则项
*/
typedef struct tagBlackListItem 
{
    WORD	        wMc;
	WORD			wType;
	WORD            wPackID;
	CString	        strUrl;
} BLACKLISTITEM;

typedef std::vector<BLACKLISTITEM>   AdRuleArray;

typedef struct tagBrowserItem
{
	BOOL			bDefault;
	ATL::CString	strPath;
	ATL::CString	strFileName;
	HICON			hIcon;
	ATL::CString	strText;
	BOOL			bUserAdd;
}BROWSERITEM;

typedef struct tagOemItem
{
	ATL::CString	strTitle;
	ATL::CString	strUrl;
}OemItem;

typedef enum 
{
	UT_VIRUSWEB		= 1,
	UT_FISHWEB		= 3,
	UT_ADVWEB		= 4,

}UrlType;

#define	    KCOMMON_DEFAULT_BROWSER_REY_PATH		TEXT("HTTP\\shell\\open\\command")
#define	    KCOMMON_DEFAULT_BROWSER_REY_PATH_VISTA	TEXT("KWS.HTTP\\shell\\open\\command")


interface IBlackListCallback
{
public:
	virtual int HandleBlackListItem(BLACKLISTITEM& item) = 0;
};

class CSettingHlp
{
public:
	CSettingHlp();
	~CSettingHlp();
	static CSettingHlp& Instance();

	BOOL GetIEStartPage( ATL::CString& strPage);
	BOOL CreateComBoBox( HWND hParent,KComboBox* pComBoBox, UINT uID );
	CString UrlToName( LPCTSTR pszUrl );
	CString NameToUrl( LPCTSTR pszUrl );
	bool ChildToParentRect( HWND hChild, HWND hParent, LPRECT rect );
	HRESULT	GetCustomMainPage(void);

	//上边2个角弯角
	void RoundWinRect( CRect &rect, int nCor, HWND hWnd );
	//这个角弯角
	void RoundWinRect4( CRect &rect, int nCor, HWND hWnd );
	void FluxToString(double flux, ATL::CString& str);

	BOOL RegSetStartPage(PCWCH pcwszUrl, HKEY hKey);
	BOOL GetBrowserInfo( std::vector<BROWSERITEM*>& vecList );
	BOOL SetDefBrowser( LPCTSTR pszBrowPath, BOOL bLock = FALSE );
	BOOL GetBlackList(std::vector<BLACKLISTITEM>& vecList, int nPackID = -1);
	BOOL GetBlackListEx(IBlackListCallback &callback, int nPackID = -1);
	BOOL GetUserAllAdRulePackNames(AdRulePackArray& packarray);
	BOOL GetAllAdRuleTotal(AdRuleTotalMap &totalmap);
    static unsigned __stdcall ChkHomepageThread(void* pThis);
protected:
	BOOL GetDefaultBrowser(std::wstring& wstrDefaultBrowser);
	BOOL setDefaultBrowser( LPCWSTR wstrBrowserPath , BOOL bLock = FALSE );
	void ProcessUnhandledMessages();//hub
	std::vector<OemItem>	m_vecOemInfo;
    std::wstring m_wstrSp;
    BOOL m_bWhiteSp;
};