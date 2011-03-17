#pragma once

namespace ksm
{

//
// 卸载事件
//
enum UninstEvent : LONG
{
	UE_Unknown = 0,
	UE_Update,					// 更新
	UE_Add,						// 添加
	UE_Delete,					// 删除
	UE_Modify,					// 修改

	UE_Data_Loaded,				// 数据已加载
	UE_Data_Failed,				// 数据加载失败
	UE_Data_Completed,			// 数据加载完成

	UE_Invoke_Uninst,			// 调用标准卸载程序
	UE_Invoke_Failed,			// 调用标准卸载程序失败
	UE_Invoke_Succeeded,		// 调用标准卸载程序成功
	UE_Invoke_Canceled,			// 用户取消执行标准卸载程序
	UE_Enum_Multi,				// 发现多个程序安装在同一目录
	UE_Enum_End,				// 正常枚举结束
	UE_Sweeping,				// 清扫中...
	UE_Sweep_Failed,			// 卸载中出现错误

	UE_Sweep_Begin,				// 清扫开始
	UE_Sweep_End,				// 清扫结束
	UE_Refresh_Begin,			// 刷新开始
	UE_Refresh_End,				// 刷新结束

	UE_Enum_Too_Much,			// 残留项太多
};

//
// 软件数据掩码
//
enum SoftDataMask : ULONG
{
	SDM_None			= (0<<0),
	SDM_Key				= (1<<0),
	SDM_Main_Path		= (1<<1),
	SDM_Description		= (1<<2),
	SDM_Info_Url		= (1<<3),
	SDM_PinYin			= (1<<4),
	SDM_Size			= (1<<5),
	SDM_LastUse			= (1<<6),
	SDM_Type			= (1<<7),
	SDM_All				= (0xFFFFFFFF),
};

//
// 软件数据
//
typedef struct SoftData
{
	ULONG			_mask;

	LPCWSTR			_pcKey;			// keyword
	LPCWSTR			_pcDisplayName;
	LPCWSTR			_pcMainPath;	// 可能为空
	LPCWSTR			_pcDescript;	// 可能为空
	LPCWSTR			_pcInfoUrl;		// 可能为空
	LPCWSTR			_pcSpellWhole;	// 可能为空
	LPCWSTR			_pcSpellAcronym;// 可能为空
	LPCWSTR			_pcIconLocation;// 可能为空
	LPCWSTR			_pcLogoUrl;		// 可能为空
	ULONGLONG		_size;			// 可能为0
	LONG			_lastUse;		// 可能为0
	LONG			_id;
	LONG			_type;
} *PSoftData;
typedef const SoftData* PCSoftData;

//
// 项目属性
//
enum SoftItemAttri : ULONG
{
	SIA_Unknown			= (0<<0),
	SIA_File			= (1<<0),
	SIA_Link			= (1<<1),
	SIA_Directory		= (1<<2),
	SIA_Registry		= (1<<3),
	SIA_Start			= (1<<4),
	SIA_Quick			= (1<<5),
	SIA_Desktop			= (1<<6),
	SIA_Process			= (1<<7),
	SIA_Tray			= (1<<8),
};

//
// 软件残留项
//
typedef struct SoftRubbish
{
	SoftItemAttri	_type;	// 开始、快速、桌面、注册表
	LPCWSTR			_pData;
} *PSoftRubbish;
typedef const SoftRubbish* PCSoftRubbish;

//
// 软件数据枚举接口
//
class ISoftDataEnum
{
public:
	virtual BOOL __stdcall EnumFirst(PSoftData pData) = 0;
	virtual BOOL __stdcall EnumNext(PSoftData pData) = 0;
};

//
// 软件链接枚举接口
//
class ISoftLinkEnum
{
public:
	virtual BOOL __stdcall EnumFirst(LPCWSTR *pKey) = 0;
	virtual BOOL __stdcall EnumNext(LPCWSTR *pKey) = 0;
};

//
// 软件残留项枚举接口
//
class ISoftRubbishEnum
{
public:
	virtual BOOL __stdcall EnumFirst(PSoftRubbish pData) = 0;
	virtual BOOL __stdcall EnumNext(PSoftRubbish pData) = 0;
};

//
// 软件卸载接口
//
class ISoftDataSweep
{
public:
	virtual void __stdcall Uninitialize() = 0;
	virtual LPCWSTR __stdcall GetKey() = 0;

