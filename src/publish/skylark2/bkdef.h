/**
* @file    bkdef.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-15 12:55
*/

#ifndef BKDEF_H
#define BKDEF_H

#include <assert.h>
#include <unknwn.h>
#include "skylarkbase.h"

NS_SKYLARK_BEGIN

// ------------------------------------------------------------
// 引擎的接口版本

#define BKENG_VERSION_0001      1                   ///< 初始版本
#define BKENG_CURRENT_VERSION   BKENG_VERSION_0001  ///< bkeng接口的版本,当接口发生改变的时候,修改此宏

#define BKENG_PROTOCOL_VERSION  2                   ///< bkeng使用的服务端协议的

// ------------------------------------------------------------
// 掩码操作
#define BKENG_MATCH_BITS(dst__, bits__)     (((dst__) & (bits__)) == (bits__))


#define BKENG_SET_BITS(dst__, bits__)       do {(dst__) |= (bits__);} while(0)


#define BKENG_UNSET_BITS(dst__, bits__)     do {(dst__) &= ~bits__;} while(0)






// ------------------------------------------------------------
// 模块掩码

#define BKENG_MODULE_DB         0x00000001          ///< bkdb.mod
#define BKENG_MODULE_CACHE      0x00000002          ///< bkcache.mod
#define BKENG_MODULE_AVE        0x00000004          ///< bkave.mod


// ------------------------------------------------------------
// 扫描引擎的黑白机设置,注意与 fileengine/ksbwmanager.h 保持同步

#define BKENG_BWMTYPE_GENERAL   0x0000FFFF          ///< 通用
#define BKENG_BWMTYPE_EXTEND    0x000000FF          ///< Extend, 包括NE/LE/MZ/其它普通文件等
#define BKENG_BWMTYPE_PE        0x00000001          ///< Extend2, 处理特定的PE文件
#define BKENG_BWMTYPE_SCRIPT    0x00000002          ///< 脚本, 包括JS/HTML
#define BKENG_BWMTYPE_PEH       0x00000100          ///< 启发式, 处理PE文件, 对性能有影响, 需要专门分离出来

// ------------------------------------------------------------
// 文件引擎扫描和清除设置掩码
#define BKAVE_SCAN_MASK_SCAN_FILE_IN_ARCHIVE    0x00000001  ///< 扫描压缩包中的文件
#define BKAVE_SCAN_MASK_SCAN_FILE_IN_PACK       0x00000002  ///< 扫描脱壳后的文件
#define BKAVE_SCAN_MASK_SCAN_EXE_ONLY           0x00000004  ///< 仅扫描可执行文件
#define BKAVE_SCAN_MASK_DEFAULT                 (BKAVE_SCAN_MASK_SCAN_FILE_IN_ARCHIVE | BKAVE_SCAN_MASK_SCAN_FILE_IN_PACK)

#define BKAVE_CLEAN_MASK_AUTO_CLEAN             0x00000001  ///< 自动清除
#define BKAVE_CLEAN_MASK_DELETE_IF_FAIL         0x00000002  ///< 清除失败的时候删除文件
#define BKAVE_CLEAN_MASK_NONE                   0x00000000  ///< 默认不自动清除
#define BKAVE_CLEAN_MASK_DEFAULT                (BKAVE_CLEAN_MASK_AUTO_CLEAN | BKAVE_CLEAN_MASK_DELETE_IF_FAIL)

// ------------------------------------------------------------
// 扫描结果定义

#define BKENG_SCAN_RESULT_NOT_SCANNED                   0   ///< 没有病毒
#define BKENG_SCAN_RESULT_SAFE                          1   ///< 没有病毒
#define BKENG_SCAN_RESULT_FAILED_TO_SCAN                2   ///< 扫描失败(通常认为无毒)
#define BKENG_SCAN_RESULT_VIRUS_NEED_DELETE             3   ///< 病毒需要删除
#define BKENG_SCAN_RESULT_VIRUS_NEED_DISINFECT          4   ///< 病毒需要清除(感染型,压缩包)
#define BKENG_SCAN_RESULT_VIRUS_NO_OP                   5   ///< 病毒(无法清除)
#define BKENG_SCAN_RESULT_VIRUS_NEED_DISABLE_AUTORUN    6   ///< 禁止自启动


// ------------------------------------------------------------
// 清除结果定义
#define BKENG_CLEAN_RESULT_NO_OP                        1   ///< 未清除

#define BKENG_CLEAN_RESULT_DELETE                       2   ///< 已删除
#define BKENG_CLEAN_RESULT_DISINFECT                    3   ///< 已清除(修复)

#define BKENG_CLEAN_RESULT_FAILED_TO_DELETE             4   ///< 删除失败
#define BKENG_CLEAN_RESULT_FAILED_TO_DISINFECT          5   ///< 清除失败

#define BKENG_CLEAN_RESULT_DELETE_NEED_REBOOT           6   ///< 删除需要重启
#define BKENG_CLEAN_RESULT_DISINFECT_NEED_REBOOT        7   ///< 清除需要重启

