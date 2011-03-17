/**
* @file    bkave.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-14 15:41
*/

#ifndef BKAVE_H
#define BKAVE_H

#include <unknwn.h>
#include "skylarkbase.h"
#include "bkavedef.h"

NS_SKYLARK_BEGIN


/// 获取扫描细节信息的接口
class IBKAvEngineScanDetail
{
public:

    /**
    * @brief        给出扫描过程中的细节信息
    * @param[in]    扫描的虚拟路径
    * @param[in]    子对象的扫描结果
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */ 
    virtual HRESULT __stdcall OnScanDetail(
        LPCWSTR                     lpszVirtualPath,
        const BKAVE_ENGINE_RESULT*  pResult) = 0;

    /**
    * @brief        空闲时候的回调
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */ 
    virtual HRESULT __stdcall OnIdle() = 0;
};











/**
* @brief    文件引擎修复文件的结果对象
*/
class __declspec(uuid("B62CBBBF-D56B-4cfd-9A36-61B0AAC5EF72"))
IBKAveCleanFileObj: public IUnknown
{
public:

    /**
    * @brief        关闭文件
    * @remark       如果不是必要的情况,尽可能在对象引用计数为0的时候自动关闭.\n
    *               被关闭的文件,可以用 ReOpen 重新打开.
    */
    virtual HRESULT STDMETHODCALLTYPE Close() = 0;


    /**
    * @brief        设置对象被最终释放时删除文件
    * @param[in]    bEnableDeleteOnRelease  是否删除
    * @remark       这里不使用 FILE_FLAG_DELETE_ON_CLOSE 标志,所以在Close文件的时候,文件任然可以存在
    */
    virtual HRESULT STDMETHODCALLTYPE DeleteOnRelease(BOOL bEnableDeleteOnRelease) = 0;


    /**
    * @brief        获取文件的物理路径
    * @param[out]   pbstrPhysicalPath   返回文件的物理路径(包含文件名)
    */
    virtual HRESULT STDMETHODCALLTYPE GetPhysicalPath(BSTR* pbstrPhysicalPath) = 0;

};













/// 文件引擎接口
class __declspec(uuid("FD469B0C-4D9D-40f8-AA21-7F3F50E20A7B"))
IBKAvEngine: public IUnknown
{
public:

    /**
    * @brief        初始化接口
    * @param[in]    pInit           初始化参数
    */ 
    virtual HRESULT STDMETHODCALLTYPE Initialize(BKAVE_ENGINE_INIT* pInit = NULL) = 0;

    /**
    * @brief        反初始化接口
    */ 
    virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;

    /**
    * @brief        获取设置
    * @param[out]   pSettings       返回设置
    */ 
    virtual HRESULT STDMETHODCALLTYPE GetSettings(BKAVE_ENGINE_SETTINGS* pSettings) = 0;

    /**
    * @brief        获取设置
    * @param[in]    pSettings       新设置,如果为NULL,引擎会恢复默认设置
    */ 
    virtual HRESULT STDMETHODCALLTYPE SetSettings(const BKAVE_ENGINE_SETTINGS* pSettings) = 0;

    /**
    * @brief        扫描文件
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    pScanParam      扫描参数
    * @param[out]   pScanResult     文件扫描结果
    * @param[in]    piScanDetail    获取扫描细节的回调接口,可以为NULL
    * @retval       S_OK                                扫描成功
    * @retval       S_KSE_BWM_SKIP_TYPE_BY_SETTINGS     根据设置过滤类型的文件
    */ 
    virtual HRESULT STDMETHODCALLTYPE Scan(
        LPCWSTR                 lpszFilePath,
        const BKAVE_SCAN_PARAM* pScanParam,
        BKAVE_ENGINE_RESULT*    pScanResult,
        IBKAvEngineScanDetail*  piScanDetail = NULL) = 0;

    /**
    * @brief        清除文件
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    pScanParam      扫描参数
    * @param[out]   pScanResult     文件扫描结果
    * @param[out]   ppiCleanFileObj 如果文件有修复动作,那么这里返回修复后的临时文件
    * @param[in]    piScanDetail    获取扫描细节的回调接口,可以为NULL
    * @retval       S_OK                                扫描成功
    * @retval       S_KSE_BWM_SKIP_TYPE_BY_SETTINGS     根据设置过滤类型的文件
    */ 
    virtual HRESULT STDMETHODCALLTYPE Clean(
        LPCWSTR                 lpszFilePath,
        const BKAVE_SCAN_PARAM* pScanParam,
        BKAVE_ENGINE_RESULT*    pScanResult,
        IBKAveCleanFileObj**    ppiCleanFileObj,
        IBKAvEngineScanDetail*  piScanDetail = NULL) = 0;

    /**
    * @brief        强制将缓存输出到数据库
    */
    virtual HRESULT STDMETHODCALLTYPE FlushCache() = 0;

    /**
    * @brief        获取已加载的病毒库版本号
    * @param[in]    pSignVersion        病毒库版本的结构体
    * @remark       需要病毒库已加载
    */ 
    virtual HRESULT STDMETHODCALLTYPE GetLoadedSignVersion(
        BKAVE_SIGN_VERSION* pSignVersion) = 0;
};







