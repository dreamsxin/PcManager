/**
* @file    bkperf.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-29 19:59
*/

#ifndef BKPERF_H
#define BKPERF_H

#include <utility>
#include <atlcoll.h>
#include "bkdef.h"

NS_SKYLARK_BEGIN



//////////////////////////////////////////////////////////////////////////
// 性能测试参数

// ------------------------------------------------------------
// 云查杀的性能测试参数
#define BKENG_PERF_LOCAL_SCAN                   1       ///< LocalScan(前置扫描)
#define BKENG_PERF_REVIEW_SCAN                  2       ///< ReviewScan(后置扫描)
#define BKENG_PERF_QUERY                        3       ///< Query(联网查询)
#define BKENG_PERF_REQUERY                      4       ///< Requery(重试的联网查询)

#define BKENG_PERF_NORMALIZE_PATH               10      ///< 正规化文件路径
#define BKENG_PERF_CALC_FILE_TAG                11      ///< 计算文件tag
#define BKENG_PERF_CREATE_FILE                  12      ///< 打开文件
#define BKENG_PERF_CHECK_PE                     13      ///< 检查是否是PE(不包含打开文件的时间)
#define BKENG_PERF_HASH_FILE                    14      ///< 计算文件hash(不包含打开文件的时间)
#define BKENG_PERF_WINTRUST                     15      ///< 检查数字签名
#define BKENG_PERF_ENCODE                       16      ///< 编码协议
#define BKENG_PERF_DECODE                       17      ///< 解码协议

#define BKENG_PERF_LOOKUP_PATH_TO_NONPE         30      ///< 查询缓存 path - nonpe
#define BKENG_PERF_LOOKUP_PATH_TO_WINTRUST      31      ///< 查询缓存 path - wintrust
#define BKENG_PERF_LOOKUP_PATH_TO_HASH          32      ///< 查询缓存 path - hash
#define BKENG_PERF_LOOKUP_HASH_TO_LEVEL         33      ///< 查询缓存 hash - level

#define BKENG_PERF_UPDATE_PATH_TO_NONPE         40      ///< 更新缓存 path - nonpe     
#define BKENG_PERF_UPDATE_PATH_TO_WINTRUST      41      ///< 更新缓存 path - wintrust
#define BKENG_PERF_UPDATE_PATH_TO_HASH          42      ///< 更新缓存 path - hash
#define BKENG_PERF_UPDATE_HASH_TO_LEVEL         43      ///< 更新缓存 hash - level

#define BKENG_PERF_FLUSH_PATH_TO_NONPE          50      ///< FLUSH缓存 path - nonpe
#define BKENG_PERF_FLUSH_PATH_TO_WINTRUST       51      ///< FLUSH缓存 path - wintrust
#define BKENG_PERF_FLUSH_PATH_TO_HASH           52      ///< FLUSH缓存 path - hash
#define BKENG_PERF_FLUSH_HASH_TO_LEVEL          53      ///< FLUSH缓存 hash - level

// ------------------------------------------------------------
// 文件引擎的性能测试参数

#define BKENG_PERF_AVENG_MIN                    100     ///< ...

#define BKENG_PERF_AVENG_GET_TYPE               100     ///< 获取文件类型
#define BKENG_PERF_AVENG_SCAN                   101     ///< 文件引擎扫描
#define BKENG_PERF_AVENG_CLEAN                  102     ///< 文件引擎清除
#define BKENG_PERF_AVENG_BWM_SCAN               103     ///< 调用黑白机扫描
#define BKENG_PERF_AVENG_BWM_CLEAN              104     ///< 调用黑白机清除
#define BKENG_PERF_AVENG_BWM_RESCAN             105     ///< 调用黑白机扫描清除后的文件
#define BKENG_PERF_AVENG_GET_VIRUS_NAME         106     ///< 获取病毒名
#define BKENG_PERF_AVENG_OPEN_SCAN_FILE         107     ///< 打开扫描文件
#define BKENG_PERF_AVENG_CREATE_TEMP_FILE       108     ///< 创建临时文件
#define BKENG_PERF_AVENG_CALC_FILE_TAG          109     ///< 计算文件ave tag
#define BKENG_PERF_AVENG_PRE_GET_TYPE           110     ///< 获取文件类型(前置)

#define BKENG_PERF_AVENG_ARC_GET_TYPE           120     ///< 获取压缩包类型
#define BKENG_PERF_AVENG_ARC_EXTRACT            121     ///< 解压压缩包
#define BKENG_PERF_AVENG_ARC_EXTRACT_SINGLE     122     ///< 解压压缩包中的单个文件

