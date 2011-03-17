//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   IBkWhileList.h
//  Creator     :   chenguicheng
//  Date        :   2010-07-31
//  Comment     :   kwsplugin.dll 使用卫士 白名单操作回调
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef	_I_BKWHILELIST_2010_9_7_H
#define	_I_BKWHILELIST_2010_9_7_H 

interface IBKWhileList
{
	virtual BOOL AddBKWhileList( LPCTSTR pszFilePath ) = 0;
	virtual BOOL DelBKWhileList( LPCTSTR pszFilePath ) = 0;
};

#endif	// #ifndef _I_BKWHILELIST_2010_9_7_H 