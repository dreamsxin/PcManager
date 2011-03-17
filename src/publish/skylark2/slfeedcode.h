/**
* @file    slfeedcode.h
* @brief   ...
* @author  zhangrui
* @date    2009-03-02  15:21
*/

#ifndef SLFEEDCODE_H
#define SLFEEDCODE_H

//////////////////////////////////////////////////////////////////////////
#define SLK_FEEDCODE_MAX            65535   ///< code 的最大值

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_ENV_INF = 32
#define SLK_FEEDCODE_AV_DETECT          1       ///< 杀软检测
#define SLK_FEEDCODE_FW_DETECT          2       ///< 防火墙检测
#define SLK_FEEDCODE_OS_DETECT          3       ///< 操作系统检测
#define SLK_FEEDCODE_FS_DETECT          4       ///< 文件系统检测
#define SLK_FEEDCODE_SOFTWARE_DETECT    5       ///< 软件安装收集

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_PROTO = 33
#define SLK_FEEDCODE_SLPROTO_QUERY_SYS_1ST              1001    ///< 第一次查询系统信息
#define SLK_FEEDCODE_SLPROTO_QUERY_SYS_2ND              1002    ///< 第二次查询系统信息
#define SLK_FEEDCODE_SLPROTO_QUERY_SYS_3RD              1003    ///< 第三次查询系统信息
#define SLK_FEEDCODE_SLPROTO_UPLOAD_SYS                 1004    ///< 上传系统信息
#define SLK_FEEDCODE_SLPROTO_QUERY_PROCESS              1005    ///< 查询进程信息
#define SLK_FEEDCODE_SLPROTO_UPLOAD_PROCESS             1006    ///< 上传进程信息
#define SLK_FEEDCODE_SLPROTO_UPLOAD_FILE                1007    ///< 上传文件
#define SLK_FEEDCODE_SLPROTO_LOST_UPLOADED_SYS          1008    ///< 无法查询到上传过的信息
#define SLK_FEEDCODE_SLPROTO_QUERY_SYSTEM32             1009    ///< 查询system32目录下的文件时发生错误
#define SLK_FEEDCODE_SLPROTO_UPLOAD_SYSTEM32            1010    ///< 上传system32目录下的文件时发生错误

#define SLK_FEEDCODE_SLPROTO_SYS_SCAN_ACCOMPLISH        1100    ///< 完成系统扫描

#define SLK_FEEDCODE_SLPROTO_MEH_FAILED_TO_QUERY_FILE   2001    ///< meh扫描无法打开文件

#define SLK_FEEDCODE_SLPROTO_ENCRYPT_ERROR              3001    ///< 加密错误
#define SLK_FEEDCODE_SLPROTO_DECRYPT_ERROR              3002    ///< 解密错误

#define SLK_FEEDCODE_SLPROTO_FAILED_TO_UPLOAD_FILE      4001    ///< 上传文件失败
#define SLK_FEEDCODE_SLPROTO_FAILED_TO_UPLOAD_FILE_SIZE 4002    ///< 上传文件失败,附带文件大小

#define SLK_FEEDCODE_SLPROTO_STANDALONE_FAILED_TO_UPLOAD_FILE   5001    ///< 独立上传文件失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_SUCCEED_TO_UPLOAD_FILE  5002    ///< 独立上传文件成功
#define SLK_FEEDCODE_SLPROTO_STANDALONE_INTERRUPT_UPLOAD_FILE   5003    ///< 独立上传文件中断
#define SLK_FEEDCODE_SLPROTO_STANDALONE_AUTO_FAILED_TO_INIT     5004    ///< 自动上传任务表初始化失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_MANUAL_FAILED_TO_INIT   5005    ///< 手动上传任务表初始化失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_AUTO_FAILED_TO_APPEND   5006    ///< 自动上传任务表添加失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_MANUAL_FAILED_TO_APPEND 5007    ///< 手动上传任务表添加失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_AUTO_FAILED_TO_TOTAL    5008    ///< 自动上传任务表查询总数失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_MANUAL_FAILED_TO_TOTAL  5009    ///< 手动上传任务表查询总数失败

