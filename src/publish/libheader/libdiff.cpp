#include "stdafx.h"
#include "libdiff.h"
#include <atlfile.h>
#include <stlsoft/memory/auto_buffer.hpp>
using namespace stlsoft;
#include <libheader/libheader.h>
#include <open_vcdiff/inc/vcdecoder.h>
using namespace open_vcdiff;

namespace
{

class Output2File2 : public OutputStringInterface
{
public:
	Output2File2(CAtlFile &file) : _file(file), _totalBytes(0) { }

	virtual OutputStringInterface& append(const char* s, size_t n)
	{ _totalBytes += n; _file.Write(s, n); return *this; }

	virtual void clear() {}

	virtual void push_back(char c) 
	{ _totalBytes += 1; _file.Write(&c, 1); }

	// 总共需要保留的字节数 = 当前接收的字节数 + 通知保留的字节数
	virtual void ReserveAdditionalBytes(size_t res_arg) {}

	virtual size_t size() const 
	{ ULONGLONG pos = 0; _file.GetPosition(pos); return static_cast<size_t>(pos); }

	size_t GetTotalBytes() const { return _totalBytes; }

private:
	size_t _totalBytes;
	CAtlFile &_file;
};


}
//////////////////////////////////////////////////////////////////////////
HRESULT LibDiffCombine(LPCWSTR pOldLibPath, LPCWSTR pDiffLibPath, LPCWSTR pNewLibPath)
{
	// 加载Delta
	BkDatLibContent delta;
	CDataFileLoader	loader;
	if(!loader.GetLibDatContent(pDiffLibPath, delta)) return ::HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);

	// 加载字典文件
	CAtlFile dictFile;
	HRESULT hr = dictFile.Create(pOldLibPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
	if(!SUCCEEDED(hr)) return hr;

	ULONGLONG dictSize;
	hr = dictFile.GetSize(dictSize);
	if(!SUCCEEDED(hr)) return hr;

	auto_buffer<char> dict(static_cast<size_t>(dictSize));
	if(dict.empty()) return ::HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);

	DWORD bytesRead;
	hr = dictFile.Read(dict.data(), static_cast<DWORD>(dict.size()), bytesRead);
	if(!SUCCEEDED(hr)) return hr;

	dictFile.Close();

	// 创建目标文件
	CAtlFile targetFile;
	hr = targetFile.Create(pNewLibPath, GENERIC_READ | GENERIC_WRITE, 0, CREATE_ALWAYS);
	if(!SUCCEEDED(hr)) return hr;

	//
	// 开始合并
	//
	//@Note
	// Dict(Source) + Delta => Target
	//
	Output2File2 output2File(targetFile);
	{
		VCDiffStreamingDecoder decoder;
		decoder.StartDecoding(&dict[0], dict.size());

		size_t beg = 0;
		size_t end = static_cast<size_t>(delta.nLen);
		LPCSTR pDelta = reinterpret_cast<LPCSTR>(delta.pBuffer);

		while(beg < end)
		{
			static const size_t MAX_THUNK_SIZE = 16*1024;

			size_t size = end - beg;
			if(size > MAX_THUNK_SIZE) size = MAX_THUNK_SIZE;

			if(!decoder.DecodeChunkToInterface(pDelta + beg, size, &output2File))
				return ::HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);

			beg += size;
		}

		if(!decoder.FinishDecoding())
			return ::HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}

	return S_OK;
}