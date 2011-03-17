#pragma once

namespace PUB_FUNC
{
	class CPubFunction
	{
	public:
		static CPubFunction* GetPubFuncPtr()
		{
			static CPubFunction func;
			return &func;
		}
		CPubFunction(void);
		~CPubFunction(void);
	public:
		int ParseHostItem(CString strHostItem, CString& strIP, CString& strName);
		BOOL IsNum(TCHAR ch);
	};
}


