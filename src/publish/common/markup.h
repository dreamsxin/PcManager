// Markup.h: interface for the CMarkup class.
//
// Markup Release 9.0
// Copyright (C) 1999-2007 First Objective Software, Inc. All rights reserved
// Go to www.firstobject.com for the latest CMarkup and EDOM documentation
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.

// #define MARKUP_STL

#if !defined(AFX_MARKUP_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_)
#define AFX_MARKUP_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_

// Text type and function defines (compiler and build-option dependent)
// Define MARKUP_STDC to force standard C defines in Visual Studio
// 
#if _MSC_VER > 1000
#include <tchar.h>
#pragma once
#pragma warning(disable:4996) // suppress VS 2005 deprecated function warnings
#ifdef MARKUP_STL
#pragma warning(disable:4786) // for std::string
#endif
#endif
#if _MSC_VER > 1000 && ! defined(MARKUP_STDC)
#define MCD_CHAR _TCHAR
#define MCD_PCSZ const _TCHAR*
#define MCD_PSZCPY _tcscpy
#define MCD_PSZLEN (int)_tcslen
#define MCD_PSZCHR _tcschr
#define MCD_PSZSTR _tcsstr
#define MCD_PSZNCPY _tcsncpy
#define MCD_PSZTOL _tcstol
#define MCD_PSZNCMP _tcsncmp
#define MCD_PSZNICMP _tcsnicmp
#define MCD_SPRINTF _stprintf
#define MCD_FOPEN _tfopen
#ifdef _WIN32_WCE
#define MCD_CLEN(p) 1
#else // not WinCE
#define MCD_CLEN(p) (int)_tclen(p)
#if ! defined(UNICODE)
#define MCD_STRERROR strerror(errno)
#elif _MSC_VER >= 1310 // VC 7.1
#define MCD_STRERROR _wcserror(errno)
#endif
#endif

#else // STDC
#if _MSC_VER >= 1200
#define strnicmp _strnicmp
#define wcsnicmp _wcsnicmp
#define wcserror _wcserror
#endif
#ifdef UNICODE
#define MCD_CHAR wchar_t
#define MCD_PCSZ const wchar_t*
#define MCD_PSZCPY wcscpy
#define MCD_PSZLEN (int)wcslen
#define MCD_PSZCHR wcschr
#define MCD_PSZSTR wcsstr
#define MCD_PSZNCPY wcsncpy
#define MCD_PSZTOL wcstol
#define MCD_PSZNCMP wcsncmp
#define MCD_PSZNICMP wcsnicmp
#define MCD_SPRINTF swprintf
#define MCD_FOPEN _wfopen
#define MCD_STRERROR wcserror(errno)
#ifndef _T
#define _T(s) L s
#endif
#define MCD_CLEN(p) 1
#else // STDC non-UNICODE
#define MCD_CHAR char
#define MCD_PCSZ const char*
#define MCD_PSZCPY strcpy
#define MCD_PSZLEN (int)strlen
#define MCD_PSZCHR strchr
#define MCD_PSZSTR strstr
#define MCD_PSZNCPY strncpy
#define MCD_PSZTOL strtol
#define MCD_PSZNCMP strncmp
#define MCD_PSZNICMP strnicmp
#define MCD_SPRINTF sprintf
#define MCD_FOPEN fopen
#define MCD_STRERROR strerror(errno)
#ifndef _T
#define _T(s) s
#endif
#define MCD_CLEN(p) 1
// #define MCD_CLEN(p) (int)mblen(p,MB_CUR_MAX)
#endif
#endif

