/**
* @file    bkatrundef.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-27 11:07
*/

#ifndef BKATRUNDEF_H
#define BKATRUNDEF_H

#include "skylark2\bkdef.h"

NS_SKYLARK_BEGIN

#define BKMOD_NAME_BKATRUN      L"katrun.dll"

#define SLATR_None                  0           ///< 无效取值
#define SLATR_All                   1           ///< 扫描所有
#define SLATR_No_Existing           0xFFFFFFFF  ///< ...
#define SLATR_Group_Min             0x10000000  ///< 最小值,小于值的id不应当被使用


/// 参考 sysinternals autorun.exe 9.51.0.0


#define SLATR_Group_Mask                                                    0xFFF00000      ///< ...

#define SLATR_RegKey_Parent__SW_Classes                                     L"SOFTWARE\\Classes"                                        ///< ...
#define SLATR_RegKey_Parent__SW_MS                                          L"SOFTWARE\\Microsoft"                                      ///< ...
#define SLATR_RegKey_Parent__SW_MS_Windows                                  L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion"             ///< ...
#define SLATR_RegKey_Parent__SW_MS_Windows_NT                               L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"          ///< ...
#define SLATR_RegKey_Parent__Terminal_Server_MS_Windows                     L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion"  ///< ...
#define SLATR_RegKey_Parent__Policies_MS_Windows_System_Scripts             L"SOFTWARE\\Policies\\Microsoft\\Windows\\System\\Scripts"  ///< ...
#define SLATR_RegKey_Parent__SW_MS_IE                                       L"SOFTWARE\\Microsoft\\Internet Explorer"                   ///< ...
#define SLATR_RegKey_Parent__CurrentControlSet                              L"System\\CurrentControlSet"                                ///< ...
#define SLATR_RegKey_Parent__CurrentControlSet_Control                      L"System\\CurrentControlSet\\Control"                       ///< ...
#define SLATR_RegKey_Parent__CurrentControlSet_Services                     L"System\\CurrentControlSet\\Services"                      ///< ...
#define SLATR_RegKey_Parent__Control_Panel                                  L"Control Panel"                                            ///< ...
#define SLART_RegKey_Parent__CLSID                                          L"CLSID"
//////////////////////////////////////////////////////////////////////////
// 注册表启动项
#define SLATR_Group_RegKey_Startup_Min                                      0x10100000  ///< ...
#define SLATR_Group_RegKey_Startup_Max                                      0x101FFFFF  ///< ...
#define SLATR_Group_RegKey_Startup                                          0x10100000  ///< 注册表启动项的类型
                                                                                 


