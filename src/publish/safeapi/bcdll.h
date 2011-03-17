#ifndef _BCDLL_H_
#define _BCDLL_H_
//-----------------------------------------------------------------------------//
[
	uuid("33E0CD4A-EC93-4dfd-AC23-FCFB53201BEC")
]
interface IBootCheck /*   :   IUnknown*/
{	
    // 初始化全局同步命名对象
	virtual HRESULT STDMETHODCALLTYPE Init(PVOID pv) = 0;

    // 保留
	virtual void    STDMETHODCALLTYPE Unit() = 0;

    // 安装驱动
	virtual HRESULT STDMETHODCALLTYPE InstallDriver(PWCHAR pwstrSysPath) = 0;

    // 卸载驱动
	virtual HRESULT STDMETHODCALLTYPE UninstallDriver() = 0;


	virtual HRESULT STDMETHODCALLTYPE DeleteAllConfigFile(VOID) = 0;


	virtual HRESULT STDMETHODCALLTYPE BootCleanOneFile( PWCHAR pwstrFilePath) = 0;
	virtual HRESULT STDMETHODCALLTYPE BootReplaceFile( PWCHAR pwstrSrcFile, PWCHAR pwstrDstFile) = 0;
	virtual HRESULT STDMETHODCALLTYPE BootCleanOneRegKey( PWCHAR pwstrKeyPath) = 0;
	virtual HRESULT STDMETHODCALLTYPE BootCleanOneRegValue( PWCHAR pwstrKeyPath, PWCHAR pwstrValueName) = 0;
	virtual HRESULT STDMETHODCALLTYPE BootEmptyOneRegKey( PWCHAR pwstrKeyPath) = 0;
	virtual HRESULT STDMETHODCALLTYPE BootRelpaceRegValue( PWCHAR pwstrKeyPath, PWCHAR pwstrValueName, PVOID pNewValueBuffer, ULONG ulBufferLength, ULONG ulValueType) = 0;
	virtual HRESULT STDMETHODCALLTYPE AddDriverApprovedMD5( PWCHAR pwstrMD5) = 0;
	virtual HRESULT STDMETHODCALLTYPE AddDriverBlockedMD5( PWCHAR pwstrMD5) = 0;

	virtual HRESULT STDMETHODCALLTYPE Release() = 0;
};
//-----------------------------------------------------------------------------//
typedef  BOOL (__stdcall *PFN_BeikeCreateBootCheck)(IID iid, PVOID *ppv);
//-----------------------------------------------------------------------------//
#endif