/**
* @file     KDirectory.h
* @brief    提供文件夹拷贝功能
* @details  
* @author   林杰(Linjie@kingsoft.com)
* @date     2009-6-17
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
                // 搜索选项
                enum SearchOption
                {
                    // Includes only the current directory in a search.
                    TopDirectoryOnly,

                    // Includes the current directory and all the subdirectories in a search operation.
                    AllDirectories
                };

                class KDirectory
                {
                public:
                    /**
                    * @brief   实现目录（含子目录）拷贝功能
                    * @param   srcDirName      源目录,可以有最后的\，也可以没有
                    * @param   destDirName  目标目录,可以有最后的\，也可以没有
                    * @return  bool
                    * @note 如果目标目录已存在会覆盖，源目录不存在返回false，
                    *       复制过程中有文件失败返回false.
                            若源目录 或目标目录为空，则返回false
                    */
                    static bool Copy(const wstring& srcDirName, const wstring& destDirName);

                    
					
					/**
					 * @brief 实现删除目录（含子目录）功能
					 * @param wstring dirPath 要删除的目录路径
					 * @param bool deleteRoot 是否删除顶层目录，
					 *		  如果为true，将删除整个目录，(默认)
					 *		  如果为false，则删除该目录下的所有文件和子文件夹。
					 * @return bool
					 * @note 若dirPath为空，则返回false
					 */
					static bool Delete(const wstring& dirPath, bool deleteRoot = true);

					
					/**
					 * @brief 判断目录是否存在
					 * @param wstring dirPath
					 * @return bool
					 * @note 若dirPath为空，则返回false
					 */
					static bool Exists(const wstring& dirPath);


					/**
					 * @brief 获取当前路径
					 * @return std::wstring
					 * @note 
					 */
					static wstring GetCurrentDirectory();

					
					/**
					 * @brief 设置当前路径
					 * @param const wstring & path
					 * @return bool
					 * @note 若path为空，则返回false
					 */
					static bool SetCurrentDirectory(const wstring& path);

                    /**
                    * @brief   获取指定目录下所有文件
                    * @param   path 目录路径
                    * @param   option  搜索选项, 默认为TopDirectoryOnly
                    * @return  vector<wstring>  所有文件路径
                    * @note    若path为空，则返回false
                    */
                    static vector<wstring> GetFiles(const wstring& path, SearchOption option = TopDirectoryOnly, wstring fileNamePattern = L"*.*");

                    /**
                    * @brief   获取指定目录下所有目录
                    * @param   path   目录路径
                    * @param   option 搜索选项, 默认为TopDirectoryOnly
                    * @return  vector<wstring> 所有目录路径
                    * @note    若path为空，则返回false
                    */
                    static vector<wstring> GetDirectories(const wstring& path, SearchOption option = TopDirectoryOnly);


                    /**
                    * @brief   Creates all directories and subdirectories as specified by path.
                    * @param   path The directory path to create. 
                    * @return  bool
                    * @note    若path为空，则返回false
                    */
                    static bool CreateDirectory(const wstring& path);

                    /**
                    * @brief   重命名目录
                    * @param   oldPath 旧目录路径
                    * @param   newPath 新目录路径
                    * @param   bool deleteWhenNewPathExist  当新目录存在时，是否删除
                    * @return  bool
                    * @note    如果是新目录与旧目录不在一个父目录下，就会执行移动到新路径下。
                    *          若path为空，则返回false
                    * @example
                               Rename(L"c:\\test", L"c:\\test2");
                               Rename(L"c:\\test", L"d:\\test2", false);
                    */
                    static bool Rename(const wstring& oldPath, const wstring& newPath, bool deleteWhenNewPathExist = false);
                };
            }
		}
	}
}