#define SLK_FEEDCODE_SLPROTO_STANDALONE_BACKUP_FAILED_TO_INIT   5010    ///< 备份文件上传任务表初始化失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_BACKUP_FAILED_TO_APPEND 5011    ///< 备份文件上传任务表添加失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_BACKUP_FAILED_TO_TOTAL  5012    ///< 备份文件上传任务表查询总数失败
#define SLK_FEEDCODE_SLPROTO_STANDALONE_BACKUP_FAILED_TO_CALL   5013    ///< 备份文件上传过程中有某个文件失败导致剩余没有上传

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_PROTO_AUTORUN = 34
#define SLK_FEEDCODE_SLPROTOATR_INIT_AUTORUN            1       ///< 初始化autorun
#define SLK_FEEDCODE_SLPROTOATR_NO_ENTRY_FOUND          2       ///< autorun没有扫描到任何文件
#define SLK_FEEDCODE_SLPROTOATR_FAILED_TO_QUERY_FILE    3       ///< autorun扫描时,查询文件失败
#define SLK_FEEDCODE_SLPROTOATR_FAILED_TO_REPORT_FILE   4       ///< autorun扫描时,报告文件信息失败
#define SLK_FEEDCODE_SLPROTOATR_FAILED_TO_UPLOAD_FILE   5       ///< autorun扫描时,上传文件失败

#define SLK_FEEDCODE_SLPROTOATR_MISSING_FILE            1001    ///< autorun扫描时,无法打开文件

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_AUTORUN = 35
#define SLK_FEEDCODE_SLAUTORUN__APPEND_MIN                      1000    ///< ...
#define SLK_FEEDCODE_SLAUTORUN__APPEND_REGVALUE_AS_STARTUP      1101    ///< Append_RegValue_As_Startup时, 解析失败的字符串
#define SLK_FEEDCODE_SLAUTORUN__APPEND_REGVALUE_AS_STARTUP_LIST 1201    ///< Append_RegValue_As_Startup_List时, 解析失败的字符串
#define SLK_FEEDCODE_SLAUTORUN__APPEND_SERVICE__IMAGE_PATH      1301    ///< Append_Service时,解析 ImagePath 失败
#define SLK_FEEDCODE_SLAUTORUN__APPEND_SERVICE__SERVICE_DLL     1302    ///< Append_Service时,解析 ServiceDll 失败
#define SLK_FEEDCODE_SLAUTORUN__APPEND_SCHEDULED_TASK_APP       1303    ///< Append_Scheduled_Task时,解析执行文件路径失败
#define SLK_FEEDCODE_SLAUTORUN__APPEND_STARTUP_FOLDER_FILE      1304    ///< Append_Scheduled_Task时,解析执行文件路径失败
#define SLK_FEEDCODE_SLAUTORUN__APPEND_MAX                      9999    ///< ...

#define SLK_FEEDCODE_SLAUTORUN__FAILED_TO_PARSE_EXISTING_FILE  10000    ///< 文件存在,但解析失败(通常是bug)
#define SLK_FEEDCODE_SLAUTORUN__MISSING_FILE_IN_COMMAND_LINE   10001    ///< 没有找到本应该在命令行中的文件,比如rundll32.exe的参数

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_CACHE_DB = 36
#define SLK_FEEDCODE_SLCACHEDB__FAILED_TO_OPEN_DATABASE         1       ///< 打开数据库失败
#define SLK_FEEDCODE_SLCACHEDB__FAILED_TO_INIT_DATABASE         2       ///< 初始化数据库失败
#define SLK_FEEDCODE_SLCACHEDB__REBUILD_DB_AFTER_FAILURE_OPEN   3       ///< 打开数据库失败后重建数据库
#define SLK_FEEDCODE_SLCACHEDB__REBUILD_DB_AFTER_FAILURE_INIT   4       ///< 初始化数据库失败后重建数据库
#define SLK_FEEDCODE_SLCACHEDB__FAILED_TO_REOPEN_AFTER_REBUILD  5       ///< 重建数据库后打开失败

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_TRJSCAN = 37
#define SLK_FEEDCODE_SLTRJSCAN__SUCCEEDED_TO_DELETE_FILE                1       ///< 直接删除文件成功
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_CREATE_RECYCLE                2       ///< 创建回收目录失败

#define SLK_FEEDCODE_SLTRKSCAN__FAILED_TO_MOVE_FILE                     3       ///< 移动文件失败

#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_REBOOT_MOVE_RECYCLE_FILE      4       ///< MoveFileEx回收文件失败
#define SLK_FEEDCODE_SLTRJSCAN__REMOVE_RECYCLE_FILE_NEED_REBOOT         5       ///< 删除回收文件后需要重启

#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_REBOOT_MOVE_ORIGINAL_FILE     7       ///< MoveFileEx原始文件失败
#define SLK_FEEDCODE_SLTRJSCAN__REMOVE_ORIGINAL_FILE_NEED_REBOOT        8       ///< 删除原始文件后需要重启



#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_DENY_EXECUTE                  11      ///< 禁用文件执行权限失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_GET_FILE_ATTRIBUTES           12      ///< 获取文件属性失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_SET_FILE_ATTRIBUTES           13      ///< 修改文件属性失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_ALLOW_DELETE                  14      ///< 启用文件删除权限失败

