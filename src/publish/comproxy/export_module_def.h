#pragma once

// export module
// CBkscan
// #define BKCOM_EXPORT_VER(module)   BKCOM_EXPORT_MODULE_##module##_VER
// #define BKCOM_EXPORT_MODULE(module) BKCOM_EXPORT_MODULE_##module
// #define BKCOM_EXPORT_MODULE_METHOD(module, method)  BKCOM_EXPORT_##module##_METHOD_##method

#define BKCOM_EXPORT_MODULE_CBkScan  0
#define BKCOM_EXPORT_MODULE_CBkScan_VER  0
    #define BKCOM_EXPORT_CBkScan_METHOD_Scan 0
    #define BKCOM_EXPORT_CBkScan_METHOD_Pause 1     
    #define BKCOM_EXPORT_CBkScan_METHOD_Resume 2     
    #define BKCOM_EXPORT_CBkScan_METHOD_Stop 3       
    #define BKCOM_EXPORT_CBkScan_METHOD_QueryScanInfo 4
    #define BKCOM_EXPORT_CBkScan_METHOD_QueryCleanInfo 5
    #define BKCOM_EXPORT_CBkScan_METHOD_QueryFileInfo 6
    #define BKCOM_EXPORT_CBkScan_METHOD_Clean 7
    #define BKCOM_EXPORT_CBkScan_METHOD_QueryLastScanInfo 8
    #define BKCOM_EXPORT_CBkScan_METHOD_SetScanSetting 9
    #define BKCOM_EXPORT_CBkScan_METHOD_GetScanSetting 10
    #define BKCOM_EXPORT_CBkScan_METHOD_QuerySteps 11
    #define BKCOM_EXPORT_CBkScan_METHOD_ReportFile 12
    #define BKCOM_EXPORT_CBkScan_METHOD_Scan2 13

// CBkScanSingleFile
#define BKCOM_EXPORT_MODULE_CBkScanSingleFile  1
#define BKCOM_EXPORT_MODULE_CBkScanSingleFile_VER 0
    #define BKCOM_EXPORT_CBkScanSingleFile_METHOD_ScanFile 0
    #define BKCOM_EXPORT_CBkScanSingleFile_METHOD_CleanFile    1
    #define BKCOM_EXPORT_CBkScanSingleFile_METHOD_ForceClean   2

// CBkServiceCall
#define BKCOM_EXPORT_MODULE_CBkServiceCall 2
#define BKCOM_EXPORT_MODULE_CBkServiceCall_VER 0
    #define BKCOM_EXPORT_CBkServiceCall_METHOD_Execute 0

// CBkRescan
#define BKCOM_EXPORT_MODULE_CBkRescan 3
#define BKCOM_EXPORT_MODULE_CBkRescan_VER 0
    #define BKCOM_EXPORT_CBkRescan_METHOD_GetRescanResult 0

// CBkUtility
#define BKCOM_EXPORT_MODULE_CBkUtility 4
#define BKCOM_EXPORT_MODULE_CBkUtility_VER 0
    #define BKCOM_EXPORT_CBkUtility_METHOD_ReportFile           0
    #define BKCOM_EXPORT_CBkUtility_METHOD_SettingChange        1
    #define BKCOM_EXPORT_CBkUtility_METHOD_QueryLastScanInfo    2
    #define BKCOM_EXPORT_CBkUtility_METHOD_EnableAvEngine       3
    #define BKCOM_EXPORT_CBkUtility_METHOD_DisableAvEngine      4
    #define BKCOM_EXPORT_CBkUtility_METHOD_LoadDaemonAvSign     5
    #define BKCOM_EXPORT_CBkUtility_METHOD_UnloadDaemonAvSign   6
    #define BKCOM_EXPORT_CBkUtility_METHOD_ReloadAvSign         7
    #define BKCOM_EXPORT_CBkUtility_METHOD_WhiteListUpdate      8

// CBkEcho
#define BKCOM_EXPORT_MODULE_CBkEcho 5
#define BKCOM_EXPORT_MODULE_CBkEcho_VER 0
    #define BKCOM_EXPORT_CBkEcho_METHOD_Echo 0
    #define BKCOM_EXPORT_CBkEcho_METHOD_GetEchoResult 1
    #define BKCOM_EXPORT_CBkEcho_METHOD_Stop 2

// CBkPluginReport
#define BKCOM_EXPORT_MODULE_CBkPluginReport 6
#define BKCOM_EXPORT_MODULE_CBkPluginReport_VER 0
    #define BKCOM_EXPORT_CBkPluginReport_METHOD_PluginReport 0


// CBkScanMultiFile
#define BKCOM_EXPORT_MODULE_CBkScanMultiFile 7
#define BKCOM_EXPORT_MODULE_CBkScanMultiFile_VER 0
    #define BKCOM_EXPORT_CBkScanMultiFile_METHOD_ScanHash 0
    #define BKCOM_EXPORT_CBkScanMultiFile_METHOD_ScanFile 1 