#define BKENG_CLEAN_RESULT_NEED_DELETE                  8   ///< 需要删除(内部结果)
#define BKENG_CLEAN_RESULT_DISINFECT_NEED_REPLACE       9   ///< 清除后需要替换(内部结果)

#define BKENG_CLEAN_RESULT_DISABLE_AUTORUN              10  ///< 禁止自启动
#define BKENG_CLEAN_RESULT_FAILED_TO_DISABLE_AUTORUN    11  ///< 禁止自启动失败

inline BOOL BKEngCleanNeedReboot(DWORD dwCleanResult)
{
    switch (dwCleanResult)
    {
    case BKENG_CLEAN_RESULT_DELETE_NEED_REBOOT:
    case BKENG_CLEAN_RESULT_DISINFECT_NEED_REBOOT:
        return TRUE;
    }

    return FALSE;
}



// ------------------------------------------------------------
// 文件类型掩码
#define BKENG_TYPE_MASK_SKIP        0x00000001  ///< [不可过滤]不可能有病毒的文件
#define BKENG_TYPE_MASK_EXE         0x00000002  ///< [不可过滤]可执行文件
#define BKENG_TYPE_MASK_RTF         0x00000004  ///< RTF文件, 包括Office文件
#define BKENG_TYPE_MASK_ARCH        0x00000008  ///< [不可过滤]压缩文件
#define BKENG_TYPE_MASK_PIC         0x00000010  ///< 图形文件
#define BKENG_TYPE_MASK_META        0x00000020  ///< [不可过滤]复合文件
#define BKENG_TYPE_MASK_TEXT        0x00000040  ///< 文本文件
#define BKENG_TYPE_MASK_SHELL       0x00000080  ///< [不可过滤]加壳文件


#define BKENG_TYPE_MASK_NONE        0x00000000  ///< 所有文件
#define BKENG_TYPE_MASK_ALL         0xFFFFFFFF  ///< 所有类型的文件

/// 常用的过滤掩码组合
#define BKENG_TYPE_MASK_NON_EXE     (BKENG_TYPE_MASK_RTF | BKENG_TYPE_MASK_PIC | BKENG_TYPE_MASK_TEXT)
#define BKENG_TYPE_MASK_DEFAULT     BKENG_TYPE_MASK_NONE



// ------------------------------------------------------------
// 
#define BKCLOUD_MASK_DISABLE_NETWORK_QUERY      0x00000001  ///< 禁止联网查询
//#define BKCLOUD_MASK_FORCE_HASH_FILE                0x00000002  ///< 强制计算文件的hash
#define BKCLOUD_MASK_QUERY_BLACK_CACHE          0x00000004  ///< 查询黑level缓存
#define BKCLOUD_MASK_WRITE_CACHE_AFTER_QUERY    0x00000008  ///< 尽可能快的输出缓存
#define BKCLOUD_MASK_LOW_RESOURCE_USAGE         0x00000010  ///< 降低资源占用
#define BKCLOUD_MASK_USE_CLOUD_HEURIST          0x00000020  ///< 使用云端启发
#define BKCLOUD_MASK_FORCE_BOOT_CLEAN           0x00000040  ///< 强制重启后清除
#define BKCLOUD_MASK_DEFAULT                    (BKCLOUD_MASK_USE_CLOUD_HEURIST)   ///< 默认设置

// ------------------------------------------------------------
// 缓存掩码
#define BKENG_CACHE_MASK_PATH_TO_HASH       0x00000001  ///< 缓存 path - hash       (禁用会导致总是计算hash)
#define BKENG_CACHE_MASK_HASH_TO_LEVEL      0x00000002  ///< 缓存 hash - level      (禁用会导致总是联网查询)
#define BKENG_CACHE_MASK_PATH_TO_WINTRUST   0x00000004  ///< 缓存 path - wintrust   (禁用会导致总是验证文件数字签名的完整性)
#define BKENG_CACHE_MASK_PATH_TO_NONPE      0x00000008  ///< 缓存 path - nonpe      (禁用会导致总是打开文件判断是否是PE)
#define BKENG_CACHE_MASK_PATH_TO_AVEFT      0x00000010  ///< 缓存 path - aveft      (禁用会导致ave总是打开文件判断文件类型)
#define BKENG_CACHE_MASK_PATH_TO_AVEBWM     0x00000020  ///< 缓存 path - avebwm     (禁用会导致ave总是打开文件进行黑白机扫描)

#define BKENG_CACHE_MASK_NONE               0x00000000
#define BKENG_CACHE_MASK_ALL                0xFFFFFFFF




// ------------------------------------------------------------
// LocalScan (前置扫描)
#define BKENG_LOCAL_SCAN_MASK_NONE              0x00000000
#define BKENG_LOCAL_SCAN_MASK_ALL               0xFFFFFFFF


