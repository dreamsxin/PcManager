#pragma once

namespace ksm
{

//
// 软件信息掩码
//
enum SoftInfoMask : ULONG
{
	SIM_None			= (0<<0),
	SIM_Grade			= (1<<0),
};

//
// 软件信息
//
typedef struct SoftInfo
{
	SoftInfoMask	_mask;

	LONG			_id;
	LPCWSTR			_pcGrade;
} *PSoftInfo;
typedef const SoftInfo* PCSoftInfo;

//
// 软件信息枚举接口
//
class ISoftInfoEnum
{
public:
	virtual BOOL __stdcall EnumFirst(PSoftInfo pSoftInfo) = 0;
	virtual BOOL __stdcall EnumNext(PSoftInfo pSoftInfo) = 0;
};

//
// 软件信息查询回调接口
//
class ISoftInfoQueryNotify
{
public:
	virtual void __stdcall SoftQueryNotify(ISoftInfoEnum *pEnum, void *pParams) = 0;
};

//
// 软件信息查询接口
//
class __declspec(uuid("C590FCA3-87D3-47a1-9BEC-CCBA920353C9")) ISoftInfoQuery
{
public:
	virtual void __stdcall SetNotify(ISoftInfoQueryNotify *pNotify) = 0;
	virtual BOOL __stdcall Initialize(LPCWSTR pKSafePath) = 0;
	virtual void __stdcall Uninitialize() = 0;

	// 增加查询软件
	virtual void __stdcall ClsSoft() = 0;
	virtual void __stdcall AddSoft(LONG id, SoftInfoMask mask = SIM_Grade) = 0;

	// 查询分数
	virtual BOOL __stdcall Query(void *pParams = NULL) = 0;
};

}