/**
* @file    slfeedschema.h
* @brief   ...
* @author  zhangrui
* @date    2009-03-03  11:43
*/

#ifndef SLFEEDSCHEMA_H
#define SLFEEDSCHEMA_H

//////////////////////////////////////////////////////////////////////////
// 16以下的id为LSE保留,相关的 feedcode 查询 lse的 bugcode.h 和 infocode.h
//////////////////////////////////////////////////////////////////////////
//    bugfeed       ->  feeddbg
//        id        ->
//        mid       ->  mid
//                  ->  ver
//        time      ->  time
//        ip        ->  ip
//        ptype     ->  schema
//        code      ->  code
//                  ->  par
//        content   ->  msg
//
//////////////////////////////////////////////////////////////////////////
//    infourl       ->  feedsta
//        id        ->
//        mid       ->  mid
//                  ->  ver
//        time      ->  time
//        ip        ->  ip
//        ptype     ->  schema
//        code      ->  code
//                  ->  par
//        content   ->
//
//////////////////////////////////////////////////////////////////////////
//    activeinfo    ->  feedadt
//        mid       ->  mid
//                  ->  ver
//        ip        ->  ip
//        type      ->  pid
//        tid1      ->  chid
//                  ->  act
//  
#define SLK_SCHEMA_LSE_PT_UPDATE               1       ///< 升级程序
#define SLK_SCHEMA_LSE_PT_LSE                  2       ///< LSE
#define SLK_SCHEMA_LSE_PT_OGSLDR               3       ///< 网游嵌入模块
#define SLK_SCHEMA_LSE_PT_SKYLARK              4       ///< Skylark功能模块
#define SLK_SCHEMA_LSE_PT_OGSVR                5       ///< 网游保护服务
#define SLK_SCHEMA_LSE_PT_AV_DETECT            6       ///< 杀软检测(已废弃)
#define SLK_SCHEMA_LSE_PT_FW_DETECT            8       ///< 防火墙检测(已废弃)
#define SLK_SCHEMA_LSE_PT_OS_DETECT            9       ///< 操作系统版本检测(已废弃)
#define SLK_SCHEMA_LSE_PT_PP_SVR               7       ///< 大脚帐号保护
#define SLK_SCHEMA_LSE_PT_ANTIARP              10      ///< ARP 防火墙
#define SLK_SCHEMA_LSE_PT_FILE_SYS_DETECT      11      ///< 文件系统探测(已废弃)
#define SLK_SCHEMA_LSE_PT_UPDATE_VC_LOG        12      ///< 版本check记录
#define SLK_SCHEMA_LSE_PT_BIGFOOT_DOWNLOADER   13      ///< 大脚下载器
#define SLK_SCHEMA_LSE_PT_BKSECPRT             14      ///< 进程保护模块
#define SLK_SCHEMA_LSE_PT_SKYLARK_PROTO        15      ///< skylark客户端(v1)

//////////////////////////////////////////////////////////////////////////
// [16, 127] 由skylark使用
#define SLK_SCHEMA_TEST             16      ///< 测试用
#define SLK_SCHEMA_UPDATE           17      ///< 升级模块

#define SLK_SCHEMA_ENV_INF          32      ///< 环境信息收集
#define SLK_SCHEMA_PROTO            33      ///< level查询和上报
#define SLK_SCHEMA_PROTO_AUTORUN    34      ///< autorun查询和上报
#define SLK_SCHEMA_AUTORUN          35      ///< autorun模块本身的查询和上报
#define SLK_SCHEMA_CACHE_DB         36      ///< skylark缓存数据库
#define SLK_SCHEMA_TRJSCAN          37      ///< skylark扫描模块

#define SLK_SCHEMA_BEIKE_ANTIARP    38      ///< 贝壳arp防火墙

#define SLK_SCHEMA_ELIVESAFE        39      ///< 天翼beikeserv

#define SLK_SCHEMA_BACKUP           40      ///< skylark备份模块

//////////////////////////////////////////////////////////////////////////
// 其他模块从 64 开始定义
#define SLK_SHEMA_VKILLER           64      ///< 木马专杀
#define SLK_SHEMA_OLSCAN            65      ///< 在线杀毒

#define SLK_SCHMEA_MAX              256     ///< 最大256


#endif//SLFEEDSCHEMA_H