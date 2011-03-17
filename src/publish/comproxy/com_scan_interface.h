#pragma once
#include "comdata_scan_def.h"

class   IBkScan
{
public:
    virtual HRESULT Scan( int nScanMode, BOOL bBackGround, CAtlArray<CString>& lpParam ) = 0;     // Mode : 扫描类型（快速扫描、关键目录扫描、全盘扫描、自定义扫描，右键扫描）
                                                                                // lpParam可供传递额外参数（如自定义扫描目录等）
    virtual HRESULT Scan2( int nScanMode, BOOL bBackGround, CAtlArray<BK_SCAN_ADDITIONAL_PATH>& lpParam ) = 0;     // Mode : 扫描类型（快速扫描、关键目录扫描、全盘扫描、自定义扫描，右键扫描）
                                                                                // lpParam可供传递额外扫描目录，同时可以指定该目录是否递归扫描
    virtual HRESULT QuerySteps( int nScanMode, CAtlArray<int>& steps ) = 0;                    // 查询扫描步骤数
    virtual HRESULT Pause() = 0;                                                // 暂停扫描
    virtual HRESULT Resume() = 0;                                               // 恢复扫描
    virtual HRESULT Stop() = 0;                                                 // 停止扫描
    virtual HRESULT QueryScanInfo( BK_SCAN_INFO& scanInfo ) = 0;                // 查询扫描信息 scanInfo : 获取到的扫描信息
    virtual HRESULT QueryCleanInfo( BK_CLEAN_INFO& cleanInfo ) = 0;             // 查询清除信息 cleanInfo: 获取到清除信息
    virtual HRESULT QueryFileInfo( int nType, DWORD dwIndex, BK_FILE_INFO& fileInfo ) = 0;  // 查询指定类型文件信息 nType : 文件类型 dwIndex : 文件索引 fileInfo : 文件信息
    virtual HRESULT Clean( CAtlArray<DWORD>& fileIndex ) = 0;                   // 清除选定文件 fileIndex : 选定文件的索引
    virtual HRESULT QueryLastScanInfo( int nScanMode, BK_SCAN_INFO& scanInfo ) = 0;            // 查询上次扫描结果 scanInfo : 获取到的上次扫描信息
    virtual HRESULT SetScanSetting( BK_SCAN_SETTING& setting ) = 0;             // 设置扫描选项 settings : 要设置的扫描选项
    virtual HRESULT GetScanSetting( BK_SCAN_SETTING& setting ) = 0;             // 获取扫描选项 settings : 获取到的扫描选项
    virtual HRESULT ReportFile() = 0;                                           // 上报本次扫描出来的未知文件。
};

class   IBkScanSingleFile
{
public:
    virtual HRESULT ScanFile( CString& strFile, DWORD dwScanMask, DWORD dwScanPolicy, BK_FILE_RESULT& fr ) = 0;       // 扫描指定文件 strFile : 指定的文件路径 dwScanPolicy : 扫描策略 fr : 返回的扫描结果
    virtual HRESULT CleanFile( CString& strFile, DWORD dwScanMask, DWORD dwScanPolicy, BK_FILE_RESULT& fr ) = 0;      // 清除指定文件
    virtual HRESULT ForceClean( CString& strFile, BK_FORCE_CLEAN_DETAIL& detail, BK_FILE_RESULT& fr ) = 0; // 强制清除某个文件
    //virtual HRESULT SetScanSetting( BK_SCAN_SETTING& setting ) = 0;             // 设置扫描选项 settings : 要设置的扫描选项
    //virtual HRESULT GetScanSetting( BK_SCAN_SETTING& setting ) = 0;             // 获取扫描选项 settings : 获取到的扫描选项
};

class   IBkScanMultiFile
{
public:
    virtual HRESULT ScanHash( CAtlArray<CString>& hashArray, DWORD dwScanPolicy, CAtlArray<BK_FILE_RESULT>& frArray ) = 0;
    virtual HRESULT ScanFile( CAtlArray<CString>& fileArray, DWORD dwScanPolicy, CAtlArray<BK_FILE_RESULT>& frArray ) = 0;
};