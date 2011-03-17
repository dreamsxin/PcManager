//////////////////////////////////////////////////////////////////////////
//   File Name: BkSkin
// Description: BkWindow Skin Definition
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkobject.h"
#include "bkimage.h"
#include <bkres/bkpngpool.h>

// State Define
enum {
    BkWndState_Normal       = 0x00000000UL, 
    BkWndState_Hover        = 0x00000001UL, 
    BkWndState_PushDown     = 0x00000002UL, 
    BkWndState_Check        = 0x00000004UL, 
    BkWndState_Invisible    = 0x00000008UL, 
    BkWndState_Disable      = 0x00000010UL, 
};

#define IIF_STATE2(the_state, normal_value, hover_value) \
    (((the_state) & BkWndState_Hover) ? (hover_value) : (normal_value))

#define IIF_STATE3(the_state, normal_value, hover_value, pushdown_value) \
    (((the_state) & BkWndState_PushDown) ? (pushdown_value) : IIF_STATE2(the_state, normal_value, hover_value))

#define IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value) \
    (((the_state) & BkWndState_Disable) ? (disable_value) : IIF_STATE3(the_state, normal_value, hover_value, pushdown_value))

class CBkSkinBase : public CBkObject
{
public:
    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState) = NULL;

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return TRUE;
    }

    enum {
        Frame_Part_All        = 0x0000002FUL, 
        Frame_Part_Top        = 0x00000001UL, 
        Frame_Part_Middle     = 0x00000002UL, 
        Frame_Part_Bottom     = 0x00000004UL, 
        Frame_Part_Left       = 0x00000008UL, 
        Frame_Part_Center     = 0x00000010UL, 
        Frame_Part_Right      = 0x00000020UL, 
    };

    static void HorzExtendDraw(CDCHandle &dc, CBkImage &imgDraw, CRect &rcDraw, LONG lSkinLeft, int nSubImage = -1)
    {
        SIZE sizeSkin;

        imgDraw.GetImageSize(sizeSkin);

        if (sizeSkin.cx > rcDraw.Width())
            rcDraw.right = rcDraw.left + sizeSkin.cx;

        if (imgDraw.GetSubImageWidth() > 0)
        {
            sizeSkin.cx = imgDraw.GetSubImageWidth();
        }

        imgDraw.BitBlt(
            dc, 
            rcDraw.left, rcDraw.top, 
            lSkinLeft, sizeSkin.cy, 
            0, 0, 
            SRCCOPY, nSubImage
            );
        imgDraw.BitBlt(
            dc, 
            rcDraw.right - sizeSkin.cx + lSkinLeft + 1, rcDraw.top, 
            sizeSkin.cx - lSkinLeft - 1, sizeSkin.cy, 
            lSkinLeft + 1, 0, 
            SRCCOPY, nSubImage
            );
        imgDraw.StretchBlt(
            dc, 
            rcDraw.left + lSkinLeft, rcDraw.top, 
            rcDraw.Width() - sizeSkin.cx + 1, sizeSkin.cy, 
            lSkinLeft, 0, 
            1, sizeSkin.cy, 
            SRCCOPY, nSubImage
            );
    }

    static void FrameDraw(CDCHandle &dc, CBkImage &imgDraw, CRect &rcDraw, LONG lSkinLeft, LONG lSkinTop, COLORREF crBg, UINT uDrawPart = Frame_Part_All)
    {
        SIZE sizeSkin;
        CRect rcClient = rcDraw;

        ATLASSERT(dc.m_hDC);
        ATLASSERT(imgDraw.m_hBitmap);
        ATLASSERT(rcDraw.Width() && rcDraw.Height());

        imgDraw.GetImageSize(sizeSkin);

        rcClient.DeflateRect(
            (uDrawPart & Frame_Part_Left)   ? lSkinLeft                     : 0, 
            (uDrawPart & Frame_Part_Top)    ? lSkinTop                      : 0, 
            (uDrawPart & Frame_Part_Right)  ? (sizeSkin.cx - lSkinLeft - 1) : 0, 
            (uDrawPart & Frame_Part_Bottom) ? (sizeSkin.cy - lSkinTop - 1)  : 0 
            );

        if ((Frame_Part_Left | Frame_Part_Top) == (uDrawPart & (Frame_Part_Left | Frame_Part_Top)))
        {
            imgDraw.BitBlt(
                dc, 
                rcDraw.left, rcDraw.top, 
                lSkinLeft, lSkinTop, 
                0, 0, 
                SRCCOPY
                );
        }
        if ((Frame_Part_Right | Frame_Part_Top) == (uDrawPart & (Frame_Part_Right | Frame_Part_Top)))
        {
            imgDraw.BitBlt(
                dc, 
                rcClient.right, rcDraw.top, 
                sizeSkin.cx - lSkinLeft - 1, lSkinTop, 
                lSkinLeft + 1, 0, 
                SRCCOPY
                );
        }
        if ((Frame_Part_Left | Frame_Part_Bottom) == (uDrawPart & (Frame_Part_Left | Frame_Part_Bottom)))
        {
            imgDraw.BitBlt(
                dc, 
                rcDraw.left, rcClient.bottom, 
                lSkinLeft, lSkinTop, 
                0, lSkinTop + 1, 
                SRCCOPY
                );
        }
        if ((Frame_Part_Right | Frame_Part_Bottom) == (uDrawPart & (Frame_Part_Right | Frame_Part_Bottom)))
        {
            imgDraw.BitBlt(
                dc, 
                rcClient.right, rcClient.bottom, 
                sizeSkin.cx - lSkinLeft - 1, lSkinTop, 
                lSkinLeft + 1, lSkinTop + 1, 
                SRCCOPY
                );
        }
        if (Frame_Part_Top == (uDrawPart & Frame_Part_Top))
        {
            imgDraw.StretchBlt(
                dc, 
                rcClient.left, rcDraw.top, 
                rcClient.Width(), lSkinTop, 
                lSkinLeft, 0, 
                1, lSkinTop, 
                SRCCOPY
                );
        }
        if (Frame_Part_Left == (uDrawPart & Frame_Part_Left))
        {
            imgDraw.StretchBlt(
                dc, 
                rcDraw.left, rcClient.top, 
                lSkinLeft, rcClient.Height(), 
                0, lSkinTop, 
                lSkinLeft, 1, 
                SRCCOPY
                );
        }
        if (Frame_Part_Bottom == (uDrawPart & Frame_Part_Bottom))
        {
            imgDraw.StretchBlt(
                dc, 
                rcClient.left, rcDraw.bottom - sizeSkin.cy + lSkinTop + 1, 
                rcClient.Width(), sizeSkin.cy - lSkinTop - 1, 
                lSkinLeft, lSkinTop + 1, 
                1, sizeSkin.cy - lSkinTop - 1, 
                SRCCOPY
                );
        }
        if (Frame_Part_Right == (uDrawPart & Frame_Part_Right))
        {
            imgDraw.StretchBlt(
                dc, 
                rcClient.right, rcClient.top, 
                sizeSkin.cx - lSkinLeft - 1, rcClient.Height(), 
                lSkinLeft + 1, lSkinTop, 
                sizeSkin.cx - lSkinLeft - 1, 1, 
                SRCCOPY
                );
        }

        if (CLR_INVALID != crBg)
            dc.FillSolidRect(rcClient, crBg);
    }

    typedef struct _FRG_PARAM 
    {
        LONG lOffset;
        COLORREF crColor;
    } FRG_PARAM;

    typedef BOOL (WINAPI * FnGradientFill)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);

    static BOOL WINAPI GradientFill2(HDC hDC, PTRIVERTEX pVertices, DWORD nVertices, PVOID pMeshElements, ULONG nMeshElements, ULONG dwMode)
    {
        HMODULE hMod = ::LoadLibrary(L"msimg32.dll");
        if (hMod)
        {
            FnGradientFill pfnGradientFill = (FnGradientFill)::GetProcAddress(hMod, "GradientFill");
            if (pfnGradientFill)
                pfnGradientFill(hDC, pVertices, nVertices, pMeshElements, nMeshElements, dwMode);
            ::FreeLibrary(hMod);
        }

        return TRUE;
    }

    static void GradientFillRectV(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount)
    {
        GRADIENT_RECT gRect = {0, 1};
        TRIVERTEX vert[2] = {
            {rcFill.left, rcFill.top, 0, 0, 0, 0}, 
            {rcFill.right, rcFill.top, 0, 0, 0, 0}
        };
        int i = 0;

        for (i = 1; i < nCount && vert[0].y <= rcFill.bottom; i ++)
        {
            vert[0].y = vert[1].y;
            vert[1].y += params[i].lOffset;
            vert[0].Red     = GetRValue(params[i - 1].crColor) << 8;
            vert[0].Green   = GetGValue(params[i - 1].crColor) << 8;
            vert[0].Blue    = GetBValue(params[i - 1].crColor) << 8;
            vert[1].Red     = GetRValue(params[i].crColor) << 8;
            vert[1].Green   = GetGValue(params[i].crColor) << 8;
            vert[1].Blue    = GetBValue(params[i].crColor) << 8;

            HMODULE hMod = ::LoadLibrary(L"msimg32.dll");
            if (hMod)
            {
                GradientFill2(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
            }
        }
    }

    static void GradientFillRectH(HDC hdc, CRect &rcFill, FRG_PARAM params[], int nCount)
    {
        GRADIENT_RECT gRect = {0, 1};
        TRIVERTEX vert[2] = {
            {rcFill.left, rcFill.top, 0, 0, 0, 0}, 
            {rcFill.left, rcFill.bottom, 0, 0, 0, 0}
        };
        int i = 0;

        for (i = 1; i < nCount && vert[0].x <= rcFill.right; i ++)
        {
            vert[0].x = vert[1].x;
            vert[1].x += params[i].lOffset;
            vert[0].Red     = GetRValue(params[i - 1].crColor) << 8;
            vert[0].Green   = GetGValue(params[i - 1].crColor) << 8;
            vert[0].Blue    = GetBValue(params[i - 1].crColor) << 8;
            vert[1].Red     = GetRValue(params[i].crColor) << 8;
            vert[1].Green   = GetGValue(params[i].crColor) << 8;
            vert[1].Blue    = GetBValue(params[i].crColor) << 8;
            GradientFill2(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
        }
    }

    static void GradientFillRectV(HDC hdc, CRect &rcFill, COLORREF crTop, COLORREF crBottom)
    {
        FRG_PARAM frgDraw[2] = {
            {0, crTop}, 
            {rcFill.Height(), crBottom}, 
        };

        GradientFillRectV(hdc, rcFill, frgDraw, 2);
    }

    static void GradientFillRectH(HDC hdc, CRect &rcFill, COLORREF crLeft, COLORREF crRight)
    {
        FRG_PARAM frgDraw[2] = {
            {0, crLeft}, 
            {rcFill.Width(), crRight}, 
        };

        GradientFillRectH(hdc, rcFill, frgDraw, 2);
    }
};

class CBkImageSkin
    : public CBkImage
    , public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkImageSkin, "imglst")

