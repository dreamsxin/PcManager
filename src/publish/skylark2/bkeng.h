/**
* @file    bkeng.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-09 21:42
*/

#ifndef BKENG_H
#define BKENG_H

#include <unknwn.h>
#include "skylarkbase.h"
#include "bkengdef.h"
#include "bklevel.h"

NS_SKYLARK_BEGIN

/**
* @brief 扫描引擎调用接口
*/
class __declspec(uuid("7F70ECF3-22FB-4c55-B6EC-1C8C0B2EFCDA"))
IBKEngineCallback
{
public:

    /**
    * @brief        扫描文件前调用
    * @param[in]    lpszFilePath    原始文件路径
    * @param[in]    pvFlag          提交文件时传入的回调令牌
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */
    virtual HRESULT STDMETHODCALLTYPE BeforeScanFile(
        LPCWSTR             lpszFilePath,
        void*               pvFlag) = 0;


    /**
    * @brief        扫描文件后调用
    * @param[in]    lpszFilePath    原始文件路径
    * @param[in]    pResult         当前文件的扫描结果
    * @param[in]    pvFlag          提交文件时传入的回调令牌
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */
    virtual HRESULT STDMETHODCALLTYPE AfterScanFile(
        LPCWSTR             lpszFilePath,
        const BKENG_RESULT* pResult,
        void*               pvFlag) = 0;


    /**
    * @brief        清除文件前调用 (禁用自动清除时,不会调用此回调)
    * @param[in]    lpszFilePath    原始文件路径
    * @param[in]    pResult         当前文件的扫描结果
    * @param[in]    pvFlag          提交文件时传入的回调令牌
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */
    virtual HRESULT STDMETHODCALLTYPE BeforeCleanFile(
        LPCWSTR             lpszFilePath,
        const BKENG_RESULT* pResult,
        void*               pvFlag) = 0;


    /**
    * @brief        清除文件后调用 (禁用自动清除时,不会调用此回调)
    * @param[in]    lpszFilePath    原始文件路径
    * @param[in]    pResult         当前文件的清除结果
    * @param[in]    pvFlag          提交文件时传入的回调令牌
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */
    virtual HRESULT STDMETHODCALLTYPE AfterCleanFile(
        LPCWSTR             lpszFilePath,
        const BKENG_RESULT* pResult,
        void*               pvFlag) = 0;


    /**
    * @brief        扫描过程中返回细节 (如壳信息,压缩包中的病毒等...)
    * @param[in]    lpszVirtualPath 扫描对象的虚拟路径
    * @param[in]    pResult         扫描对象的扫描结果
    * @param[in]    pvFlag          提交文件时传入的回调令牌
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */ 
    virtual HRESULT STDMETHODCALLTYPE OnScanDetail(
        LPCWSTR             lpszVirtualPath,
        const BKENG_RESULT* pResult,
        void*               pvFlag) = 0;


    /**
    * @brief        空闲时候的回调
    * @param[in]    lpszFilePath    当前正在扫描的文件 (如果当前没有正在扫描的文件,这里可能为NULL)
    * @param[in]    pvFlag          提交文件时传入的回调令牌
    * @return       返回E_FAIL或其他错误会导致扫描中断
    */ 
    virtual HRESULT STDMETHODCALLTYPE OnIdle() = 0;



    /**
    * @brief        结束时回调
    * @param[in]    hrFinishCode    扫描返回的错误码
    */ 
    virtual HRESULT STDMETHODCALLTYPE OnFinish(
        HRESULT             hrFinishCode) = 0;
};







