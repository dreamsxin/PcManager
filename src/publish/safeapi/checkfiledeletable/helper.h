#ifndef _HELPER_H_
#define _HELPER_H_
//-----------------------------------------------------------------------------//
//
// c:\windows\system32\userinit.exe, c:\windows\xxx.exe 这样的buffer, 分成对应的
// 一段段的buffer, 以逗号作为分隔, 并去掉逗号后面的空格
//
// plStringCount: 返回实际字符串个数
//
// Return:
//			ERROR_SUCCESS
//			ERROR_MORE_DATA
//
LONG
ParseSpecialBuffer1(
					IN OUT PWCHAR pwszStringBuffer,
					IN OUT PWCHAR aString[],
					IN OUT PLONG plStringCount
					);

LONG
ParseSpecialBuffer2_MultiString(
								IN OUT PVOID pMultiStringBuffer,
								IN LONG lBufferSize,
								IN OUT PWCHAR aString[],
								IN OUT PLONG plStringCount
								);

//
// is c:\a.txt like path
//
BOOL
IsFullPath (
			PWCHAR pwstrPath
			);

//
// simply add systemdirectory ahead
//
LONG
MakeFullPath (
			  IN  PWCHAR pwstrDest,
			  IN  LONG lDestSizeInWCHAR,
			  OUT PWCHAR pwstrSource,
			  IN  LONG lSourceSizeInWCHAR
			  );

BOOL
IsFileExist (
			 PWCHAR pwstrFilePath
			 );

BOOL
MatchPath (
		   PWCHAR pwszPath1,
		   PWCHAR pwszPath2
		   );
//-----------------------------------------------------------------------------//
#endif