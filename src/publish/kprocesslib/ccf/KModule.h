/**
* @file     KModule.h
* @brief    提供系统环境相关的函数
* @details  
*/

#pragma once

#include <shlobj.h>

using namespace std;

namespace Kingsoft
{
	namespace Test
	{
		namespace CCF
		{
			namespace System
			{
				class KModule
				{
				public:

					/**
					* @brief 获取当前模块的目录路径
					* @return std::wstring 路径
					* @note 
					*/
					static wstring GetCurrentModuleDirectory();

					/**
					* @brief 获取当前模块的文件名
					* @return std::wstring 文件名
					* @note 
					*/
					static wstring GetCurrentModuleFileName();

					static wstring GetCurrentModuleFilePath();
				};
			}
		}
	}
}