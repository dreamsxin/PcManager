#pragma once

//////////////////////////////////////////////////////////////////////////
// Notify Messages For BkWin User

#define MSG_BK_NOTIFY(ctrlid) NOTIFY_ID_HANDLER_EX(ctrlid, OnBkNotify_##ctrlid)

#define BK_NOTIFY_MAP(ctrlid)                   \
    LRESULT OnBkNotify_##ctrlid(LPNMHDR pnmh)   \
    {                                           \
        UINT_PTR  uCode = pnmh->code;           \

#define BK_NOTIFY_MAP_END()                     \
        SetMsgHandled(FALSE);                   \
        return FALSE;                           \
    }                                           \

#define BK_CHAIN_NOTIFY_MAP(ChainClass, ctrlid)                 \
        if(ChainClass::OnBkNotify_##ctrlid(pnmh))               \
            return TRUE;                                        \

// Command Notify
#define BKNM_COMMAND  1

// void OnBkItemCommand()
#define BK_NOTIFY_ID_COMMAND(itemid, func)                                  \
    if (BKNM_COMMAND == uCode && itemid == ((LPBKNMCOMMAND)pnmh)->uItemID)  \
    {                                                                       \
        func();                                                             \
        return TRUE;                                                        \
    }                                                                       \

#define BK_NOTIFY_ID_COMMAND_EX(itemidbegin, itemidend, func)					\
	if (BKNM_COMMAND == uCode && itemidbegin <= ((LPBKNMCOMMAND)pnmh)->uItemID	\
		&& itemidend >= ((LPBKNMCOMMAND)pnmh)->uItemID )						\
	{																			\
		func(((LPBKNMCOMMAND)pnmh)->uItemID);                                   \
		return TRUE;															\
	}																			\
// void OnBkCommand(UINT uItemID, LPCSTR szItemClass)
#define BK_NOTIFY_COMMAND(func)                                                     \
    if (BKNM_COMMAND == uCode)                                                      \
    {                                                                               \
        func(((LPBKNMCOMMAND)pnmh)->uItemID, ((LPBKNMCOMMAND)pnmh)->szItemClass);   \
        return TRUE;                                                                \
    }                                                                               \

typedef struct _BKNMCOMMAND
{
    NMHDR       hdr;
    UINT        uItemID;
    LPCSTR      szItemClass;
} BKNMCOMMAND, *LPBKNMCOMMAND;

// Real Window Resized Notify
#define BKNM_REALWND_RESIZED 2

// void OnBkRealWndResized(CRect rcWnd)
#define BK_NOTIFY_REALWND_RESIZED(itemid, func)                                             \
    if (BKNM_REALWND_RESIZED == uCode && itemid == ((LPBKNMREALWNDRESIZED)pnmh)->uItemID)   \
    {                                                                                       \
        func(((LPBKNMREALWNDRESIZED)pnmh)->rcWnd);                                          \
        return TRUE;                                                                        \
    }                                                                                       \

typedef struct _BKNMREALWNDRESIZED
{
    NMHDR       hdr;
    UINT        uItemID;
    CRect       rcWnd;
} BKNMREALWNDRESIZED, *LPBKNMREALWNDRESIZED;

// Mouse Hover Notify
// void OnBkMouseHover()
#define BKNM_MOUSEHOVER 3

#define BK_NOTIFY_MOUSEHOVER(func)                                          \
    if (BKNM_MOUSEHOVER == uCode)                                           \
    {                                                                       \
        func();                                                             \
        return TRUE;                                                        \
    }                                                                       \

// Mouse Leave Notify
// void OnBkMouseLeave()
#define BKNM_MOUSELEAVE 4

#define BK_NOTIFY_MOUSELEAVE(func)                                          \
    if (BKNM_MOUSELEAVE == uCode)                                           \
    {                                                                       \
        func();                                                             \
        return TRUE;                                                        \
    }                                                                       \

// Tab Sel Change Notify
#define BKNM_TAB_SELCHANGE  5

typedef struct _BKNMTABSELCHANGE
{
    NMHDR       hdr;
    UINT        uTabID;
    UINT        uTabItemIDNew;
    UINT        uTabItemIDOld;
    BOOL        bCancel;
} BKNMTABSELCHANGE, *LPBKNMTABSELCHANGE;

// BOOL OnBkTabSelChange(int nTabItemIDOld, int nTabItemIDNew)
#define BK_NOTIFY_TAB_SELCHANGE(tabid, func)                                        \
    if (BKNM_TAB_SELCHANGE == uCode && tabid == ((LPBKNMTABSELCHANGE)pnmh)->uTabID) \
    {                                                                               \
        BOOL bRet = func(                                                           \
            ((LPBKNMTABSELCHANGE)pnmh)->uTabItemIDOld,                              \
            ((LPBKNMTABSELCHANGE)pnmh)->uTabItemIDNew);                             \
        if (!bRet)                                                                  \
            ((LPBKNMTABSELCHANGE)pnmh)->bCancel = TRUE;                             \
        return TRUE;                                                                \
    }                                                                               \

