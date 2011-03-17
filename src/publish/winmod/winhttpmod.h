/**
* @file    winhttpmod.h
* @brief   ...
* @author  bbcallen
* @date    2010-05-04 16:17
*/

#ifndef WINHTTPMOD_H
#define WINHTTPMOD_H

#include <assert.h>
#include "winhttpmodport.h"
#pragma comment(lib, "winhttp.lib")

#pragma warning(push)
#pragma warning(disable: 4239)  // Conversion from integral type to enum type is not strictly allowed. 

#include "winhttpmoddownloadfile.h"
#include "winhttpmodsession.h"

#pragma warning(pop)

#endif//WINHTTPMOD_H