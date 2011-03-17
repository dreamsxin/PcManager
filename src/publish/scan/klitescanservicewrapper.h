#pragma once
#include "kselitecanservice_def.h"

class ILiteScanService;

class KLiteScanServiceWrapper
{
public:
	KLiteScanServiceWrapper(void);
	~KLiteScanServiceWrapper(void);

	int Initialize();

	int Uninitialize();

	bool IsInitialized();

	/**
	* @brief        创建一个扫描会话
	* @param[out]   scanHandle    返回的会话句柄
	* @return		0 成功,其他为失败错误码
	* @remark       线程安全操作, 
	*/
	virtual int __stdcall CreateScanSession(
		/* [in]  */ const KXE_NULL_PARAEMETER&,     
		/* [out] */ S_KSE_LITESCAN_SESSION& scanHandle
		);

	/**
	* @brief        关闭一个扫描会话
	* @param[in]    ScanHandle 通过CreateScanSession获取
	* @return		0 成功,其他为失败错误码
	* @remark       线程安全操作, 当有多个Session时为保证退出效率,要安排好NotifyStopScan()与CloseScanSession的顺序
	*/
	virtual int __stdcall CloseScanSession(
		/* [in] */ const S_KSE_LITESCAN_SESSION& ScanHandle,
		/* [out]*/ KXE_NULL_PARAEMETER& nullParm
		);


	/**
	* @brief        通知扫描会话停止
	* @param[in]    ScanHandle 通过CreateScanSession获取
	* @return		0 成功,其他为失败错误码
	* @remark       线程安全操作,调用此接口后,对应的ScanHandle不能再做扫描，如需要继续扫描，就Close掉再创建一个新的。
	*/
	virtual int __stdcall NotifyStopScan(
		/* [in] */ const S_KSE_LITESCAN_SESSION& ScanHandle,
		/* [out]*/ KXE_NULL_PARAEMETER& nullParm
		);


	/**
	* @brief         执行扫描
	* @param [in]    ScanHandle 通过CreateScanSession获取
	* @param [in]    ScanTarget 扫描目标的相关信息
	* @param [out]   ScanResults 扫描目标对应的扫描结果，与ScanTarget中的vecScanFiles一一对应
	* @return		 0 成功,其他为失败错误码
	* @remark        不能在两个线程里对同一个ScanHandle做调用,如果需要多线程,为每个线程创建一个ScanHandle
	*/
	virtual int __stdcall ScanFiles(
		/* [in] */ const S_KSE_LITESCAN_SESSION& ScanHandle,
		/* [in] */ const S_KSE_LITESCAN_TARGET_INFO& ScanTarget,
		/* [out]*/ std::vector<S_KSE_LITESCAN_RESULT>& ScanResults
		);


	/**
	* @brief        单独处理一个文件,不再进扫描流程，直接删除并备份
	* @param[in]    处理操作相关信息
	* @param[in]    处理成功时的相关结果信息(返回值为0时此结果有效)
	* @return		0 成功,其他为失败错误码
	* @remark       线程安全操作
	*/
	virtual int __stdcall ProcessFile(
		/* [in]  */ const S_KSE_PROCESS_FILE_INFO& FileInfo,
		/* [out]*/  S_KSE_PROCESS_FILE_RESULT& ProcessResult
		);

private:
	bool m_bIsInitialized;
;
};
