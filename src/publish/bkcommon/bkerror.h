/**
* @file    bkerror.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-11 15:28
*/

#ifndef BKERROR_H
#define BKERROR_H

#include <winerror.h>


//////////////////////////////////////////////////////////////////////////
// 判断是否是windows错误码
//////////////////////////////////////////////////////////////////////////

/// 判断是否是windows错误
#define IS_WIN32_ERROR(x)                           ((0xFFFF0000 & x) == 0x80070000)



//////////////////////////////////////////////////////////////////////////
// skylark
//////////////////////////////////////////////////////////////////////////
#define MAKE_SKYLARK_SUCCESS(x)                     HRESULT(0x60010000 | (0xFFFF & x))
#define MAKE_SKYLARK_ERROR(x)                       HRESULT(0xA0010000 | (0xFFFF & x))
#define IS_SKYLARK_MOD_HRESULT(x)                   ((0x3FFF0000 & x) == 0x20010000)


//////////////////////////////////////////////////////////////////////////
// http
//////////////////////////////////////////////////////////////////////////
#define MAKE_SKYLARK_HTTP_SUCCESS(x)                HRESULT(0x60020000 | (0xFFFF & x))
#define MAKE_SKYLARK_HTTP_ERROR(x)                  HRESULT(0xA0020000 | (0xFFFF & x))
#define IS_SKYLARK_HTTP_HRESULT(x)                  ((0x3FFF0000 & x) == 0x20020000)


//////////////////////////////////////////////////////////////////////////
// sqlite
//////////////////////////////////////////////////////////////////////////
#define MAKE_SKYLARK_SQLITE_SUCCESS(x)              HRESULT(0x60030000 | (0xFFFF & x))
#define MAKE_SKYLARK_SQLITE_ERROR(x)                HRESULT(0xA0030000 | (0xFFFF & x))
#define IS_SKYLARK_SQLITE_HRESULT(x)                ((0x3FFF0000 & x) == 0x20030000)


//////////////////////////////////////////////////////////////////////////
// skylark server
//////////////////////////////////////////////////////////////////////////
#define MAKE_SKYLARK_SERVER_SUCCESS(x)              HRESULT(0x60040000 | (0xFFFF & x))
#define MAKE_SKYLARK_SERVER_ERROR(x)                HRESULT(0xA0040000 | (0xFFFF & x))
#define IS_SKYLARK_SERVER_HRESULT(x)                ((0x3FFF0000 & x) == 0x20040000)

//////////////////////////////////////////////////////////////////////////
// 引擎
//////////////////////////////////////////////////////////////////////////
#define MAKE_BKENG_SUCCESS(x)                       HRESULT(0x60050000 | (0xFFFF & x))
#define MAKE_BKENG_ERROR(x)                         HRESULT(0xA0050000 | (0xFFFF & x))
#define IS_BKENG_HRESULT(x)                         ((0x3FFF0000 & x) == 0x20050000)





//////////////////////////////////////////////////////////////////////////
// skylark
//////////////////////////////////////////////////////////////////////////
#define ERROR_SKYLARK_CACHED_HASH                           0x0001  ///< 缓存的hash
#define ERROR_SKYLARK_CACHED_LEVEL                          0x0002  ///< 缓存的level
#define ERROR_SKYLARK_CACHED_WINTRUST                       0x0003  ///< 缓存的WinTrust结果
#define ERROR_SKYLARK_BWLIB_LEVEL                           0x0004  ///< 从黑白库获取的level

#define ERROR_SKYLARK_RSP_NO_FILE_LEVEL                     0x0005  ///< 回复中没有可用的文件level
#define ERROR_SKYLARK_RSP_NO_PROCESS_LEVEL                  0x0006  ///< 回复中没有可用的进程level

#define ERROR_SKYLARK_QUERY_FILE_RSP_ERROR                  0x0010  ///< 解析文件hash查询的结果时发生错误
#define ERROR_SKYLARK_UPLOAD_FILE_RSP_ERROR                 0x0011  ///< 解析文件    上传的结果时发生错误
#define ERROR_SKYLARK_QUERY_PROCESS_RSP_ERROR               0x0012  ///< 解析进程hash查询的结果时发生错误
#define ERROR_SKYLARK_UPLOAD_PROCESS_RSP_ERROR              0x0013  ///< 解析进程hash上传的结果时发生错误
#define ERROR_SKYLARK_QUERY_SYSTEM_RSP_ERROR                0x0014  ///< 解析系统hash查询的结果时发生错误
#define ERROR_SKYLARK_UPLOAD_SYSTEM_RSP_ERROR               0x0015  ///< 解析系统hash上传的结果时发生错误
#define ERROR_SKYLARK_CMD_ENCRYPT_ERROR                     0x0030  ///< 协议命令加密错误
#define ERROR_SKYLARK_RSP_DECRYPT_ERROR                     0x0031  ///< 协议回复解密错误
#define ERROR_SKYLARK_RSP_CORRUPT_ERROR                     0x0032  ///< 协议回复的hash值不匹配

