/**
* @file     KPath.h
* @brief    路径相关函数
* @details  
* @author   林杰(Linjie@kingsoft.com)
* @date     2009-4-3
*/

#pragma once

using namespace std;

namespace Kingsoft
{
	namespace Test
	{
		namespace CCF
		{
            namespace IO
            {
                /**
                * 处理Windows路径
                */
                class KPath
                {
                public:					
					/**
					* @brief   获取当前Exe所在的目录路径
					* @return  目录路径，最后没有斜杠
					* @note	   废弃，请使用KModule::GetCurrentModuleDirectory()。
					*		   之后找个时间来删除掉这个函数及批量修改用到这个函数的工程
					*/
					static wstring GetCurrentModuleFileDir();



					/**
					 * @brief 获取全路径
					 * @param const wstring & path
					 * @return std::wstring
					 * @note 
					 */
					static wstring GetFullPath(const wstring& path);

                    /**
                    * @brief   判断路径是否存在
                    */
                    static bool  Exists(const wstring& path);

                    /**
                    * @brief   合并路径，
                    * @param   pathDir  绝对路径
                    * @param   pathFile 相对路径
                    * @return  std::wstring  合并后的路径
                    * @note
                    *      该函数也可以执行追加路径的功能，如c:\\abc , abc.txt 执行后是c:\\abc\\abc.txt
                    * @example
                    *       wstring path = L"c:\\abc\\uuu";
                    *       wstring pathMore = L"..\\g.txt";
                    *       path = KPath::Combine(path, pathMore);
                    *       // path ="c:\\abc\\g.txt"
                    */
                    static wstring Combine(const wstring& pathDir, const wstring& pathFile);

                    /**
                    * @brief   移除路径最后的反斜杠
                    * @param   path 路径
                    * @return  如果路径最后有反斜杠，那就返回移除反斜杠的路径；如果没有，则返回路径。
                    * @example
                    *      wstring path = L"c:\\abc\\";
                    *      wstring actual = L"";
                    *      actual = KPath::RemoveLastBackslash(path);
                    *      // actual = L"c:\\abc"
                    */
                    static wstring RemoveLastBackslash(const wstring& path);

					/**
					 * @brief 获取目录路径
					 * @param const wstring & path
					 * @return std::wstring
					 * @note 
					 */
					static wstring GetDirectoryName(const wstring& path);

					/**
					 * @brief 获取文件名
					 * @param path
					 * @return std::wstring
					 * @note 
					 */
					static wstring GetFileName(const wstring& path);


					/*
					* @brief 获取文件的扩展名
					* @param  path
					* @return 
					* @note 
					*/
					static wstring GetExtension(const wstring& path);


                    /**
                    * @brief   获取文件名，没有扩展名
                    * @param   path 路径， 如：c:\example.txt
                    * @return  返回文件名，没有扩展名。 如：example
                    */
                    static wstring GetFileNameWithoutExtension(const wstring& path);

                };
            }
		}
	}
}