/**
* @brief 扫描引擎调用接口
*/
class __declspec(uuid("479D062C-5D89-4694-9818-3BA5BFECEBCB"))
IBKEngine: public IUnknown
{
public:

    /**
    * @brief        初始化扫描引擎
    * @param[in]    pvReserved      保留参数
    */
    virtual HRESULT STDMETHODCALLTYPE Initialize(void* pvReserved = NULL) = 0;


    /**
    * @brief        反初始化扫描引擎
    * @remark       务必确保在 Scan 返回后才调用反初始化
    */
    virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;


    /**
    * @brief        获取扫描设置
    * @param[out]   pSettings   保存返回的扫描设置
    */
    virtual HRESULT STDMETHODCALLTYPE GetScanSettings(BKENG_SETTINGS* pSettings) = 0;


    /**
    * @brief        修改扫描设置
    * @param[in]    pSettings   扫描设置,如果为NULL,会应用默认设置
    * @remark       在扫描过程中修改设置将引起未定义的行为
    */
    virtual HRESULT STDMETHODCALLTYPE SetScanSettings(const BKENG_SETTINGS* pSettings) = 0;



    /**
    * @brief        将扫描状态复位,以便进行再一次的扫描
    * @remark       在同一引擎实例的两次Scan之间,务必调用一次Reset
    */
    virtual HRESULT STDMETHODCALLTYPE Reset() = 0;



    /**
    * @brief        将一个文件添加到扫描队列
    * @param[in]    pszFileFullName 文件的完整路径
    * @param[in]    pFindData       枚举文件时候的结构体,如果不为NULL,可以加速扫描速度
    * @param[in]    pvFlag          调用者定义的绑定参数,回调时原样返回
    * @param[in]    dwHighWaterMark 高水位线,当等待扫描的文件超过 dwHighWaterMark 时,会阻塞,直到文件数低于 dwHighWaterMark,如果为0,则不阻塞
    * @remark       如果队列太满,会导致调用阻塞
    * @retval       S_OK            文件被提交
    * @retval       E_ABORT         文件未提交(超时或中断)
    */
    virtual HRESULT STDMETHODCALLTYPE AppendFile(
        LPCWSTR                 pszFileFullName,
        const WIN32_FIND_DATAW* pFindData,
        void*                   pvFlag          = NULL,
        DWORD                   dwHighWaterMark = 5000) = 0;



    /**
    * @brief        将一个hash添加到扫描队列
    * @param[in]    pHashScanParam  hash扫描参数
    * @param[in]    pFindData       枚举文件时候的结构体,如果不为NULL,可以加速扫描速度
    * @param[in]    pvFlag          调用者定义的绑定参数,回调时原样返回
    * @param[in]    dwHighWaterMark 高水位线,当等待扫描的文件超过 dwHighWaterMark 时,会阻塞,直到文件数低于 dwHighWaterMark,如果为0,则不阻塞
    * @remark       如果队列太满,会导致调用阻塞
    * @retval       S_OK            文件被提交
    * @retval       E_ABORT         文件未提交(超时或中断)
    */
    virtual HRESULT STDMETHODCALLTYPE AppendHash(
        const BKENG_HASH_SCAN_PARAM*    pHashScanParam,
        void*                           pvFlag          = NULL,
        DWORD                           dwHighWaterMark = 5000) = 0;



    /**
    * @brief        提交任务
    * @remark       表示已经完成所有文件的提交,未进行在线查询的文件将被立即查询
    */
    virtual HRESULT STDMETHODCALLTYPE Commit() = 0;



    /**
    * @brief        开始异步扫描
    * @param[in]    piCallback      回调接口
    * @remark       该调用为非阻塞操作,必须确保扫描期间,piCallback始终有效。
    *               如果前一次扫描没有结束就调用AsyncScan,会导致未定义的行为
    */
    virtual HRESULT STDMETHODCALLTYPE AsyncScan(IBKEngineCallback* piCallback) = 0;



    /**
    * @brief        中断一个扫描任务
    * @remark       该调用为非阻塞操作,会触发 Scan 立即返回 E_ABORT,
    *               扫描确实退出后,会调用piCallback->OnFinish()。
    *               如果引擎处于暂停中,该调用会导致死锁
    */
    virtual HRESULT STDMETHODCALLTYPE NotifyStop() = 0;



    /**
    * @brief        等待扫描结束或因中断而退出
    * @param[in]    dwWaitTime  等待时间,设为0可以用来测试扫描是否结束
    * @return       返回值同 WaitForSingleObject
    */
    virtual DWORD   STDMETHODCALLTYPE WaitForFinish(DWORD dwWaitTime) = 0;



    /**
    * @brief        暂停一个扫描任务
    * @remark       该调用为确认扫描暂停,回调不再返回新的结果时才返回
    */
    virtual HRESULT STDMETHODCALLTYPE Pause() = 0;



    /**
    * @brief        恢复一个扫描任务
    */
    virtual HRESULT STDMETHODCALLTYPE Resume() = 0;


    //////////////////////////////////////////////////////////////////////////
    // IBKEngine_v4 new interfaces


    /**
    * @brief        将一个文件添加到扫描队列
    * @param[in]    pszFileFullName 文件的完整路径
    * @param[in]    pScanParam      扫描参数,可以为NULL
    * @remark       如果队列太满,会导致调用阻塞
    * @retval       S_OK            文件被提交
    * @retval       E_ABORT         文件未提交(超时或中断)
    */
    virtual HRESULT STDMETHODCALLTYPE AppendFileEx(
        LPCWSTR             pszFileFullName,
        BKENG_SCAN_PARAM*   pScanParam) = 0;

    /**
    * @brief        强制扫描队列中暂存的文件
    */
    virtual HRESULT STDMETHODCALLTYPE Flush() = 0;
};