// Logon
#define SLATR_HKLM_CurCtrlSet_Ctrl_TermServ_Wds_rdpwd__StartupPrograms      0x10101001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\[StartupPrograms]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__Appsetup                         0x10101002  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[AppSetup]
#define SLATR_HKLM_Policies_MS_Windows_System_Scripts_Startup               0x10101003  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\System\\Scripts\\Startup\\*\\*\\[Script]
#define SLATR_HKCU_Policies_MS_Windows_System_Scripts_Logon                 0x10101004  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logon\\*\\*\\[Script]
#define SLATR_HKLM_Policies_MS_Windows_System_Scripts_Logon                 0x10101005  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logon\\*\\*\\[Script]
#define SLATR_HKLM_MS_Windows_NT_Windows__Userinit                          0x10101006  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[Userinit]
#define SLATR_HKCU_MS_Windows_Policies_System__Shell                        0x10101007  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\[Shell]
#define SLATR_HKCU_MS_Windows_NT_Winlogon__Shell                            0x10101008  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[Shell]
#define SLATR_HKLM_MS_Windows_Policies_System__Shell                        0x10101009  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\[Shell]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__Shell                            0x1010100A  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[Shell]
// Winlogon => SLATR_HKLM_MS_Windows_NT_Winlogon__Taskman
#define SLATR_HKLM_MS_Windows_NT_TermServ_MS_Windows__Runonce               0x1010100B  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion\\Runonce
#define SLATR_HKLM_MS_Windows_NT_TermServ_MS_Windows__RunonceEx             0x1010100C  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion\\RunonceEx
#define SLATR_HKLM_MS_Windows_NT_TermServ_MS_Windows__Run                   0x1010100D  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
#define SLATR_HKLM_MS_Windows_Run                                           0x1010100E  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run
#define SLATR_HKLM_MS_Windows_RunOnceEx                                     0x1010100F  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx\\*
#define SLATR_HKLM_MS_Windows_RunOnce                                       0x10101010  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\*
// C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup
// C:\Users\bbcallen\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
#define SLATR_HKCU_MS_Windows_NT_Windows__Load                              0x10101011  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\[load]
#define SLATR_HKCU_MS_Windows_NT_Windows__Run                               0x10101012  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\[run]
#define SLATR_HKLM_MS_Windows_Policies_Explorer_Run                         0x10101013  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run\\*
#define SLATR_HKCU_MS_Windows_Policies_Explorer_Run                         0x10101014  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run\\*
#define SLATR_HKCU_MS_Windows_Run                                           0x10101015  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run
#define SLATR_HKCU_MS_Windows_RunOnce                                       0x10101016  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce\\*
#define SLATR_HKCU_MS_Windows_NT_TermServ_MS_Windows__Runonce               0x10101017  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion\\Runonce
#define SLATR_HKCU_MS_Windows_NT_TermServ_MS_Windows__RunonceEx             0x10101018  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion\\RunonceEx
#define SLATR_HKCU_MS_Windows_NT_TermServ_MS_Windows__Run                   0x10101019  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\Install\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
#define SLATR_HKCU_Policies_MS_Windows_System_Scripts_Logoff                0x1010101A  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff\\*\\*\\[Script]
#define SLATR_HKLM_Policies_MS_Windows_System_Scripts_Shutdown              0x1010101B  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\System\\Scripts\\Shutdown\\*\\*\\[Script]



