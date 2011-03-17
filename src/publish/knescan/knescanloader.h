/**
* @file    knescanloader.h
* @brief   ...
* @author  bbcallen
* @date    2010-07-08 11:02
*/

#ifndef KNESCANLOADER_H
#define KNESCANLOADER_H

#include "knescan.h"

EXTERN_C HRESULT WINAPI KnEScanLoad(LPCWSTR lpszEngineDir = NULL);

EXTERN_C HRESULT WINAPI KnEScanFile(DWORD dwSource, LPCTSTR lpFilePath, KnEScanResult& Result);

#endif//KNESCANLOADER_H