// String type and function defines (compiler and build-option dependent)
// Define MARKUP_STL to use STL strings
//
#ifdef MARKUP_STL
#include <string>
#ifdef UNICODE
#define MCD_STR std::wstring
#else
#define MCD_STR std::string
#endif
#define MCD_2PCSZ(s) s.c_str()
#define MCD_STRLENGTH(s) (int)s.size()
#define MCD_STRCLEAR(s) s.erase()
#define MCD_STRISEMPTY(s) s.empty()
#define MCD_STRMID(s,n,l) s.substr(n,l)
#define MCD_STRASSIGN(s,p,n) s.assign(p,n)
#define MCD_STRCAPACITY(s) (int)s.capacity()
#define MCD_STRINSERTREPLACE(d,i,r,s) d.replace(i,r,s)
#define MCD_GETBUFFER(s,n) new MCD_CHAR[n+1]
#define MCD_RELEASEBUFFER(s,p,n) s.assign(p,n); delete[]p
#define MCD_BLDRESERVE(s,n) s.reserve(n)
#define MCD_BLDCHECK(s,n,d) ;
#define MCD_BLDRELEASE(s) ;
#define MCD_BLDAPPENDN(s,p,n) s.append(p,n)
#define MCD_BLDAPPEND(s,p) s.append(p)
#define MCD_BLDAPPEND1(s,c) s+=(MCD_CHAR)(c)
#else // MFC
//#include <afx.h>
#define MCD_STR CString
#define MCD_2PCSZ(s) ((MCD_PCSZ)s)
#define MCD_STRLENGTH(s) s.GetLength()
#define MCD_STRCLEAR(s) s.Empty()
#define MCD_STRISEMPTY(s) s.IsEmpty()
#define MCD_STRMID(s,n,l) s.Mid(n,l)
#define MCD_STRASSIGN(s,p,n) memcpy(s.GetBuffer(n),p,(n)*sizeof(MCD_CHAR));s.ReleaseBuffer(n);
#define MCD_STRCAPACITY(s) (((CStringData*)((MCD_PCSZ)s)-1)->nAllocLength)
#define MCD_GETBUFFER(s,n) s.GetBuffer(n)
#define MCD_RELEASEBUFFER(s,p,n) s.ReleaseBuffer(n)
#define MCD_BLDRESERVE(s,n) MCD_CHAR*pD=s.GetBuffer(n); int nL=0
#define MCD_BLDCHECK(s,n,d) if(nL+d>n){s.ReleaseBuffer(nL);n<<=2;pD=s.GetBuffer(n);}
#define MCD_BLDRELEASE(s) s.ReleaseBuffer(nL)
#define MCD_BLDAPPENDN(s,p,n) MCD_PSZNCPY(&pD[nL],p,n);nL+=n
#define MCD_BLDAPPEND(s,p) MCD_PSZCPY(&pD[nL],p);nL+=MCD_PSZLEN(p)
#define MCD_BLDAPPEND1(s,c) pD[nL++]=(MCD_CHAR)(c)
#endif

#ifdef _DEBUG
#define _DS(i) (i?&(MCD_2PCSZ(m_strDoc))[m_aPos[i].nStart]:0)
#define MARKUP_SETDEBUGSTATE m_pMainDS=_DS(m_iPos); m_pChildDS=_DS(m_iPosChild)
#else
#define MARKUP_SETDEBUGSTATE
#endif

class CMarkup
{
public:
	// allow function args to accept string objects as constant string pointers
	struct MCD_CSTR
	{
		MCD_CSTR() { pcsz=NULL; };
		MCD_CSTR( MCD_PCSZ p ) { pcsz=p; };
		MCD_CSTR( const MCD_STR& s ) { pcsz = MCD_2PCSZ(s); };
		operator MCD_PCSZ() const { return pcsz; };
		MCD_PCSZ pcsz;
	};

	CMarkup() { SetDoc( NULL ); InitDocFlags(); };
	CMarkup( MCD_CSTR szDoc ) { SetDoc( szDoc ); InitDocFlags(); };
	CMarkup( int nFlags ) { SetDoc( NULL ); m_nFlags = nFlags; };
	CMarkup( const CMarkup& markup ) { *this = markup; };
	void operator=( const CMarkup& markup );
	~CMarkup() {};

