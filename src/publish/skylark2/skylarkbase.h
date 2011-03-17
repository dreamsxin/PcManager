/**
* @file    skylarkbase.h
* @brief   ...
* @author  zhangrui
* @date    2008-12-15  12:23
*/

#ifndef SKYLARKBASE_H
#define SKYLARKBASE_H

#ifndef NS_SKYLARK_BEGIN
#define NS_SKYLARK_BEGIN    namespace Skylark { ///< declare namespace Skylark
#define NS_SKYLARK_END      };                  ///< declare namespace Skylark
#define NS_SKYLARK_USING    using namespace Skylark;
#endif//NS_SKYLARK_BEGIN

///< for namespace WinMod
namespace WinMod {}

NS_SKYLARK_BEGIN

#define SKYLARK_INTERNET_AGENT              L"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"       ///< skylark http agent

#define SKYLARK_BASE_FEED_VER               29          ///< bugfeed version number
#define SLPROTO_CLIENT_VER                  SKYLARK_BASE_FEED_VER   ///< protocol version number

#define SKYLARK_MD5_BYTES                   16          ///< md5的长度
#define SKYLARK_SHA1_BYTES                  20          ///< sha1的长度
#define SKYLARK_MD5EX_BYTES                 20          ///< md5+crc32的长度

#define SKYLARK_VIRUS_NAME_MAX_SIZE         64          ///< skylark病毒名的长度


using namespace WinMod;


/// skylark 的句柄类型定义
typedef void* SLHANDLE;




#ifndef _MT
#error Multithread C-Runtime lib required.
#endif//_MT


#undef  SKYLARK_MIN_DEBUG
#undef  SKYLARK_DEBUG
#undef  SKYLARK_MIN_RELEASE
#undef  SKYLARK_RELEASE


//#ifdef  _DLL
//#   ifdef  _DEBUG
//#       error Multithreaded Debug DLL is not supplied. Choose a lib to link manually
//#   else
//#       error Multithreaded DLL is not supplied. Choose a lib to link manually
//#   endif//_DEBUG
//#else
//#   ifdef  _DEBUG
//#       ifdef _ATL_MIN_CRT
//#           define SKYLARK_MIN_DEBUG    ///< ...
//#       else
//#           define SKYLARK_DEBUG        ///< ...
//#       endif//_ATL_MIN_CRT
//#   else
//#       ifdef _ATL_MIN_CRT
//#           define SKYLARK_MIN_RELEASE  ///< ...
//#       else
//#           define SKYLARK_RELEASE      ///< ...
//#       endif//_ATL_MIN_CRT
//#   endif//_DEBUG
//#endif//_DLL


NS_SKYLARK_END


/*
#ifndef _ATL_MIN_CRT
    #ifdef _DEBUG
        #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
        #include <crtdbg.h>
        #define new DEBUG_CLIENTBLOCK
    #endif
#endif//_ATL_MIN_CRT
*/


#endif//SKYLARKBASE_H