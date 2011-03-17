#include "stdafx.h"

#include "KModule.h"
#include "KPath.h"

using namespace Kingsoft::Test::CCF::System;
using namespace Kingsoft::Test::CCF::IO;

wstring KModule::GetCurrentModuleDirectory()
{
	wstring filePath = KModule::GetCurrentModuleFilePath();

	return KPath::GetDirectoryName(filePath);
}

wstring KModule::GetCurrentModuleFileName()
{
	wstring filePath = KModule::GetCurrentModuleFilePath();

	return KPath::GetFileName(filePath);
}

wstring KModule::GetCurrentModuleFilePath()
{
	DWORD ret;
	WCHAR filePath[MAX_PATH] = {0};

	ret = ::GetModuleFileName(NULL, filePath, MAX_PATH);

	if (ret == 0)
	{
		return L"";
	}
	
	return filePath;
}