#define ERROR_SKYLARK_WIN_TRUST_FILE                        0x0040  ///< 文件包含数字签名
#define ERROR_SKYLARK_NON_PE_FILE                           0x0041  ///< 非PE文件
#define ERROR_SKYLARK_IS_SFC_PROTECTED                      0x0042  ///< 受sfc文件保护
#define ERROR_SKYLARK_TRUSTED_INSTALLER                     0x0043  ///< 受信任的owner
#define ERROR_SKYLARK_NON_EXE_FILE                          0x0044  ///< 非可执行文件

#define ERROR_SKYLARK_NO_FILES_BE_HASHED                    0x0050  ///< 没有文件被hash
#define ERROR_SKYLARK_NO_PROCESSES_BE_HASHED                0x0051  ///< 没有进程被hash

#define ERROR_SKYLARK_TOO_LARGE_FILE                        0x0060  ///< 文件太大
#define ERROR_SKYLARK_TOO_SMALL_FILE                        0x0061  ///< 文件太小
#define ERROR_SKYLARK_EMPTY_FILE                            0x0062  ///< 空文件 

#define ERROR_SKYLARK_PENDING_SCAN                          0x0071  ///< 延迟查询
#define ERROR_SKYLARK_NO_NEED_TO_UPLOAD                     0x0072  ///< 文件不需要上传

#define ERROR_SKYLARK_B3A_SCAN                              0x0080  ///< 应用B3A扫描
#define ERROR_SKYLARK_B3A_NO_NEED_TO_FIX                    0x0081  ///< 不需要使用B3A进行修复
#define ERROR_SKYLARK_B3A_FAILED_TO_FIX                     0x0082  ///< B3A修复失败

#define ERROR_SKYLARK_LEVEL_CAN_NOT_CLEAN                   0x0090  ///< level不需要clean

#define ERROR_SKYLARK_REQUIRED_SKIP                         0x1001  ///< 要求跳过目标



#define ERROR_SKYLARK_OVERWRITE_INFECTED_SYS_FILE           0x2001  ///< 覆盖被感染的系统文件
#define ERROR_SKYLARK_REMOVE_RECYCLE_FILE_NEED_REBOOT       0x2002  ///< 删除更名后的文件需要重启
#define ERROR_SKYLARK_REMOVE_ORIGINAL_FILE_NEED_REBOOT      0x2003  ///< 删除原始文件后需要重启
#define ERROR_SKYLARK_CAN_NOT_REMOVE_DIRECTORY              0x2004  ///< 无法删除目录
#define ERROR_SKYLARK_BOOTCLEAN_RECYCLE_FILE_NEED_REBOOT    0x2005  ///< bootclean更名后的文件需要重启
#define ERROR_SKYLARK_BOOTCLEAN_ORIGINAL_FILE_NEED_REBOOT   0x2006  ///< bootclean原始文件后需要重启


#define ERROR_SKYLARK_CAN_NOT_REPLACE_DIRECTORY                     0x2101  ///< 传入的参数是目录,无法替换
#define ERROR_SKYLARK_REPLACE_FILE_NEED_REBOOT                      0x2102  ///< 替换文件后需要重启
#define ERROR_SKYLARK_COPY_FOR_REPLACE                              0x2103  ///< 拷贝用来替换的文件
#define ERROR_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE                 0x2104  ///< 调用MoveFileEx替换文件失败
#define ERROR_SKYLARK_REPLACE_FILE_BUT_FAILED_TO_DELETE_RECYCLE     0x2105  ///< 替换文件成功,但是删除回收文件失败
#define ERROR_SKYLARK_REPLACE_FILE_NEED_REBOOT_DELETE_RECYCLE       0x2106  ///< 替换文件成功,需要重启删除回收文件

#define ERROR_SKYLARK_BACKUP_FAILED_TO_REPLACE_NONNULL_DIRECTORY    0x21B1  ///< 无法替换非空目录
#define ERROR_SKYLARK_BACKUP_FAILED_TO_ALL_DELETE                   0x21B2  ///< 所有删除方法都失败
#define ERROR_SKYLARK_BACKUP_FAILED_TO_FIND_SPECI_FILE              0x21B3  ///< 找不到指定文件
#define ERROR_SKYLARK_BACKUP_NO_MORE_DISK_SPACE                     0x21B4  ///< 磁盘空间不足

