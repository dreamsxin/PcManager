/**
* @file    slfeedaudit.h
* @brief   ...
* @author  zhangrui
* @date    2009-03-18  16:56
*/

#ifndef SLFEEDAUDIT_H
#define SLFEEDAUDIT_H

//////////////////////////////////////////////////////////////////////////
// 审计 pid 产品ID
#define SLK_PID_UNKNOWN                     0       ///< 未知的产品id
#define SLK_PID_UPDATE                      1       ///< 升级程序
#define SLK_PID_LSE                         2       ///< LSE
#define SLK_PID_OGSLDR                      3       ///< 网游嵌入模块
#define SLK_PID_SKYLARK                     4       ///< Skylark功能模块
#define SLK_PID_OGSVR                       5       ///< 网游保护服务
#define SLK_PID_AV_DETECT                   6       ///< 杀软检测
#define SLK_PID_FW_DETECT                   8       ///< 防火墙检测
#define SLK_PID_OS_DETECT                   9       ///< 操作系统版本检测
#define SLK_PID_PP_SVR                      7       ///< 大脚帐号保护
#define SLK_PID_ANTIARP                     10      ///< ARP 防火墙
#define SLK_PID_FILE_SYS_DETECT             11      ///< 文件系统探测
#define SLK_PID_UPDATE_VC_LOG               12      ///< 版本check记录
#define SLK_PID_BIGFOOT_DOWNLOADER          13      ///< 大脚下载器
#define SLK_PID_BKSECPRT                    14      ///< 进程保护模块
#define SLK_PID_SKYLARK_PROTO               15      ///< skylark客户端
// 以上为LSE v1及相关产品使用的pid

#define SLK_PID_VKILLER                     16      ///< 木马专杀客户端
#define SLK_PID_BIGFOOT_EMBEDDED            17      ///< 大脚的嵌入模块
#define SLK_PID_SKYLARKSCAN_TOOL            18      ///< 命令行扫描工具

#define SLK_PID_SKYLARK_FOR_SOGOU           19      ///< 搜狗

#define SLK_PID_ELIVE_SAFE                  20      ///< 电信天翼live服务
#define SLK_PID_BEIKE_ANTIARP               21      ///< 贝壳的arp防火墙
#define SLK_PID_BEIKE_ONLINESCAN            22      ///< 贝壳在线杀毒
#define SLK_PID_KSAFE                       23      ///< 金山卫士的pid

#define SLK_PID_MAX                         256     ///< pid 的最大值


//////////////////////////////////////////////////////////////////////////
// 审计 chid 通道ID
#define SLK_CHID_DEFAULT            0       ///< 默认的通道

// LSE用到的CHID
#define SLK_CHID_LSE_SKYLARK2       2       ///< 使用skylark2的lse服务

// SLK_PID_VKILLER 用到的CHID
#define SLK_CHID_BIGFOOT            1       ///< vkiller 通过大脚发布的的通道
#define SLK_CHID_BBS                2       ///< vkiller 通过论坛发布的的通道
#define SLK_CHID_BAOFENG            3       ///< vkiller 通过暴风发布的的通道
#define SLK_CHID_KINGSOFT           4       ///< vkiller 通过金山发布的的通道
#define SLK_CHID_ELIVE              5       ///< vkiller 通过电信ELive发布的的通道
#define SLK_CHID_9DUAN              6       ///< vkiller 通过9段网娱发布的的通道
#define SLK_CHID_JIANDANYOU         7       ///< vkiller 通过简单游发布的的通道
#define SLK_CHID_LANGANG            8       ///< vkiller 通过蓝港发布的的通道
#define SLK_CHID_CHENGJISIHAN       9       ///< vkiller 通过麒麟成吉思汗发布的的通道
#define SLK_CHID_KINGSOFTGAME       11      ///< vkiller 通过金山游戏发布的的通道
#define SLK_CHID_798                12      ///< vkiller 通过798发布的的通道
#define SLK_CHID_OEM                13      ///< vkiller 无定制需求但需要单独统计的版本通道
#define SLK_CHID_BIGFOOT_WOW        14      ///< vkiller 通过魔兽大脚发布的的通道
#define SLK_CHID_BIGFOOT_JX3        15      ///< vkiller 通过剑三大脚发布的的通道
#define SLK_CHID_OEM_CIBA           16      ///< vkiller 通过词霸发布的通道
#define SLK_CHID_BAOYI              17      ///< vkiller 通过宝易网发布的通道
#define SLK_CHID_W2K                18      ///< vkiller win2k专用版本的通道
#define SLK_CHID_ANTIARP            19      ///< vkiller 通过arp防火墙捆绑的通道
#define SLK_CHID_YY                 20      ///< vkiller 通过YY发布的通道
#define SLK_CHID_SUBANG             21      ///< vkiller 速帮的版本通道
#define SLK_CHID_KSG_FS2            22      ///< vkiller 封神榜2通道
#define SLK_CHID_KSG_JXW2           23      ///< vkiller 剑侠2外传通道

#define SLK_CHID_VIRSCAN_ORG        10      ///< 给virscan.org提供的扫描工具

#define SLK_CHID_MAX                65536   ///< chid 的最大值


//////////////////////////////////////////////////////////////////////////
// 审计 action 行为ID
#define SLK_FEEDAUDIT_ACT_UNKNOWN                       0       ///< 无意义
#define SLK_FEEDAUDIT_ACT_PRODUCT_INSTALL               1       ///< 安装: 当安装程序完成安装时
#define SLK_FEEDAUDIT_ACT_PRODUCT_UNINSTALL             2       ///< 卸载: 当卸载程序完成卸载时
#define SLK_FEEDAUDIT_ACT_PRODUCT_ACTIVE                3       ///< 活跃: 产品的任何模块执行时
#define SLK_FEEDAUDIT_ACT_PRODUCT_FIRST_ACTIVE          4       ///< 首次活跃: 第一次"活跃"发生时
#define SLK_FEEDAUDIT_ACT_PRODUCT_INTERACTIVE           5       ///< 交互活跃: 产品被用户点击使用时
#define SLK_FEEDAUDIT_ACT_PRODUCT_FIRST_INTERACTIVE     6       ///< 首次交互活跃: 第一次"交互活跃发生时"

#define SLK_FEEDAUDIT_ACT_PRODUCT_ACTIVE_FOR_UPDATE     51      ///< 活跃：升级程序循序的。

// SLK_PID_VKILLER 用到的ACT
#define SLK_FEEDAUDIT_ACT_PRODUCT_SCHEDULE_ACTIVE       7       ///< 计划任务活跃
#define SLK_FEEDAUDIT_ACT_PRODUCT_FASTCHECK_ACTIVE      8       ///< 三秒扫描活跃
#define SLK_FEEDAUDIT_ACT_PRODUCT_SINGLESCAN_ACTIVE     9       ///< 单个文件扫描活跃
#define SLK_FEEDAUDIT_ACT_PRODUCT_TAKE_SCORE            10      ///< 打分活跃
#define SLK_FEEDAUDIT_ACT_PRODUCT_YYCHECK_ACTIVE        11       ///< 三秒扫描活跃

#define SLK_FEEDAUDIT_ACT_MAX                           256     ///< 最大的audit action id

#endif//SLFEEDAUDIT_H