/** 
* @file bkrescanbasedata.h
* @brief 基本数据定义
* @author BAO<baoderong@kingsoft.com>
* @date October 30,2010
*/

#ifndef BKRESCANBASEDATA_H
#define BKRESCANBASEDATA_H

#include "bkrstype.h"

#define MAX_CHANNEL_LENGTH 16		/// 通道号的最大长度
#define MAX_VIRUSNAME_LENGTH 60	/// 云查杀返回的病毒名最大长度，返回的为CP_UTF8的，最大长度为60个字符，目前为止病毒名全为英文字符

/** 
* @enum BKRS_ScanSatus
* @brief 扫描结果返回值
*/
typedef enum
{
	enumBKRS_White = 0,		/// 无毒
	enumBKRS_Black,			/// 有毒
	enumBKRS_Unknown,		/// 未知
	enumBKRS_Scanned,		/// 已经完全扫描过了，还是未知的文件，即恢文件
	enumBKRS_Dropped		/// 文件被丢弃（如删除）
} BKRS_ScanStatus;

/** 
* @enum BKRS_ScanSatus
* @brief 扫描结果返回值
*/
typedef enum
{
	enumBKRS_Null = -1,

	/// handle结果
	enumBKRS_Handled = 0,				/// 表示已知文件已经处理过了
	enumBKRS_UserSkipHandled = 1,		/// 表示用户不处理此文件

	/// scan结果
	enumBKRS_NeedHandled = 0x100,		/// 表示从未知变已知，还未处理，需要处理
}BKRS_HandleStatus;

/** 
* @enum BKRS_HistoryQueryType
* @brief 回扫历史文件查询类型
*/
typedef enum
{
	enumHistoryQuery_All = 0,			/// 查询所有的文件
	enumHistoryQuery_White,				/// 查询白的文件
	enumHistoryQuery_Black,				/// 查询黑的文件
	enumHistoryQuery_Scanned,			/// 查询恢的文件，即已经扫描过的文件
	enumHistoryQuery_Dropped,			/// 查询丢弃的文件
	enumHistoryQuery_Unknown			/// 查询未知的文件
} BKRS_HistoryQueryType;

/** 
* @enum BKRS_RTScannedQueryType
* @brief 最新回扫过的文件查询类型
*/
typedef enum
{
	enumRTScannedQuery_All = 0,			/// 查询所有最新处理过的文件
	enumRTScannedQuery_White,				/// 查询所有最新处理过的白文件
	enumRTScannedQuery_Black,				/// 查询所有最新处理过的黑文件
	enumRTScannedQuery_Scanned,			/// 查询所有最新处理过的恢文件的文件，即已经扫描过的文件
	enumRTScannedQuery_Dropped			/// 查询所有最新丢弃文件的文件
} BKRS_RTScannedQueryType;

typedef enum
{
	enumBKRS_From_FastScan = 0x01,			/// 文件来自快速扫描
	enumBKRS_From_FileMonitor = 0x02,		/// 文件来自文件监控
	enumBKRS_From_FullDiskScan = 0x04,		/// 文件来自全盘扫描
	enumBKRS_From_RSS_KXESCANSP = 0x08,		/// 文件来自毒霸查杀sp
	enumBKRS_From_RSS_USBMONITOR = 0x10,	/// 文件来自毒霸U盘监控
	enumBKRS_From_ALL = 0x80				/// 不分来源（实时交互和回扫模块不要用）
}BKRS_FileFromType;

/** 
* @struct BKRS_QueryFileInfo
* @brief 查询返回的文件信息
*/
typedef struct  
{
	wchar_t pszFileName[MAX_PATH];			/// 文件名
	BYTE pszMD5[16];						/// 文件的MD5
	FILETIME stLastScanTime;				/// 文件检出时间（Unknown为上次回扫的时间）
	BKRS_ScanStatus enumFileType;			/// 文件黑白类型
	BKRS_HandleStatus enumHandleStatus;		/// 文件变已知后有没有被处理
	char szVirusName[MAX_VIRUSNAME_LENGTH];	/// 文件对应的病毒名
} BKRS_QueryFileInfo;

#endif
