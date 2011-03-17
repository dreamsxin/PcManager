/** 
* @file bkrserror.h
* @brief 回扫自定义错误码文件
* @author BAO<baoderong@kingsoft.com>
* @date November 15,2010
*/

#ifndef BKRS_ERROR_H
#define BKRS_ERROR_H

/// 自定义的错误代码
#define BKRS_E_NOTINITIALIZED 0x81000001		/// 类未成功初始化
#define BKRS_E_DBERRORED 0x81000002			/// 数据库管理系统出错
#define BKRS_E_DBBUSY 0x81000003			/// 数据库管理系统正忙
#define BKRS_E_MAKENAME 0x81000004			/// 生成文件名出错
#define BKRS_E_REINIT 0x81000005			/// 回扫类重复初始化
#define BKRS_E_DBINIT 0x81000003			/// 数据库管理系统初始化出错

#endif
