#pragma once
#include "comdata_utility_def.h"

class   IBkReScan
{
public:
    virtual HRESULT GetRescanResult( CAtlArray<BK_FILE_RESULT>& result ) = 0;
};

class   IBkUtility
{
public:
    virtual HRESULT ReportFile() = 0;
    virtual HRESULT SettingChange() = 0;
    virtual HRESULT QueryLastScanInfo( DWORD dwScanMode, BK_SCAN_INFO& ScanInfo ) = 0;

    // 对本地引擎的操作
    virtual HRESULT EnableAvEngine() = 0;
    virtual HRESULT DisableAvEngine() = 0;

    // 对本地常驻的病毒库的操作,
    // 加载常驻病毒库是为了避免病毒库被反复加载,卸载导致的颠簸
    // 如果没有必要,通常不需要从服务外调用
    virtual HRESULT LoadDaemonAvSign() = 0;
    virtual HRESULT UnloadDaemonAvSign() = 0;

    // 病毒库升级后调用该函数,通常由Tray调用
    virtual HRESULT ReloadAvSign() = 0;

    // 本地白名单更新
    virtual HRESULT WhiteListUpdate() = 0;
};

class   IBkEcho
{
public:
    virtual HRESULT Echo( CAtlArray<CString>& FileList ) = 0;
    virtual HRESULT GetEchoResult( BK_ECHO_RESULT& result ) = 0;    // S_FALSE : 握手中; S_OK ：握手成功; 其余握手失败
    virtual HRESULT Stop() = 0;
};

class   IBkPluginReport
{
public:
    virtual HRESULT PluginReport( CAtlArray<BK_PLUGIN_INFO>& PluginList ) = 0;
};