// Explorer
#define SLATR_HKLM_Classes_Protocols_Filter                                 0x10102001  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\Protocols\\Filter\\*\\[CLSID]={*}
#define SLATR_HKLM_Classes_Protocols_Handler                                0x10102002  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\Protocols\\Handler\\*\\[CLSID]={*}
#define SLATR_HKCU_MS_IE_Desktop_Components                                 0x10102003  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Internet Explorer\\Desktop\\Components\\*\\[Source]
#define SLATR_HKLM_MS_Active_Setup_Installed_Components                     0x10102004  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{*}
#define SLATR_HKCU_MS_Active_Setup_Installed_Components                     0x10102005  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{*}
#define SLATR_HKLM_MS_Windows_Explorer_SharedTaskScheduler                  0x10102006  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\SharedTaskScheduler\\{*}
#define SLATR_HKLM_MS_Windows_ShellServiceObjectDelayLoad                   0x10102007  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad\\[*]={*}
#define SLATR_HKCU_MS_Windows_ShellServiceObjectDelayLoad                   0x10102008  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad\\[*]={*}
#define SLATR_HKLM_MS_Windows_Explorer_ShellExecuteHooks                    0x10102009  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellExecuteHooks\\{*}
#define SLATR_HKCU_Classes_Asterisk_ShellEx_ContextMenuHandlers             0x1010200A  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Classes\\*\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKLM_Classes_Asterisk_ShellEx_ContextMenuHandlers             0x1010200B  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\*\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKCU_Classes_AllFileSystemObjects_ShellEx_ContextMenuHandlers 0x1010200C  ///<  HKEY_CURRENT_USER\\Software\\Classes\\AllFileSystemObjects\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKLM_Classes_AllFileSystemObjects_ShellEx_ContextMenuHandlers 0x1010200D  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\AllFileSystemObjects\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKCU_Classes_Directory_ShellEx_ContextMenuHandlers            0x1010200E  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Directory\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKLM_Classes_Directory_ShellEx_ContextMenuHandlers            0x1010200F  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Directory\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKCU_Classes_Directory_ShellEx_DragDropHandlers               0x10102010  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Directory\\ShellEx\\DragDropHandlers\\{*}
#define SLATR_HKLM_Classes_Directory_ShellEx_DragDropHandlers               0x10102011  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Directory\\ShellEx\\DragDropHandlers\\{*}
#define SLATR_HKCU_Classes_Directory_ShellEx_PropertySheetHandlers          0x10102012  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Directory\\ShellEx\\PropertySheetHandlers\\{*}
#define SLATR_HKLM_Classes_Directory_ShellEx_PropertySheetHandlers          0x10102013  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Directory\\ShellEx\\PropertySheetHandlers\\{*}
#define SLATR_HKCU_Classes_Directory_ShellEx_CopyHookHandlers               0x10102014  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Directory\\ShellEx\\CopyHookHandlers\\{*}
#define SLATR_HKLM_Classes_Directory_ShellEx_CopyHookHandlers               0x10102015  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Directory\\ShellEx\\CopyHookHandlers\\{*}
#define SLATR_HKCU_Classes_Folder_ShellEx_ColumnHandlers                    0x10102016  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Folder\\ShellEx\\ColumnHandlers\\{*}
#define SLATR_HKLM_Classes_Folder_ShellEx_ColumnHandlers                    0x10102017  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Folder\\ShellEx\\ColumnHandlers\\{*}
#define SLATR_HKCU_Classes_Folder_ShellEx_ContextMenuHandlers               0x10102018  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Folder\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKLM_Classes_Folder_ShellEx_ContextMenuHandlers               0x10102019  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Folder\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKCU_Classes_Directory_Background_ShellEx_ContextMenuHandlers 0x1010201A  ///<  HKEY_CURRENT_USER\\Software\\Classes\\Directory\\Background\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKLM_Classes_Directory_Background_ShellEx_ContextMenuHandlers 0x1010201B  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\Directory\\Background\\ShellEx\\ContextMenuHandlers\\{*}
#define SLATR_HKCU_MS_Windows_Explorer_ShellIconOverlayIdentifiers          0x1010201C  ///<  HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers\\[*]={*}
#define SLATR_HKLM_MS_Windows_Explorer_ShellIconOverlayIdentifiers          0x1010201D  ///< HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers\\[*]={*}
#define SLATR_HKCU_MS_Ctf_LangBarAddin                                      0x1010201E  ///<  HKEY_CURRENT_USER\\Software\\Microsoft\\Ctf\\LangBarAddin\\[*]={*}
#define SLATR_HKLM_MS_Ctf_LangBarAddin                                      0x1010201F  ///< HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Ctf\\LangBarAddin\\[*]={*}
#define SLATR_HKCU_MS_Windows_Shell_Extensions_Approved                     0x10102020  ///<  HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved\\[*]={*}
#define SLATR_HKLM_MS_Windows_Shell_Extensions_Approved                     0x10102021  ///< HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved\\[*]={*}



// Internet Explorer
#define SLATR_HKLM_MS_Windows_Explorer_Browser_Helper_Objects               0x10103001  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\{*}
#define SLATR_HKCU_MS_IE_UrlSearchHooks                                     0x10103002  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Internet Explorer\\UrlSearchHooks\\[{*}]
#define SLATR_HKLM_MS_IE_Toolbar                                            0x10103003  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\Toolbar\\[{*}]		
#define SLATR_HKCU_MS_IE_Explorer_Bars                                      0x10103004  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Internet Explorer\\Explorer Bars\\[{*}]
#define SLATR_HKLM_MS_IE_Explorer_Bars                                      0x10103005  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\Explorer Bars\\[{*}]
#define SLATR_HKCU_MS_IE_Extensions_exec                                    0x10103006  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Internet Explorer\\Extensions\\*\\[exec]
#define SLATR_HKLM_MS_IE_Extensions_exec                                    0x10103007  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\Extensions\\*\\[exec]
#define SLATR_HKCU_MS_IE_Toolbar                                            0x10103008  ///< HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Internet Explorer\\Toolbar\\[{*}]		
#define SLATR_HKCU_MS_Windows_Explorer_Browser_Helper_Objects               0x10103009  ///< HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects\\{*}
#define SLATR_HKLM_MS_IE_UrlSearchHooks                                     0x10103010  ///<  HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\UrlSearchHooks\\[{*}]
#define SLATR_HKCU_MS_IE_Extensions_clsid                                   0x10103006  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Internet Explorer\\Extensions\\*\\[clsid]
#define SLATR_HKLM_MS_IE_Extensions_clsid                                   0x10103007  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\Extensions\\*\\[clsid]


