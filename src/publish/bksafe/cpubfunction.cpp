#include "StdAfx.h"
#include "cpubfunction.h"
using namespace PUB_FUNC;

CPubFunction::CPubFunction(void)
{
}

CPubFunction::~CPubFunction(void)
{
}

BOOL CPubFunction::IsNum(TCHAR ch)
{
	BOOL bRet = FALSE;
	if (ch >= L'0' && ch <= L'9')
		bRet = TRUE;

	return bRet;
}

int CPubFunction::ParseHostItem(CString strHostItem, CString& strIP, CString& strName)
{
	int nRet = -1;
	if (TRUE == strHostItem.IsEmpty())
		return nRet;

	CString ip, name;
	int nCount = strHostItem.GetLength();

	for (int i = 0; i < nCount; i++)
	{
		if (TRUE == IsNum(strHostItem[i]) ||
			L'.' == strHostItem[i])
		{
			strIP += strHostItem[i];
		}
		else
		{
			break;
		}
	}
	if (FALSE == strIP.IsEmpty())
	{
		strName = strHostItem;
		strName.Replace(strIP, L"");
		strName.Replace(L"\t", L"");
		strName.Replace(L" ", L"");
		strName.TrimLeft();
		nRet = 0;
	}

	return nRet;
}
