/**
* @file    ISoftChecker.h
* @brief   检测软件安装卸载信息
* @author  liveck
* @date    2010/04/07 21:50
*/

#pragma once
#include "ISoftManager.h"
/**
return:
	0 continue, no acttion
	1 stop
*/
typedef int (*InsCheckCallBack)(CString id, CString name, CString ver, CString curver, CString published, CString filename, void* param);



/**
return:
	0 continue, no acttion
	1 stop
*/

// key  --  注册表键名
// disp_icon -- 软件图标
// loc  -- 位置  （用于强力清扫）
// uni_cmd 卸载命令
typedef int (*UniCheckCallBack)(CString key,CString name,CString disp_icon,CString loc,CString uni_cmd,CString parent,void* param);

typedef int (*UniCheckCallBackEx)(void* mp,void* param);

//升级回调
typedef int  (__stdcall *pfnUpdateCheckCallBack)(void* mp, void* param);


/**
return:
	0 continue, no acttion
	1 stop
	2 delete to recycler
	3 really delete

*/
typedef int (*PowerSweepCallBack)(CString type,CString path,void* param);
typedef bool (*PowerSweepFilterCallBack)(const LPCTSTR lpszFolder, PVOID param);

class __declspec(uuid("C431B499-65C1-4bc3-A91C-1EA2927DB7E2"))  ISoftChecker
{
public:
	///检测机器上已安装所有的软件
	virtual int CheckAllInstalled(InsCheckCallBack func,void* para) = 0;

	///检测特定的软件是否安装
	virtual int CheckOneInstalled(CString id,InsCheckCallBack func,void* para)= 0;

	///检测卸载项  --- 获得key列表
	virtual int CheckAll2Uninstall(UniCheckCallBack func,void* para)= 0;

	///检测指定的卸载项是否存在   -1 -- 不存在  0 -- 有卸载项
	virtual int CheckOne2Uninstall(CString key,UniCheckCallBack func,void* para)= 0;

	///正常卸载  
	virtual int Uninstall(CString cmd)=0;

	///用key获取软件的id，获取id之后可以通过manager得到详尽的信息
	virtual const wchar_t* FindUninstallId(CString key)=0;
	///用display_name获取软件的id，获取id之后可以通过manager得到详尽的信息
	virtual const wchar_t* FindUninstallIdByName( CString name ) = 0;

	///安装，与卸载相比，不等待子进程
	virtual int Install(CString cmd)=0;

	///强力清扫 -1 -- 不需清扫  0 -- 有清扫项   1  -- 取消
	virtual int PowerSweep(CString key,CString loc,PowerSweepCallBack func, void* para, PowerSweepFilterCallBack fnFilter)=0;


	///用于差量更新
	virtual int Combine(CString diff_file)=0;

	///加载库，检测最后修改时间，如果不同，说明有变化，则重新加载，否则直接返回
	virtual int Load(CString lib_file="",bool reload=true)=0;

	virtual size_t GetSoftCount()=0;
};

class __declspec(uuid("917CB47F-8493-4186-B37D-5639E9E2C23B"))  ISoftCheckerEx:public ISoftChecker
{
public:
	//取得所有的卸载项信息
	/*
	k 是键
	name 是卸载项名称
	ico 是display icon
	loc 是安装路径
	pr 是parent
	py_ 拼音
	pq_ 拼音
	si 是大小 单位是字节
	lastuse 最后使用时间
	stat 安装状态 ： 已安装
	其他项同软件信息
	
	*/
	virtual int CheckAll2Uninstall(UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) = 0;

	//检测某项是否存在
	virtual int CheckOne2Uninstall(CString key ,UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) = 0;

	//搜索
	virtual void* SearchUninstallItem(CString word) = 0;
	//获取搜索结果，返回的是名称
	virtual wchar_t* GetNextItem(void* pos) = 0;
	//搜索之后用这个清理
	virtual void FinalizeGet(void* pos) = 0;
	//加载卸载信息库
	virtual int LoadUnInfo(CString lib_file) = 0;

	//释放卸载信息库
	virtual int FreeUnInfo() = 0; 
	//用于卸载信息库的合并
	virtual int Combine_UniInfo( CString diff_file ) = 0;

	//开始菜单，快速启动，桌面，进程，启动项（暂不支持），有托盘图标的软件
	enum SoftType {STARTMENU=0,QUICKLAUNCH,DESKTOP,PS_MODULE,STARTUP,NOTIFY_ICON};

	//根据不同类型取得卸载信息
	virtual int CheckAll2UninstallByType(SoftType tp,UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) = 0;

	//清理快捷方式 ： loc : loc 安装路径
	virtual int RemoveLnk(SoftType tp,CString loc,PowerSweepCallBack func,void* para) = 0;

	//清理残留项 : loc -> path 具体路径
	/*
	如果在回调中没有删除则会记录到残余中
	*/
	virtual int RemoveRemainds(CString name,CString ins_loc,CString type,CString loc,PowerSweepCallBack func,void* para) = 0;

	//计算软件大小
	virtual __int64 CountSize(CString loc) = 0;

	//检测所有的残留项……
	/*
	type 类型 注册表 文件 目录
	path 具体的路径
	loc		关联的安装目录
	name 
	*/
	virtual int CheckAll2Remains(UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) = 0;

	//分组取得残留项
	/*
	*/

	virtual int CheckAll2RemainsByGroup(UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) =0;

	virtual int CheckAll2RemainsByNameAndLoc(CString name,CString loc,UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) =0;



	//从缓存中取得信息
	virtual int CheckAll2UninstallCache(UniCheckCallBackEx func,GetInfoCallback cbfun,void* mp,void* para) = 0;
	//建立缓存
	virtual int MakeAll2UninstallCache()	=	0;

	//加载拼音库
	virtual int LoadFonts(CString strFile) = 0;

	//获取升级缓存
	virtual int GetUpdateFromCache(pfnUpdateCheckCallBack pfnCallBack, GetInfoCallback cbfun, void* mp, void* para) = 0;

	//建立升级缓存, 界面做了异步检测处理, 这里就不再做异步保存了, 但结构不好
	virtual int MakeUpdateCache() = 0;

};