public:
    CBkImageSkin()
    {

    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        CBkImage::Draw(dc, rcDraw.left, rcDraw.top, dwState);
    }

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};

        GetImageSize(ret);

        if (0 != m_lSubImageWidth)
            ret.cx = m_lSubImageWidth;

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return (0 == m_lSubImageWidth);
    }

protected:

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("src", *(CBkImage *)(this), TRUE)
        BKWIN_ENUM_ATTRIBUTE("mode", int, TRUE)
        BKWIN_ENUM_VALUE("none", CBkImage::ModeNone)
        BKWIN_ENUM_VALUE("mask", CBkImage::ModeMaskColor)
        BKWIN_ENUM_VALUE("alpha", CBkImage::ModeAlpha)
        BKWIN_ENUM_END(m_nTransparentMode)
        BKWIN_COLOR_ATTRIBUTE("maskcolor", m_crMask, TRUE)
        BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkSkinImgFrame : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinImgFrame, "imgframe")

public:
    CBkSkinImgFrame()
        : m_crBg(CLR_INVALID)
        , m_lSkinParamLeft(0)
        , m_lSkinParamTop(0)
        , m_uDrawPart(Frame_Part_All)
    {
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        if (m_imgSkin.M_HOBJECT)
        {
            FrameDraw(dc, m_imgSkin, rcDraw, m_lSkinParamLeft, m_lSkinParamTop, m_crBg, m_uDrawPart);
        }
    }

    virtual BOOL IgnoreState()
    {
        return m_imgSkin.IgnoreState();
    }

