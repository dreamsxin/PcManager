//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   IKwsPlugin.h
//  Creator     :   chenguicheng
//  Date        :   2010-07-31
//  Comment     :   Interface for the ws use kws's dlgs.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef	_I_KWS_PLUGIN_2010_07_31_H
#define	_I_KWS_PLUGIN_2010_07_31_H 
#include <Unknwn.h>
#include "settingpagedef.h"
#include "ibkwhilelist.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1300) 
[
	uuid("69B4968D-B1C9-43d7-B3B4-1F458850F6EC")
]
#else //_MSC_VER
	interface __declspec(uuid("69B4968D-B1C9-43d7-B3B4-1F458850F6EC")) IKwsPlugIn;
#endif//_MSC_VER

interface IKwsPlugIn : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE DoModalHistory(HWND hParent,	LPRECT lpRect = NULL) = 0;
	virtual HRESULT STDMETHODCALLTYPE DoModalSetting(HWND hParent, 
													IBKWhileList* pList,
													EM_INIT_PAGE emPage = em_trustlist_page, 
													LPRECT lpRect = NULL) = 0;

	virtual HRESULT STDMETHODCALLTYPE DoModalAdFilter(HWND hParent, LPRECT lpRect = NULL) = 0;
};


#endif	// #ifndef _I_KWS_PLUGIN_2010_07_31_H 