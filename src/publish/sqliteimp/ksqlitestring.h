////////////////////////////////////////////////////////////////
//
//	Filename: 	ksqlitestring.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-8-2  17: 52
//	Comment:	
//
///////////////////////////////////////////////////////////////
#pragma once

#include <stdlib.h>
#include <string>

class KSqliteString: public std::string
{
public:
	KSqliteString(const char *pszStr):std::string(pszStr == NULL ? "" : pszStr)
	{}

	operator __int64()
	{
		//return static_cast<__int64>(atoi(c_str()));
		return _strtoi64(c_str(), NULL, 10);
	}

	operator unsigned  __int64()
	{
		//return static_cast<unsigned __int64>(strtoul(c_str(),NULL, 10));
		return _strtoui64(c_str(),NULL, 10);
	}

	operator const char *()
	{
		return c_str();
	}
};