#define ERROR_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_DIRECTORY        0x21D1  ///< 无法还原文件(指定的恢复位置是一个目录)
#define ERROR_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_FILE             0x21D2  ///< 无法还原文件(指定的恢复位置存在一个文件)
#define ERROR_SKYLARK_RETRIEVE_MISSING_BACKUP_FILE                  0x21D3  ///< 备份数据不存在
#define ERROR_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_FILE               0x21D4  ///< 解压文件的时候发生错误
#define ERROR_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_BACKUP_INFO        0x21D5  ///< 获取备份文件信息的时候发生错误
#define ERROR_SKYLARK_RETRIEVE_FAILED_TO_REPLACE_FILE               0x21D6  ///< 替换文件的时候发生错误


#define ERROR_SKYLARK_SFM_LOCAL_DOWNLOAD_NOT_MATCH          0x2201  ///< SFM(系统文件修复模块) 本地下载的文件与索引中的不匹配
#define ERROR_SKYLARK_SFM_MISSING_URL_DATA                  0x2202  ///< SFM 没有找到url配置
#define ERROR_SKYLARK_SFM_MISSING_INDEX_DATA                0x2203  ///< SFM 没有找到index配置
#define ERROR_SKYLARK_SFM_MISSING_MODULE_IN_INDEX           0x2204  ///< SFM 没有在index找到模块信息
#define ERROR_SKYLARK_SFM_MISSING_ENTRY                     0x2205  ///< SFM 没有在硬代码中找到系统文件entry
#define ERROR_SKYLARK_SFM_INVALID_URL_DATA                  0x2206  ///< SFM 找到的url配置无效
#define ERROR_SKYLARK_SFM_INVALID_INDEX_DATA                0x2207  ///< SFM 找到的index配置无效
#define ERROR_SKYLARK_SFM_DOWNLOAD_FILE_MISMATCH            0x2208  ///< SFM 下载的文件不匹配
#define ERROR_SKYLARK_SFM_NEED_REBOOT                       0x2209  ///< SFM 修复文件后需要重启
#define ERROR_SKYLARK_SFM_FILE_NOT_IN_THIS_OS               0x220A  ///< SFC 指定的系统文件不属于操作系统

#define ERROR_SKYLARK_SCSAPI_FAILED_TO_CALL_COPY_FILE       0x2301  ///< scs_copy_file   返回失败
#define ERROR_SKYLARK_SCSAPI_FAILED_TO_CALL_DELETE_FILE     0x2302  ///< scs_delete_file 返回失败
#define ERROR_SKYLARK_SCSAPI_FAILED_TO_CALL_FILE_EXIST      0x2303  ///< scs_file_exists 返回失败


#define ERROR_SKYLARK_ONLY_SUPPORTED_ON_X86                 0x3001  ///< 仅支持x86
#define ERROR_SKYLARK_ONLY_SUPPORTED_ON_WINXP_OR_LATER      0x3002  ///< 仅支持xp及以后系统




//////////////////////////////////////////////////////////////////////////
// Information
#define S_SKYLARK_CACHED_HASHED                         MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_CACHED_HASH)             ///< ...
#define S_SKYLARK_CACHED_LEVEL                          MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_CACHED_LEVEL)            ///< ...
#define S_SKYLARK_CACHED_WINTRUST                       MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_CACHED_WINTRUST)         ///< ...
#define S_SKYLARK_BWLIB_LEVEL                           MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_BWLIB_LEVEL)             ///< ...

#define S_SKYLARK_WIN_TRUST_FILE                        MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_WIN_TRUST_FILE)          ///< ...
#define S_SKYLARK_NON_PE_FILE                           MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_NON_PE_FILE)             ///< ...
#define S_SKYLARK_IS_SFC_PROTECTED                      MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_IS_SFC_PROTECTED)        ///< ...
#define S_SKYLARK_TRUSTED_INSTALLER                     MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_TRUSTED_INSTALLER)       ///< ...
#define S_SKYLARK_NON_EXE_FILE                          MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_NON_EXE_FILE)            ///< ...

#define S_SKYLARK_TOO_LARGE_FILE                        MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_TOO_LARGE_FILE)          ///< ...
#define S_SKYLARK_TOO_SMALL_FILE                        MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_TOO_SMALL_FILE)          ///< ...
#define S_SKYLARK_EMPTY_FILE                            MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_EMPTY_FILE)              ///< ...

#define S_SKYLARK_PENDING_SCAN                          MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_PENDING_SCAN)            ///< ...
#define S_SKYLARK_NO_NEED_TO_UPLOAD                     MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_NO_NEED_TO_UPLOAD)       ///< ...

#define S_SKYLARK_B3A_SCAN                              MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_B3A_SCAN)                ///< ...

#define S_SKYLARK_REQUIRED_SKIP                         MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_REQUIRED_SKIP)             ///< ...