/**
* @brief    简化的扫描引擎调用接口,不需要实现回调
* @remark   该接口不使用回调,以确保调用的简单,但在批量查询的时候会非常慢
*/
class __declspec(uuid("5112296D-AA01-4dc6-8744-EADABF858D1D"))
IBKEngineLite: public IUnknown
{
public:

    /**
    * @brief        初始化扫描引擎
    * @param[in]    pvReserved      保留参数
    */
    virtual HRESULT STDMETHODCALLTYPE Initialize(void* pvReserved = NULL) = 0;


    /**
    * @brief        反初始化扫描引擎
    * @remark       务必确保在 Scan 返回后才调用反初始化
    */
    virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;


    /**
    * @brief        获取扫描设置
    * @param[out]   pSettings   保存返回的扫描设置
    */
    virtual HRESULT STDMETHODCALLTYPE GetScanSettings(BKENG_SETTINGS* pSettings) = 0;


    /**
    * @brief        修改扫描设置
    * @param[in]    pSettings   扫描设置,如果为NULL,会应用默认设置
    * @remark       在扫描过程中修改设置将引起未定义的行为
    */
    virtual HRESULT STDMETHODCALLTYPE SetScanSettings(const BKENG_SETTINGS* pSettings) = 0;


    /**
    * @brief        扫描文件
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    dwScanMask      扫描掩码,取值为BKENG_SCAN_MASK_xxx的组合
    * @param[out]   pScanResult     文件扫描结果
    * @param[in]    piCallback      文件扫描结果,可以为NULL(通过用于清除前的某些操作)
    * @param[in]    pvReserved      保留参数,未使用
    * @remark       如果函数返回失败,那么pScanResult中的数据将无意义
    */ 
    virtual HRESULT STDMETHODCALLTYPE Scan(
        LPCWSTR             lpszFilePath,
        DWORD               dwScanMask,
        BKENG_RESULT*       pScanResult,
        IBKEngineCallback*  piCallback = NULL,
        void*               pvReserved = NULL) = 0;


    /**
    * @brief        扫描hash
    * @param[in]    pHashScanParam  hash扫描参数
    * @param[out]   pScanResult     文件扫描结果
    * @param[in]    piCallback      文件扫描结果,可以为NULL(通过用于清除前的某些操作)
    * @param[in]    pvReserved      保留参数,未使用
    * @remark       如果函数返回失败,那么pScanResult中的数据将无意义
    */ 
    virtual HRESULT STDMETHODCALLTYPE ScanHash(
        const BKENG_HASH_SCAN_PARAM*    pHashScanParam,
        BKENG_RESULT*                   pScanResult,
        IBKEngineCallback*              piCallback = NULL,
        void*                           pvReserved = NULL) = 0;


    /**
    * @brief        根据扫描结果清除文件
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    dwScanMask      扫描掩码,取值为BKENG_SCAN_MASK_xxx的组合
    * @param[in]    pScanResult     文件上次扫描结果
    * @param[out]   pCleanResult    文件清除结果
    * @param[in]    piCallback      文件扫描结果,可以为NULL(通过用于清除前的某些操作)
    * @param[in]    pvReserved      保留参数,未使用
    * @remark       如果函数返回失败,那么pScanResult中的数据将无意义
    */ 
    virtual HRESULT STDMETHODCALLTYPE HintClean(
        LPCWSTR             lpszFilePath,
        DWORD               dwScanMask,
        const BKENG_RESULT* pScanResult,
        BKENG_RESULT*       pCleanResult,
        IBKEngineCallback*  piCallback = NULL,
        void*               pvReserved = NULL) = 0;


    /**
    * @brief        中断正在进行的扫描
    */
    virtual HRESULT STDMETHODCALLTYPE Interrupt() = 0;
};