// Scheduled Tasks

// Services

// Drivers



// Codec
#define SLATR_HKLM_MS_Windows_NT_Drivers32                                  0x10107001  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32\\[*]
#define SLATR_HKLM_Classes_Filter                                           0x10107002  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\Filter\\{*}
#define SLATR_HKLM_Classes_083863F1_70DE_11d0_BD40_00A0C911CE86_Instance    0x10107003  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance
#define SLATR_HKLM_Classes_AC757296_3522_4E11_9862_C17BE5A1767E_Instance    0x10107004  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\CLSID\\{AC757296-3522-4E11-9862-C17BE5A1767E}\\Instance
#define SLATR_HKLM_Classes_7ED96837_96F0_4812_B211_F13C24117ED3_Instance    0x10107005  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance
#define SLATR_HKLM_Classes_ABE3B9A4_257D_4B97_BD1A_294AF496222E_Instance    0x10107006  ///< HKEY_LOCAL_MACHINE\\Software\\Classes\\CLSID\\{ABE3B9A4-257D-4B97-BD1A-294AF496222E}\\Instance


// Boot Execute
#define SLATR_HKLM_CurCtrlSet_Ctrl_Session_Manager__BootExecute             0x10108001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\[BootExecute]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Session_Manager__SetupExecute            0x10108002  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\[SetupExecute]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Session_Manager__Execute                 0x10108003  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\[Execute]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Session_Manager__S0InitialCommand        0x10108004  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\[S0InitialCommand]
#define SLATR_HKLM_CurCtrlSet_Ctrl__ServiceControlManagerExtension          0x10108005  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\[ServiceControlManagerExtension]

// Image Hijacks
#define SLATR_HKLM_MS_Windows_NT_Image_File_Execution_Options               0x10109001  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\*\\[*]
#define SLATR_HKLM_MS_Command_Processor__Autorun                            0x10109002  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Command Processor\\[Autorun]
#define SLATR_HKCU_MS_Command_Processor__Autorun                            0x10109003  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Command Processor\\[Autorun]
#define SLATR_HKCU_Classes_Exefile_Shell_Open_Command                       0x10109004  ///<  HKEY_CURRENT_USER\\SOFTWARE\\Classes\\Exefile\\Shell\\Open\\Command\\[@]
#define SLATR_HKLM_Classes_Exefile_Shell_Open_Command                       0x10109005  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\Exefile\\Shell\\Open\\Command\\[@]

// AppInit
#define SLATR_HKLM_MS_Windows_NT_Windows__AppInit_DLLs                      0x1010A001  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\[AppInit_DLLs]

// KnownDlls
#define SLATR_HKLM_CurCtrlSet_Ctrl_Session_Manager_KnownDlls                0x1010B001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\KnownDlls\\[*]
                                                                                    
// Winlogon
#define SLATR_HKLM_MS_Windows_NT_Winlogon__ServiceControllerStart			0x1010C001  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[ServiceControllerStart]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__LsaStart			                0x1010C002  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[LsaStart]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__System                           0x1010C003  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[System]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__UIHost                           0x1010C004  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[UIHost]
#define SLATR_HKLM_MS_Windows_NT_Winlogon_Notify__DllName                   0x1010C005  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify\\[DLLName]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__GinaDLL                          0x1010C006  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[GinaDLL]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__Taskman                          0x1010C007  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[Taskman]
#define SLATR_HKCU_Control_Panel_Desktop                                    0x1010C008  ///<  HKEY_CURRENT_USER\\Control Panel\\Desktop\\[ScrnSave.exe]
#define SLATR_HKLM_CurCtrlSet_Ctrl_BootVerificationProgram__ImagePath       0x1010C009  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\BootVerificationProgram\\[ImagePath]
#define SLATR_HKLM_MS_Windows_NT_Winlogon__SaveDumpStart                    0x1010C00A  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\[SaveDumpStart]

// Winsock Providers
#define SLATR_HKLM_CurCtrlSet_Svc_WinSock2_Namespace_Catalog5               0x1010D001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries\\*\\[LibraryPath]
#define SLATR_HKLM_CurCtrlSet_Svc_WinSock2_Protocol_Catalog9                0x1010D002  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries\\*\\[PackedCatalogItem]
                                                                                     