	// Navigate
	bool Load( MCD_CSTR szFileName );
	bool SetDoc( MCD_PCSZ szDoc );
	bool SetDoc( const MCD_STR& strDoc );
	bool IsWellFormed();
	bool FindElem( MCD_CSTR szName=NULL );
	bool FindChildElem( MCD_CSTR szName=NULL );
	bool IntoElem();
	bool OutOfElem();
	void ResetChildPos() { x_SetPos(m_iPosParent,m_iPos,0); };
	void ResetMainPos() { x_SetPos(m_iPosParent,0,0); };
	void ResetPos() { x_SetPos(0,0,0); };
	MCD_STR GetTagName() const;
	MCD_STR GetChildTagName() const { return x_GetTagName(m_iPosChild); };
	MCD_STR GetData() const { return x_GetData(m_iPos); };
	MCD_STR GetChildData() const { return x_GetData(m_iPosChild); };
	MCD_STR GetElemContent() const { return x_GetElemContent(m_iPos); };
	MCD_STR GetAttrib( MCD_CSTR szAttrib ) const { return x_GetAttrib(m_iPos,szAttrib); };
	MCD_STR GetChildAttrib( MCD_CSTR szAttrib ) const { return x_GetAttrib(m_iPosChild,szAttrib); };
	MCD_STR GetAttribName( int n ) const;
	int FindNode( int nType=0 );
	int GetNodeType() { return m_nNodeType; };
	bool SavePos( MCD_CSTR szPosName=_T("") );
	bool RestorePos( MCD_CSTR szPosName=_T("") );
	const MCD_STR& GetError() const { return m_strError; };
	int GetDocFlags() const { return m_nFlags; };
	void SetDocFlags( int nFlags ) { m_nFlags = nFlags; };
	enum MarkupDocFlags
	{
		MDF_IGNORECASE = 8,
	};
	enum MarkupNodeFlags
	{
		MNF_WITHCDATA      = 0x01,
		MNF_WITHNOLINES    = 0x02,
		MNF_WITHXHTMLSPACE = 0x04,
		MNF_WITHREFS       = 0x08,
		MNF_WITHNOEND      = 0x10,
		MNF_ESCAPEQUOTES  = 0x100,
		MNF_NONENDED   = 0x100000,
		MNF_ILLDATA    = 0x200000,
	};
	enum MarkupNodeType
	{
		MNT_ELEMENT					= 1,  // 0x01
		MNT_TEXT					= 2,  // 0x02
		MNT_WHITESPACE				= 4,  // 0x04
		MNT_CDATA_SECTION			= 8,  // 0x08
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
		MNT_COMMENT					= 32, // 0x20
		MNT_DOCUMENT_TYPE			= 64, // 0x40
		MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
		MNT_LONE_END_TAG			= 128,// 0x80
		MNT_NODE_ERROR              = 32768 // 0x8000
	};

