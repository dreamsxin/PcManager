////////////////////////////////////////////////////////////////////////////////
//      
//      File for url cache manager
//      
//      File      : urlcachedata.h
//      Version   : 1.0
//      Comment   : Õ¯∂‹∑¥∆€’©Õ¯’æºÏ≤‚urlπ‹¿Ì∆˜
//      
//      Create at : 2010-2-3
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
using namespace std;

#include "../common/sharememlock.h"
#include "urlcachedef.h"
#include "../common/kis_lock.h"

//////////////////////////////////////////////////////////////////////////
class KUrlCacheData
{
private:
	UrlCacheMapDescription* m_pMapDescription;
	UrlCacheFileMapItem*	m_UrlMapData;

public:
	KUrlCacheData();
	~KUrlCacheData();

	HRESULT	Init(UrlCacheFileMapItem* pItem, LPVOID pMapData, BOOL bCheckFull);

	HRESULT	LockRead();
	HRESULT	FindItem(UrlCacheItem& item);
	INT		GetItemCnt();
	HRESULT	GetItem(INT nIndex, UrlCacheItem& item);
	HRESULT	UnLockRead();

	HRESULT	LockWrite();
	HRESULT AddUrlCacheItem(UrlCacheItem& item);
	HRESULT UpdateUrlCacheItem(UrlCacheItem& item);
	HRESULT	UnLockWrite();

	HRESULT	UpdateItemToSafe(BYTE md5[16]);

	DWORD	ResetUrlQueryCnt();
	DWORD	AddUrlQueryCnt();
	HRESULT	ClearCache();
private:
	HRESULT	InsertItem(UrlCacheItem& item, LONG nIndex);
};

