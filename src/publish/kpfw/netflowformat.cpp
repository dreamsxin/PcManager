#include "stdafx.h"
#include "netflowformat.h"

void NetFlowToString( ULONGLONG netFlow, ATL::CString& str )
{
	
	// ULONGLONG 直接转float，double在Release开优化的情况下会出错，导致显示f
	// 所以自己实现一个转换
	static const wchar_t* szShowUnit[9] = {L"B", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB", L"ZB", L"YB"};
	
	if (0 == netFlow)
	{
		str = L"0KB";
		return;
	}

	int nUnitCur	= 0;
	ULONGLONG numBeforePoint = 0;
	double numAfterPoint = 0;
	
	do
	{
		numBeforePoint = netFlow / 1024;
		numAfterPoint = (netFlow % 1024) / 1024.0;
		netFlow /= 1024;
		nUnitCur ++;
	}while (numBeforePoint > 1024);

	if (nUnitCur < 9)
	{
		WCHAR szRet[20] = {0};
		// 如果numAfterPoint 大于0.1 则保留一位，否则保留2位小数

		if (numAfterPoint > 0.094)
		{
			// 乘以10表示精確到小数点后一位，乘以100则是2位，依此类推
			// +0.5f 是为了实现四舍五入。
			int integerNumAfterPoint = (int)(numAfterPoint*10 + 0.5f);
			if (integerNumAfterPoint >= 10)
			{
				// 避免出现2位小数
				numBeforePoint++;
				integerNumAfterPoint = 0;
			}

			swprintf_s(szRet, 20, L"%I64u.%01d", numBeforePoint, integerNumAfterPoint);
		}
		else
		{
			// 乘以10表示精確到小数点后一位，乘以100则是2位，依此类推
			// +0.5f 是为了实现四舍五入。
			int integerNumAfterPoint = (int)(numAfterPoint*100 + 0.5f);
			if (integerNumAfterPoint >= 100)
			{
				// 避免出现3位小数
				numBeforePoint++;
				integerNumAfterPoint = 0;
			}

			swprintf_s(szRet, 20, L"%I64u.%02d", numBeforePoint, integerNumAfterPoint);
		}

		str = szRet;
		str.Append(szShowUnit[nUnitCur]);
	}
}

void NetFlowToString2( ULONGLONG netFlow, ATL::CString& str )
{

	// ULONGLONG 直接转float，double在Release开优化的情况下会出错，导致显示f
	// 所以自己实现一个转换
	static const wchar_t* szShowUnit[9] = {L"B", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB", L"ZB", L"YB"};

	if (0 == netFlow)
	{
		str = L"0KB";
		return;
	}

	int nUnitCur	= 0;
	ULONGLONG numBeforePoint = 0;
	double numAfterPoint = 0;

	do
	{
		numBeforePoint = netFlow / 1024;
		numAfterPoint = (netFlow % 1024) / 1024.0;
		netFlow /= 1024;
		nUnitCur ++;
	}while (numBeforePoint > 1024);

	if (nUnitCur < 9)
	{
		WCHAR szRet[20] = {0};
		// 如果nUnitCur 大于2 则保留一位，否则不保留

		if (nUnitCur > 1)
		{
			// 乘以10表示精確到小数点后一位，乘以100则是2位，依此类推
			// +0.5f 是为了实现四舍五入。
			int integerNumAfterPoint = (int)(numAfterPoint*10 + 0.5f);
			if (integerNumAfterPoint >= 10)
			{
				// 避免出现2位小数
				numBeforePoint++;
				integerNumAfterPoint = 0;
			}

			swprintf_s(szRet, 20, L"%I64u.%01d", numBeforePoint, integerNumAfterPoint);
		}
		else
		{
			swprintf_s(szRet, 20, L"%I64u", numBeforePoint);
		}

		str = szRet;
		str.Append(szShowUnit[nUnitCur]);
	}
}