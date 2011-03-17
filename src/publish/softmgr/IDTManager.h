/**
* @file    IDtManager.h
* @brief   下载管理
* @author  liveck
* @date    2010/04/09 20:11
*/

#pragma once

enum DTManager_Stat{TASK_DONE=0,TASK_DOWNING,TASK_PAUSED,TASK_CONECTING,TASK_ERROR,TASK_ERROR_MD5};


typedef void (*GetTaskCallback)(void* tk,void* para);
static void GetTaskCallbackByArr(void* tk,void* para)
{
	CAtlArray<void*>* tasks=(CAtlArray<void*>*)para;
	tasks->Add(tk);
}

//获取任务信息的回调
typedef void (*InfoCallBack)(__int64 cur,__int64 all,__int64 speed,DWORD time,void* para);

//任务进度发生变化的回调
/**
* tk是任务上下文 
* para是自定义回调的参数
*
*/
typedef void (*DtStateChangeNotifyCallBack)(DTManager_Stat st,void* tk,void* para);

class __declspec(uuid("43D2ACDB-2D19-4637-8BE9-4B5FF020A2E3")) IDTManager
{
public:
	virtual void Init(CAtlMap<DtStateChangeNotifyCallBack,void*>* calls)=0;

	/**
	* 新建任务，下载从cur开始
	* @return   void*
	* @param CAtlArray<CString> * urls 最后一个url会被单线程处理
	* @param CString localfile
	* @param int64 cur
	*/
	virtual void* NewTask(CAtlArray<CString>* urls,CString md5,CString localfile)=0;

	/**
	* 恢复被停止的任务 
	* @return   void
	* @param void * context
	* @remark 多次resume会死机哦！
	*/
	virtual void ResumeTask(void* context)=0;

	/**
	* 停止任务
	* @return   void
	* @param void * context
	*/
	virtual void PauseTask(void* context)=0;

	/**
	* 停止任务
	* @return   void
	* @param void * context
	*/
	virtual void StopTask(void* context)=0;

	/**
	* 取消任务，回收context
	* @return   void
	* @param void * context
	*/
	virtual void CancelTask(void* context)=0;

	/**
	* 查询任务数量
	* @return   size_t
	*/
	virtual size_t GetTaskCount()=0;

	virtual int GetTasks(GetTaskCallback func,void* para )=0;

	/**
	* 查询任务信息
	* @return   void
	* @param void * tk
	* @param InfoCallBack func
	* @param void * para
	*/
	virtual void QueryTask(void* tk,InfoCallBack func,void* para)=0;
};