#define S_SKYLARK_OVERWRITE_INFECTED_SYS_FILE           MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_OVERWRITE_INFECTED_SYS_FILE) ///< ...

#define S_SKYLARK_REMOVE_RECYCLE_FILE_NEED_REBOOT       MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_REMOVE_RECYCLE_FILE_NEED_REBOOT)     ///< ...
#define S_SKYLARK_REMOVE_ORIGINAL_FILE_NEED_REBOOT      MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_REMOVE_ORIGINAL_FILE_NEED_REBOOT)    ///< ...
#define S_SKYLARK_BOOTCLEAN_RECYCLE_FILE_NEED_REBOOT    MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_BOOTCLEAN_RECYCLE_FILE_NEED_REBOOT)              ///< ...
#define S_SKYLARK_BOOTCLEAN_ORIGINAL_FILE_NEED_REBOOT   MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_BOOTCLEAN_ORIGINAL_FILE_NEED_REBOOT)             ///< ...

#define S_SKYLARK_CAN_NOT_REPLACE_DIRECTORY                     MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_CAN_NOT_REPLACE_DIRECTORY)                   ///< ...
#define S_SKYLARK_REPLACE_FILE_NEED_REBOOT                      MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_REPLACE_FILE_NEED_REBOOT)                    ///< ...
#define S_SKYLARK_COPY_FOR_REPLACE                              MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_COPY_FOR_REPLACE)                            ///< ...
//#define S_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE                 MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE)               ///< ...
#define S_SKYLARK_REPLACE_FILE_BUT_FAILED_TO_DELETE_RECYCLE     MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_REPLACE_FILE_BUT_FAILED_TO_DELETE_RECYCLE)   ///< ...
#define S_SKYLARK_REPLACE_FILE_NEED_REBOOT_DELETE_RECYCLE       MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_REPLACE_FILE_NEED_REBOOT_DELETE_RECYCLE)     ///< ...


#define S_SKYLARK_SFM_NEED_REBOOT                               MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_SFM_NEED_REBOOT)                             ///< ...


#define S_SKYLARK_ONLY_SUPPORTED_ON_X86                 MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_ONLY_SUPPORTED_ON_X86)               ///< ...
#define S_SKYLARK_ONLY_SUPPORTED_ON_WINXP_OR_LATER      MAKE_SKYLARK_SUCCESS(ERROR_SKYLARK_ONLY_SUPPORTED_ON_WINXP_OR_LATER)    ///< ...








//////////////////////////////////////////////////////////////////////////
// Error
#define E_SKYLARK_RSP_NO_FILE_LEVEL                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RSP_NO_FILE_LEVEL)          ///< ...
#define E_SKYLARK_RSP_NO_PROCESS_LEVEL                  MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RSP_NO_PROCESS_LEVEL)       ///< ...
#define E_SKYLARK_NO_FILES_BE_HASHED                    MAKE_SKYLARK_ERROR(ERROR_SKYLARK_NO_FILES_BE_HASHED)        ///< ...
#define E_SKYLARK_NO_PROCESSES_BE_HASHED                MAKE_SKYLARK_ERROR(ERROR_SKYLARK_NO_PROCESSES_BE_HASHED)    ///< ...


/// 协议的回复解析错误
#define E_SKYLARK_QUERY_FILE_RSP_ERROR                  MAKE_SKYLARK_ERROR(ERROR_SKYLARK_QUERY_FILE_RSP_ERROR)      ///< ...
#define E_SKYLARK_UPLOAD_FILE_RSP_ERROR                 MAKE_SKYLARK_ERROR(ERROR_SKYLARK_UPLOAD_FILE_RSP_ERROR)     ///< ...
#define E_SKYLARK_QUERY_PROCESS_RSP_ERROR               MAKE_SKYLARK_ERROR(ERROR_SKYLARK_QUERY_PROCESS_RSP_ERROR)   ///< ...
#define E_SKYLARK_UPLOAD_PROCESS_RSP_ERROR              MAKE_SKYLARK_ERROR(ERROR_SKYLARK_UPLOAD_PROCESS_RSP_ERROR)  ///< ...
#define E_SKYLARK_QUERY_SYSTEM_RSP_ERROR                MAKE_SKYLARK_ERROR(ERROR_SKYLARK_QUERY_SYSTEM_RSP_ERROR)    ///< ...
#define E_SKYLARK_UPLOAD_SYSTEM_RSP_ERROR               MAKE_SKYLARK_ERROR(ERROR_SKYLARK_UPLOAD_SYSTEM_RSP_ERROR)   ///< ...

/// 协议的加解密错误
#define E_SKYLARK_CMD_ENCRYPT_ERROR                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_CMD_ENCRYPT_ERROR)         ///< ...
#define E_SKYLARK_RSP_DECRYPT_ERROR                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RSP_DECRYPT_ERROR)         ///< ...
#define E_SKYLARK_RSP_CORRUPT_ERROR                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RSP_CORRUPT_ERROR)         ///< ...

