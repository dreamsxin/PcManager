///////////////////////////////////////////////////////////////
//
//	Filename: 	KwsSetting.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-3-18  19:06
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _KwsSetting_old_h_
#define _KwsSetting_old_h_
#include <vector>
#include <string>
#include <atlstr.h>

using namespace std;

namespace kws_old_ipc{


typedef int SHINELEVEL;
#define SHINELEVEL_NORMAL 2
#define SHINELEVEL_HIGH   5
#define SHINELEVEL_LOW    1


typedef struct AdGuidVersion
{
    AdGuidVersion()
    {
        strguid = "";
        dwversion = 0;
    }
    std::string strguid;
    DWORD dwversion;
}*pAdGuidVersion;


typedef struct AdGuidVerID
{
    AdGuidVerID()
    {
        nid = 1;
    }
    int nid;
    AdGuidVersion adguidver;
}*pAdGuidVerID;

class KwsSetting
{
public:
	KwsSetting();
	~KwsSetting();

public:
	// 开启关闭保护
	BOOL IsKwsEnable();
	BOOL SetKwsEnable( BOOL bEnable );
	BOOL IsKwsEnableEx( BOOL& bEnable ); // 返回值判断是否服务异常


	// 托盘图标
	BOOL IsTrayIconEnable();
	BOOL EnableTrayIcon( BOOL bEnable );


	// 在线升级
	BOOL OnlineUpdate();


	// 闪框效果
	int IsShineEnable();
	BOOL SetShineEnable( int bEnable );

	COLORREF GetSafeColor();
	BOOL SetSafeColor( COLORREF cr );

	COLORREF GetWarnColor();
	BOOL SetWarnColor( COLORREF cr );

	COLORREF GetDangerColor();
	BOOL SetDangerColor( COLORREF cr );


	
	SHINELEVEL GetShineLevel();
	BOOL SetShineLevel( SHINELEVEL nLevel ); 


	// 黑名单
	//BOOL GetBlackUrlList( std::vector<std::string>& vecList ); 
	//BOOL SetBlackUrlList( const std::vector<std::string>& vecList );
	//BOOL IsUrlInBlackList( const char* pszUrl );

	// 检测是否有需要升级(打开主界面调起升级程序的状态)
	BOOL GetUpdStatus(DWORD& dwValue);
	BOOL SetUpdStatus(DWORD dwValue);

	//  自定义保护进程
	void GetShieldProcess(std::vector<std::wstring> &vecProcesses);
	BOOL SetShieldProcess(const std::vector<std::wstring> vecProcesses);

	// 获取最后一次升级的时间
	BOOL GetLastUpdateTime(LPWSTR strTime, size_t nSize);
	BOOL SetLastUpdateTime(LPCWSTR strTime);

    BOOL KwsSetTime(INT64 nTime, LPCWSTR lpwname);
    BOOL KwsGetTime(INT64 *nTime, LPCWSTR lpwname);

	// 用户最后一次反馈的时间
	BOOL GetLastFeedbackTime(INT64* nTime);
	BOOL SetLastFeedbackTime(INT64 nTime = 0);

	// 获取Uuid
	BOOL GetUuid(std::wstring &strUuid);

	// 获取通道号
	BOOL GetChannelCode(DWORD& dwMajCode, DWORD& dwSlvCode);

	// 是否第一次运行
	BOOL IsFirstRun();
	BOOL SetFirstRun();

	//DWORD GetDangerCount();
	BOOL SetDangerCount(DWORD uCount);
	DWORD GetProtectedDay();
	BOOL SetSetupTime(INT64 time = 0);

	// 保护程序时托盘闪动后是否提示　（方法已经作废）
	BOOL IsShowTrayBalloon();
	BOOL SetShowTrayBalloon(BOOL bShow);

	// 托盘闪动
	BOOL IsShowTrayShine();
	BOOL SetShowTrayShine(BOOL bShow);


	// 密保检测
	BOOL IsScanedKPP();
	BOOL SetScanedKPP(BOOL bScaned);

	// 搜索引擎
	BOOL GetSearchEngine(DWORD &dwVal);
	BOOL SetSearchEngine(DWORD dwVal);

	// 是否需要弹阻塞框
	BOOL IsNeedBlockDialog();
	BOOL SetNeedBlockDialog(BOOL bNeed);

	// 最后一次上报活跃的时间
	BOOL GetLastReportTime(INT64* nTime);
	BOOL SetLastReportTime(INT64 nTime = 0);

	// 是否需要推荐密保
	BOOL SetNeedRcmKpp(BOOL bVal);

	// 是否遨游版
	BOOL IsForMaxthon();

	//是否是毒霸版
	BOOL IsForDuba();

