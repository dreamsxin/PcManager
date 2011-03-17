//////////////////////////////////////////////////////////////////////
///		@file		kxesystemservice_def.h
///		@author		luopeng
///		@date		2008-10-06 09:07:58
///
///		@brief		系统服务的数据结构定义
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../include/kxe_serialize.h"

/**
 * @defgroup kxebase_system_service_provider_data_struct KXEngine Architecture System Service Provider Data Struct
 * @{
 */

/**
* @brief    驱动器类型
*/
enum EM_KXE_LOCALDRIVER_TYPE
{
    em_Driver_UNKNOWN          = 0,   ///< The drive type cannot be determined.
    em_Driver_NO_ROOT_DIR,            ///< The root path is invalid; for example, there is no volume is mounted at the path.
    em_Driver_DRIVE_REMOVABLE,        ///< The drive has removable media; for example, a floppy drive or flash card reader.
    em_Driver_FIXED,                  ///< The drive has fixed media; for example, a hard drive, flash drive, or thumb drive.
    em_Driver_DRIVE_REMOTE,           ///< The drive is a remote (network) drive.
    em_Driver_DRIVE_CDROM,            ///< The drive is a CD-ROM drive.
    em_Driver_RAMDISK                 ///< The drive is a RAM disk.
};
KXE_JSON_DDX_ENUM(EM_KXE_LOCALDRIVER_TYPE);


/**
* @brief    本地驱动器名称及类型
*/
typedef struct _KXE_LOGIC_DRIVER_INFO
{
    std::wstring            strDriverName;  ///< 驱动器的名称
    EM_KXE_LOCALDRIVER_TYPE emDriverType;   ///< 驱动器类型
}KXE_LOGIC_DRIVER_INFO;

KXE_JSON_DDX_BEGIN(KXE_LOGIC_DRIVER_INFO)
    KXE_JSON_DDX_MEMBER("DriverName", strDriverName)
    KXE_JSON_DDX_MEMBER("DriverType", emDriverType)
KXE_JSON_DDX_END()
/**
 * @}
 */