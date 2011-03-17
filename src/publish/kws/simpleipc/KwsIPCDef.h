///////////////////////////////////////////////////////////////
//
//	Filename: 	KwsIPCDef.h
//	Creator:	lichenglin  <lichenglin@kingsoft.com>
//	Date:		2009-3-31  17:23
//	Comment:	
//
///////////////////////////////////////////////////////////////


#ifndef _KwsIPCDef_h_
#define _KwsIPCDef_h_

//////////////////////////////////////////////////////////////////////////
//
// 错误码
//
//////////////////////////////////////////////////////////////////////////

#define KWSIPCERR_OK           0  // OK
#define KWSIPCERR_UNKNOWN      1  // 
#define KWSIPCERR_INVALIDARG   2  // 无效参数
#define KWSIPCERR_DATASIZE     3  // 数据包太大, 我们比较挫, 不支持大数据
#define KWSIPCERR_OBJECT       4  // 服务端未启动, 或打开通信对象失败
#define KWSIPCERR_TIMEOUT      5  // 等待超时




//////////////////////////////////////////////////////////////////////////
//
// 协议
//
//////////////////////////////////////////////////////////////////////////

#define MAX_PRTCL_DATA_SIZE  4096    

#define PRTCL_UNKNOWN            0

//-------------------------------------
// 弹出泡泡
#define PRTCL_SHOWPOPO           1
#define PK_MSG      "msg"        /*[in]*/
#define PK_BTNTEXT  "btn"        /*[in]*/  
#define PK_STYLE    "style"      /*[in]*/    // "0": 绿泡, "1": 黄泡, "2": 红泡
#define PK_REASON	"reason"	 /*[in]*/    // 表示弹泡泡的原因
#define PK_URL		"url"	 /*[in]*/		// 表示弹泡泡的网址
#define PK_BROWSER  "browser"               // 表示弹泡泡的浏览器

//-------------------------------------
// 设置托盘变灰状态 
#define PRTCL_SETTRAYSTATUS      2
// in:
#define PK_STATUS   "status"     /*[in]*/    // "0":变灰, "1": 变亮


//-------------------------------------
// 显示托盘
#define PRTCL_SHOWTRAY           3
// in:
#define PK_SHOWTRAY "showtray"   /*[in]*/    // "0":不显示 "1": 显示
// out:


//-------------------------------------
// 记录日志
#define PRTCL_LOG                4
// in:
#define PK_LOGFILE  "file"       /*[in]*/
#define PK_LOGMSG   "msg"        /*[in]*/
#define PK_LOGTYPE  "type"       /*[in]*/
typedef int LOGTYPE;
#define TY_ALL      0
#define TY_TROJAN   1
#define TY_FISHING  2

//-------------------------------------
// 退出托盘程序
#define PRTCL_QUIT               5


//-------------------------------------
// 获取托盘状态
#define PRTCL_QUERYTRAYSTATUS    6
#define PK_TRAYSTATUS "ts"       /*[out]*/    // 详见 TRAYSTATUS

#define PK_ACTIVESESSION    "session"     //当前活动用户Session

// 服务器端会将一直保留状态量（除了气泡，取过就会置为无气泡）
typedef int TRAYSTATUS;
#define TS_DEFAULT	0x00000000	 // 默认状态（图标亮，不隐藏，不退出，无气泡）
#define TS_GRAY     0x00000001   // 图标变灰
#define TS_HIDE     0x00000002   // 图标隐藏
#define TS_QUIT     0x00000004   // 需要退出程序
#define TS_POPO     0x00000010   // 有泡泡需要弹出
#define TS_PROTECTPROGRAM     0x00000020   // 有被保护进程
#define TS_TROJAN	0x00000040	 // 防挂马开启
#define TS_FISH		0x00000080	 // 防钓鱼开启
#define TS_ENGINE	0x00000100	 // 搜索引擎保护
#define TS_FILTER	0x00000200	 // 网页净化



//-------------------------------------
// 查询泡泡, 供托盘使用
#define PRTCL_QUERYPOPOMSG       7
#define PK_POPOEXISTS "pe"       /*[out]*/    // 是否有泡泡需要弹出   "0" 没有   1"有"
//#define PK_MSG      "msg"      /*[out]*/    // 同: PRTCL_SHOWPOPO
//#define PK_BTNTEXT  "btn"      /*[out]*/    // 同: PRTCL_SHOWPOPO
//#define PK_STYLE    "style"    /*[out]*/    // 同: PRTCL_SHOWPOPO