#define E_SKYLARK_CACHED_WINTRUST                       MAKE_SKYLARK_ERROR(ERROR_SKYLARK_CACHED_WINTRUST)           ///< ...

#define E_SKYLARK_WIN_TRUST_FILE                        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_WIN_TRUST_FILE)            ///< ...
#define E_SKYLARK_NON_PE_FILE                           MAKE_SKYLARK_ERROR(ERROR_SKYLARK_NON_PE_FILE)               ///< ...
#define E_SKYLARK_IS_SFC_PROTECTED                      MAKE_SKYLARK_ERROR(ERROR_SKYLARK_IS_SFC_PROTECTED)          ///< ...
#define E_SKYLARK_TRUSTED_INSTALLER                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_TRUSTED_INSTALLER)         ///< ...
#define E_SKYLARK_NON_EXE_FILE                          MAKE_SKYLARK_ERROR(ERROR_SKYLARK_NON_EXE_FILE)              ///< ...

#define E_SKYLARK_PENDING_SCAN                          MAKE_SKYLARK_ERROR(ERROR_SKYLARK_PENDING_SCAN)              ///< ...


#define E_SKYLARK_TOO_LARGE_FILE                        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_TOO_LARGE_FILE)            ///< ...
#define E_SKYLARK_TOO_SMALL_FILE                        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_TOO_SMALL_FILE)            ///< ...
#define E_SKYLARK_EMPTY_FILE                            MAKE_SKYLARK_ERROR(ERROR_SKYLARK_EMPTY_FILE)                ///< ...

#define E_SKYLARK_B3A_NO_NEED_TO_FIX                    MAKE_SKYLARK_ERROR(ERROR_SKYLARK_B3A_NO_NEED_TO_FIX)        ///< ...
#define E_SKYLARK_B3A_FAILED_TO_FIX                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_B3A_FAILED_TO_FIX)         ///< ...

#define E_SKYLARK_LEVEL_CAN_NOT_CLEAN                   MAKE_SKYLARK_ERROR(ERROR_SKYLARK_LEVEL_CAN_NOT_CLEAN)       ///< ...

#define E_SKYLARK_CAN_NOT_REMOVE_DIRECTORY              MAKE_SKYLARK_ERROR(ERROR_SKYLARK_CAN_NOT_REMOVE_DIRECTORY)              ///< ...
#define E_SKYLARK_REMOVE_RECYCLE_FILE_NEED_REBOOT       MAKE_SKYLARK_ERROR(ERROR_SKYLARK_REMOVE_RECYCLE_FILE_NEED_REBOOT)       ///< ...
#define E_SKYLARK_REMOVE_ORIGINAL_FILE_NEED_REBOOT      MAKE_SKYLARK_ERROR(ERROR_SKYLARK_REMOVE_ORIGINAL_FILE_NEED_REBOOT)      ///< ...
#define E_SKYLARK_BOOTCLEAN_RECYCLE_FILE_NEED_REBOOT    MAKE_SKYLARK_ERROR(ERROR_SKYLARK_BOOTCLEAN_RECYCLE_FILE_NEED_REBOOT)    ///< ...
#define E_SKYLARK_BOOTCLEAN_ORIGINAL_FILE_NEED_REBOOT   MAKE_SKYLARK_ERROR(ERROR_SKYLARK_BOOTCLEAN_ORIGINAL_FILE_NEED_REBOOT)   ///< ...

#define E_SKYLARK_CAN_NOT_REPLACE_DIRECTORY                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_CAN_NOT_REPLACE_DIRECTORY)                     ///< ...
#define E_SKYLARK_REPLACE_FILE_NEED_REBOOT                      MAKE_SKYLARK_ERROR(ERROR_SKYLARK_REPLACE_FILE_NEED_REBOOT)                      ///< ...
#define E_SKYLARK_COPY_FOR_REPLACE                              MAKE_SKYLARK_ERROR(ERROR_SKYLARK_COPY_FOR_REPLACE)                              ///< ...
#define E_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE                 MAKE_SKYLARK_ERROR(ERROR_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE)                 ///< ...
#define E_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE                 MAKE_SKYLARK_ERROR(ERROR_SKYLARK_FAILED_TO_REBOOT_REPLACE_FILE)                 ///< ...
#define E_SKYLARK_REPLACE_FILE_BUT_FAILED_TO_DELETE_RECYCLE     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_REPLACE_FILE_BUT_FAILED_TO_DELETE_RECYCLE)     ///< ...
#define E_SKYLARK_REPLACE_FILE_NEED_REBOOT_DELETE_RECYCLE       MAKE_SKYLARK_ERROR(ERROR_SKYLARK_REPLACE_FILE_NEED_REBOOT_DELETE_RECYCLE)       ///< ...




