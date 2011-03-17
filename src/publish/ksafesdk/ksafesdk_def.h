/**
* @file    ksafesdk_def.h
* @brief   ...
* @author  bbcallen
* @date    2010-08-06 11:34
*/

#ifndef KSAFESDK_DEF_H
#define KSAFESDK_DEF_H

#pragma pack(push,1)


#define KSAFE_S_M_ASYNC         0x00000001      ///< 异步扫描


// 扫描结果
enum KSAFE_RESULT
{
    KSAFE_S_R_UNKNOWN           = 0,        ///< 未知文件(无法判断为安全的文件)
    KSAFE_S_R_PENDING           = 1,        ///< 用于异步模式(暂未开放)
    KSAFE_S_R_ERROR             = 2,        ///< 发生错误

    KSAFE_S_R_SAFE              = 100,      ///< 安全的文件
    KSAFE_S_R_TRUST             = 101,      ///< 信任的文件

    KSAFE_S_R_DETECTED          = 1000,     ///< 发现是病毒

    // 仅当清除时,有可能返回下面的结果
    KSAFE_S_R_DISINFECTED       = 2001,     ///< 已修复
    KSAFE_S_R_DELETED           = 2002,     ///< 已删除
    KSAFE_S_R_FAILED_TO_CLEAN   = 2003,     ///< 清除失败
};



// 回调返回值
enum KSAFE_CALLBACK_RET
{
    KSAFE_CLBK_OK = 0,                  ///< 暂不支持其他的回调返回值
};



// 回调事件
enum KSAFE_EVENT
{
    KSAFE_EVT_OK            = 0,        ///<    安全文件

    KSAFE_EVT_INPROGRESS    = 1,        ///<    扫描进行中
                                        ///<    dwParam1        无意义
                                        ///<    dwParam2        无意义
                                        ///<    pObjectName     文件名
                                        ///<    pVirusName      无意义

    KSAFE_EVT_RESULT        = 101,      ///<    扫描结束,返回结果
                                        ///<    dwParam1        含义为 KSAFE_RESULT
                                        ///<    dwParam2        无意义
                                        ///<    pObjectName     文件名
                                        ///<    pVirusName      病毒名

    KSAFE_EVT_ERROR         = 65536,    ///<    扫描发生错误
};



// 回调函数
typedef KSAFE_CALLBACK_RET (__cdecl *PFN_ksafeCallbackW) (
    KSAFE_EVENT     Event,
    unsigned long   dwParam1,
    unsigned long   dwParam2,
    const wchar_t*  pObjectName,
    const wchar_t*  pVirusName,
    void*           pUserContext);

#pragma pack(pop)

#endif//KSAFESDK_DEF_H