/**
* @file    winmodinet.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:08
*/

#ifndef WINMODINET_H
#define WINMODINET_H

#include <assert.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#pragma warning(push)
#pragma warning(disable: 4239)  // Conversion from integral type to enum type is not strictly allowed. 

#include "winmodinetsession.h"
#include "winmodinethttpdownloadfile.h"

#pragma warning(pop)

#endif//WINMODINET_H