#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_BOOT_CLEAN_RECYCLE_FILE       21      ///< 调用Bootclean删除回收文件失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_BOOT_CLEAN_ORIGINAL_FILE      22      ///< 调用Bootclean删除原始文件失败

#define SLK_FEEDCODE_SLTRJSCAN__BOOT_CLEAN_RECYCLE_FILE_NEED_REBOOT     23      ///< 调用Bootclean删除回收文件后需要重启
#define SLK_FEEDCODE_SLTRJSCAN__BOOT_CLEAN_ORIGINAL_FILE_NEED_REBOOT    24      ///< 调用Bootclean删除原始文件后需要重启


#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_OCCUPY_FILE                   25      ///< 占坑文件失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_TERMINATE_PROCESS             26      ///< 结束进程失败

#define SLK_FEEDCODE_SLTRJSCAN__TERMINATE_PROCESS_FOR_2ND_CHANCE        27      ///< 在第二次结束进程过程中结束的进程




#define SLK_FEEDCODE_SLTRJSCAN__SUCCEEDED_TO_REPLACE_FILE                   1001    ///< 直接覆盖文件成功
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_COPY_FOR_REPLACE                  1002    ///< 拷贝用来覆盖的文件时失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_TO_REBOOT_REPLACE_FILE               1003    ///< 调用MoveFileEx
#define SLK_FEEDCODE_SLTRJSCAN__REPLACE_FILE_NEED_REBOOT                    1004    ///< MoveFileEx替换文件后需要重启
#define SLK_FEEDCODE_SLTRJSCAN__REPLACE_FILE_BUT_FAILED_TO_DELETE_RECYCLE   1005    ///< 替换文件成功,但是被替换的文件删除失败
#define SLK_FEEDCODE_SLTRJSCAN__REPLACE_FILE_NEED_REBOOT_DELETE_RECYCLE     1006    ///< 替换文件成功,需要重启删除回收的文件
#define SLK_FEEDCODE_SLTRJSCAN__REPLACE_FILE_DELETE_DEST_BUT_FAILED_TO_COPY 1007    ///< 替换文件,移走文件成功,但是拷贝文件到原位置失败
#define SLK_FEEDCODE_SLTRJSCAN__REPLACE_FILE_FAILED_TO_MOVE_FILE            1008    ///< 替换文件时,移走原有文件失败
#define SLK_FEEDCODE_SLTRJSCAN__REPLACE_FILE_FAILED_TO_CREATE_RECYCLE_DIR   1009    ///< 替换文件时,创建回收目录失败
#define SLK_FEEDCODE_SLTRJSCAN__FAILED_BOOT_REPLACE                         1010    ///< bootreplace失败


#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_SEARCH_INDEX                  2001    ///< 查找索引失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_INVALID_INDEX                           2002    ///< 无效的索引
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_LOCAL_MAKE_DOWNLOAD_PATH      2003    ///< 构造本地下载路径失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_DOWNLOAD_FILE                 2004    ///< 下载文件失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_DOWNLOAD_FILE_MISMATCH                  2005    ///< 下载的文件不匹配
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_LOAD_SFC_OS_DLL               2006    ///< 加载sfc_os.dll失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_DISABLE_SFC_OF_DLL_CACHE_FILE 2007    ///< 禁用dllcache文件的sfc失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_DISABLE_SFC_OF_SYSTEM_FILE    2008    ///< 禁用system文件的sfc失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_REPLACE_SFC_OF_DLL_CACHE_FILE 2009    ///< 替换dllcache文件失败
#define SLK_FEEDCODE_SLTRJSCAN__SFM_FAILED_TO_REPLACE_SFC_OF_SYSTEM_FILE    2010    ///< 禁用system文件失败

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_BEIKE_ANTIARP = 38
//service error
#define SLK_FEEDCODE_ANTIARP__SERVICE_DUMP_FILE_NUM                                         1      ///< 本地dump文件统计  
#define SLK_FEEDCODE_ANTIARP__SERVICE_LOAD_BEIKECMM_DLL                                     2      ///< 加载dll文件beikecmm.dll失败
#define SLK_FEEDCODE_ANTIARP__SERVICE_OPEN_DEVICE										    3      ///< 打开驱动设备对象失败
#define SLK_FEEDCODE_ANTIARP__SERVICE_CREATE_NAME_PIPE									    4      ///< 打开命名管道失败
#define SLK_FEEDCODE_ANTIARP__SERVICE_CONNECT_NAMED_PIPE 								    5      ///< 等待命名管道连接失败
#define SLK_FEEDCODE_ANTIARP__SERVICE_LOAD_BEIKENETCFG_DLL								    6      ///< 加载dll文件beikenetcfg.dll失败
#define SLK_FEEDCODE_ANTIARP__SERVICE_ARP_ATTAC                                             7      ///< 发送ARP攻击
#define SLK_FEEDCODE_ANTIARP__SERVICE_GET_GATEWAY                                           9      ///< 得网关失败
//update error
#define SLK_FEEDCODE_ANTIARP__UPDATE_CALLBACK_ON_ERROR									  51      ///< OnError回调
#define SLK_FEEDCODE_ANTIARP__UPDATE_GET_MODULE_NAME                                      52      ///< 得模块名失败
#define SLK_FEEDCODE_ANTIARP__UPDATE_CREATE_UPDATE_INTERFACE                              53      ///< 创建更新接口失败
#define SLK_FEEDCODE_ANTIARP__UPDATE_COPY_SYS_FILE                                        54      ///< 拷贝驱动失败

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_ELIVESAFE = 39
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_CALL_UPDATE                                   1       ///< 调用升级失败
#define SLK_FEEDCODE_ELIVESAFE__MISSING_ELIVEBEIKE_DLL                                  2       ///< 没有找到elivebeike.dll
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OBTAIN_CALLER_TOKEN                           3       ///< 获取调用者 token失败
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OBTAIN_ELIVE_TOKEN                            4       ///< 获取elive token失败
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OBTAIN_EXPLORER_TOKEN                         5       ///< 获取explorer token失败
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OBTAIN_ANY_DESKTOP_TOKEN                      6       ///< 没能获取任何桌面token

