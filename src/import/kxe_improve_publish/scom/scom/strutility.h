/********************************************************************
* CreatedOn: 2005-9-1   11:33
* FileName: StrUtility.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef  STRUTILITY_H
#define	 STRUTILITY_H
#include <string>
#include <algorithm>

//不区分大小写的字符串相等比较
inline bool nocaseEqual(char c1, char c2)
{
	return toupper(c1) == toupper(c2);
}
inline bool strNocaseEqual(const std::string &str1, const std::string &str2)
{
	if (str1.size() == str2.size() &&
		std::equal(str1.begin(),str1.end(),
		str2.begin(),nocaseEqual))
	{
		return true;
	}
	return false;
}
//不区分大小写的字符串小于比较
class KStringNoCaseCmp
{
	public:
	static  bool nocaseCompare(char c1, char c2)
	{
		return toupper(c1) < toupper(c2);
	}
	static bool strNocaseCompare(const std::string &str1, const std::string &str2)
	{
		return std::lexicographical_compare(
			str1.begin(), str1.end(),
			str2.begin(), str2.end(),
			nocaseCompare);
	}
	bool operator()(const std::string &str1, const std::string &str2) const
	{
		return strNocaseCompare(str1, str2);
	}
};

#ifdef WIN32
inline bool stringEqual(const std::string &str1, const std::string &str2)
{
    return strNocaseEqual(str1, str2);
}
#else
inline bool stringEqual(const std::string &str1, const std::string &str2)
{
	return (str1 == str2);
}
#endif





#endif //STRUTILITY_H