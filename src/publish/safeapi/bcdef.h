/**
* @file    bcdef.h
* @brief   ...
* @author  bbcallen
* @date    2009-07-27 17:58
*/

#ifndef BCDEF_H
#define BCDEF_H

// 配置文件的名字,将前缀K改为Beike
#define FileBootCleanConfigFileName     L"KSafeFileBootClean.config"
#define FileBootReplaceConfigFileName   L"KSafeFileBootReplace.config"
#define RegBootCleanConfigFileName      L"KSafeRegBootClean.config"
#define DriverApprovedConfigFileName    L"KSafeDriverApproved.config"
#define DriverBlockedConfigFileName     L"KSafeDriverBlocked.config"
#define FileBootOccupyConfigFileName    L"KSafeFileBootOccupy.config"

#define FileBootCleanMutexName          L"39CACF56-10A3-41ef-BD02-B73C5BC14BC3"
#define FileBootReplaceMutexName        L"D83E501F-F14F-4a10-9423-94AEC0E71072"
#define RegBootCleanMutexName           L"DA848379-14CF-427c-8406-A55D31955FAA"
#define DriverApprovedMutexName         L"A62AC955-2AF9-43b1-A8BF-1C5B8850E37A"
#define DriverBlockedMutexName          L"0016CDF0-76A5-4543-98B1-F13810CEA6AB"

#define SafeapiRegPath                  L"SYSTEM\\CurrentControlSet\\Control\\KSafe\\Safeapi"

#define BC_ServiceName	                L"KSafeBootCheck"
#define BC_ServiceGroup                 L"Boot Bus Extender"
#define BC_DriverFileName               L"ksafebc.sys"

#define BC_KACHA                        L"kacha"


#define NS_SAFEAPI_BEGIN                namespace SafeApi {
#define NS_SAFEAPI_END                  }


#endif//BCDEF_H