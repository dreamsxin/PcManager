////////////////////////////////////////////////////////////////////////////////
//      
//      File for url cache manager
//      
//      File      : urlcachedata.h
//      Version   : 1.0
//      Comment   : 网盾反欺诈网站检测url数据描述
//      
//      Create at : 2010-2-3
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../common/sharememlock.h"

typedef enum _URL_TYPE
{
	enumUrlType_UnIdent = -1,		// 未鉴定
	enumUrlType_ExtAttri = 0x80000000,		// 扩展属性
	enumUrlType_Unk = 0,		// 未知
	enumUrlType_InIdent = 1,     // 鉴定中
	enumUrlType_Safe = 2,         // 安全网站
	enumUrlType_Horse = 4,       // 被挂马
	enumUrlType_Phishing = 8,    // 钓鱼网址
	enumUrlType_PhishingDownload = 16,   // 欺诈下载
	enumUrlType_AdSide = 32,      // 广告网站
	enumUrlType_Sexy = 64,         // 色情网站
	enumUrlType_LineToMalSide = 128,        // 重定向到恶意网站
	enumUrlType_ExploitAttSide = 256,        // 带缓冲区溢出攻击网站
	enumUrlType_MalSide = 512,                 // 其它恶意网站
	enumUrlType_IEIndex = 1024,		// 从ieindex中导入的host
	enumUrlType_WhiteDomain = 2048,		// 域名在白名单中
} URL_TYPE;

#define MAKE_URL_TYPE(p, v, t) { *p = v | t ;}

typedef enum _URL_TYPE_EXT
{
	enumUrlTypeExt_None=0,
	enumUrlTypeExt_QQPhishing,           // QQ欺诈网站 包括中奖之类的
	enumUrlTypeExt_TaobaoPhishing,  // Taobao的欺诈网站
	enumUrlTypeExt_PhishingDownLink,   // 下载链接是欺诈
	enumUrlTypeExt_PhishingLink,      // 下载链接是欺诈
	enumUrlTypeExt_Relocat,             // 需要重导向到网站上的
} URL_TYPE_EXT;

//////////////////////////////////////////////////////////////////////////
/*
提交
http://www.pc120.net/check.asp?url=xxxxxx
xxxxx为提交的URL全网址

返回值
1::80000000::00000000::0::(重定向网址)

各项返回值用双冒号分隔

A:
网址鉴定状态
0->未鉴定(还未入列队鉴定)
1->已鉴定完 有结果的
2->鉴定中(几分钟后会有结果)

B:
网址类型(1) DWORD
UrlType_HaveExtType	0x80000000	//是否还有下一个扩展属性
UrlType_Unk		0x0	// 未知网站
UrlType_Safe		0x1	// 安全网站
UrlType_Horse	0x2	// 被挂马
UrlType_Phishing	0x4	// 钓鱼网站
UrlType_AdSide	0x10	// 广告网站
UrlType_Sexy		0x20	// 成人网站
UrlType_LinkToMalSide	0x40	//网站重定向到恶意网站
UrlType_ExploitAttSide	0x80	// 网站包含恶意溢出代码
UrlType_MalSide	0x100	// 其它恶意网站

C:
网址类型(2) DWORD
UrlTypeExt_HaveExtType	0x80000000

D:
是否需要重定向
0->不需重定向
1->重定向到指定网址

E:
最后有一个重定向网址
重定向到网站的URL不需重定向的话可为空

网址类型是至少有一项,由最高位决定是否还有下一个网址类型,
它是由标志位组成的 如一个网站是一个已知的钓鱼网站,
而且服务器要重定向到htt
那么返回值应该是
1::00000004::1::
*/

#define UrlType_HaveExtType	0x80000000	//是否还有下一个扩展属性
#define UrlType_Unk		0x0	// 未知网站
#define UrlType_Safe		0x1	// 安全网站
#define UrlType_Horse	0x2	// 被挂马
#define UrlType_Phishing	0x4	// 钓鱼网站
#define UrlType_AdSide	0x10	// 广告网站
#define UrlType_Sexy		0x20	// 成人网站
#define UrlType_LinkToMalSide	0x40	//网站重定向到恶意网站
#define UrlType_ExploitAttSide	0x80	// 网站包含恶意溢出代码
#define UrlType_MalSide	0x100	// 其它恶意网站

typedef enum _MATCH_TYPE
{
	enumUndef = 0,
	enumDomainWithoutPort,
	enumDomainWithPort,
	enumParantDomain,
	enumFullUrl,
	enumUrlWithoutParam,
	enumIPAddress,
	enumIPAddressWithPort
} MATCH_TYPE;

//////////////////////////////////////////////////////////////////////////
struct UrlCacheItem
{
	BYTE		urlMd5[16];
	URL_TYPE	enumUrlType;
	MATCH_TYPE	enumMatchType;
	__time64_t	lastUpdateTime;
	DWORD		m_nReserve[4];

	UrlCacheItem&	operator= (const UrlCacheItem& item2)
	{
		memcpy(this, &item2, sizeof(UrlCacheItem));
		return *this;
	}
};

inline bool operator< (const UrlCacheItem& item1, const UrlCacheItem& item2)
{
	return memcmp(item1.urlMd5, item2.urlMd5, sizeof(item1.urlMd5)) < 0;
}

inline bool operator== (const UrlCacheItem& item1, const UrlCacheItem& item2)
{
	return memcmp(item1.urlMd5, item2.urlMd5, sizeof(item1.urlMd5)) == 0;
}

typedef struct _tagUrlCacheMapDescription
{
	ShareMemLock			m_lock;
	DWORD					m_nMaxItemCnt;
	DWORD					m_nUsedItemCnt;
	DWORD					m_nReserve[10];
	UrlCacheItem			m_CacheItems[1];
}UrlCacheMapDescription;

#define URL_CACHE_MAPDATA_NAME		(TEXT("Global\\34CA103F-299E-49eb-A0B1-BC09D2D1A64D"))

//////////////////////////////////////////////////////////////////////////
typedef struct _tagUrlCacheFileMapItem
{
	volatile DWORD			m_nMapType;				// map的类型
	volatile DWORD			m_nOffset;				// 相对于文件头位置
	volatile DWORD			m_nMapSize;				// 保留的大小
	volatile DWORD			m_nDataVersion;			// map的数据版本
}UrlCacheFileMapItem;

#define URL_CACHE_MAX_MAP_TABLE		(20)
#define URL_CACHE_FILE_MAGIC		('ULC')
#define URL_CAHCE_FILE_VERSION		((1 << 16) | 1)

typedef struct _tagUrlCacheFileHeader
{
	volatile DWORD			m_nMagic;				// 'URLC'
	volatile DWORD			m_nFileVersion;			// 文件版本
	ShareMemLock	m_Lock;					// 读写锁
	volatile DWORD			m_nMapCnt;				// 已经使用的map个数
	UrlCacheFileMapItem	m_MapTable[URL_CACHE_MAX_MAP_TABLE]; // map描述符
}UrlCacheFileHeader;

#define URL_CACHE_HOST				0
#define URL_CACHE_BLACKURL			1

#define URL_CACHE_SIZE			(1024*1024)