	virtual ISoftRubbishEnum* __stdcall DataUninstEnum() = 0;
	virtual ISoftRubbishEnum* __stdcall DataSweepingEnum() = 0;

	virtual BOOL __stdcall DataSetSweep(PCSoftRubbish pcData) = 0;
	virtual BOOL __stdcall DataSweep(BOOL recycle) = 0;
	virtual BOOL __stdcall DataSoftExists() = 0;
	virtual BOOL __stdcall DataCancelUninst(LPCWSTR pKey) = 0;

	virtual LPCWSTR __stdcall GetMainDir() = 0;
};

class ISoftDataSweep2 : public ISoftDataSweep
{
public:
	virtual ULONGLONG __stdcall GetRubbishSize() = 0;
};

//
// 残留项清扫接口
//
class ISoftRubbishSweep
{
public:
	virtual void __stdcall Uninitialize() = 0;
	
	virtual ISoftRubbishEnum* __stdcall RubbishSweepingEnum() = 0;

	virtual BOOL __stdcall RubbishSetSweep(PCSoftRubbish pcData) = 0;
	virtual BOOL __stdcall RubbishSweep(BOOL recycle) = 0;
	virtual BOOL __stdcall RubbishCancelSweep() = 0;
};

//
// 软件卸载回调接口
//
class ISoftUnincallNotify
{
public:
	// 软件数据事件
	virtual void __stdcall SoftDataEvent(UninstEvent event, ISoftDataEnum *pEnum) = 0;
	// 软件卸载事件
	virtual void __stdcall SoftDataSweepEvent(UninstEvent event, ISoftDataSweep *pSweep) = 0;

	// 快捷方式进程事件
	virtual void __stdcall SoftLinkEvent(UninstEvent event, SoftItemAttri type, ISoftLinkEnum *pData) = 0;

	// 残留项事件
	virtual void __stdcall SoftRubbishEvent(UninstEvent event, ISoftRubbishEnum *pEnum) = 0;
	// 残留项清扫事件
	virtual void __stdcall SoftRubbishSweepEvent(UninstEvent event, ISoftRubbishSweep *pSweep) = 0;
};

//
// 软件卸载接口
//
class __declspec(uuid("F3E296D1-40AD-4ad1-B109-B8C993920D25")) ISoftUninstall
{
public:
	virtual void __stdcall SetNotify(ISoftUnincallNotify *pNotify) = 0;
	virtual BOOL __stdcall Initialize(LPCWSTR pKSafePath) = 0;
	virtual void __stdcall Uninitialize() = 0;

	//
	// 软件刷新操作
	//
	//@Remark
	// 1.强制刷新（下同）
	// 异步操作，重建缓存
	// 2.非强制刷新（下同）
	// 同步操作，直接读取缓存数据
	//
	virtual BOOL __stdcall DataRefresh(BOOL force) = 0;
	virtual ISoftDataSweep* __stdcall DataSweep(LPCWSTR pKey) = 0;

	//
	// 快捷方式操作
	//
	// 扫描桌面、快速启动栏、开始菜单与进程以及托盘
	virtual BOOL __stdcall LinkRefresh(SoftItemAttri type, BOOL force) = 0;

	//
	// 残留项操作
	//
	virtual BOOL __stdcall RubbishRefresh() = 0;
	virtual ISoftRubbishSweep* __stdcall RubbishSweep() = 0;
};

class __declspec(uuid("0A7E1A75-3BC8-4872-BADA-21224D60020D")) ISoftUninstall2 : public ISoftUninstall
{
public:
	virtual ISoftDataSweep2* __stdcall DataSweep2(LPCWSTR pKey) = 0;
};

}