#define BKENG_PERF_AVENG_UNPACK_GET_TYPE        130     ///< 获取壳类型
#define BKENG_PERF_AVENG_UNPACK_UNPACK          131     ///< 脱壳

#define BKENG_PERF_LOOKUP_PATH_TO_AVEFT         140     ///< 查询缓存 path - ave file type
#define BKENG_PERF_LOOKUP_PATH_TO_AVEBWM        141     ///< 查询缓存 path - ave bwm result

#define BKENG_PERF_UPDATE_PATH_TO_AVEFT         150     ///< 更新缓存 path - ave file type
#define BKENG_PERF_UPDATE_PATH_TO_AVEBWM        151     ///< 查询缓存 path - ave bwm result

#define BKENG_PERF_FLUSH_PATH_TO_AVEFT          160     ///< FLUSH缓存 path - ave file type
#define BKENG_PERF_FLUSH_PATH_TO_AVEBWM         161     ///< 查询缓存 path - ave bwm result

#define BKENG_PERF_LOCAL_HEURIST                170     ///< 本地启发式

#define BKENG_PERF_KCLOUD_LOCAL_DETECT_1ST      180     ///< 毒霸云本地,第一趟
#define BKENG_PERF_KCLOUD_LOCAL_DETECT_2ND      181     ///< 毒霸云本地,第二趟
#define BKENG_PERF_KCLOUD_NET_DETECT            182     ///< 毒霸云联网

#define BKENG_PERF_AVENG_MAX                    199     ///< ...

// ------------------------------------------------------------

#define BKENG_PERF_MAX_ITEM                     199     ///< ...

//////////////////////////////////////////////////////////////////////////







// ------------------------------------------------------------
// 性能测试数据

struct BKENG_PERF_DATA
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    UINT32      m_uPerfCount;           ///< 测试次数
    UINT64      m_uPerfTotal;           ///< 测试总时间
    UINT64      m_uTopPerf[10];         ///< 耗时最高的10次测试的时间

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKENG_PERF_DATA* pStruct)
{
    assert(pStruct);
    pStruct->uSize          = sizeof(BKENG_PERF_DATA);   

    pStruct->m_uPerfCount   = 0;
    pStruct->m_uPerfTotal   = 0;
    memset(pStruct->m_uTopPerf, 0, sizeof(pStruct->m_uTopPerf));
}






/**
* @brief    性能参数查看器
*/
class __declspec(uuid("0B93BC23-BEBC-482b-BDD3-9BC68868AD0E"))
IBKEngPerformanceView: public IUnknown
{
public:

    /**
    * @brief        获取数据
    * @param[in]    dwPerfID    数据项的ID,取值为 BKENG_PERF_xxx
    * @param[out]   dwPerfData  性能数据
    */
    virtual HRESULT STDMETHODCALLTYPE GetPerformanceData(
        DWORD               dwPerfID,
        BKENG_PERF_DATA*    pPerfData) = 0;

    /**
    * @brief        将Tick转换成系统时间
    * @param[in]    uTick       tick数
    * @remark       单位为 100 ns
    */
    virtual ULONGLONG STDMETHODCALLTYPE TickToSystemTime(ULONGLONG uTick) = 0;
};




/**
* @brief    性能参数获取接口
*/
class __declspec(uuid("33FC2590-C8BE-44c1-9E02-8DED6244C76D"))
IBKEngPerformanceCounter: public IUnknown
{
public:

    /**
    * @brief        重置数据
    */
    virtual void STDMETHODCALLTYPE ResetPerf() = 0;

    /**
    * @brief        获取当前计时
    */
    virtual ULONGLONG STDMETHODCALLTYPE GetTick() = 0;

    /**
    * @brief        添加数据
    * @param[in]    dwPerfID    数据项的ID,取值为 BKENG_PERF_xxx
    * @param[in]    uTickDiff   性能数据
    */
    virtual void STDMETHODCALLTYPE AddPerf(DWORD dwPerfID, ULONGLONG uTickDiff) = 0;

    /**
    * @brief        将Tick转换成系统时间
    * @param[in]    uTick       tick数
    * @remark       单位为 100 ns
    */
    virtual ULONGLONG STDMETHODCALLTYPE TickToSystemTime(ULONGLONG uTick) = 0;
};




NS_SKYLARK_END

#endif//BKPERF_H