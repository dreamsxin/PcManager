//////////////////////////////////////////////////////////////////////
///		@file		kxeservicemgrevent_def.h
///		@author		luopeng
///		@date		2009-3-3 09:07:58
///
///		@brief		基础子系统的事件定义
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../include/idl.h"
#include "../include/kxe_serialize.h"

/**
* @defgroup kxeservice_manager_event_define_group KXEngine Base Event Define
* @{
*/

/**
* @brief 服务提供者启动成功后的事件数据结构定义
*/
typedef struct _KXE_BASE_SERVICE_PROVIDER_STARTUP
{
	KSCLSID clsidSP;
} KXE_BASE_SERVICE_PROVIDER_STARTUP;

KXE_JSON_DDX_BEGIN(KXE_BASE_SERVICE_PROVIDER_STARTUP)
	KXE_JSON_DDX_MEMBER("ServiceProvider", clsidSP);
KXE_JSON_DDX_END()

/**
 * @brief 服务提供者启动成功后的事件ID定义,绑定了事件定义KXE_BASE_SERVICE_PROVIDER_STARTUP
 * @see KXE_FILE_MON_STATUS
 */
#define KXE_SERVICE_PROVIDER_STARTUP_EVENT 1


KXE_DECLARE_EVENT_ID_TYPE(
	enum_SubSystem_Base, 
	KXE_SERVICE_PROVIDER_STARTUP_EVENT, 
	KXE_BASE_SERVICE_PROVIDER_STARTUP
	);

/**
 * @}
 */


