/* -------------------------------------------------------------------------
//	FileName	：	settingpagedef.h
//	Creator		：	zhoufeng
//	Date		：	2010/5/11 10:28:28
//	Brief		：	设置对话框的一些通用定义
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __SETTINGPAGEDEF_H__
#define __SETTINGPAGEDEF_H__

// -------------------------------------------------------------------------
enum EM_INIT_PAGE
{
	em_trustlist_page		= 0,		// 白名单设置
	em_adblack_page			= 1,		// 广告过滤规则设置
	em_adfilter_page		= 2,		// 广告拦截
	em_download_safe_page   = 3,		// 下载安全设置
	em_setbrower_page       = 4         // 浏览器设置
};

class ISettingUI
{
public:

	/**
	* @brief        查询某设置页是否已经修改了设置
	* @return		TRUE 已经修改，否则，为修改
	*/
	virtual BOOL __stdcall IsApplyEnable() = 0;

	/**
	* @brief        让某设置页保存设置
	* @return		TRUE 保存成功，否则保存失败
	*/
	virtual BOOL __stdcall DoSettingApply() = 0;

};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __SETTINGPAGEDEF_H__ */
