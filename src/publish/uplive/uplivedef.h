#pragma once
#include "bkxdx/bkxdx.h"
#include "bkxdx/bkxdx_cursor_bkjson.h"    // use bkjson implements
#include "bkxdx/bkxdx_exchange_c.h"       // support c type exchange
#include "bkxdx/bkxdx_exchange_atl.h"     // support ATL class exchange

typedef enum
{
    enumUpdPolicyTypeFast = 0,				//触发quick.ini
    enumUpdPolicyTypeSlow = 1,				//触发slow.ini
};

// 升级类型，库升级，bin升级
typedef enum
{
    enumUpdTypeLibUpdate = 0,				//库升级
    enumUpdTypeBinUpdate = 1,				//模块升级
};

// 文件类型
typedef enum
{
    enumTypeUndefined   = -1,
    enumTypeUpdatePack  = 0,				//升级包文件
    enumTypeLibFile     = 1,				//全量库文件文件
    enumTypeLibFileDiff = 2,				//增量库文件
    enumTypeBinFile     = 3,				//二进制(模块)文件
    enumTypeHotfix      = 4,				//紧急补丁修复文件
};

// 更新状态
typedef enum 
{
    enumUpdStatusNone              = 0,    // 占位符
    enumUpdStatusCheckVersion      = 1,    // 检查版本
    enumUpdStatusCheckFileList     = 2,    // 分析文件列表
    enumUpdStatusDownloadFiles     = 3,    // 下载文件
    enumUpdStatusUpdateFiles       = 4,    // 更新文件
    enumUpdStatusUpdateFilesFinished = 5,  // 完成文件更新
    enumUpdStatusCombine           = 6,    // 合并文件
    enumUpdStatusAction            = 7,    // 执行升级后动作
    enumUpdStatusNotify            = 8,    // 执行升级后通知
    enumUpdStatusFinished          = 9,    // 完成更新
	enumUpdNothing				   = 10,   // 新增加状态，没有发现需要更新的内容 [12/24/2010 zhangbaoliang]
};

typedef enum
{
    enumUpdOpBackup                 = 0,
    enumUpdOpReplace                = 1,
    enumUpdOpRebootReplace          = 2,
};

typedef struct _UPD_HINT 
{
    CString strTitle;
    CString strHint;
    CString strDetailUrl;
}UPD_HINT, *PUPD_HINT;

XDX_CLASS_BEGIN(UPD_HINT)
    XDX_MEMBER(strTitle)
    XDX_MEMBER(strHint)
    XDX_MEMBER(strDetailUrl)
XDX_CLASS_END
