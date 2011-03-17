#pragma once
#include "public.h"

NS_BEGIN(KIS)
NS_BEGIN(KOemKsfe)

/*
OEM信息控制
Duba有很多特定的版本，这些版本可能在功能多少或者一些界面表现上存在一些差异，
为了能够统一所有版本，我们必须做到二进制用的是同一份，也就是所有功能集合。
安装包因为是次性的，可以为特定版本作非常特殊的定制，但是升级服务器、代码相关
二进制文件、病毒库，官方语言包都要做到只维护一份。

一、几个概念：
1、[oem file]：kis.dat、oem.dat，其格式为加密二进制文件，用以决定Duba的一些
行为，其内容参见oem_category的说明
2、[oem dir]：位于 <root/oem> 下，其命名规则为一GUID，其根下存放oem.dat和其它
文件，在kis中，其作用是用以覆盖程序默认行为

之所以存在[oem file]，是为了方便升级维护，当对程序进行一些较小的定制时，
可以利用它完成。

二、相关文件目录结构：
<kis>
|	kis.dll
|	kis.dat
|	<oem>
			-----| <{7E2E5FCB-C0A9-43e3-994A-4954A529A686}>
			-----| oem.dat
			-----| kav32.xml
			-----| ...
			-----| Kav32_Logo.bmp

三、程序实现逻辑
	1、kis.dll初始化时遍历<oem>子目录，找到第一个有效的[oem dir]
	2、如果[oem dir]不存在oem.dat，则以根目录的kis.dat作为kis的[oem file]
	3、kslang在Load时，如果在[oem dir]存在相同文件名的文件，则使用[oem dir]的文件
		否则以当前语言目录为准，这个过程，以一个文件为单位
	4、kslang在加载相应xml之后，如果[oem file]中存在PatchCmds，则会对相应内容
		进行修改，比如增删菜单项、修改文字内容、换图片等等

*/
//注意事项：
//1、因为考虑到效率问题而采用了数字作为键值，所以也决定了一旦定义了一个功能号
//	 则永远不能改变其数值，否则需要可能会引起功能点混乱。
//2、请在每个功能点后请按照已经有的格式写清楚功能含义，及数据类型
//	 如果一个功能ID不再使用了，请保留点位
//3、目前只支持32位整数类型和字符串类型，其中Bool类型以0和1来代替FALSE和TRUE
//4、一旦发布后，新增值必须添加在后端
enum oem_category
{								// 类型					含义
	oemc_start = 0x0,			//int					代码实现用
	oemc_VirusFreeShow,			//int					是否卫士主界面打开软件管理模块
	oemc_MoreOptimizeShow,		//int					是否卫士主界面显示“更多优化”
	oemc_SearchEngineProtectShow,//int					是否卫士主界面显示“搜索引擎”项	
	oemc_SoftwareMangerShow,	//int					是否显示软件管理
	oemc_trayShow,				//int					是否显示托盘
	oemc_netmonterShow,			//int					是否显示流量监控
	oemc_OtherCheckName,		//LPCWSTR				增加新的互斥变量新增的字符串
	oemc_DownProtectedColor,	//int					下载保护的title的文字颜色
	oemc_SafeLinkURL,			//LPCWSTR				安全链接的URL路径
	oemc_IconBig,				//int					指定的加载的ICON的值
	oemc_IconSmall,				//int					指定的加载的ICON的值（small）
	oemc_SubKeyStr,				//LPCWSTR				指定SOFTWARE下的项名称，如SOFTWARE\\KSafe时，为KSafe
	oemc_IsOem,					//int					是否是oem版本，1==百度OEM
	oemc_Appdata,				//LPCWSTR				APP的路径
	oemc_Reclye,				//LPCWSTR				RECYCLE的路径
	oemc_KwspluginName,			//LPCWSTR				网盾需要的DLL的文件名
	oemc_ServiceName,			//LPCWSTR				服务名
	oemc_KWSCShowName,			//LPCWSTR				金山网盾拦截广告的标题
	oemc_IsOpenNetM,			//int					是否打开网络测速
	oemc_SetCloudAddr,			//int					云安全鉴定说明的url
	oemc_BBSUrl,				//LPCWSTR				BBS指定的路径
	oemc_RunTrayName,			//LPCWSTR				ksafetray的启动项名
	oemc_RunKSafeSvcName,		//LPCWSTR				ksafesvc的启动项名
	oemc_FishBlockUrl,			//LPCWSTR				钓鱼网站拦截显示的地址
	oemc_UpdataUrlMsg,			//LPCWSTR				弹泡滴的升级的Url
	oemc_UpdataUrlShow,			//LPCWSTR				升级的Url
	oemc_UpdataUrlQuick,		//LPCWSTR				升级的Url
	oemc_NetmonPriLevel,		//int					流量监控的等级
	oemc_end					//int					代码实现用
};

/*
* @brief								设定OEM文件的全路径
* @param lpFilePath					--- 指定需要设置的OEM文件的全路径，如果使用默认，则会从oem目录中查找到的第一个文件，
*										如果ome目录中不存在文件，则读原来的文件。
* @remark							--- 该函数如果需要调用，必须在查询函数之前调用
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  SetOEMFullPathA(IN LPCSTR lpFilePath = NULL);
STDAPIN_(int)  SetOEMFullPathW(IN LPCWSTR lpFilePath = NULL);


/*
* @brief								查询Oem的值。
* @param nKey						--- 需要查询的key
* @param nValue						--- 查询到的结果
* @param lpFilePath					--- 指定需要查询的文件的路径，如果不指定。则会到oem目录中查找
* @return 
*							   TRUE --- 查找成功
*							   FALSE--- 查找失败
*/
STDAPIN_(int)  QueryOEMInt(IN KOemKsfe::oem_category nKey, IN int& nValue); 

/*
* @brief								查询Oem的值。
* @param nKey						--- 需要查询的key
* @param lpValue					--- 查询到的结果,如果该值为NULL,则nSize返回需要的大小
* @param nSize						--- 查询到的结果的字符串长度。如果lpValue传入时，为NULL，则返回长度的实际大小
* @param lpFilePath					--- 指定需要查询的文件的路径，如果不指定。则会到oem目录中查找
* @remark							--- 建议使用：
std::wstring strValue;
CallFunc<int, KIS::KOemKsfe::oem_category, LPWSTR, size_t&>(KIS::KOemKsfe::QueryOEMStrW, KIS::KOemKsfe::XXX, strValue);
* @return 
*							   TRUE --- 查找成功
*							   FALSE--- 查找失败
*/
STDAPIN_(int)  QueryOEMStrA(IN KOemKsfe::oem_category nKey, OUT LPSTR lpValue, IN OUT size_t& nSize);
STDAPIN_(int)  QueryOEMStrW(IN KOemKsfe::oem_category nKey, OUT LPWSTR lpValue, IN OUT size_t& nSize);


/*
* @brief								将所有OEM的配置信息写入
* @return 
*							   TRUE --- 查找成功
*							   FALSE--- 查找失败
*/
STDAPIN_(int)  WriteAllOemData(void);


NS_END(KOemKsfe)
NS_END(KIS)