#define E_SKYLARK_SFM_LOCAL_DOWNLOAD_NOT_MATCH                  MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_LOCAL_DOWNLOAD_NOT_MATCH)          ///< ...
#define E_SKYLARK_SFM_MISSING_URL_DATA                          MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_MISSING_URL_DATA)                  ///< ...
#define E_SKYLARK_SFM_MISSING_INDEX_DATA                        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_MISSING_INDEX_DATA)                ///< ...
#define E_SKYLARK_SFM_MISSING_MODULE_IN_INDEX                   MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_MISSING_MODULE_IN_INDEX)           ///< ...
#define E_SKYLARK_SFM_MISSING_ENTRY                             MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_MISSING_ENTRY)                     ///< ...
#define E_SKYLARK_SFM_INVALID_URL_DATA                          MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_INVALID_URL_DATA)                  ///< ...
#define E_SKYLARK_SFM_INVALID_INDEX_DATA                        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_INVALID_INDEX_DATA)                ///< ...
#define E_SKYLARK_SFM_DOWNLOAD_FILE_MISMATCH                    MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_DOWNLOAD_FILE_MISMATCH)            ///< ...
#define E_SKYLARK_SFM_FILE_NOT_IN_THIS_OS                       MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SFM_FILE_NOT_IN_THIS_OS)               ///< ...

#define E_SKYLARK_SCSAPI_FAILED_TO_CALL_COPY_FILE               MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SCSAPI_FAILED_TO_CALL_COPY_FILE)       ///< ...
#define E_SKYLARK_SCSAPI_FAILED_TO_CALL_DELETE_FILE             MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SCSAPI_FAILED_TO_CALL_DELETE_FILE)     ///< ...
#define E_SKYLARK_SCSAPI_FAILED_TO_CALL_FILE_EXIST              MAKE_SKYLARK_ERROR(ERROR_SKYLARK_SCSAPI_FAILED_TO_CALL_FILE_EXIST)      ///< ...


#define E_SKYLARK_ONLY_SUPPORTED_ON_X86                         MAKE_SKYLARK_ERROR(ERROR_SKYLARK_ONLY_SUPPORTED_ON_X86)                 ///< ...
#define E_SKYLARK_ONLY_SUPPORTED_ON_WINXP_OR_LATER              MAKE_SKYLARK_ERROR(ERROR_SKYLARK_ONLY_SUPPORTED_ON_WINXP_OR_LATER)      ///< ...


#define E_SKYLARK_BACKUP_FAILED_TO_DELETE_NONNULL_DIRECTORY     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_BACKUP_FAILED_TO_REPLACE_NONNULL_DIRECTORY)    ///< ...
#define E_SKYLARK_BACKUP_FAILED_TO_ALL_DELETE                   MAKE_SKYLARK_ERROR(ERROR_SKYLARK_BACKUP_FAILED_TO_ALL_DELETE)                   ///< ...
#define E_SKYLARK_BACKUP_FAILED_TO_FIND_SPECI_FILE              MAKE_SKYLARK_ERROR(ERROR_SKYLARK_BACKUP_FAILED_TO_FIND_SPECI_FILE)              ///< ...
#define E_SKYLARK_BACKUP_NO_MORE_DISK_SPACE                     MAKE_SKYLARK_ERROR(ERROR_SKYLARK_BACKUP_NO_MORE_DISK_SPACE)                     ///< ...

#define E_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_DIRECTORY        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_DIRECTORY)        ///< ...
#define E_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_FILE             MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_FILE)             ///< ...
#define E_SKYLARK_RETRIEVE_MISSING_BACKUP_FILE                  MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RETRIEVE_MISSING_BACKUP_FILE)                  ///< ...
#define E_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_FILE               MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_FILE)               ///< ...
#define E_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_BACKUP_INFO        MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_BACKUP_INFO)        ///< ...
#define E_SKYLARK_RETRIEVE_FAILED_TO_REPLACE_FILE               MAKE_SKYLARK_ERROR(ERROR_SKYLARK_RETRIEVE_FAILED_TO_REPLACE_FILE)               ///< ...


//////////////////////////////////////////////////////////////////////////
// end of skylark
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// skylark server
//////////////////////////////////////////////////////////////////////////

#define ERROR_SLPTL_G_RET_SUCCESS                   0       ///< 成功
#define ERROR_SLPTL_G_RET_ERR_OUT_OF_SERVICE        1       ///< 服务器暂停服务(旁路开关)
#define ERROR_SLPTL_G_RET_ERR_PARAM                 2       ///< 参数错误