/**
* @brief    上传接口
*/
class __declspec(uuid("38E7A42B-9F47-4774-85E5-20892039B922"))
IBKEngUploader: public IUnknown
{
public:

    /**
    * @brief        初始化扫描引擎
    * @param[in]    pvReserved      保留参数
    */
    virtual HRESULT STDMETHODCALLTYPE Initialize(void* pvReserved = NULL) = 0;


    /**
    * @brief        反初始化扫描引擎
    */
    virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;


    /**
    * @brief        扫描文件
    * @param[in]    lpszFilePath    文件路径
    * @param[in]    piCallback      上传回调
    * @param[in]    pUploadParam    上传参数
    */ 
    virtual HRESULT STDMETHODCALLTYPE Upload(
        LPCWSTR             lpszFilePath,
        IBKProgress*        piCallback,
        BKENG_UPLOAD_PARAM* pUploadParam) = 0;


    /**
    * @brief        中断一个扫描任务
    * @remark       该调用为非阻塞操作,会触发 Upload 立即返回失败
    */
    virtual HRESULT STDMETHODCALLTYPE NotifyStop() = 0;
};






/**
* @brief    握手协议
*/
class __declspec(uuid("9FC31DD2-F1EF-4964-A34A-C52B0F79C488"))
IBKEngineEcho: public IUnknown
{
public:

    /**
    * @brief        初始化握手对象
    * @param[in]    pvReserved      保留参数
    */
    virtual HRESULT STDMETHODCALLTYPE Initialize(void* pvReserved = NULL) = 0;


    /**
    * @brief        反初始化握手对象
    */
    virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;


    /**
    * @brief        检测文件
    * @param[in]    lpszFilePath    要检测的文件路径
    * @param[in]    pvFlag          调用者定义的绑定参数
    */ 
    virtual HRESULT STDMETHODCALLTYPE AppendFile(
        LPCWSTR lpszFilePath,
        void*   pvToken) = 0;


    /**
    * @brief        检测文件
    * @param[out]   pEchoResult     返回握手结果
    */ 
    virtual HRESULT STDMETHODCALLTYPE Echo(
        BKENG_ECHO_RESULT*  pEchoResult) = 0;


    /**
    * @brief        获取文件检测结果
    * @param[in]    pvToken         标识要获取的文件结果
    * @param[out]   pFileResult     返回握手结果
    */ 
    virtual HRESULT STDMETHODCALLTYPE GetEchoResult(
        void*           pvToken,
        BKENG_RESULT*   pFileResult) = 0;


    /**
    * @brief        中断正在进行的扫描
    */
    virtual HRESULT STDMETHODCALLTYPE Interrupt() = 0;


    /**
    * @brief        获取握手设置
    * @param[out]   pSettings   保存返回的握手设置
    */
    virtual HRESULT STDMETHODCALLTYPE GetEchoSettings(BKENG_ECHO_SETTINGS* pSettings) = 0;


    /**
    * @brief        修改握手设置
    * @param[in]    pSettings   握手设置,如果为NULL,会应用默认设置
    * @remark       在握手过程中修改设置将引起未定义的行为
    */
    virtual HRESULT STDMETHODCALLTYPE SetEchoSettings(const BKENG_ECHO_SETTINGS* pSettings) = 0;
};




/// 获取内部组件用的接口
class __declspec(uuid("44D6DC32-B247-4496-8CF8-C611A2D9B3A9"))
IBKEngineInternal: public IUnknown
{
public:

    
    /**
    * @brief        获取内部组件接口
    * @param[id]    riid    接口iid
    * @param[out]   ppvObj  返回接口指针
    */
    virtual HRESULT STDMETHODCALLTYPE QueryInternalComponent(REFIID riid, void** ppvObj) = 0;
};