//-------------------------------------
// 创建进程及其他
#define PRTCL_CALL_AS_USER       8
#define PRTCL_CALL_AS_SYSTEM     9
#define PRTCL_CALL_AS_USER_OR_SYSTEM       13   // 创建用户进程，如果是vista下非管理员用户则创建系统进程
#define PRTCL_INJECT_SVC			14		  // 注入svchost
#define PRTCL_REG_CHANGE			15		  // 刷新注册表cache
#define PRTCL_GET_SPDIR             16        // 获取网盾sp所在目录
#define PRTCL_GET_KKT				17        // 获取网盾的版本类型（独立、卫士、毒霸）
#define PK_CMDLINE    "cmdline"  /*[in]*/     // 进程命令行, 如果不带全路径 默认当前目录
#define PK_SPDIR      "spdir"   /*[out]*/    // sp所在目录
#define PK_KKT		  "kkt"      /*[out]*/    // 网盾的版本类型


//-------------------------------------
// 当前保护的进程
#define PRTCL_SET_PROTECTPROGRAM     10
#define PRTCL_GET_PROTECTPROGRAM     11
#define PK_PROGRAMPATH    "programpath"  /*[in]*/     // 当前保护的程序全路径
#define PK_PROGRAM_PID    "pid"  /*[in]*/     // 当前保护程序的pid


//-------------------------------------
// 删除日志文件
#define PRTCL_DELETELOG                12


//-------------------------------------
// 获取、保存设置
#define PRTCL_GET_PROFILESETTING		20
#define PRTCL_SET_PROFILESETTING		21
#define PRTCL_GET_REGSTRINGSETTING      22
#define PRTCL_SET_REGSTRINGSETTING      23
#define PRTCL_GET_REGDWORDSETTING       24
#define PRTCL_SET_REGDWORDSETTING       25
#define PRTCL_DELETE_REGSETTING         26
#define PRTCL_SET_CRREGSTRINGSETTING	27
#define PK_KEY        "key"
#define PK_SUBKEY	  "subkey"
#define PK_VALUE      "value"
#define PK_DEFAULT    "default"

//-------------------------------------
// 上报信息
#define PRTCL_REPORT_INFO				30
#define PK_REPORT_MC			"mc"
#define PK_REPORT_SC			"sc"
#define PK_REPORT_INFO			"info"

#define PRTCL_REPORT_URL				31
#define PK_REPORT_URL			"url"

#define PRTCL_REPORT_INFO_NEW				32
#define PK_REPORT_MC			"mc"
#define PK_REPORT_SC			"sc"
#define PK_REPORT_INFO			"info"

//-------------------------------------
// 黑名单
#define PRTCL_BL_ADD					40
#define PRTCL_BL_DELETE					41
#define PK_BL_URL				"url"
#define PK_BL_REASON			"reason"

#define PRTCL_BL_EXPORT					42
#define PRTCL_BL_IMPORT					43
#define PRTCL_BL_DESUBSCRIBE            44
#define PK_BL_ADNAME            "adname"
#define PK_BL_ADDATE            "addate"
#define PK_BL_FILEPATH			"path"
#define PK_BL_ITEMFLAG          "flag"
#define PK_BL_SUBRULEIID        "ruleid"

#define PRTCL_BL_ADDEX				    45
#define PK_BL_FTURL			    "fturl"
#define PK_BL_EXIST             "urlexist"

//-------------------------------------
// 恶意首页
#define PRTCL_SSP_ADD					50
#define PRTCL_SSP_DELETE				51
#define PRTCL_SSP_GET					52
#define PRTCL_SSP_DELAUTO				53
#define PK_SSP_URL				"ssp"
#define PK_SSP_TYPE				"type"

//-------------------------------------
// 网页净化
#define PRTCL_FR_ADD					60
#define PRTCL_FR_DELETE					61
#define PK_FR_RULE				"rule"
#define PK_FR_SET				"set"

#define PRTCL_FR_EXPORT					62
#define PRTCL_FR_IMPORT					63
#define PK_FR_FILEPATH			"path"

//记录危险网址给地址栏弹泡提供依据
#define PRTCL_DAN                       64

#define PRTCL_BL_INITADIDNAME           66
#define PRTCL_BL_INSERTBLURLID          67
#define PRTCL_BL_GETBLURLADNAME         68
#define PRTCL_BL_UPDATEIDNAME           69
#define PRTCL_BL_GETADIDOFNAME          70
#define PRTCL_BL_GETADIDOFDATE          71
#define PRTCL_BL_GETFREEADID            72
// in:
#define PK_URLTMPFILE           "durltmp"   /*[in]*/
#define PK_URLMSG               "url"       /*[in]*/

#endif // _KwsIPCDef_h_
