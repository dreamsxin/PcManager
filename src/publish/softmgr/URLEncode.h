#ifndef __CURLENCODE_H_
#define __CURLENCODE_H_

class CURLEncode
{
public:
	CURLEncode() { };
	virtual ~CURLEncode() { };
	CString URLEncode(const wchar_t* lpURL);
	CString URLEncode2(const wchar_t* lpURL);
};

#endif //__CURLENCODE_H_