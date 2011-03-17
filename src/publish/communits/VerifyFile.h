//
// dummyz@126.com

#ifndef _VERIFY_FILE_INC_
#define _VERIFY_FILE_INC_

HANDLE  AcquireVerifyHandle();
void	ReleaseVerifyHandle(HANDLE hHandle);

// 返回 0 失败
// 返回 1 通过 win 签名
// 返回 2 通过 cat 签名 
int		VerifyFile(HANDLE hHandle, LPCTSTR lpPath);
BOOL	IsVerifiedFile(LPCTSTR lpPath);

#endif /* _VERIFY_FILE_INC_ */