/// 文件清除工具接口
class __declspec(uuid("550A1550-366D-42f8-8347-BCB9F382A641"))
IBKEngFileToolkit: public IUnknown
{
public:

    /**
    * @brief        删除文件
    * @param[in]    lpszFilePath    文件路径
    * @retval       S_OK                            扫描成功
    * @retval       S_BKE_DELETE_FILE_NEED_REBOOT   需要重启
    */ 
    virtual HRESULT STDMETHODCALLTYPE DeleteFile(LPCWSTR lpszFilePath) = 0;


    /**
    * @brief        替换文件
    * @param[in]    lpszDstFile     替换的目标文件
    * @param[in]    lpszSrcFile     替换的源文件
    * @param[in]    bKeepSrcFile    保留源文件
    * @retval       S_OK                            扫描成功
    * @retval       S_BKE_REPLACE_FILE_NEED_REBOOT  需要重启
    */ 
    virtual HRESULT STDMETHODCALLTYPE ReplaceFile(
        LPCWSTR lpszDstFile,
        LPCWSTR lpszSrcFile,
        BOOL    bKeepSrcFile = FALSE) = 0;


    /**
    * @brief        强制重启删除文件
    * @param[in]    lpszFilePath    文件路径
    * @retval       S_OK                            扫描成功
    * @retval       S_BKE_DELETE_FILE_NEED_REBOOT   需要重启
    */ 
    virtual HRESULT STDMETHODCALLTYPE ForceBootDeleteFile(LPCWSTR lpszFilePath) = 0;
};





/// 文件引擎控制器
class __declspec(uuid("4105C1FF-F8E2-4d2c-8801-4332600A5730"))
IBKEngAveController: public IUnknown
{
public:

    /**
    * @brief        从内存安装文件引擎
    * @param[in]    pAveLoad        加载参数
    * @remark       新创建的引擎实例,将会使用文件引擎.
    *               已经创建的引擎实例仍然使用老的设置.
    *               该调用不会自动加载病毒库.
    */ 
    virtual HRESULT STDMETHODCALLTYPE LoadAvEngine(BKENG_AVE_LOAD* pAveLoad) = 0;


    /**
    * @brief        从内存卸载文件引擎
    * @param[in]    pvReserved      保留参数
    * @remark       新创建的引擎对象,不会使用文件引擎.
    *               已经创建的引擎实例仍然使用老的设置.
    *               退出时,调用者不需要手动卸载引擎
    */
    virtual HRESULT STDMETHODCALLTYPE UnloadAvEngine() = 0;


    /**
    * @brief        加载常驻病毒库,加载的病毒库会长期驻留内存
    * @param[in]    pvReserved      保留参数
    * @remark       如果常驻病毒库已经加载,这里不会做任何事情
    */ 
    virtual HRESULT STDMETHODCALLTYPE LoadDaemonSign() = 0;


    /**
    * @brief        卸载常驻病毒库
    * @param[in]    pvReserved      保留参数
    * @remark       如果常驻病毒库没有加载,这里不会做任何事情
    *               退出时,调用者不需要手动卸载病毒库
    */ 
    virtual HRESULT STDMETHODCALLTYPE UnloadDaemonSign() = 0;


    /**
    * @brief        通知所有引擎实例更新病毒库
    * @param[in]    pvReserved      保留参数
    * @remark       如果重新加载病毒库的时候, 有引擎正在扫描, 该引擎使用的病毒库不会被立即替换, 而是等到扫描完成时替换
    */ 
    virtual HRESULT STDMETHODCALLTYPE ReloadSign() = 0;


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
};






NS_SKYLARK_END




#define FN_BKEngInitialize              "BKEngInitialize"
#define FN_BKEngUninitialize            "BKEngUninitialize"
#define FN_BKEngCreateObject			"BKEngCreateObject"


/**
* @brief    初始化扫描模块
*/
EXTERN_C HRESULT WINAPI BKEngInitialize(Skylark::BKENG_PLATFORM_INIT* pInit);
/// typedef for BKEngInitialize
typedef HRESULT (WINAPI *PFN_BKEngInitialize)(Skylark::BKENG_PLATFORM_INIT* pInit);


/**
* @brief    反初始化扫描模块
*/
EXTERN_C HRESULT WINAPI BKEngUninitialize();
/// typedef for BKEngUninitialize
typedef HRESULT (WINAPI *PFN_BKEngUninitialize)();


/**
* @brief    创建扫描引擎实例
* @param    dwBKEngVer  BKEng的版本号,默认使用BKENG_CURRENT_VERSION,传入0表示不作版本检查
* @param    riid        要创建的对象的iid
* @param    ppvObj      返回创建的对象
*/
EXTERN_C HRESULT WINAPI BKEngCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKEngCreateObject
typedef HRESULT (WINAPI *PFN_BKEngCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);


#endif//BKENG_H