// Print Monitors                                                                    
#define SLATR_HKLM_CurCtrlSet_Ctrl_Print_Monitors                           0x1010E001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Print\\Monitors\\*\\[Driver]
                                                                                     
// Lsa Providers                                                                     
#define SLATR_HKLM_CurCtrlSet_Ctrl_SecurityProviders                        0x1010F001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\SecurityProviders\\[SecurityProviders]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Lsa__Authentication_Packages             0x1010F002  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Lsa\\[Authentication Packages]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Lsa__Notification_Packages               0x1010F003  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Lsa\\[Notification Packages]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Lsa__Security_Packages                   0x1010F004  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Lsa\\[Security Packages]
#define SLATR_HKLM_MS_Windows_Authentication_Credential_Providers           0x1010F005  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers\\{*}
#define SLATR_HKLM_MS_Windows_Authentication_Credential_Provider_Filters    0x1010F006  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Provider Filter\\{*}
#define SLATR_HKLM_MS_Windows_Authentication_PLAP_Providers                 0x1010F007  ///< HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\PLAP Providers\\{*}

// Network Providers                                                             
#define SLATR_HKLM_CurCtrlSet_Ctrl_NetworkProvider_Order__ProviderOrder     0x10110001  ///< HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\NetworkProvider\\Order\\[ProviderOrder]

                                                                                 
                                                                                 
                                                                                 
                                                  
                                                                               
//////////////////////////////////////////////////////////////////////////     
// 服务
#define SLATR_Group_Services_Min                                        0x10200000  ///< ...
#define SLATR_Group_Services_Max                                        0x102FFFFF  ///< ...
#define SLATR_Group_Services                                            0x10200000  ///< ...
                                                                               
#define SLATR_HKLM_Services                                             0x10200001  ///< HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services



//////////////////////////////////////////////////////////////////////////     
// 计划任务
#define SLATR_Group_Scheduled_Tasks_Min                                 0x10300000  ///< ...
#define SLATR_Group_Scheduled_Tasks_Max                                 0x103FFFFF  ///< ...
#define SLATR_Group_Scheduled_Tasks                                     0x10300000  ///< ...

#define SLATR_Scheduled_Tasks                                           0x10300001  ///< ...



//////////////////////////////////////////////////////////////////////////
// 启动目录
#define SLATR_Group_Startup_Folder_Min                                  0x10400000  ///< ...
#define SLATR_Group_Startup_Folder_Max                                  0x104FFFFF  ///< ...
#define SLATR_Group_Startup_Folder                                      0x10400000  ///< ...

#define SLATR_Folder_CSIDL_COMMON_STARTUP                               0x10400001  ///< C:\\Documents and Settings\\All Users\\Start Menu\\Programs\\Startup
#define SLATR_Folder_CSIDL_STARTUP                                      0x10400002  ///< C:\\Documents and Settings\\username\\Start Menu\\Programs\\Startup
#define SLATR_Folder_CSIDL_COMMON_ALTSTARTUP                            0x10400003  ///< ...
#define SLATR_Folder_CSIDL_ALTSTARTUP                                   0x10400004  ///< ...




//////////////////////////////////////////////////////////////////////////
// autorun.inf
#define SLATR_Group_Startup_Ini_Min                                     0x10500000  ///< ...
#define SLATR_Group_Startup_Ini_Max                                     0x105FFFFF  ///< ...
#define SLATR_Group_Startup_Ini                                         0x10500000  ///< ...

#define SLATR_Ini_Autorun_Inf_At_Disk_Root                              0x10500001  ///< 根目录的autorun.inf