/// 病毒库管理工具包
class __declspec(uuid("5A1B6478-9636-401d-8C4D-91AD18F92508"))
IBKAveSignToolkit: public IUnknown
{
public:

    /**
    * @brief        获取已加载的常驻病毒库版本号
    * @param[in]    pSignVersion        病毒库版本的结构体
    * @remark       需要病毒库已加载
    */ 
    virtual HRESULT STDMETHODCALLTYPE GetDaemonSignVersion(
        BKAVE_SIGN_VERSION* pSignVersion) = 0;


    /**
    * @brief        获取本地病毒库版本号
    * @param[in]    lpszSignIniPath     signs.ini的路径,如果为NULL,会从全局设置获取
    * @param[in]    pSignVersion        病毒库版本的结构体
    * @remark       不需要病毒库已加载
    */ 
    virtual HRESULT STDMETHODCALLTYPE GetLocalSignVersion(
        LPCWSTR             lpszSignIniPath,
        BKAVE_SIGN_VERSION* pSignVersion) = 0;



    /**
    * @brief        加载常驻病毒库,加载的病毒库会长期驻留内存
    * @remark       如果已经加载了病毒库,这里会直接返回
    */ 
    virtual HRESULT STDMETHODCALLTYPE LoadDaemonSign() = 0;


    /**
    * @brief        卸载常驻病毒库,加载的病毒库会长期驻留内存
    * @remark       如果已经加载了病毒库,这里会直接返回
    */ 
    virtual HRESULT STDMETHODCALLTYPE UnloadDaemonSign() = 0;


    /**
    * @brief        重新加载病毒库,加载的病毒库会长期驻留内存
    * @remark       如果重新加载病毒库的时候, 有引擎正在扫描, 该引擎使用的病毒库不会被立即替换, 而是等到扫描完成时替换
    */ 
    virtual HRESULT STDMETHODCALLTYPE ReloadSign() = 0;
};



/// 获取文件基本类型接口
class __declspec(uuid("BE23349C-611A-48cd-A6F2-4C7CCB7AFEE9"))
IBKAveFileTypeIdentifier : public IUnknown
{
public:


    /**
	* @brief        获取文件的基本类型
	* @param[in]    lpszFilePath    要检查的文件路径
    * @param[in]    dwFileTag       文件标签
    * @param[out]   pdwFileType     返回文件类型,取值见 ksfiletype.h
	* @return       标准HRESULT返回值
	**/
	virtual BOOL    STDMETHODCALLTYPE LookupCachedFileType(
        LPCWSTR lpszFilePath,
        DWORD   dwFileTag,
        DWORD*  pdwFileType) = 0;


    /**
	* @brief        获取文件的基本类型
	* @param[in]    lpszFilePath    要检查的文件路径
    * @param[in]    dwFileTag       文件标签
    * @param[in]    dwFileType      文件类型,取值见 ksfiletype.h
	* @return       标准HRESULT返回值
	**/
	virtual HRESULT STDMETHODCALLTYPE UpdateFileTypeToCache(
        LPCWSTR lpszFilePath,
        DWORD   dwFileTag,
        DWORD   dwFileType) = 0;


	/**
    * @brief        获取文件的类型
    * @param[in]    hFile           要检查的文件句柄
    * @param[out]   pdwFileType     返回文件类型,取值见 ksfiletype.h
	* @return       标准HRESULT返回值
	**/
	virtual HRESULT STDMETHODCALLTYPE GetFileType(
        HANDLE  hFile,
        DWORD*  pdwFileType) = 0;


    /**
    * @brief        强制将缓存输出到数据库
    */
    virtual HRESULT STDMETHODCALLTYPE FlushFileTypeCache() = 0;


    /**
    * @brief        文件类型是否需要被Ave过滤
    */
    virtual BOOL    STDMETHODCALLTYPE IsFileTypeNeedSkipByAve(DWORD dwFileType) = 0;
};






NS_SKYLARK_END






#define  FN_BKAveInitialize     "BKAveInitialize"       ///< export BKAveInitialize
#define  FN_BKAveUninitialize   "BKAveUninitialize"     ///< export BKAveUninitialize
#define  FN_BKAveCreateObject   "BKAveCreateObject"     ///< export BKAveCreateObject

/**
* @brief    初始化扫描模块
* @param    dwFlag  保留,未使用
*/
EXTERN_C HRESULT WINAPI BKAveInitialize(Skylark::BKAVE_PLATFORM_INIT* pInit);
/// typedef for BKAveInitialize
typedef HRESULT (WINAPI *PFN_BKAveInitialize)(Skylark::BKAVE_PLATFORM_INIT* pInit);



/**
* @brief    反初始化扫描模块
* @remark   调用反初始化前必须确保该模块导出的所有对象都被释放
*/
EXTERN_C HRESULT WINAPI BKAveUninitialize();
/// typedef for BKAveUninitialize
typedef HRESULT (WINAPI *PFN_BKAveUninitialize)();



/**
* @brief    创建BKAve的模块
* @param    dwBKEngVer  BKEng的版本号,默认使用BKENG_CURRENT_VERSION,传入0表示不作版本检查
* @param    riid        要创建的对象的iid
* @param    ppvObj      返回创建的对象
*/
EXTERN_C HRESULT WINAPI BKAveCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKAveCreateObject
typedef HRESULT (WINAPI *PFN_BKAveCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);



#endif//BKAVE_H