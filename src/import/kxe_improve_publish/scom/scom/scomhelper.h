//=============================================================================
/**
* @file scomhelper.h
* @brief 
* @author qiuruifeng <qiuruifeng@kingsoft.com>
* @date 2008-6-5   10:18
*/
//=============================================================================
#ifndef SCOMHELPER_H
#define SCOMHELPER_H

#include "SCOMDef.h"
#include <stdio.h>
#include <iosfwd>
#include <iostream>
#include <iomanip>

inline std::ostream& operator<<(std::ostream& os, RGUID rguid)
{
	os	<< std::hex << std::uppercase << std::setfill('0') 
		<< std::setw(8) << rguid.Data1 << "-" 
		<< std::setw(4) << rguid.Data2 << "-" 
		<< std::setw(4) << rguid.Data3 << "-" 
		<< std::setw(2) << (unsigned short)rguid.Data4[0] 
	<< std::setw(2) << (unsigned short)rguid.Data4[1] << "-";
	for (int i = 2; i < 8; ++i)
		os << std::setw(2) << (unsigned short)rguid.Data4[i];
	std::dec(os);
	return os;
}

inline std::istream& operator>>(std::istream& is, KSGUID& guid)
{
	char buffer[37] = { 0 };
	
//#if _MSC_VER >= 1400
//	is._Readsome_s(buffer, 36, 36);
//#else
	is.readsome(buffer, 36);
//#endif
	char* pszStoped;
	guid.Data1 = strtoul(buffer, &pszStoped, 16);
	assert(pszStoped == buffer + 8);
	guid.Data2 = (unsigned short)strtoul(&buffer[9], &pszStoped, 16);
	assert(pszStoped == buffer + 13);
	guid.Data3 = (unsigned short)strtoul(&buffer[14], &pszStoped, 16);
	assert(pszStoped == buffer + 18);
	unsigned short usTemp = (unsigned short)strtoul(&buffer[19], &pszStoped, 16);
	assert(pszStoped == buffer + 23);
	guid.Data4[1] = usTemp & 0xff;
	guid.Data4[0] = usTemp >> 8;

	// 处理KSGUID的最后12位
	buffer[2] = '\0';
	for (int i = 24; i < 36; i += 2)
	{
//#if _MSC_VER >= 1400
//		strncpy_s(buffer, 3, &buffer[i], 2);
//#else
		strncpy(buffer, &buffer[i], 2);
//#endif
		guid.Data4[(i - 24) / 2 + 2] = (unsigned char)strtoul(buffer, &pszStoped, 16);
		assert(pszStoped == buffer + 2);
	}
	return is;
}

inline std::wostream& operator<<(std::wostream& os, RGUID rguid)
{
	os << std::hex << std::uppercase << std::setfill(L'0') 
		<< std::setw(8) << rguid.Data1 << L"-" 
		<< std::setw(4) << (unsigned long)rguid.Data2 << L"-" 
		<< std::setw(4) << (unsigned long)rguid.Data3 << L"-" 
		<< std::setw(2) << (unsigned long)rguid.Data4[0] 
	<< std::setw(2) << (unsigned long)rguid.Data4[1] << L"-";
	for (int i = 2; i < 8; ++i)
		os << std::setw(2) << (unsigned long)rguid.Data4[i];
	std::dec(os);
	return os;
}

inline std::wistream& operator>>(std::wistream& is, KSGUID& guid)
{
	wchar_t buffer[37] = { 0 };	
//#if _MSC_VER >= 1400
//	is._Readsome_s(buffer, 36, 36);
//#else
	is.readsome(buffer, 36);
//#endif
	wchar_t* pszStoped;
	guid.Data1 = wcstoul(buffer, &pszStoped, 16);
	assert(pszStoped == buffer + 8);
	guid.Data2 = (unsigned short)wcstoul(&buffer[9], &pszStoped, 16);
	assert(pszStoped == buffer + 13);
	guid.Data3 = (unsigned short)wcstoul(&buffer[14], &pszStoped, 16);
	assert(pszStoped == buffer + 18);
	unsigned short usTemp = (unsigned short)wcstoul(&buffer[19], &pszStoped, 16);
	assert(pszStoped == buffer + 23);
	guid.Data4[1] = usTemp & 0xff;
	guid.Data4[0] = usTemp >> 8;

	// 处理KSGUID的最后12位
	buffer[2] = '\0';
	for (int i = 24; i < 36; i += 2)
	{
//#if _MSC_VER >= 1400
//		wcsncpy_s(buffer, 3, &buffer[i], 2);
//#else
		wcsncpy(buffer, &buffer[i], 2);
//#endif
		guid.Data4[(i - 24) / 2 + 2] = (unsigned char)wcstoul(buffer, &pszStoped, 16);
		assert(pszStoped == buffer + 2);
	}
	return is;
}


#endif //SCOMHELPER_H