protected:
    CBkImageSkin m_imgSkin;
    LONG m_lSkinParamLeft;
    LONG m_lSkinParamTop;
    COLORREF m_crBg;
    UINT m_uDrawPart;

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_CHAIN_ATTRIBUTE(m_imgSkin, TRUE)
//         BKWIN_UINT_ATTRIBUTE("src", m_imgSkin, TRUE)
        BKWIN_COLOR_ATTRIBUTE("crbg", m_crBg, TRUE)
        BKWIN_INT_ATTRIBUTE("left", m_lSkinParamLeft, TRUE)
        BKWIN_INT_ATTRIBUTE("top", m_lSkinParamTop, TRUE)
        BKWIN_ENUM_ATTRIBUTE("part", UINT, TRUE)
            BKWIN_ENUM_VALUE("all", Frame_Part_All)
            BKWIN_ENUM_VALUE("top", (Frame_Part_All & ~Frame_Part_Bottom))
            BKWIN_ENUM_VALUE("middle", (Frame_Part_All & ~(Frame_Part_Bottom | Frame_Part_Top)))
            BKWIN_ENUM_VALUE("bottom", (Frame_Part_All & ~Frame_Part_Top))
            BKWIN_ENUM_VALUE("left", (Frame_Part_All & ~Frame_Part_Right))
            BKWIN_ENUM_VALUE("center", (Frame_Part_All & ~(Frame_Part_Right | Frame_Part_Left)))
            BKWIN_ENUM_VALUE("right", (Frame_Part_All & ~Frame_Part_Left))
        BKWIN_ENUM_END(m_uDrawPart)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
};