// ------------------------------------------------------------
// ReviewScan (后置扫描)
#define BKENG_REVIEW_SCAN_MASK_WINTRUST                 0x00000001  ///< 复查数字签名
#define BKENG_REVIEW_SCAN_MASK_AVENGINE                 0x00000002  ///< 使用文件引擎复查
#define BKENG_REVIEW_SCAN_MASK_RESERVED_0               0x00000004  ///< ...
#define BKENG_REVIEW_SCAN_MASK_AVENGINE_SCAN_NON_PE     0x00000008  ///< 使用文件引擎扫描非PE文件
#define BKENG_REVIEW_SCAN_MASK_AVENGINE_SCAN_NON_EXE    0x00000010  ///< 使用文件引擎扫描非可执行文件,仅当 BKENG_REVIEW_SCAN_MASK_AVENGINE_SCAN_NON_PE 被设置才有效
#define BKENG_REVIEW_SCAN_MASK_USE_LOCAL_HEURIST        0x00000020  ///< 使用本地启发

#define BKENG_REVIEW_SCAN_MASK_NONE                     0x00000000
#define BKENG_REVIEW_SCAN_MASK_ALL                      0xFFFFFFFF
#define BKENG_REVIEW_SCAN_MASK_DEFAULT                  (BKENG_REVIEW_SCAN_MASK_WINTRUST | BKENG_REVIEW_SCAN_MASK_AVENGINE | BKENG_REVIEW_SCAN_MASK_USE_LOCAL_HEURIST)



// ------------------------------------------------------------
// Append Scan Mask 单个文件的扫描设置
#define BKENG_SCAN_MASK_USE_LOCAL_HEURIST               0x00000001  ///< 使用本地启发式
#define BKENG_SCAN_MASK_USE_CLOUD_HEURIST_LOW_RISK      0x00000002  ///< 使用云端启发的低风险
#define BKENG_SCAN_MASK_USE_CLOUD_HEURIST_HIGH_RISK     0x00000004  ///< 使用云端启发的高风险
#define BKENG_SCAN_MASK_FROM_AUTORUNS                   0x00010000  ///< 来自启动项
#define BKENG_SCAN_MASK_HINT_AVP_VIRUS                  0x00020000  ///< 提示该文件是卡巴报毒的文件,清除时不重新扫描,直接清除
#define BKENG_SCAN_MASK_HINT_CURABLE                    0x00040000  ///< 提示该文件可修复

#define BKENG_SCAN_MASK_NONE                    0x00000000
#define BKENG_SCAN_MASK_ALL                     0xFFFFFFFF
#define BKENG_SCAN_MASK_DEFAULT                 BKENG_SCAN_MASK_NONE







// ------------------------------------------------------------
// 文件引擎设置
#define BKAVE_MASK_NO_DAEMON_SIGN           0x00000001  ///< 病毒库不常驻,仅在扫描时才加载





/**
* @brief    进度回调
*/
class IBKProgress
{
public:
    /**
    * @brief        进度回调
    * @param[in]    dwTotal     总量
    * @param[in]    dwFinished  完成量
    */
    virtual HRESULT STDMETHODCALLTYPE OnProgress(
        DWORD   dwTotal,
        DWORD   dwFinished) = 0;
};


/**
* @brief    工厂方法
*/
class __declspec(uuid("C0C6F1E9-69C5-44ae-B03E-E5E78B3EF79F"))
IBKFactory: public IUnknown
{
public:

    /**
    * @brief        创建组件实例
    * @param[id]    riid    接口iid
    * @param[out]   ppvObj  返回接口指针
    */
    virtual HRESULT STDMETHODCALLTYPE CreateInstance(REFIID riid, void** ppvObj) = 0;
};









// ------------------------------------------------------------
// 病毒库版本结构

/// 病毒库版本结构
struct BKAVE_SIGN_VERSION
{
    // ------------------------------------------------------------
    // Version[1, ?]

    UINT16      uYear;                  ///< 年
    UINT16      uMonth;                 ///< 月
    UINT16      uDay;                   ///< 日
    UINT16      uBuild;                 ///< build号

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKAVE_SIGN_VERSION* pStruct)
{  
    pStruct->uYear      = 0;
    pStruct->uMonth     = 0;
    pStruct->uDay       = 0;
    pStruct->uBuild     = 0;
}







#define BKENG_DETAIL_AVP_BEFORE_SCAN    101     ///< avp扫描开始
#define BKENG_DETAIL_AVP_INPROGRESS     102     ///< avp扫描进行中描
#define BKENG_DETAIL_AVP_SCAN_OBJECT    103     ///< avp扫描完成一个对象
#define BKENG_DETAIL_AVP_AFTER_SCAN     104     ///< avp扫描完成

#define BKENG_DETAIL_AFTER_LOCAL_SCAN   201     ///< 正在调用本地扫描



// ------------------------------------------------------------
// 查杀引擎的扫描细节

/// 查杀引擎扫描细节
struct BKENG_SCAN_DETAIL
{
    UINT32  uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    DWORD   dwDetailType;       ///< 细节类型,取值为BKENG_DETAIL_xxx
    DWORD   dwUsedScanWeight;   ///< 消耗的扫描权重

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_SCAN_DETAIL* pStruct)
{  
    assert(pStruct);
    pStruct->uSize                  = sizeof(BKENG_SCAN_DETAIL);

    pStruct->dwDetailType           = 0;
    pStruct->dwUsedScanWeight       = 0;
}





NS_SKYLARK_END

#endif//BKDEF_H