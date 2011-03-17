//////////////////////////////////////////////////////////////////////
//  @ File		KSRTrustList.h
//  @ version	1.0
//  @ Author	梁光彩<liangguangcai@kingsoft.com>
//  @ Date		[2010-3-16, 14:43:44]
//  @ Brief		系统修复用户设置的信任列表
//					
//  @ Last changed
//    *Who		
//    *When		
//    *What		
//////////////////////////////////////////////////////////////////////

#pragma once

//	用户信任列表配置文件名
#define USER_TRUST_LIST_INI_FILE_NAME_W			L"tl.dat"
//	Url 信任列表
#define USER_TRUST_LIST_URL_SECTION_NAME_W		L"UrlList"
//	文件信任列表
#define USER_TRUST_LIST_FILE_SECTION_NAME_W		L"FileList"
//	注册表路径信任列表
#define USER_TRUST_LIST_REG_SECTION_NAME_W		L"RegList"
//	系统修复脚本 UUID 信任列表
#define USER_TRUST_LIST_SCRIPT_SECTION_NAME_W	L"ScriptList"

typedef enum _EM_TRUST_ITEM_TYPE
{
	enum_TRUST_ITEM_TYPE_INVALID	= 0,	///< 无效类型
	enum_TRUST_ITEM_TYPE_URL		= 1,	///< 信任 Url
	enum_TRUST_ITEM_TYPE_FILE		= 2,	///< 信任文件
	enum_TRUST_ITEM_TYPE_REG		= 3,	///< 信任注册表路径
	enum_TRUST_ITEM_TYPE_SCRIPT		= 4		///< 信任脚本 UUID
} EM_TRUST_ITEM_TYPE;

typedef enum _EM_SPECIAL_ACTION_TYPE
{
	enum_SPECIAL_ACTION_TYPE_REFRESH_DESKTOP	= 1,	///< 刷新桌面
	enum_SPECIAL_ACTION_TYPE_REBOOT				= 2		///< 重启电脑
} EM_SPECIAL_ACTION_TYPE;