class CBkSkinImgHorzExtend : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinImgHorzExtend, "imghorzex")
public:
    CBkSkinImgHorzExtend()
        : /*
        m_lSkinSubWidth(0)
                , */
        m_lSkinParamLeft(0)
    {
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        if (m_imgSkin.M_HOBJECT)
        {
//             m_imgSkin.SetSubImageWidth(m_lSkinSubWidth);
            HorzExtendDraw(
                dc, m_imgSkin, rcDraw, m_lSkinParamLeft, 
                (-1 == dwState) ? -1 : IIF_STATE4(dwState, 0, 1, 2, 3)
//                (dwState & BkWndState_PushDown) ? 2 : ((dwState & BkWndState_Hover) ? 1 : 0)
                );
        }
    }

    SIZE GetSkinSize()
    {
        SIZE size;

        m_imgSkin.GetImageSize(size);

//         LONG lSubImageWidth = m_imgSkin.GetSubImageWidth();
//         if (lSubImageWidth > 0)
//             size.cx = lSubImageWidth;

        size.cx = 0;

        return size;
    }

    virtual BOOL IgnoreState()
    {
        return m_imgSkin.IgnoreState();
    }

protected:
    CBkImageSkin m_imgSkin;
//     LONG m_lSkinSubWidth;
    LONG m_lSkinParamLeft;

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_CHAIN_ATTRIBUTE(m_imgSkin, TRUE)
//         BKWIN_UINT_ATTRIBUTE("src", m_imgSkin, TRUE)
//         BKWIN_INT_ATTRIBUTE("subwidth", m_lSkinSubWidth, TRUE)
        BKWIN_INT_ATTRIBUTE("left", m_lSkinParamLeft, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
};

class CBkSkinButton : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinButton, "button")
public:
    CBkSkinButton()
        : m_crBorder(RGB(0x70, 0x70, 0x70))
        , m_crBg(RGB(0xEE, 0xEE, 0xEE))
        , m_crBgUpNormal(RGB(0xEE, 0xEE, 0xEE))
        , m_crBgUpHover(RGB(0xEE, 0xEE, 0xEE))
        , m_crBgUpPush(RGB(0xCE, 0xCE, 0xCE))
        , m_crBgDownNormal(RGB(0xD6, 0xD6, 0xD6))
        , m_crBgDownHover(RGB(0xE0, 0xE0, 0xE0))
        , m_crBgDownPush(RGB(0xC0, 0xC0, 0xC0))
    {

    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        CPen penFrame;
        CRect rcBg = rcDraw;

        dc.FillSolidRect(rcDraw, m_crBg);

        rcBg.DeflateRect(2, 2);

        if (BkWndState_Disable == (BkWndState_Disable & dwState))
        {
            
        }
        else
            GradientFillRectV(
                dc, rcBg, 
                IIF_STATE3(dwState, m_crBgUpNormal, m_crBgUpHover, m_crBgUpPush), 
                IIF_STATE3(dwState, m_crBgDownNormal, m_crBgDownHover, m_crBgDownPush));

//         rcBg.DeflateRect(2, 2, 2, rcDraw.Height() / 2);
//         dc.FillSolidRect(
//             rcBg, 
//             IIF_STATE3(dwState, m_crBgUpNormal, m_crBgUpHover, m_crBgUpPush)
//             );
// 
//         rcBg.OffsetRect(0, rcBg.Height());
//         dc.FillSolidRect(
//             rcBg, 
//             IIF_STATE3(dwState, m_crBgDownNormal, m_crBgDownHover, m_crBgDownPush)
//             );

        penFrame.CreatePen(
            PS_SOLID, 
            1, 
            m_crBorder
            );

        HPEN hpenOld = dc.SelectPen(penFrame);
        HBRUSH hbshOld = NULL, hbshNull = (HBRUSH)::GetStockObject(NULL_BRUSH);

        hbshOld = dc.SelectBrush(hbshNull);

        dc.Rectangle(rcDraw);

        //dc.RoundRect(rcDraw, CPoint(2, 2));

//         if (dwState & BkWndState_PushDown)
//         {
//             rcDraw.DeflateRect(1, 1, 0, 0);
// 
//             dc.Rectangle(rcDraw);
//         }

        dc.SelectBrush(hbshOld);
        dc.SelectPen(hpenOld);
    }

    virtual BOOL IgnoreState()
    {
        return FALSE;
    }

