///////////////////////////////////////////////////////////////
//
//	Filename: 	KwsIPCWarpper.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-4-2  11:33
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _KwsIPCWarpper_old_h_
#define _KwsIPCWarpper_old_h_
#include "KwsSimpleIPCClientOld.h"
#include <atlstr.h>
#include <string>
#include <vector>


namespace kws_old_ipc {

BOOL KwsIsIPCInit();


//////////////////////////////////////////////////////////////////////////
//
// 托盘相关
//
//////////////////////////////////////////////////////////////////////////

/**
* @brief   获取托盘状态 (包括 是否变灰、是否隐藏、是否应该退出、是否需要弹泡泡)
*/
int KwsQueryTrayStatus( TRAYSTATUS* pnTrayStatus , DWORD *pdwSession);



/**
* @brief   设置托盘点亮状态 ( 点亮 or 变暗 )
           为兼容老代码 暂时没改名字
*/
int SetTrayStatus( bool bOn );

/**
* @brief   设置是否显示托盘
*/
int ShowTray( bool bShow );


/**
* @brief   退出托盘
*/
int KwsQuitTray();



//////////////////////////////////////////////////////////////////////////
//
// 泡泡相关
//
//////////////////////////////////////////////////////////////////////////

/**
* @brief   把泡泡信息添加到服务的泡泡队列中, 
           托盘会从队列中获取并弹出泡泡

	nReason 表示弹出泡泡的原因（默认-1表示没有原因）	
*/
int ShowPopo2( int nPopoStyle, const wchar_t* pwszMsg, bool bForceNewPage, int nReason = -1);

/**
* @brief   从服务的泡泡队列里获取一个泡泡信息
           如果pbExists返回FALSE, 表示泡泡队列为空
*/
struct POPOINFO
{
	int  nPopoStyle;
	std::wstring strMsg;
	std::wstring strBtn;
};

int QueryPopoMessage( BOOL* pbExists, POPOINFO* pPopoInfo );



//////////////////////////////////////////////////////////////////////////
//
// 日志相关
//
//////////////////////////////////////////////////////////////////////////
/**
* @brief   记录日志
*/
int KwsLog( const wchar_t* pwszFile, const wchar_t* pwszMsg );

/**
* @brief   删除日志
*/
int KwsDeleteLog( const wchar_t* pwszFile, LOGTYPE lt = TY_ALL);


//////////////////////////////////////////////////////////////////////////
//
// 调用升级
//
//////////////////////////////////////////////////////////////////////////
int KwsCallAsUser( const wchar_t* pszCmdline );

int KwsCallAsSystem( const wchar_t* pszCmdline );

int KwsCallAsUserOrSystem( const wchar_t* pszCmdline );

int KwsInjectSvc(const wchar_t* pszBp);

int KwsRegNotifyChange();

//////////////////////////////////////////////////////////////////////////
//
// 当前被保护进程
//
//////////////////////////////////////////////////////////////////////////
int KwsSetProgramPath( const wchar_t* pszPath, DWORD dwProcessID );

int KwsGetProgramPath( wchar_t* pszPath, int nSize, DWORD* pdwProcessID);

//////////////////////////////////////////////////////////////////////////
//
// Setting
//
//////////////////////////////////////////////////////////////////////////
int KwsSetProfileStringValue(const wchar_t *pszKey, const wchar_t *pszValue);

int KwsGetProfileStringValue(const wchar_t *pszKey, std::wstring &strValue, const wchar_t *pszDefault);

int KwsSetRegStringValue(const wchar_t *pszKey, const wchar_t *pszSubKey, const wchar_t *pszValue);

int KwsGetRegStringValue(const wchar_t *pszKey, const wchar_t *pszSubKey, std::wstring &strValue, const wchar_t *pszDefault);

int KwsSetRegDWORDValue(const wchar_t *pszKey, const wchar_t *pszSubKey, DWORD dwValue);

int KwsGetRegDWORDValue(const wchar_t *pszKey, const wchar_t *pszSubKey, DWORD &dwValue, DWORD dwDefault);

int KwsDeleteRegValue(const wchar_t *pszKey, const wchar_t *pszSubKey);

int KwsSetCRRegStringValue(const wchar_t *pszKey, const wchar_t *pszSubKey, const wchar_t *pszValue);

//////////////////////////////////////////////////////////////////////////
//
// 黑名单
//
//////////////////////////////////////////////////////////////////////////
int KwsBlackListAdd(int nReason, const wchar_t *pszUrl);

int KwsBlackListDelete(const wchar_t *pszUrl);

int KwsBlackListExport(const wchar_t *pszFilePath, int nADID = -1);

int KwsBlackListImport(const wchar_t *pszFilePath, 
                       const wchar_t *pszItemFlag = L"1",
                       const wchar_t *psziid = L"10");

int KwsBlackListUnSubscribe(const wchar_t *pszItemFlag = L"1",
                            const wchar_t *psziid = L"10");

int KwsBlackListAddEx(int nReason, const wchar_t *pszUrl, std::wstring& strFtUrl,
                      std::wstring& wstrexist, int nADID = 0);

//////////////////////////////////////////////////////////////////////////
//
// 恶意首页
//
//////////////////////////////////////////////////////////////////////////
/**
*  nType 表示恶意首页类型（1表示用户定义的，2表示非用户定义的）	
*/
int KwsSSPAdd(int nType, const wchar_t *pszUrl);

int KwsSSPDelete(const wchar_t *pszUrl);

/**
*  nType 表示恶意首页类型（1表示用户定义的，2表示非用户定义的，0表示获取所有）	
*/
int KwsSSPGet(int nType, std::vector<std::wstring>& vecSsps);

/**
*  删除自动扫描的恶意首页
*/
int KwsSSPDeleteAuto();

//////////////////////////////////////////////////////////////////////////
//
// 网页净化
//
//////////////////////////////////////////////////////////////////////////
int KwsFRAdd(int nSet, const wchar_t *pszFR);

int KwsFRDelete(const wchar_t *pszFR);

int KwsFRExport(const wchar_t *pszFilePath);

int KwsFRImport(const wchar_t *pszFilePath);

//////////////////////////////////////////////////////////////////////////
/**
* @brief   记录日志
*/
int KwsWriteDanUrl( const wchar_t* pwszFile, const wchar_t* pwszMsg );

//////////////////////////////////////////////////////////////////////////

/**
*  获取sp所在目录
*/
int KwsGetSpDir(char* pszDir, unsigned uLen);

/**
*  获取网盾版本类型
*/
int KwsGetKwsType(int* pnType);

}

#endif // _KwsIPCWarpper_h_