//////////////////
////DOS虚拟机程序
#define SLATR_HKLM_CurCtrlSet_Ctrl_WOW_cmdline                          0x10510002 ///<//HKLM\System\CurrentControlSet\Control\WOW\[cmdline]
#define SLATR_HKLM_CurCtrlSet_Ctrl_WOW_wowcmdline                       0x10510003 ///<//HKLM\System\CurrentControlSet\Control\WOW\[wowcmdline]
//浏览器动态组件
#define SLATR_HKLM_MS_CodeStoreDatabase_DistributionUnits               0x10510004 ///<//HKLM\SOFTWARE\Microsoft\Code Store Database\Distribution Units\{*}
//禁用的启动项
#define SLATR_HKLM_MS_SharedTools_MSConfig_startupreg_command           0x10510005 ///<//HKLM\SOFTWARE\Microsoft\Shared Tools\MSConfig\startupreg\*\[command]
//RPC网络远程调用协议
#define SLATR_HKLM_MS_Rpc_ClientProtocols_ncacn_http                    0x10510006 ///<//HKLM\SOFTWARE\Microsoft\Rpc\ClientProtocols\[ncacn_http]
#define SLATR_HKLM_MS_Rpc_ClientProtocols_ncacn_ip_tcp                  0x10510007 ///<//HKLM\SOFTWARE\Microsoft\Rpc\ClientProtocols\[ncacn_ip_tcp]
#define SLATR_HKLM_MS_Rpc_ClientProtocols_ncacn_np                      0x10510008 ///<//HKLM\SOFTWARE\Microsoft\Rpc\ClientProtocols\[ncacn_np]
#define SLATR_HKLM_MS_Rpc_ClientProtocols_ncadg_ip_udp                  0x10510009 ///<//HKLM\SOFTWARE\Microsoft\Rpc\ClientProtocols\[ncadg_ip_udp]
#define SLATR_HKLM_MS_Rpc_SecurityService                               0x10510010 ///<//HKLM\SOFTWARE\Microsoft\Rpc\SecurityService\[*]
//命令行工具
#define SLATR_HKLM_CurCtrlSet_Ctrl_SessionManager_Environment_ComSpec           0x10510011 ///<//HKLM\System\CurrentControlSet\Control\Session Manager\Environment\[ComSpec]
//运行框程序关联
#define SLATR_HKLM_MS_Windows_CurrentVersion_Apppaths                   0x10510012 ///<//HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\*\[默认]
//浏览器插件
#define SLATR_HKLM_MS_Windows_CurrentVersion_Ext_Stats                  0x10510013 ///<//HKLM\\Software\Microsoft\Windows\CurrentVersion\Ext\Stats\{*}
#define SLATR_HKCU_MS_Windows_CurrentVersion_Ext_Stats                  0x10510014 ///<//HKCU\Software\Microsoft\Windows\CurrentVersion\Ext\Stats\{*}
//浏览器右键菜单
#define SLATR_HKLM_MS_IE_MenuExt                                        0x10510015 ///<//HKLM\Software\Microsoft\Internet Explorer\MenuExt\[默认]
#define SLATR_HKCU_MS_IE_MenuExt                                        0x10510016 ///<//HKCU\Software\Microsoft\Internet Explorer\MenuExt\[默认]
//右键菜单
#define SLATR_HKCR_shellex_ContextMenuHandlers                          0x10510017 ///<//HKCR\*\shellex\ContextMenuHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_AllFilessystemobjects_shellex_ContextMenuHandlers    0x10510018 ///<//HKCR\AllFilesystemObjects\shellex\ContextMenuHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Directory_shellex_ContextMenuHandlers                0x10510019 ///<//HKCR\Directory\shellex\ContextMenuHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Folder_shellex_ContextMenuHandlers                   0x10510020 ///<//HKCR\Folder\shellex\ContextMenuHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Directory_Background_shellex_ContextMenuHandlers     0x10510021 ///<//HKCR\Directory\Background\ShellEx\ContextMenuHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_CLSID_20D04FE0_3AEA_1069_A2D8_08002B30309D_shell_Command 0x10510022 ///<//HKCR\CLSID\{20D04FE0-3AEA-1069-A2D8-08002B30309D}\shell\*\*\command\[默认]
#define SLATR_HKCR_CLSID_450D8FBA_AD25_11D0_98A8_0800361B1103_shell_Command 0x10510023 ///<//HKCR\CLSID\{450D8FBA-AD25-11D0-98A8-0800361B1103}\shell\*\*\command\[默认]
#define SLATR_HKCR_CLSID_208D2C60_3AEA_1069_A2D7_08002B30309D_shell_Command 0x10510024 ///<//HKCR\CLSID\{208D2C60-3AEA-1069-A2D7-08002B30309D}\shell\*\*\command\[默认]
#define SLATR_HKCR_CLSID_7be9d83c_a729_4d97_b5a7_1b7313c39e0a_shell_Command 0x10510025 ///<//HKCR\CLSID\{7be9d83c-a729-4d97-b5a7-1b7313c39e0a}\shell\*\*\command\[默认]
#define SLATR_HKCR_CLSID_48e7caab_b918_4e58_a94d_505519c795dc_shell_Command 0x10510026 ///<//HKCR\CLSID\{48e7caab-b918-4e58-a94d-505519c795dc}\shell\*\*\command\[默认]
#define SLATR_HKCR_CLSID_871C5380_42A0_1069_A2EA_08002B30309D_shell_Command 0x10510027 ///<//HKCR\CLSID\{871C5380-42A0-1069-A2EA-08002B30309D}\shell\*\*\command\[默认]
////其它敏感启动项
#define SLATR_HKCR_Directory_shellex_PropertyShelltHandlers                 0x10510028 ///<//HKCR\Directory\shellex\PropertySheetHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Directory_shellex_CopyHookHandlers                       0x10510029 ///<//HKCR\Directory\shellex\CopyHookHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Directory_shellex_DragDropHandlers                       0x10510030 ///<//HKCR\Directory\shellex\DragDropHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Folder_shellex_ColumnHandlers                            0x10510032 ///<//HKCR\Folder\shellex\ColumnHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Folder_shellex_DragDropHandlers                          0x10510034 ///<//HKCR\Folder\shellex\DragDropHandlers\目录名为CLSID或默认键为CLSID
#define SLATR_HKCR_Folder_shellex_PropertySheetHandlers                     0x10510035 ///<//HKCR\Folder\shellex\PropertySheetHandlers\目录名为CLSID或默认键为CLSID
//系统动态组件
#define SLATR_HKLM_MS_Active_Setup_Installed_Components_clsid               0x10510036 ///<//HKLM\SOFTWARE\Microsoft\Active Setup\Installed Components\{*}