protected:

    COLORREF m_crBg;
    COLORREF m_crBorder;
    COLORREF m_crBgUpNormal;
    COLORREF m_crBgUpHover;
    COLORREF m_crBgUpPush;
    COLORREF m_crBgDownNormal;
    COLORREF m_crBgDownHover;
    COLORREF m_crBgDownPush;

public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_COLOR_ATTRIBUTE("bg", m_crBg, TRUE)
        BKWIN_COLOR_ATTRIBUTE("border", m_crBorder, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgup", m_crBgUpNormal, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bguphover", m_crBgUpHover, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bguppush", m_crBgUpPush, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgdown", m_crBgDownNormal, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgdownhover", m_crBgDownHover, TRUE)
        BKWIN_COLOR_ATTRIBUTE("bgdownpush", m_crBgDownPush, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

class CBkSkinGradation
    : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkSkinGradation, "gradation")
public:
    CBkSkinGradation()
        : m_uDirection(0)
        , m_crFrom(CLR_INVALID)
        , m_crTo(CLR_INVALID)
        , m_nSize(0)
    {
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        CRect rcGradation = rcDraw;
        CRgn rgnDraw;

        rgnDraw.CreateRectRgn(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

        dc.FillSolidRect(rcDraw, m_crTo);

        int nID = dc.SaveDC();

        dc.SelectClipRgn(rgnDraw);

        if (0 == m_uDirection)
        {
            if (0 < m_nSize)
                rcGradation.right = rcGradation.left + m_nSize;
            GradientFillRectH(dc, rcGradation, m_crFrom, m_crTo);
        }
        else
        {
            if (0 < m_nSize)
                rcGradation.bottom = rcGradation.top + m_nSize;
            GradientFillRectV(dc, rcGradation, m_crFrom, m_crTo);
        }

        dc.RestoreDC(nID);
    }

protected:
    COLORREF m_crFrom;
    COLORREF m_crTo;
    UINT     m_uDirection;
    int      m_nSize; 
public:
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_COLOR_ATTRIBUTE("from", m_crFrom, TRUE)
        BKWIN_COLOR_ATTRIBUTE("to", m_crTo, TRUE)
        BKWIN_INT_ATTRIBUTE("size", m_nSize, TRUE)
        BKWIN_ENUM_ATTRIBUTE("direction", UINT, TRUE)
            BKWIN_ENUM_VALUE("horz", 0)
            BKWIN_ENUM_VALUE("vert", 1)
        BKWIN_ENUM_END(m_uDirection)
    BKWIN_DECLARE_ATTRIBUTES_END()

private:
};

class CBkPngSkin
    : public CBkSkinBase
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPngSkin, "png")

