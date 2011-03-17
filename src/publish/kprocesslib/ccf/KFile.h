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
				class KFile
				{
				private:
					WCHAR szFileName[MAX_PATH];

					/**
					* @brief 获取文件的句柄
					* @[in] param const wstring& filename	要获取句柄的文件名
					* @return HANDLE	文件句柄，失败返回INVALID_HANDLE_VALUE
					* @note:	返回的句柄需用CloseHandle释放。
					*/
					static HANDLE GetFileHandle(const wstring& filename);
				public:
					KFile(LPCWSTR szFileName);
					WCHAR* DigitalSignature() const;

					
					/**
					 * @brief 获取文件的主版本号和次版本号
					 * @param fileName 文件路径
					 * @param DWORD * pdwFileVersionMS 主版本号
					 * @param DWORD * pdwFileVersionLS 次版本号
					 * @return bool
					 * @note 
					 */
					static bool GetFileVersion(const wstring& fileName, DWORD *pdwFileVersionMS, DWORD *pdwFileVersionLS);

					
					/**
					 * @brief 获取文件的版本号字符串，格式为：2009.06.20.02
					 * @param fileName 文件路径
					 * @param fileVersion 版本号字符串
					 * @return bool
					 * @note 如果格式不是xxxx.xx.xx.xx，请调用
					 *       GetFileVersion(const wstring& fileName, 
					 *                      DWORD *pdwFileVersionMS, 
					 *                      DWORD *pdwFileVersionLS);
					 *       然后再自行拼接。
					 */
					static bool GetFileVersion(const wstring& fileName, wstring& fileVersion);
					
					/**
					 * @brief 文件是否存在
					 * @param const wstring & path
					 * @return bool
					 * @note 
					 */
					static bool Exists(const wstring& path);

					/**
					 * @brief 拷贝文件
					 * @param const wstring & sourceFileName
					 * @param const wstring & destFileName
					 * @param bool overwrite
					 * @return bool
					 * @note 
					 */
					static bool Copy(const wstring& sourceFileName, const wstring& destFileName, bool overwrite = true);

					
					/**
					 * @brief 删除文件
					 * @param const wstring & path
					 * @return bool
					 * @note 当传入的文件不存在时，也返回true
					 */
					static bool Delete(const wstring& path);

                    /**
                    * @brief 重命名文件
                    * @param oldPath  旧文件路径
                    * @param newPath  新文件路径
                    * @param replaceWhenExist  当新文件路径存在是，是否删除，默认否，返回false
                    * @return bool
                    * @note 当传入的文件不存在时，返回false
                    * @example
                             Rename(L"c:\\test.txt", L"c:\\test2.txt");
                    */
                    static bool Rename(const wstring& oldPath, const wstring& newPath, bool replaceWhenExist = false);

                    /**
                    * @brief   传建一个文本文件
                    * @param   filePath 文件路径
                    * @param   fileContent 文件内容
                    * @param   bool isUTF8 是否用UTF8编码， 默认true
                    * @param   bool isUseBom 是否添加UTF8 Bom，默认true
                    * @example
                              CreateTextFile(L"c:\\test.txt", L"Kingsoft");
                    */
                    static bool CreateTextFile(const wstring& filePath, const wstring& fileContent, bool isUTF8 = true, bool isUseBom = true);

					/**
					 * @brief 读一个文本文件的所有内容
					 * @param const wstring & path 文件路径
					 * @param bool isUTF8 是否用UTF8编码， 默认true
					 * @return std::wstring 文件的内容
					 * @note 
					 */
					static wstring ReadAllText(const wstring& path, bool isUTF8 = true);

					static wstring ReadAllTextUTF16(const wstring& path);

					/**
					* @brief 得到文件的创建时间
					* @[in] param const wstring& path 文件路径
					* @[out] param LPFILETIME lpCreateTime 输出文件的创建时间
					* @return 成功返回true, 失败返回false
					*/
					static bool GetCreateTime(const wstring& path, LPFILETIME lpCreateTime);

					/**
					* @brief 得到文件的最后访问时间
					* @[in] param const wstring& path 文件路径
					* @[out] param LPFILETIME lpCreateTime 输出文件的最后访问时间
					* @return 成功返回true, 失败返回false
					*/
					static bool GetLastAccessTime(const wstring& path, LPFILETIME lpLastAccessTime);

					/**
					* @brief 得到文件的最后修改时间
					* @[in] param const wstring& path 文件路径
					* @[out] param LPFILETIME lpCreateTime 输出文件的最后修改时间
					* @return 成功返回true, 失败返回false
					*/
					static bool GetLastWriteTime(const wstring& path, LPFILETIME lpLastWriteTime);

                    /**
                    * @brief   获取文件大小
                    * @param   const wstring & path 文件4g路径
                    * @param   LARGE_INTEGER & fizeSize 文件大小
                    * @return  bool
                    * @note    支持4G以上文件
                    */
                    static bool GetFileSize(const wstring& path, LARGE_INTEGER& fileSize);
				};
			}
		}
	}
}