	// Create
	bool Save( MCD_CSTR szFileName );
	const MCD_STR& GetDoc() const { return m_strDoc; };
	bool AddElem( MCD_CSTR szName, MCD_CSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags); };
	bool InsertElem( MCD_CSTR szName, MCD_CSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_INSERT); };
	bool AddChildElem( MCD_CSTR szName, MCD_CSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_CHILD); };
	bool InsertChildElem( MCD_CSTR szName, MCD_CSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_INSERT|MNF_CHILD); };
	bool AddElem( MCD_CSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags); };
	bool InsertElem( MCD_CSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_INSERT); };
	bool AddChildElem( MCD_CSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_CHILD); };
	bool InsertChildElem( MCD_CSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_INSERT|MNF_CHILD); };
	bool AddAttrib( MCD_CSTR szAttrib, MCD_CSTR szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool AddChildAttrib( MCD_CSTR szAttrib, MCD_CSTR szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool AddAttrib( MCD_CSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPos,szAttrib,nValue); };
	bool AddChildAttrib( MCD_CSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPosChild,szAttrib,nValue); };
	bool AddSubDoc( MCD_CSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,0); };
	bool InsertSubDoc( MCD_CSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_INSERT); };
	MCD_STR GetSubDoc() const { return x_GetSubDoc(m_iPos); };
	bool AddChildSubDoc( MCD_CSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_CHILD); };
	bool InsertChildSubDoc( MCD_CSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_CHILD|MNF_INSERT); };
	MCD_STR GetChildSubDoc() const { return x_GetSubDoc(m_iPosChild); };
	bool AddNode( int nType, MCD_CSTR szText ) { return x_AddNode(nType,szText,0); };
	bool InsertNode( int nType, MCD_CSTR szText ) { return x_AddNode(nType,szText,MNF_INSERT); };

	// Modify
	bool RemoveElem();
	bool RemoveChildElem();
	bool RemoveNode();
	bool SetAttrib( MCD_CSTR szAttrib, MCD_CSTR szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool SetChildAttrib( MCD_CSTR szAttrib, MCD_CSTR szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool SetAttrib( MCD_CSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPos,szAttrib,nValue); };
	bool SetChildAttrib( MCD_CSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPosChild,szAttrib,nValue); };
	bool SetData( MCD_CSTR szData, int nFlags=0 ) { return x_SetData(m_iPos,szData,nFlags); };
	bool SetChildData( MCD_CSTR szData, int nFlags=0 ) { return x_SetData(m_iPosChild,szData,nFlags); };
	bool SetData( int nValue ) { return x_SetData(m_iPos,nValue); };
	bool SetChildData( int nValue ) { return x_SetData(m_iPosChild,nValue); };
	bool SetElemContent( MCD_CSTR szContent ) { return x_SetElemContent(szContent); };

	// Utility
	static bool ReadTextFile( MCD_CSTR szFileName, MCD_STR& strDoc, MCD_STR* pstrError=NULL, int* pnFlags=NULL );
	static bool WriteTextFile( MCD_CSTR szFileName, MCD_STR& strDoc, MCD_STR* pstrError=NULL, int* pnFlags=NULL );
	static MCD_STR EscapeText( MCD_CSTR szText, int nFlags = 0 );
	static MCD_STR UnescapeText( MCD_CSTR szText, int nTextLength = -1 );
	static int UTF16To8( char *pszUTF8, const wchar_t* pwszUTF16, int nUTF8Count );
	static int DecodeCharUTF8( const char*& pszUTF8 );
	static void EncodeCharUTF16( int nUChar, wchar_t* pwszUTF16, int& nWideLen );
	static int UTF8To16( wchar_t* pwszUTF16, const char* pszUTF8, int nUTF8Count );
	static int DecodeCharUTF16( const wchar_t*& pwszUTF16 );
	static void EncodeCharUTF8( int nUChar, char* pszUTF8, int& nUTF8Len );
	static MCD_STR UTF8ToA( MCD_CSTR pszUTF8, int* pnFailed = NULL );
	static MCD_STR AToUTF8( MCD_CSTR pszANSI );
	static MCD_STR GetDeclaredEncoding( MCD_CSTR szDoc );

protected:

#ifdef _DEBUG
	MCD_PCSZ m_pMainDS;
	MCD_PCSZ m_pChildDS;
