#ifndef _HELPER_2_H_
#define _HELPER_2_H_
//-----------------------------------------------------------------------------//
//
//  得到的是HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services下的服务
//
LONG
RegistryHelperGetServiceImagePath (
								   IN PWCHAR pwstrServiceName,
								   OUT PWCHAR pwstrImagePath,
								   IN OUT PLONG plSizeInBytes
								   );
//-----------------------------------------------------------------------------//
#endif