// >=100:服务器内部错误(不用对用户描述详情)
#define ERROR_SLPTL_G_RET_ERR_SVR_GENERAL           100    ///< 一般性服务器错误
#define ERROR_SLPTL_G_RET_ERR_SVR_COMMUNICATION     101    ///< 服务器间网络故障
#define ERROR_SLPTL_G_RET_ERR_SVR_DB                102    ///< 数据库错误
#define ERROR_SLPTL_G_RET_ERR_SVR_CACHE             103    ///< 缓存错误
#define ERROR_SLPTL_G_RET_ERR_SVR_HTTP_SERVER       104    ///< IIS错误
#define ERROR_SLPTL_G_RET_ERR_SVR_LOGIC             105    ///< 逻辑错误

//////////////////////////////////////////////////////////////////////////
// endof skylark server
//////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////
// BKEng
//////////////////////////////////////////////////////////////////////////
// 引擎组件管理
#define ERROR_BKE_MODULE_IS_TOO_NEW                         0x0001  ///< 引擎版本太新 (传入的版本号太老)
#define ERROR_BKE_MODULE_IS_TOO_OLD                         0x0002  ///< 引擎版本太旧 (传入的版本号太新)
#define ERROR_BKE_AVENGINE_IS_NOT_VALID                     0x0003  ///< 文件引擎不可用(可以使用云引擎)

// 文件引擎流程
#define ERROR_KSE_COPY_INTERRUPTED                          0x0101
#define ERROR_KSE_FAILED_TO_OBTAIN_WRITABLE_KS_FILE         0x0102
#define ERROR_KSE_TOO_DEEP_EXTRACT                          0x0103  ///< 压缩层次太深
#define ERROR_KSE_TOO_DEEP_UNPACK                           0x0104  ///< 脱壳层次太深

// 黑白机
#define ERROR_KSE_BWM_CLEAN_FAILED_TO_CREATE_TEMP_FILE      0x0201
#define ERROR_KSE_BWM_NEED_DELETE_FILE                      0x0202
#define ERROR_KSE_BWM_FAILED_TO_SCAN_WHILE_RECLEAN          0x0203
#define ERROR_KSE_BWM_TOO_MANY_RECLEAN_ATTEMPT              0x0204
#define ERROR_KSE_BWM_FAILED_TO_CLEAN_WHILE_RECLEAN         0x0205
#define ERROR_KSE_BWM_CLEAN_RETURN_NO_VIRUS                 0x0206
#define ERROR_KSE_BWM_SKIP_TYPE_BY_SETTINGS                 0x0207
#define ERROR_KSE_BWM_CACHED_RESULT                         0x0208

// 解压缩
#define ERROR_KSE_ARC_FAILED_TO_CLEAN_VIRUS_IN_ARCHIVE      0x0301

// 脱壳

// 云查杀引擎

// 引擎流程
#define ERROR_BKE_DELETE_FILE_NEED_REBOOT                   0x0601  ///< 删除文件后需要重启
#define ERROR_BKE_REPLACE_FILE_NEED_REBOOT                  0x0602  ///< 替换文件后需要重启
#define ERROR_BKE_CLEAN_NON_VIRUS                           0x0603  ///< 清除结果遇见非病毒文件
#define ERROR_BKE_CLEAN_SFM_MISSING_INDEX                   0x0604  ///< 在索引中找不到系统文件的修复信息
#define ERROR_BKE_FAILED_TO_FIX_CRITICAL_FILE               0x0605  ///< 修复系统关键文件的时候失败

// 病毒库升级
#define ERROR_BKE_SIGN_FILE_CRC_MISMATCH                    0x0701  ///< 文件CRC校验失败
#define ERROR_BKE_SIGN_FILE_TOO_LARGE                       0x0702  ///< 特征文件过大
#define ERROR_BKE_SIGN_FILE_MISSING_IN_INDEX                0x0703  ///< 索引中没有找到特征文件的信息

#define ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_INIT     0x0710  ///< ksmerge.dll::init 失败
#define ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_CHECK    0x0711  ///< ksmerge.dll::check 失败
#define ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_MERGE    0x0712  ///< ksmerge.dll::merge 失败
#define ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_CREATE   0x0713  ///< ksmerge.dll::create 失败

#define ERROR_BKE_SIGN_FILE_FAILED_TO_UNCOMPRESS_FILE       0x0720  ///< 解压文件失败


//////////////////////////////////////////////////////////////////////////
// error
#define E_BKE_MODULE_IS_TOO_NEW                             MAKE_BKENG_ERROR(ERROR_BKE_MODULE_IS_TOO_NEW)
#define E_BKE_MODULE_IS_TOO_OLD                             MAKE_BKENG_ERROR(ERROR_BKE_MODULE_IS_TOO_OLD)

