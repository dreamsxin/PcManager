/**
* @file    bkatrun.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-27 11:06
*/

#ifndef BKATRUN_H
#define BKATRUN_H

#include "bkatrundef.h"

NS_SKYLARK_BEGIN

/// autorun扫描结果项目
class __declspec(uuid("B762E747-D0D5-4c73-8CED-0458E9DB62E0"))
IBKAutorunItem: public IUnknown
{
public:

    /// 获取启动项ID
    virtual UINT32  STDMETHODCALLTYPE GetAutorunID() = 0;

    /// 获取启动项分组ID
    virtual UINT32  STDMETHODCALLTYPE GetAutorunGroupID() = 0;               

    /// 获取启动项的信息(仅用来显示)
    virtual LPCWSTR STDMETHODCALLTYPE GetInformation() = 0;



    /// 获取启动项关联的文件数目
    virtual UINT32  STDMETHODCALLTYPE GetFileCount() = 0;

    /// 获取启动项关联的文件路径
    virtual LPCWSTR STDMETHODCALLTYPE GetFilePathAt(UINT32 uIndex) = 0;



    /// 判断指定启动项是否能被修复
    virtual BOOL    STDMETHODCALLTYPE CanBeRepaired() = 0;

    /// 修复指定启动项, 返回S_FALSE, 表明不需要修复
    virtual HRESULT STDMETHODCALLTYPE Repair(BKATRUN_REPAIR_PARAM* pRepairParam) = 0;




    //////////////////////////////////////////////////////////////////////////
    // 禁用接口,保留未实现

    ///// 判断指定启动项是否能被修复
    //virtual BOOL    STDMETHODCALLTYPE CanBeDisabled() = 0;

    ///// 修复指定启动项
    //virtual HRESULT STDMETHODCALLTYPE Disable(void* pvReserved) = 0;




    ////////////////////////////////////////////////////////////////////////////
    //// 信息接口,保留未实现

    //// 以下是获取扩展信息的接口,保留未实现
    //virtual LPCWSTR     STDMETHODCALLTYPE GetString(    UINT32 uID, LPCWSTR nullValue = L"") = 0;
    //virtual UINT32      STDMETHODCALLTYPE GetUInt(      UINT32 uID, UINT32 nullValue = 0) = 0;
    //virtual UINT64      STDMETHODCALLTYPE GetUInt64(    UINT32 uID, UINT64 nullValue = 0) = 0;
    //virtual UINT32      STDMETHODCALLTYPE GetBlob(      UINT32 uID, BYTE* pBlog, UINT32 nBlogBytes) = 0;
    //virtual UINT32      STDMETHODCALLTYPE GetBlobBytes( UINT32 uID) = 0;
};


/**
* @brief    autorun扫描器的封装
* @remark   如果要支持扫描计划任务,务必在扫描线程中初始化COM
*/
class __declspec(uuid("AC489AB3-530C-408c-9B3F-6CAFE9A3DD0C"))
IBKAutorunScanner: public IUnknown
{
public:

    /**
    * @brief        初始化扫描流程
    */ 
    virtual HRESULT STDMETHODCALLTYPE StartScan(BKATRUN_SCAN_PARAM* pScanParam) = 0;

    /**
    * @brief        扫描下一个启动项
    */ 
    virtual BOOL    STDMETHODCALLTYPE FindNextItem(IBKAutorunItem** ppiAtrItem) = 0;
};


/**
* @brief    autorun扫描器的性能测试参数
*/
class __declspec(uuid("B77E4A52-DA85-4a5d-9E72-03EAC816E73E"))
IBKAutorunPerformView: public IUnknown
{
public:
    /**
    * @brief        获取启动项扫描过程中性能参数
    * @param        dwPerfID    性能测试项目,取值为BKATR_PERF_COUNT__xxx
    */ 
    virtual DWORD   STDMETHODCALLTYPE GetPerfCount(DWORD dwPerfID) = 0;
};


NS_SKYLARK_END



#define  FN_BKAtrunCreateObject "BKAtrunCreateObject"   ///< export BKAtrunCreateObject


/**
* @brief    创建BKAtrun的模块
* @param    dwBKEngVer  BKEng的版本号,默认使用BKENG_CURRENT_VERSION,传入0表示不作版本检查
* @param    riid        要创建的对象的iid
* @param    ppvObj      返回创建的对象
*/
EXTERN_C HRESULT WINAPI BKAtrunCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKAtrunCreateObject
typedef HRESULT (WINAPI *PFN_BKAtrunCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);



#endif//BKATRUN_H