#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_MANAGER_FOR_INSTALL              101     ///< 打开服务管理器失败(安装)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_CREATE_SERVICE                                102     ///< 创建服务失败

#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_MANAGER_FOR_UNINSTALL            201     ///< 打开服务管理器失败(卸载)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_FOR_UNINSTALL                    202     ///< 打开服务失败(卸载)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_STOP_SERVICE_FOR_UNINSTALL                    203     ///< 停止服务失败(卸载)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_DELETE_SERVICE                                204     ///< 删除服务失败(卸载)

#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_MANAGER_FOR_START                301     ///< 打开服务管理器失败(启动)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_FOR_START                        302     ///< 打开服务失败(启动)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_QUERY_SERVICE_STATUS_FOR_START                303     ///< 查询服务状态失败(启动)
#define SLK_FEEDCODE_ELIVESAFE__TIMEOUT_WAITING_SERVICE_TO_STOP_FOR_START               304     ///< 等待服务停止超时(启动)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_START_SERVICE_FOR_START                       305     ///< 启动服务失败(启动)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_QUERY_SERVICE_STATUS_AFTER_START_FOR_START    306     ///< 再次查询服务状态失败(启动)
#define SLK_FEEDCODE_ELIVESAFE__SERVICE_NOT_STARTED_FOR_START                           307     ///< 服务启动失败(启动)

#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_MANAGER_FOR_STOP                 401     ///< 打开服务管理器失败(停止)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_OPEN_SERVICE_FOR_STOP                         402     ///< 打开服务失败(停止)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_QUERY_SERVICE_STATUS_FOR_STOP                 403     ///< 查询服务状态失败(停止)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_STOP_SERVICE_FOR_STOP                         404     ///< 停止服务失败(停止)
#define SLK_FEEDCODE_ELIVESAFE__FAILED_TO_STOP_SERVICE_AFTER_STOP_FOR_STOP              405     ///< 再次查询服务状态失败(停止)

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_UPDATE = 17

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_VKILLER = 64
#define SLK_FEEDCODE_VKILLER_SCAN_REPORT                            1   ///< 扫描结果上报
#define SLK_FEEDCODE_VKILLER_ECHO_RESULT                            2   ///< 握手结果上报
#define SLK_FEEDCODE_VKILLER_POPUP_SHOW                             3   ///< 泡泡弹出上报
#define SLK_FEEDCODE_VKILLER_APP_PATH                               4   ///< 程序所在目录上报

#define SLK_FEEDCODE_VKILLER_WRONG_VIRUS_STATUS                     5   ///< 统计不显示启发式的清除方式的错误

//////////////////////////////////////////////////////////////////////////
// SLK_SHEMA_OLSCAN = 65
#define SLK_FEEDCODE_OLSCAN_INSTALL_ERROR                           1   ///< 安装失败

//////////////////////////////////////////////////////////////////////////
// SLK_SCHEMA_BACKUP = 40
#define SLK_FEEDCODE_BACKUP_FAILED_TO_DELETE_NONNIL_DIRECTORY       1   ///< 删除非空目录失败
#define SLK_FEEDCODE_BACKUP_FAILED_TO_ALL_DELETE                    2   ///< 所有删除方法失败
#define SLK_FEEDCODE_BACKUP_FAILED_TO_NORMAL_RETRIEVE               3   ///< 一般的恢复备份文件失败

#endif//SLFEEDCODE_H