#define E_KSE_COPY_INTERRUPTED_X                            MAKE_BKENG_ERROR(ERROR_KSE_COPY_INTERRUPTED)
#define E_KSE_FAILED_TO_OBTAIN_WRITABLE_KS_FILE             MAKE_BKENG_ERROR(ERROR_KSE_FAILED_TO_OBTAIN_WRITABLE_KS_FILE)

#define E_KSE_BWM_CLEAN_FAILED_TO_CREATE_TEMP_FILE          MAKE_BKENG_ERROR(ERROR_KSE_BWM_CLEAN_FAILED_TO_CREATE_TEMP_FILE)
#define E_KSE_BWM_NEED_DELETE_FILE                          MAKE_BKENG_ERROR(ERROR_KSE_BWM_NEED_DELETE_FILE)
#define E_KSE_BWM_FAILED_TO_SCAN_WHILE_RECLEAN              MAKE_BKENG_ERROR(ERROR_KSE_BWM_FAILED_TO_SCAN_WHILE_RECLEAN)
#define E_KSE_BWM_TOO_MANY_RECLEAN_ATTEMPT                  MAKE_BKENG_ERROR(ERROR_KSE_BWM_TOO_MANY_RECLEAN_ATTEMPT)
#define E_KSE_BWM_FAILED_TO_CLEAN_WHILE_RECLEAN             MAKE_BKENG_ERROR(ERROR_KSE_BWM_FAILED_TO_CLEAN_WHILE_RECLEAN)
#define E_KSE_BWM_CLEAN_RETURN_NO_VIRUS                     MAKE_BKENG_ERROR(ERROR_KSE_BWM_CLEAN_RETURN_NO_VIRUS)

#define E_KSE_ARC_FAILED_TO_CLEAN_VIRUS_IN_ARCHIVE          MAKE_BKENG_ERROR(ERROR_KSE_ARC_FAILED_TO_CLEAN_VIRUS_IN_ARCHIVE)
#define E_BKE_CLEAN_NON_VIRUS                               MAKE_BKENG_ERROR(ERROR_BKE_CLEAN_NON_VIRUS)
#define E_BKE_CLEAN_SFM_MISSING_INDEX                       MAKE_BKENG_ERROR(ERROR_BKE_CLEAN_SFM_MISSING_INDEX)
#define E_BKE_FAILED_TO_FIX_CRITICAL_FILE                   MAKE_BKENG_ERROR(ERROR_BKE_FAILED_TO_FIX_CRITICAL_FILE)

#define E_BKE_SIGN_FILE_CRC_MISMATCH                        MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_CRC_MISMATCH)
#define E_BKE_SIGN_FILE_TOO_LARGE                           MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_TOO_LARGE)
#define E_BKE_SIGN_FILE_MISSING_IN_INDEX                    MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_MISSING_IN_INDEX)

#define E_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_INIT         MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_INIT)
#define E_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_CHECK        MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_CHECK)
#define E_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_MERGE        MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_MERGE)
#define E_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_CREATE       MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_FAILED_TO_CALL_KSMERGE_CREATE)

#define E_BKE_SIGN_FILE_FAILED_TO_UNCOMPRESS_FILE           MAKE_BKENG_ERROR(ERROR_BKE_SIGN_FILE_FAILED_TO_UNCOMPRESS_FILE)



//////////////////////////////////////////////////////////////////////////
// info
#define S_BKE_AVENGINE_IS_NOT_VALID                         MAKE_BKENG_SUCCESS(ERROR_BKE_AVENGINE_IS_NOT_VALID)

#define S_KSE_BWM_SKIP_TYPE_BY_SETTINGS                     MAKE_BKENG_SUCCESS(ERROR_KSE_BWM_SKIP_TYPE_BY_SETTINGS)
#define S_KSE_TOO_DEEP_EXTRACT                              MAKE_BKENG_SUCCESS(ERROR_KSE_TOO_DEEP_EXTRACT)                          
#define S_KSE_TOO_DEEP_UNPACK                               MAKE_BKENG_SUCCESS(ERROR_KSE_TOO_DEEP_UNPACK)

#define S_KSE_BWM_CACHED_RESULT                             MAKE_BKENG_SUCCESS(ERROR_KSE_BWM_CACHED_RESULT)

#define S_BKE_DELETE_FILE_NEED_REBOOT                       MAKE_BKENG_SUCCESS(ERROR_BKE_DELETE_FILE_NEED_REBOOT)
#define S_BKE_REPLACE_FILE_NEED_REBOOT                      MAKE_BKENG_SUCCESS(ERROR_BKE_REPLACE_FILE_NEED_REBOOT)

//////////////////////////////////////////////////////////////////////////
// end of kse
//////////////////////////////////////////////////////////////////////////




#endif//BKERROR_H