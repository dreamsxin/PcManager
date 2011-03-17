#pragma once
#include <string>

class KOpKsafeReg
{
public:

	static std::wstring ReplaceRegStr(IN LPCWSTR lpStr);

	//创建卫士的注册表项
	static BOOL CreateKsafeReg(LPCWSTR lpPath);

	//查询ksafe的相关键值，但是不需要传入SOFTWARE\\KSafe
	//如，要查SOFTWARE\\KSafe\\Coop 下的PreOem的值，可以如下调用：
	//std::wstring strValue;   QueryKsafePath(_T("Coop"), _T("PreOem"), strValue);
	static BOOL QueryKsafePath(std::wstring strKey, std::wstring strValueKey, OUT std::wstring& strValue);
	static BOOL QueryKsafePath(std::wstring strKey, std::wstring strValueKey, OUT DWORD& dwValue);
	static BOOL QueryKsafePath(std::wstring strKey, std::wstring strValueKey, OUT std::string& strValue);

private:
	static std::wstring _GetSubKeyStr(void);
};