//输入法
//#define SLATR_HKLM_CurCtrlSet_Ctrl_KeyboardLayouts_E0220804__Ime_File       0x10510037  ///< HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts\E0220804\[Ime File]
#define SLATR_HKLM_CurCtrlSet_Ctrl_Session_Manager_AppCertDlls__AppSecDll   0x10510038  ///< HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\AppCertDlls\[AppSecDll]

//输入法
#define SLATR_HKCU_Ime                                                      0x10600001  ///< 输入法
#define SLATR_HKCR_Shell_MuiCache                                           0x10600002  ///< HKEY_CLASSES_ROOT\Local Settings\Software\Microsoft\Windows\Shell\MuiCache
#define SLATR_HKCU_ShellNoRoam_MuiCache                                     0x10600003  ///< HKEY_CURRENT_USER\Software\Microsoft\Windows\ShellNoRoam\MUICache

#define SLATR_HKCR_Shell__exe__Open                                         0x10700010  ///< .exe open
#define SLATR_HKCR_Shell__exe__Runas                                        0x10700011  ///< .exe runas
#define SLATR_HKCR_Shell__txt__Open                                         0x10700020  ///< .txt open
#define SLATR_HKCR_Shell__asf__Open                                         0x10700030  ///< .asf open
#define SLATR_HKCR_Shell__avi__Open                                         0x10700040  ///< .avi open
#define SLATR_HKCR_Shell__bat__Open                                         0x10700050  ///< .bat open
#define SLATR_HKCR_Shell__bat__Runas                                        0x10700051  ///< .bat runas
#define SLATR_HKCR_Shell__chm__Open                                         0x10700060  ///< .chm runas
#define SLATR_HKCR_Shell__cmd__Open                                         0x10700070  ///< .cmd open
#define SLATR_HKCR_Shell__cmd__Runas                                        0x10700071  ///< .cmd open
#define SLATR_HKCR_Shell__com__Open                                         0x10700080  ///< .com open
#define SLATR_HKCR_Shell__gif__Open                                         0x10700090  ///< .gif open
#define SLATR_HKCR_Shell__hlp__Open                                         0x107000A0  ///< .hlp open
#define SLATR_HKCR_Shell__hta__Open                                         0x107000B0  ///< .hta open
#define SLATR_HKCR_Shell__htm__Open                                         0x107000C0  ///< .htm open
#define SLATR_HKCR_Shell__html_Open                                         0x107000D0  ///< .html open
#define SLATR_HKCR_Shell__inf__Open                                         0x107000E0  ///< .inf open
#define SLATR_HKCR_Shell__ini__Open                                         0x107000F0  ///< .ini open
#define SLATR_HKCR_Shell__jpg__Open                                         0x10700100  ///< .jpg open
#define SLATR_HKCR_Shell__jpeg_Open                                         0x10700110  ///< .jpeg open
#define SLATR_HKCR_Shell__m3u__Open                                         0x10700120  ///< .m3u open
#define SLATR_HKCR_Shell__reg__Open                                         0x10700130  ///< .reg open
#define SLATR_HKCR_Shell__wma__Open                                         0x10700140  ///< .wma open
#define SLATR_HKCR_Shell__mp3__Open                                         0x10700150  ///< .mp3 open
#define SLATR_HKCR_Shell__mpg__Open                                         0x10700160  ///< .mpg open
#define SLATR_HKCR_Shell__mpeg_Open                                         0x10700170  ///< .mpeg open
#define SLATR_HKCR_Shell__pif__Open                                         0x10700180  ///< .pif open
#define SLATR_HKCR_Shell__scr__Open                                         0x10700190  ///< .scr open
#define SLATR_HKCR_Shell__vbs__Open                                         0x107001A0  ///< .vbs open
#define SLATR_HKCR_Shell__js___Open                                         0x107001B0  ///< .js open



