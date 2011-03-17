/** 
* @file bkrstype.h
* @brief 后台回扫的可移植的基本数据类型定义
* @author BAO<baoderong@kingsoft.com>
* @date October 30,2010
*
* @reference 本头文件来自文件引擎中钟东的定义
*/

#ifndef BKRESTYPE_H
#define BKRESTYPE_H

/*****************************************
* ISO C99标准整数类型定义
*****************************************/
// 以下内容在GCC的<stdint.h>中定义
// stdint.h中定义了_STDINT_H
#ifndef _STDINT_H
	// 8位
	typedef signed char          int8_t;
	typedef unsigned char        uint8_t;

	// 16位
	typedef short int            int16_t;
	typedef unsigned short int   uint16_t;

	// 32位
	#ifdef _MSC_VER   // _MSC_VER是VC预定义宏
		typedef long                 int32_t;
		/**
		* @attention   DWORD在VC/Windows中是unsigned long, 而且应用极广, 只好照旧
		**/
		typedef unsigned long        uint32_t;
	#else
		typedef int                  int32_t;
		typedef unsigned int         uint32_t;
	#endif  // _MSC_VER

	// 64位
	#ifdef _MSC_VER   // _MSC_VER是VC预定义宏
		/**
		* @attention   VS2005起支持long long, 但之前的版本不支持
		**/
		typedef __int64              int64_t;
		typedef unsigned __int64     uint64_t;
	#elif defined(__GNUC__)   // __GNUC__是GCC预定义宏
		/**
		* @attention   sys/types.h定义了intXX_t, 但没有定义uintXX_t
		*              而且会根据__WORDSIZE定义int64_t为long int
		**/
		#ifdef __LP64__   // __LP64__是GCC预定义宏, 此时long是64位
			#ifndef _SYS_TYPES_H    // sys/types.h中定义了_SYS_TYPES_H
				typedef long         int64_t;
			#endif
			typedef unsigned long    uint64_t;
		#else
			#ifndef _SYS_TYPES_H    // sys/types.h中定义了_SYS_TYPES_H
				typedef long long    int64_t;
			#endif
			typedef unsigned long long   uint64_t;
		#endif
	#endif  // _MSC_VER/__GNUC__

	// 与指针长度相同的整数类型, 用于指针与整数相互转化
	/**
	* @attention   VS2005的CRT中已经定义intprt_t
	**/
	// _WIN64是VC预定义宏, __LP64__是GCC预定义宏
	#if defined(_WIN64) || defined(__LP64__)   // 64位平台
		#ifndef _INTPTR_T_DEFINED   // VS2005的CRT中定义intprt_t时定义此宏
			typedef int64_t              intptr_t;
			#define _INTPTR_T_DEFINED
		#endif
		#ifndef _UINTPTR_T_DEFINED   // VS2005的CRT中定义uintprt_t时定义此宏
			typedef uint64_t             uintptr_t;
			#define _UINTPTR_T_DEFINED
		#endif
	#else   // 32位平台
		#ifndef _INTPTR_T_DEFINED   // VS2005的CRT中定义intprt_t时定义此宏
			typedef int32_t              intptr_t;
			#define _INTPTR_T_DEFINED
		#endif
		#ifndef _UINTPTR_T_DEFINED   // VS2005的CRT中定义uintprt_t时定义此宏
			typedef uint32_t             uintptr_t;
			#define _UINTPTR_T_DEFINED
		#endif
	#endif  // defined(_WIN64) || defined(__LP64__)

#endif  // !_STDINT_H


/*****************************************
* 常用Windows风格定义
*****************************************/
// 以下内容在VC的<windef.h>和<wtype.h>中定义
// windef.h定义了_WINDEF_, wtype.h中定义了__wtypes_h__
#if !(defined(_WINDEF_) || defined(__wtypes_h__))
	#ifndef NULL
		#ifdef __cplusplus
			#define NULL   0
		#else
			#define NULL   ((void *)0)
		#endif
	#endif

	typedef int32_t   BOOL;

	#ifndef FALSE
		#define FALSE   0
	#endif

	#ifndef TRUE
		#define TRUE    1
	#endif

	// 以下定义可能会用到, 但不建议使用
	typedef uint8_t     BYTE;
	typedef uint16_t    WORD;
	typedef uint32_t    DWORD;
	typedef uint8_t*    PBYTE;
	typedef uint16_t*   PWORD;
	typedef uint32_t*   PDWORD;

	#ifndef IN
		#define IN
	#endif

	#ifndef OUT
		#define OUT
	#endif

	// IUnknown要求AddRef和Release的返回值必须是ULONG, 在LP64下没必要用64位的long
	// __LP64__是GCC预定义类型
	#ifdef __LP64__
		typedef unsigned int  ULONG;
	#else
		typedef unsigned long ULONG;
	#endif  // __LP64__
#endif  // !(_WINDEF_ || __wtypes_h__)

// 以下内容在VC的<winnt.h>中定义
// winnt.h定义了_WINNT_
#ifndef _WINNT_
	typedef int32_t    HRESULT;

	// 以下定义可能会用到, 但不建议使用
	typedef uint16_t   WCHAR;
	typedef void*      HANDLE;
	typedef void*      PVOID;
#endif  // !_WINNT_


/// 文件名字符串类型, 在Windows平台下用Unicode, 其他平台用ANSI
// _WIN32或_WIN64是VC预定义类型
#if defined(_WIN32) || defined(_WIN64)
	typedef uint16_t KCHAR;
#else
	typedef uint8_t KCHAR;
#endif  // _WIN32 || _WIN64


/// 使用和释放时需要特殊处理的指针
typedef void*      KHANDLE;


/*****************************************
* 其他定义
*****************************************/

#ifndef ASSERT
	#define ASSERT assert
#endif

#ifndef KAE_MAX
	#define KAE_MAX(a,b)   (((a) > (b)) ? (a) : (b))
#endif

#ifndef KAE_MIN
	#define KAE_MIN(a,b)   (((a) < (b)) ? (a) : (b))
#endif

#endif