#endif

	MCD_STR m_strDoc;
	MCD_STR m_strError;

	int m_iPosParent;
	int m_iPos;
	int m_iPosChild;
	int m_iPosFree;
	int m_iPosDeleted;
	int m_nNodeType;
	int m_nNodeOffset;
	int m_nNodeLength;
	int m_nFlags;

	struct ElemPos
	{
		ElemPos() {};
		ElemPos( const ElemPos& pos ) { *this = pos; };
		enum { EP_STBITS=22, EP_STMASK=0x2fffff, EP_LEVMASK=0xffff };
		int StartTagLen() const { return (nTagLengths & EP_STMASK); };
		void SetStartTagLen( int n ) { nTagLengths = (nTagLengths & ~EP_STMASK) + n; };
		void AdjustStartTagLen( int n ) { nTagLengths += n; };
		int EndTagLen() const { return (nTagLengths >> EP_STBITS); };
		void SetEndTagLen( int n ) { nTagLengths = (nTagLengths & EP_STMASK) + (n << EP_STBITS); };
		bool IsEmptyElement() { return (StartTagLen()==nLength)?true:false; };
		int StartContent() const { return nStart + StartTagLen(); };
		int ContentLen() const { return nLength - StartTagLen() - EndTagLen(); };
		int StartAfter() const { return nStart + nLength; };
		int Level() const { return nFlags & EP_LEVMASK; };
		void SetLevel( int nLev ) { nFlags = (nFlags & ~EP_LEVMASK) | nLev; };
		void ClearVirtualParent() { memset(this,0,sizeof(ElemPos)); };

		// Memory size: 8 32-bit integers == 32 bytes
		int nStart;
		int nLength;
		int nTagLengths; // 22 bits 4MB limit for start tag, 10 bits 1K limit for end tag
		int nFlags; // 16 bits flags, 16 bits level 65536 depth limit
		int iElemParent;
		int iElemChild; // first child
		int iElemNext; // next sibling
		int iElemPrev; // if this is first, iElemPrev points to last
	};

	enum MarkupNodeFlagsInternal
	{
		MNF_REPLACE    = 0x001000,
		MNF_INSERT     = 0x002000,
		MNF_CHILD      = 0x004000,
		MNF_QUOTED     = 0x008000,
		MNF_EMPTY      = 0x010000,
		MNF_DELETED    = 0x020000,
		MNF_FIRST      = 0x080000,
		MNF_PUBLIC     = 0x300000,
		MNF_ILLFORMED  = 0x800000,
		MNF_USER      = 0xf000000,
	};

	struct NodePos
	{
		NodePos() {};
		NodePos( int n ) { nFlags=n; nNodeType=0; nStart=0; nLength=0; };
		int nNodeType;
		int nStart;
		int nLength;
		int nFlags;
		MCD_STR strMeta;
	};

	struct TokenPos
	{
		TokenPos( MCD_CSTR sz, int n ) { Clear(); szDoc=sz; nTokenFlags=n; };
		void Clear() { nL=0; nR=-1; nNext=0; };
		int Length() const { return nR - nL + 1; };
		bool Match( MCD_CSTR szName )
		{
			int nLen = nR - nL + 1;
			if ( nTokenFlags & MDF_IGNORECASE )
				return ( (MCD_PSZNICMP( &szDoc[nL], szName, nLen ) == 0)
					&& ( szName[nLen] == _T('\0') || MCD_PSZCHR(_T(" =/[]"),szName[nLen]) ) );
			else
				return ( (MCD_PSZNCMP( &szDoc[nL], szName, nLen ) == 0)
					&& ( szName[nLen] == _T('\0') || MCD_PSZCHR(_T(" =/[]"),szName[nLen]) ) );
		};
		int nL;
		int nR;
		int nNext;
		MCD_PCSZ szDoc;
		int nTokenFlags;
		int nPreSpaceStart;
		int nPreSpaceLength;
	};

	struct SavedPos
	{
		SavedPos() { nSavedPosFlags=0; iPos=0; };
		MCD_STR strName;
		int iPos;
		int nSavedPosFlags;
	};

	struct SavedPosMap
	{
		SavedPosMap() { pTable = NULL; };
		~SavedPosMap() { RemoveAll(); };
		void RemoveAll() { if (pTable) Release(); pTable=NULL; };
		enum { SPM_SIZE = 7, SPM_MAIN = 1, SPM_CHILD = 2, SPM_USED = 4, SPM_LAST = 8 };
		void Release() { for (int n=0;n<SPM_SIZE;++n) if (pTable[n]) delete[] pTable[n]; delete[] pTable; };
		void AllocMapTable() { pTable = new SavedPos*[SPM_SIZE]; for (int n=0; n<SPM_SIZE; ++n) pTable[n]=NULL; };
		int Hash( MCD_PCSZ szName ) { unsigned int n=0; while (*szName) n += (unsigned int)(*szName++); return n % SPM_SIZE; };
		SavedPos** pTable;
	};
	SavedPosMap m_mapSavedPos;

	struct PosArray
	{
		PosArray() { Clear(); };
		~PosArray() { Release(); };
		enum { PA_SEGBITS = 16, PA_SEGMASK = 0xffff };
		void RemoveAll() { Release(); Clear(); };
		void Release() { for (int n=0;n<SegsUsed();++n) delete[] (char*)pSegs[n]; if (pSegs) delete[] (char*)pSegs; };
		void Clear() { nSegs=0; nSize=0; pSegs=NULL; };
		int GetSize() const { return nSize; };
		int SegsUsed() const { return ((nSize-1)>>PA_SEGBITS) + 1; };
		ElemPos& operator[](int n) const { return pSegs[n>>PA_SEGBITS][n&PA_SEGMASK]; };
		ElemPos** pSegs;
		int nSize;
		int nSegs;
	};
	PosArray m_aPos;

	struct NodeStack
	{
		NodeStack() { nTop=-1; nSize=0; pN=NULL; };
		~NodeStack() { if (pN) delete [] pN; };
		NodePos& Top() { return pN[nTop]; };
		NodePos& At( int n ) { return pN[n]; };
		void Add() { ++nTop; if (nTop==nSize) Alloc(nSize*2+6); };
		void Remove() { --nTop; };
		int TopIndex() { return nTop; };
	protected:
		void Alloc( int nNewSize ) { NodePos* pNNew = new NodePos[nNewSize]; Copy(pNNew); nSize=nNewSize; };
		void Copy( NodePos* pNNew ) { for(int n=0;n<nSize;++n) pNNew[n]=pN[n]; if (pN) delete [] pN; pN=pNNew; };
		NodePos* pN;
		int nSize;
		int nTop;
	};

	void x_SetPos( int iPosParent, int iPos, int iPosChild )
	{
		m_iPosParent = iPosParent;
		m_iPos = iPos;
		m_iPosChild = iPosChild;
		m_nNodeOffset = 0;
		m_nNodeLength = 0;
		m_nNodeType = iPos?MNT_ELEMENT:0;
		MARKUP_SETDEBUGSTATE;
	};
	int x_GetFreePos()
	{
		if ( m_iPosFree == m_aPos.GetSize() )
			x_AllocPosArray();
		return m_iPosFree++;
	};
	bool x_AllocPosArray( int nNewSize = 0 );

	void InitDocFlags()
	{
		// To always ignore case, define MARKUP_IGNORECASE
	#ifdef MARKUP_IGNORECASE
		m_nFlags = MDF_IGNORECASE;
	#else
		m_nFlags = 0;
	#endif
	};

	bool x_ParseDoc();
	int x_ParseElem( int iPos, TokenPos& token );
	static bool x_FindAny( MCD_PCSZ szDoc, int& nChar );
	static bool x_FindName( TokenPos& token );
	static MCD_STR x_GetToken( const TokenPos& token );
	int x_FindElem( int iPosParent, int iPos, MCD_PCSZ szPath ) const;
	MCD_STR x_GetPath( int iPos ) const;
	MCD_STR x_GetTagName( int iPos ) const;
	MCD_STR x_GetData( int iPos ) const;
	MCD_STR x_GetAttrib( int iPos, MCD_PCSZ szAttrib ) const;
	static MCD_STR x_EncodeCDATASection( MCD_PCSZ szData );
	bool x_AddElem( MCD_PCSZ szName, MCD_PCSZ szValue, int nFlags );
	bool x_AddElem( MCD_PCSZ szName, int nValue, int nFlags );
	MCD_STR x_GetSubDoc( int iPos ) const;
	bool x_AddSubDoc( MCD_PCSZ szSubDoc, int nFlags );
	static bool x_FindAttrib( TokenPos& token, MCD_PCSZ szAttrib, int n=0 );
	bool x_SetAttrib( int iPos, MCD_PCSZ szAttrib, MCD_PCSZ szValue );
	bool x_SetAttrib( int iPos, MCD_PCSZ szAttrib, int nValue );
	bool x_AddNode( int nNodeType, MCD_PCSZ szText, int nFlags );
	void x_RemoveNode( int iPosParent, int& iPos, int& nNodeType, int& nNodeOffset, int& nNodeLength );
	void x_AdjustForNode( int iPosParent, int iPos, int nShift );
	static bool x_CreateNode( MCD_STR& strNode, int nNodeType, MCD_PCSZ szText );
	int x_InsertNew( int iPosParent, int& iPosRel, NodePos& node );
	void x_LinkElem( int iPosParent, int iPosBefore, int iPos );
	int x_UnlinkElem( int iPos );
	int x_ReleaseSubDoc( int iPos );
	int x_ReleasePos( int iPos );
	void x_CheckSavedPos();
	static int x_ParseNode( TokenPos& token, NodePos& node );
	bool x_SetData( int iPos, MCD_PCSZ szData, int nFlags );
	bool x_SetData( int iPos, int nValue );
	int x_RemoveElem( int iPos );
	MCD_STR x_GetElemContent( int iPos ) const;
	bool x_SetElemContent( MCD_PCSZ szContent );
	void x_DocChange( int nLeft, int nReplace, const MCD_STR& strInsert );
	void x_Adjust( int iPos, int nShift, bool bAfterPos = false );
	static MCD_STR x_GetLastError();
};

#endif // !defined(AFX_MARKUP_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_)
