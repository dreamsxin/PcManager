#ifndef _KwsCmptComm_h_
#define _KwsCmptComm_h_

#include <atlstr.h>

typedef enum _E_KWS_KWSTYPE
{
	enum_KKT_Unknown,
	enum_KKT_STD,        /// 独立版
	enum_KKT_Ksafe,      /// 卫士集成版
	enum_KKT_Duba		 /// 毒霸集成版
} E_KWS_KWSTYPE;


__declspec(noinline) E_KWS_KWSTYPE GetKTBySvcProcName();   //在sp中根据服务进程名判断网盾版本

__declspec(noinline) E_KWS_KWSTYPE GetKTByIpc();  // 通过ipc通信查询网盾版本

BOOL IsKwsStdInstalled();  // 是否安装了独立报网盾

BOOL IsKwsKsafeInstalled(); // 是否安装了卫士版网盾

BOOL IsKwsDubaInstalled(); // 是否安装了毒霸版网盾

BOOL IsKwsStdExist(int* pnType = NULL);  // 网盾是否已经运行

int  GetRunningKwsType(E_KWS_KWSTYPE* peType);  // 获取运行中的网盾版本

BOOL GetCurrentStartupKWSProductName(ATL::CString& strName);

#endif