public:
    CBkPngSkin()
        : m_uResID(0)
        , m_lSubImageWidth(0)
        , m_pImg(NULL)
    {
    }

    ~CBkPngSkin()
    {
    }

    virtual BOOL Load(TiXmlElement* pXmlElem)
    {
        __super::Load(pXmlElem);
        
        if (0 == m_uResID)
            return TRUE;

        m_pImg = BkPngPool::Get(m_uResID);

        return TRUE;
    }

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState)
    {
        if (m_pImg)
        {
            Gdiplus::Graphics graphics(dc);

            SIZE size = {0, 0};
            if (m_pImg)
            {
                if (0 == m_lSubImageWidth)
                    size.cx = m_pImg->GetWidth();
                else
                    size.cx = m_lSubImageWidth;
                size.cy = m_pImg->GetHeight();
            }

            if (0 == m_lSubImageWidth)
                graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDraw.left, rcDraw.top, size.cx, size.cy));
            else
                graphics.DrawImage(m_pImg, Gdiplus::Rect(rcDraw.left, rcDraw.top, size.cx, size.cy), m_lSubImageWidth * dwState, 0, size.cx, size.cy, Gdiplus::UnitPixel);
        }
    }

    virtual SIZE GetSkinSize()
    {
        SIZE ret = {0, 0};
        if (m_pImg)
        {
            if (0 == m_lSubImageWidth)
                ret.cx = m_pImg->GetWidth();
            else
                ret.cx = m_lSubImageWidth;
            ret.cy = m_pImg->GetHeight();
        }

        return ret;
    }

    virtual BOOL IgnoreState()
    {
        return 0 == m_lSubImageWidth;
    }

protected:

    Gdiplus::Image* m_pImg;
    UINT m_uResID;
    LONG m_lSubImageWidth;

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_UINT_ATTRIBUTE("src", m_uResID, TRUE)
        BKWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
    BKWIN_DECLARE_ATTRIBUTES_END()
};

class BkSkin
{
public:
    BkSkin()
    {
    }

    ~BkSkin()
    {
        _Clear();
    }

    static BOOL LoadSkins(UINT uResID)
    {
        CStringA strXml;
        BOOL bRet = BkResManager::LoadResource(uResID, strXml);

        if (!bRet)
            return FALSE;

        return LoadSkins(strXml);
    }

    static BOOL LoadSkins(LPCSTR lpszXml)
    {
        TiXmlDocument xmlDoc;

        _Instance()->_Clear();

        xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);

        if (xmlDoc.Error())
            return FALSE;

        _Instance()->_LoadSkins(xmlDoc.RootElement());

        return TRUE;
    }

    static CBkSkinBase* GetSkin(LPCSTR lpszSkinName)
    {
        __BkSkinPool::CPair *pairRet = _Instance()->m_mapPool.Lookup(lpszSkinName);

        if (pairRet)
            return pairRet->m_value;
        else
            return NULL;
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapPool.GetCount();
    }

protected:

    typedef CAtlMap<CStringA, CBkSkinBase *> __BkSkinPool;

    static BkSkin* ms_pInstance;

    static BkSkin* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkSkin;
        return ms_pInstance;
    }

//     static BkSkin& _Instance()
//     {
//         static BkSkin s_obj;
// 
//         return s_obj;
//     }

    __BkSkinPool m_mapPool;

    void _Clear()
    {
        POSITION pos = m_mapPool.GetStartPosition();

        while (pos)
        {
            CBkSkinBase *pSkin = m_mapPool.GetNextValue(pos);
            delete pSkin;
        }

        m_mapPool.RemoveAll();
    }

    void _LoadSkins(TiXmlElement *pXmlSkinRootElem)
    {
        LPCSTR lpszSkinName = NULL, lpszTypeName = NULL;

        if (!pXmlSkinRootElem)
            return;

        if (strcmp(pXmlSkinRootElem->Value(), "skins") != 0)
            return;

        for (TiXmlElement* pXmlChild = pXmlSkinRootElem->FirstChildElement(); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
        {
            lpszSkinName = pXmlChild->Attribute("name");
            lpszTypeName = pXmlChild->Value();
            if (!lpszSkinName || !lpszTypeName)
                continue;

            CBkSkinBase *pSkin = _CreateBkSkinByName(lpszTypeName);
            if (!pSkin)
                continue;

            pSkin->Load(pXmlChild);

            /*_Instance()->*/m_mapPool[lpszSkinName] = pSkin;
        }
    }

    static CBkSkinBase* _CreateBkSkinByName(LPCSTR lpszName)
    {
        CBkSkinBase *pNewSkin = NULL;

        pNewSkin = CBkImageSkin::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinImgFrame::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinButton::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinImgHorzExtend::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkSkinGradation::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        pNewSkin = CBkPngSkin::CheckAndNew(lpszName);
        if (pNewSkin)
            return pNewSkin;

        return NULL;
    }
};

__declspec(selectany) BkSkin* BkSkin::ms_pInstance = NULL;