////////////////////////////////////////////////////////////////////////////////
//      
//      File for download file manager ui
//      
//      File      : pedownplugindef.h
//      Version   : 1.0
//      Comment   : 下载管理界面插件定义
//      
//      Create at : 2010-5-17
//      Create by : liupeng
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Unknwn.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1300) 
[
	uuid("726A0D9B-DE86-4488-8C12-22A7DDA60BC3")
]
#else //_MSC_VER
interface __declspec(uuid("726A0D9B-DE86-4488-8C12-22A7DDA60BC3")) ISystemDiagnosisPlugin;
#endif//_MSC_VER

interface IKDonwloadUIPlugIn : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE Create(HWND hWnd, LPRECT lpRect = NULL) = 0;
	virtual HRESULT STDMETHODCALLTYPE ShowWindow(INT nShow) = 0;
	virtual HRESULT STDMETHODCALLTYPE Destroy() = 0;
	virtual HRESULT STDMETHODCALLTYPE ShowFileItem(LPCWSTR strFileName, LPCWSTR strDownFile) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetBlockVirusCount(DWORD& nCount) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetGrayFileCount(DWORD& nCount) = 0;
};

