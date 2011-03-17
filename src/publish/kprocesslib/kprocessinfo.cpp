#include "StdAfx.h"
#include<map>
#include "kprocessinfo.h"
#include "kprocessitem.h"

KProcessInfo::KProcessInfo(void)
{
}

KProcessInfo::~KProcessInfo(void)
{
}

BOOL KProcessInfo::AddProcessItem( KProcessItem& processItem )
{
	// ±£Ö¤keyÎ¨Ò»
	map<int, KProcessItem>::const_iterator iter = m_mapProcessitems.find(processItem.ProcessItemId());
	if (iter != m_mapProcessitems.end())
	{
		return FALSE;
	}

	m_mapProcessitems.insert(make_pair(processItem.ProcessItemId(), processItem));

	return TRUE;
}

BOOL KProcessInfo::GetProcessItemsMap( std::map<int, KProcessItem>& mapProcessItems )
{
	mapProcessItems = m_mapProcessitems;

	return TRUE;
}

BOOL KProcessInfo::FindProcessItem( int pathIndex,int companyIndex, int signNameIndex )
{
	map<int, KProcessItem>::iterator iter = m_mapProcessitems.begin();

	for (;iter!=m_mapProcessitems.end();iter++)
	{
		if (iter->second.PathIndex() == pathIndex &&
			iter->second.CompanyIndex() == companyIndex &&
			iter->second.SignNameIndex() == signNameIndex)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL KProcessInfo::FindProcessItem( int companyIndex, int signNameIndex, vector<KProcessItem>& processItems )
{
	BOOL bRet = FALSE;
	processItems.clear();
	map<int, KProcessItem>::iterator iter = m_mapProcessitems.begin();

	for (;iter!=m_mapProcessitems.end();iter++)
	{
		if (iter->second.CompanyIndex() == companyIndex &&
			iter->second.SignNameIndex() == signNameIndex)
		{
			processItems.push_back(iter->second);
			bRet = TRUE;
		}
	}

	return bRet;
}
BOOL KProcessInfo::RemoveProcessItem( int processItemId )
{
	map<int, KProcessItem>::iterator iter = m_mapProcessitems.find(processItemId);
	if (iter != m_mapProcessitems.end())
	{
		m_mapProcessitems.erase(iter);
		return TRUE;
	}

	return FALSE;
}

std::wstring KProcessInfo::ProcessName() const
{
	return m_processName;
}

void KProcessInfo::ProcessName( std::wstring val )
{
	m_processName = val;
}