/// 启动项初始化参数
#define BKATR_SHOW_INVALID_FILE         0x00000001      ///< 启动项扫描时是否过滤掉无效的文件(启动项中指向的文件已经不存在)
#define BKATR_SHOW_ONLY_CAN_REPAIR      0x00000002      ///< 仅扫描可修复的启动项
#define BKATR_SHOW_ONLY_CAN_NOT_REPAIR  0x00000004      ///< 仅扫描不可修复的启动项


typedef enum
{
    enumScanPolicyInvalid = -1,
    enumScanPolicyFull = 0, ///< 完整策略，扫描启动项非常完整
    enumScanPolicyLite = 1, ///< 轻巧策略，扫描常用启动项
    enumScanPolicyCount,
};

struct BKATRUN_SCAN_PARAM
{
    UINT32  uSize;              ///< 结构体大小

    UINT32  uPolicy;            ///< 扫描策略，比如完整，快速
    UINT32  uResverd[ 6 ];      ///< 保留项，为以后版本做扩展
};
/// 初始化结构体
inline void BKENG_INIT(BKATRUN_SCAN_PARAM* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKATRUN_SCAN_PARAM);
    pStruct->uPolicy            = enumScanPolicyFull;
    memset(pStruct->uResverd, 0, sizeof(pStruct->uResverd));
}





/// 启动项修复参数
struct BKATRUN_REPAIR_PARAM
{
    UINT32      uSize;                  ///< 结构体大小

    // ------------------------------------------------------------
    // Version[1, ?]

    LPCWSTR     pRepairFile;            ///< [=NULL]    要修复的文件名或路径

    // ------------------------------------------------------------
    // Version[?, ?]
};
/// 初始化结构体
inline void BKENG_INIT(BKATRUN_REPAIR_PARAM* pStruct)
{
    assert(pStruct);
    pStruct->uSize              = sizeof(BKATRUN_REPAIR_PARAM);

    pStruct->pRepairFile        = NULL;
}






#define BKATR_PERF_COUNT__ACCESS_FILE               1
#define BKATR_PERF_COUNT__FIND_ALL_PATH_IN_CMDLINE  2
#define BKATR_PERF_COUNT__FIND_ALL_PATH_IN_FILELIST 3
#define BKATR_PERF_COUNT__FIND_FIRST_PATH_IN_TEXT   4
#define BKATR_PERF_COUNT__TRY_PATH_AND_EXTENSION    5
#define BKATR_PERF_COUNT__TRY_EXTENSION             6





NS_SKYLARK_END

#endif//BKATRUNDEF_H