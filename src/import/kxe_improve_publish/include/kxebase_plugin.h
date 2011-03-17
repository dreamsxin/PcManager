//////////////////////////////////////////////////////////////////////
///		@file		kxebase_plugin.h
///		@author		luopeng
///		@date		2008-9-17 09:07:58
///	
///		@brief		扩展SDK的定义方式,扩展的SDK必须定义如下五个函数
//////////////////////////////////////////////////////////////////////
#pragma once
#include "../kxebase/kxebase_plugin_marco.h"

/**
* @page kxebase_plugin_sdk KXEngine SDK Plugin
* @section kxebase_plugin_sdk_face 表现形式
* - 通过DLL使用C函数的方式将以下函数导出,并存放在plugin目录
* @section kxebase_plugin_sdk_function 函数定义
* - @ref type_kxe_sdk_plugin_initialize
* - @ref type_kxe_sdk_plugin_uninitialize
* - @ref type_kxe_sdk_plugin_start
* - @ref type_kxe_sdk_plugin_stop
* - @ref type_kxe_sdk_plugin_get_name
* @section kxebase_plugin_sdk_function_name 函数名的宏
* - @ref KXE_SDK_PLUGIN_INIT
* - @ref KXE_SDK_PLUGIN_FINI
* - @ref KXE_SDK_PLUGIN_START
* - @ref KXE_SDK_PLUGIN_STOP
* - @ref KXE_SDK_PLUGIN_GET_NAME
* 
*/

/**
 * @brief 扩展SDK需要定义的函数名,kxebase会调用此函数进行plugin初始化
 */
#define KXE_SDK_PLUGIN_INIT "kxe_sdk_plugin_initialize"

/**
* @brief 扩展SDK需要定义的函数类型,kxebase会调用此函数进行plugin初始化
*/
typedef int __stdcall type_kxe_sdk_plugin_initialize();

/**
* @brief 扩展SDK需要定义的函数名,kxebase会调用此函数进行plugin反初始化
*/
#define KXE_SDK_PLUGIN_FINI "kxe_sdk_plugin_uninitialize"

/**
* @brief 扩展SDK需要定义的函数类型,kxebase会调用此函数进行plugin反初始化
*/
typedef int __stdcall type_kxe_sdk_plugin_uninitialize();

/**
* @brief 扩展SDK需要定义的函数名,kxebase会调用此函数启动plugin
*/
#define KXE_SDK_PLUGIN_START "kxe_sdk_plugin_start"

/**
* @brief 扩展SDK需要定义的函数类型,kxebase会调用此函数启动plugin
*/
typedef int __stdcall type_kxe_sdk_plugin_start();

/**
* @brief 扩展SDK需要定义的函数名,kxebase会调用此函数停止plugin
*/
#define KXE_SDK_PLUGIN_STOP "kxe_sdk_plugin_stop"

/**
* @brief 扩展SDK需要定义的函数类型,kxebase会调用此函数启动plugin
*/
typedef int __stdcall type_kxe_sdk_plugin_stop();

/**
* @brief 扩展SDK需要定义的函数名,kxebase会调用此函数获取此plugin的名字
*/
#define KXE_SDK_PLUGIN_GET_NAME "kxe_sdk_plugin_get_name"

/**
* @brief 扩展SDK需要定义的函数类型,kxebase会调用此函数获取此plugin的名字
*/
typedef const char* __stdcall type_kxe_sdk_plugin_get_name();