	// 获取网盾版本
	BOOL GetKwsVt(DWORD& dwVt);

	//锁定首页
    BOOL GetStartPage(std::wstring& strSP);
	BOOL SetStartPage(LPCWSTR strSP);

	// 开启关闭锁定首页
	BOOL IsLockSPEnable();
	BOOL SetLockSPEnable( BOOL bEnable );
	BOOL SetLockSPFirstShow( BOOL bEnable );
	BOOL IsLockSPFirstTipShowed();

	// 下载保护	
	BOOL IsDPEnable();
	BOOL SetDPEnable( BOOL bEnable );
	BOOL GetDPSoft( DWORD &dwVal );
	BOOL SetDPSoft( DWORD dwVal );

	// 网页净化
	BOOL IsHtmFilterEnable();
	BOOL SetHtmFilterEnable( BOOL bEnable );

	// 专版
	BOOL GetSvt( DWORD &dwSvt );
	BOOL SetSvt( DWORD dwSvt );
	BOOL GetSvtName(std::wstring &strName);
	BOOL GetSvtUrl(std::wstring &strSvtUrl);
	BOOL GetUrlChn(std::wstring &strUrlChn);

	// 鼠镖
	BOOL IsMouseProEnable();
	BOOL SetMouseProEnable( BOOL bEnable );

	//是否是百度第一次执行
	DWORD GetBaiduFirstShow();
	BOOL SetBaiduFirstShow( DWORD dwvalue );
	BOOL IsCloseBaiduShowed();
	BOOL SetCloseBaiduShowed();

	// 是否需要推荐急救箱
	BOOL SetNeedRcmKsm(BOOL bVal);

	// 搜索引擎保护
	BOOL IsBrowserProEnable();
	BOOL SetBrowserProEnable( BOOL bEnable );
	BOOL GetBrowserPath(std::wstring& strBsPath);
	BOOL SetBrowserPath(LPCWSTR strBsPath);
    BOOL SetLockDefBsWarpper(BOOL bEnable, const wchar_t* pwszBsPath);

	// 浏览器锁定总开关（这个关闭则浏览器保护无效）
	BOOL IsBrowserLockEnable();
	BOOL SetBrowserLockEnable( BOOL bEnable );

	// 最后扫描恶意首页的时间
	BOOL GetLastEnumEUTime(INT64* nTime);
	BOOL SetLastEnumEUTime(INT64 nTime = 0);

	// 开启关闭钓鱼保护
	BOOL IsFishingEnable();
	BOOL SetFishingEnable( BOOL bEnable );

	// 判断当前用户有没有订阅广告
	BOOL IsAntiAdEnable();
	VOID SetAntiAdEnable(BOOL bEnable);
    BOOL IsAnyRuleSubscribed();
	// 广告的guid和version
    BOOL GetAdGuid_Version(std::vector<AdGuidVersion>& vecadguidversion);
    BOOL SetAdGuid_Version(LPCTSTR strguidversion);
    BOOL GetAdGuid_VersionAsString(std::string& strguid_version);
    //主页被改的次数
    BOOL GetSpModifyCount(DWORD& dwCount);
    BOOL IncSpModifyCount();
    BOOL ResetSpModifyCount();

	// 360浏览器弹泡提示
	BOOL NeedShow360SEPopo();
	BOOL SetShow360SEPopo(BOOL bNeed);

	// 开启关闭地址栏图标
	BOOL IsABIconEnable();
	BOOL SetABIconEnable( BOOL bEnable );

	// 开启关闭upreport上报
	BOOL IsUpReportEnable();
	BOOL SetUpReportEnable( BOOL bEnable );

    // IE提示广告过滤开关
    BOOL IsTipSpannerEnable();
    BOOL SetupTipSpannerEnable( BOOL bEnable );

    //强转到主页的次数加1
    BOOL IncReNavigateCount(BOOL bReset = 0);

    //读取强转到主页的次数
    BOOL GetReNavigateCount(DWORD& dwReNvCount);

    //检查MAXTHON浏览器锁定不再弹泡标志
    BOOL IsMaxBsptNotifyDisable();
    BOOL SetMaxBstrNotifyDisable();

    //首页推荐泡泡是否再弹
    BOOL IsLockSpNotifyDisable();
    BOOL SetLockSpNotifyDisable();
    // 判断今天已经帮用户拦截到广告URL了
    BOOL IsAnyURLBeenBlockedToday();

    // 标记今天已经帮用户拦截到广告URL了
    BOOL MarkURLBeenBlockedToday();

    //设置获取获取上次推主页扫描弹框时间
    BOOL SetLastScanSpNotifyTime(INT64 time = 0);
    BOOL GetLastScanSpNotifyTime(INT64* time);

