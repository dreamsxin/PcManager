/////////////////////////////////////////////////////////////////
//
//	Filename: 	kselitecanservice_def.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2010-10-18  20:05
//	Comment:	
//
///////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "include/kxe_serialize.h"
#include "kxedatastructdef.h"

/**
* @brief    Lite的扫描会话句柄
*/
typedef struct _S_KSE_LITESCAN_SESSION
{
    UUID    name; ///< 会话的uuid
    UUID    key;  ///< 扫描会话的密钥,用来确认 name 的有效性
}S_KSE_LITESCAN_SESSION;

KXE_JSON_DDX_BEGIN(S_KSE_LITESCAN_SESSION)
    KXE_JSON_DDX_MEMBER("name", name)
    KXE_JSON_DDX_MEMBER("key", key)
KXE_JSON_DDX_END()




//////////////////////////////////////////////////////////////////////////
// KSE_LITETASK_SCAN_MASK 扫描控制掩码

#define  KSE_LITETASK_SCAN_MASK_NOCLOUD       (ULONG)0x00000001  ///< 不使用云检测
#define  KSE_LITETASK_SCAN_MASK_AUTOFIX       (ULONG)0x00000002  ///< 自动处理病毒
#define  KSE_LITETASK_SCAN_MASK_NO_WHITECACHE (ULONG)0x00000004  ///< 不使用白名单缓存
#define  KSE_LITETASK_SCAN_MASK_NO_USERTRUST  (ULONG)0x00000008  ///< 不使用用户信任列表

/**
* @brief  检测目录结构
*/
typedef struct _S_KSE_LITESCAN_TARGET_INFO
{
    DWORD                     dwScanMask;   ///< 扫描控制掩码,见KSE_LITETASK_SCAN
    std::vector<std::wstring> vecScanFiles; ///< 扫描的文件列表
    DWORD                     dwTimeOut;    ///< 等待网络检测的超时值,0或者-1为无限等待
    _S_KSE_LITESCAN_TARGET_INFO():
    dwScanMask(0),
    dwTimeOut(0)
    {}
}S_KSE_LITESCAN_TARGET_INFO;

KXE_JSON_DDX_BEGIN(S_KSE_LITESCAN_TARGET_INFO)
KXE_JSON_DDX_MEMBER("ScanMask", dwScanMask)
KXE_JSON_DDX_MEMBER("ScanFiles", vecScanFiles)
KXE_JSON_DDX_MEMBER("TimeOut", dwTimeOut)
KXE_JSON_DDX_END()


/**
* @brief 文件安全等级
*/
enum EM_KSE_SECURITY_LEVAL
{
    EM_KSE_SECURITY_LEVAL_NONE   =   (ULONG)0x00000000, ///< 检测失败，无安全等级
    EM_KSE_SECURITY_LEVAL_GRAY   =   (ULONG)0x00000001, ///< 灰文件(非PE)
    EM_KSE_SECURITY_LEVAL_GRAYPE =   (ULONG)0x00000002, ///< 灰PE文件
    EM_KSE_SECURITY_LEVAL_SAFE   =   (ULONG)0x00000003, ///< 安全
    EM_KSE_SECURITY_LEVAL_VIRUS  =   (ULONG)0x00000004  ///< 病毒
};
KXE_JSON_DDX_ENUM(EM_KSE_SECURITY_LEVAL);

/**
* @brief   文件检测结果
*/
typedef struct _S_KSE_LITESCAN_RESULT
{
    EM_KSE_SECURITY_LEVAL emleval;            ///< 安全等级
    DWORD                 dwVirusID;          ///< 病毒ID
    std::wstring          strFileMD5;         ///< MD5字符串, 如果为L""代表没有MD5信息,(当前不支持MD5信息输出)
    std::wstring          strVirusName;       ///< 病毒名, emleval为EM_KSE_SECURITY_LEVAL_VIRUS时有效
    HRESULT               hErrorCode;         ///< 错误码
    EM_KXE_THREAT_PROCESS_RESULT emProResult; ///< 处理结果, 要求自动处理时填充
    _S_KSE_LITESCAN_RESULT():
    emleval(EM_KSE_SECURITY_LEVAL_NONE),
    dwVirusID(0),
    hErrorCode(0),
    emProResult(em_Threat_Process_No_Op)
    {}
}S_KSE_LITESCAN_RESULT;

KXE_JSON_DDX_BEGIN(S_KSE_LITESCAN_RESULT)
    KXE_JSON_DDX_MEMBER("Leval", emleval)
    KXE_JSON_DDX_MEMBER("FileMD5", strFileMD5)
    KXE_JSON_DDX_MEMBER("VirusID", dwVirusID)
    KXE_JSON_DDX_MEMBER("VirusName", strVirusName)
    KXE_JSON_DDX_MEMBER("ErrorCode", hErrorCode)
    KXE_JSON_DDX_MEMBER("ProResult", emProResult)
KXE_JSON_DDX_END()



//////////////////////////////////////////////////////////////////////////
// KSE_PROCESS_FILE_MASK 处理文件控制掩码

#define  KSE_PROCESS_FILE_MASK_NO_BOOTCLEAN (ULONG)0x00000001  ///< 不使用BootClean
#define  KSE_PROCESS_FILE_MASK_NO_BACKUP    (ULONG)0x00000002  ///< 不备份文件

/**
* @brief  处理文件的数据结构
*/
typedef struct _S_KSE_PROCESS_FILE_INFO
{
    DWORD        dwProcessMask;   ///< 处理控制掩码,见KSE_PROCESS_FILE_MASK
    std::wstring strFileName;     ///< 要处理的文件名
    std::wstring strVirusName;    ///< 文件病毒名(非必须)
    DWORD        dwVirusID;      ///< 文件病毒ID(非必须)
    _S_KSE_PROCESS_FILE_INFO():
    dwProcessMask(0),dwVirusID(0)
    {}
}S_KSE_PROCESS_FILE_INFO;

KXE_JSON_DDX_BEGIN(S_KSE_PROCESS_FILE_INFO)
    KXE_JSON_DDX_MEMBER("ProcessMask", dwProcessMask)
    KXE_JSON_DDX_MEMBER("FileName",  strFileName)
    KXE_JSON_DDX_MEMBER("VirusName", strVirusName)
    KXE_JSON_DDX_MEMBER("VirusID",  dwVirusID)
KXE_JSON_DDX_END()


/**
* @brief  处理文件的结果数据结构
*/
typedef struct _S_KSE_PROCESS_FILE_RESULT
{
    bool bNeedReBoot; 
    bool bBackUpSuccess;
}S_KSE_PROCESS_FILE_RESULT;

KXE_JSON_DDX_BEGIN(S_KSE_PROCESS_FILE_RESULT)
KXE_JSON_DDX_MEMBER("NeedReBoot",     bNeedReBoot)
KXE_JSON_DDX_MEMBER("BackUpSuccess",  bBackUpSuccess)
KXE_JSON_DDX_END()