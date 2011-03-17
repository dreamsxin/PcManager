//////////////////////////////////////////////////
//
//	bkwhilelistimp.h
//
//////////////////////////////////////////////////
#pragma once
#include "kws/kwsplugin/ibkwhilelist.h"
#include <common/whitelist.h>
#include <comproxy/bkutility.h>

class CBkWhileListImp : public IBKWhileList
{
public:
	virtual BOOL AddBKWhileList( LPCTSTR pszFilePath )
	{
		CWhiteList wl;
		CWhiteListItem wli;
		wli.m_filepath = pszFilePath;
		wl.Initialize();
		wl.AddWhiteItem(wli);
		wl.Uninitialize();

		CBkUtility util;

		util.Initialize();
		util.WhiteListUpdate();
		util.Uninitialize();

		CSafeMonitorTrayShell::WhiteListLibUpdated();

		return TRUE;
	}

	virtual BOOL DelBKWhileList( LPCTSTR pszFilePath )
	{
		CWhiteList wl;
		CAtlArray<CWhiteListItem> wliArray;
		wl.Initialize();

		CWhiteListItem wli;
		wli.m_filepath = pszFilePath;
		wliArray.Add(wli);

		if ( wliArray.GetCount() > 0 )
			wl.DeleteWhiteItem(wliArray);

		wl.Uninitialize();

		CBkUtility util;

		util.Initialize();
		util.WhiteListUpdate();
		util.Uninitialize();

		CSafeMonitorTrayShell::WhiteListLibUpdated();

		return TRUE;
	}
};