	// 判断在WIN7托盘图标显示在外面设置是否已经进行过了
	BOOL IsTrayIconShowOutCfg();
	BOOL SetTrayIconShowOut();

	//广告过滤是否需要上pc120同步
	BOOL IsWebRuleNeedCheck();
	//广告过滤设置是否上pc123同步
	void SetWebRuleCheck( BOOL bCheck );

    // 判断广告过滤的日志功能是否开启. 目前广告过滤的日志功能仅仅提供给
    // 内部使用. 所以界面上没有提供接口设置这个开关.
    BOOL IsAntiAdvLogEnabled();
	BOOL SetAntiAdvLogEnable(BOOL bEnable);

    // 最近一次IE推荐广告过滤的时间
    BOOL GetLastTipsSbanRecommendTime(INT64& iDate);
    BOOL SetLastTipsSbanRecommendTime(INT64 iDate);
    BOOL SetLastTipsSbanRecommendTimeAsNow();

    BOOL GetLastRecommendDate(UINT &nDate);
    BOOL SetLastRecommendDate();

	// 是否需要修改快捷方式
	BOOL IsNeedChangeSc();
	BOOL SetNotNeedChangeSc();

	// 用户自定义添加的浏览器
	BOOL GetSelfDefBrowserPath(std::wstring& strBsPath);
	BOOL SetSelfDefBrowserPath(LPCWSTR strBsPath);

    //开启傲游浏览器主页锁定
    BOOL IsNeedLockMaxSp();
    BOOL SetLockMaxSp(BOOL bNeed);
    BOOL SetNeverPopLockMaxSp(BOOL bNever);
    BOOL IsNeedPopLockMaxSp();

	//是否需要显示mini向导
	BOOL IsMiniSiteShow();
	BOOL SetMiniSiteShow( BOOL bShow );

	// 傲游版mini向导
	BOOL IsMxMiniSiteShow();
	BOOL SetMxMiniSiteShow(BOOL bShow);

	// 傲游版第二版mini页
	BOOL IsMxMiniSite2Show();
	BOOL SetMxMiniSite2Show(BOOL bShow);

	// 修改傲游版配置的开关
	BOOL IsNeedFixMxCfg();
	BOOL SetNeedFixMxCfg(BOOL bNeed);

	// 一键修复
	BOOL SetOnekeyAlready();

	// 开启下载安全
	BOOL IsAutoScanEnable();
	BOOL SetAutoScanEnable(BOOL b);

	BOOL IsAutoCleanVirus();
	BOOL SetAutoCleanVirus(BOOL b);

	BOOL IsShowSafePop();
	BOOL SetShowSafePop(BOOL b);

	BOOL IsShowGrayPop();
	BOOL SetShowGrayPop(BOOL b);

	BOOL IsFileHelperEnable();
	BOOL SetFileHelper(BOOL bValue);

	//3.6mini功能推荐
	BOOL IsMini36Show();
	BOOL SetMini36Show( BOOL bShow );

	//下载安全dfimport.exe是否启用
	INT	 GetDownFileImportFlag();
	BOOL SetDownFileImportFlag(INT nFlag);

	//下载安全是否显示tip泡泡
	BOOL DSNeedShowTip();
	BOOL DSSetShowTip(BOOL bShow);

    //收集Application Data\\Kingsoft\\KIS\\KCLT\\public_kws.inf信息
    //BOOL GetCommonInfo(std::string& strvalue);

private:
	LONG KwsSettingGetDWORDValue( LPCTSTR pszKey, DWORD& dwValue, DWORD dwDefault );
	LONG KwsSettingGetStringValue( LPCTSTR pszKey, ATL::CAtlString& strValue, LPCTSTR pszDefault );

	LONG KwsSettingSetDWORDValue( LPCTSTR pszKey, DWORD dwValue );
	LONG KwsSettingSetStringValue( LPCTSTR pszKey, LPCTSTR pszValue );

private:
	LONG KwsSettingGetRegDWORDValue( LPCTSTR pszKey, LPCTSTR pszSubKey, DWORD& dwValue, DWORD dwDefault );
	LONG KwsSettingGetRegStringValue( LPCTSTR pszKey, LPCTSTR pszSubKey, ATL::CAtlString& strValue, LPCTSTR pszDefault );

	LONG KwsSettingSetRegDWORDValue( LPCTSTR pszKey, LPCTSTR pszSubKey, DWORD dwValue );
	LONG KwsSettingSetRegStringValue( LPCTSTR pszKey, LPCTSTR pszSubKey, LPCTSTR pszValue );

	LONG KwsSettingDeleteRegValue( LPCTSTR pszKey, LPCTSTR pszSubKey );

	
};

}

#endif // _KwsSetting_h_
