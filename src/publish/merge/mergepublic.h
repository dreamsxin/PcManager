#ifndef __public_h__
#define __public_h__

#include <stdio.h>
#include <Windows.h>

#define MIN(x,y) (((x)<(y)) ? (x) : (y))

typedef unsigned char u_char;
typedef long pid_t;
typedef signed int ssize_t;

extern wchar_t* pszLogFileName;
extern bool	    blogfile;

#define errx(str) {logtofileW(str); goto ERROR_EXIT;};

void logtofileW(
	const wchar_t* szLogMsg
	);

void enablelog(bool benable);

#endif
