//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KOSVersion.h
//  Version     :   1.0
//  Creater     :   YangXD(yxdh)
//  Date        :   2008-4-15 14:53:56
//  Comment     :   Interface for the OS_VER namespace.
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _K_OS_VERSION_H_
#define _K_OS_VERSION_H_
#pragma once


// 只有一个名空间。
namespace OS_VER
{
	bool IsNTPlatform();
	bool IsWinXPOrLater();
	bool IsWin2KOrLater();
	bool _CheckNTVersion(unsigned long ulMajorVersion, unsigned long ulMinorVersion);
}


#endif // #ifndef _K_OS_VERSION_H_