///////////////////////////////////////////////////////////////
//
//	Filename: 	KASCom.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2007-8-26   15:45
//	Comment:	
/*

        这组头文件包帮助你实现简单的组件。
		一共有3组你需要关心的宏:


		(1) KAS_BEGIN_COM_MAP( CXXX )
				KAS_COM_INTERFACE_ENTRY( IXXX ) 或 KAS_COM_INTERFACE_ENTRY_IID( IXXX )
		    KAS_END_COM_MAP()

			用于实现IUnknown:
			这组宏需要加入到你实现接口的类里面, 帮你实现IUnknown的3个虚函数,
			例:


			假如接口声明如下:
				struct __declspec( uuid( "7CCB588B-6746-47f4-9509-DDF3A4637A63" ) )
				ICompute: public IUnknown
				{
				public:
					virtual int __stdcall Add( int a, int b ) = 0;
					virtual int __stdcall Sub( int a, int b ) = 0;
				};


            你的实现: (只需要实现需要的接口, 再加入该组宏, 帮你实现IUnknown)
				class KCompute : public ICompute
				{
				public:
					virtual int __stdcall Add( int a, int b )
					{
						return a + b;
					}

					virtual int __stdcall Sub( int a, int b )
					{
						return a - b;
					}

					KAS_BEGIN_COM_MAP( KCompute )
						KAS_COM_INTERFACE_ENTRY( ICompute )
					KAS_END_COM_MAP()
				};



		(2) KAS_BEGIN_OBJECT_MAP()
				KAS_OBJECT_ENTRY( IXXX, CXXX )
			KAS_END_OBJECT_MAP()
			
			DllGetClassObject的实现
		    我们的简单COM规范不使用CLSID, 不使用类厂, 只需要通过一个接口ID从DLL中创建
			出实例, 而接口ID和其创建的实例的对应关系需要该组宏来指定:
			例: 
				KAS_BEGIN_OBJECT_MAP()
					KAS_OBJECT_ENTRY( ICompute, KCompute )
				KAS_END_OBJECT_MAP()



		(3) USE_DEFAULT_COM_PROC  或 USE_DEFAULT_COM_PROCEX
		   
			前者实现了 DllCanUnloadNow 和 DllGetClassObject, 
			后者实现了 DllCanUnloadNow 和 DllGetClassObject 和 DllMain
			省去你手工写这写函数.

			你也可以不使用这两个宏, 自己实现上述函数
		    
			最后, 不要忘了写.def文件  : )
			    LIBRARY      "xxxmodule"
				EXPORTS
					DllGetClassObject				PRIVATE
					DllCanUnloadNow					PRIVATE


*/
//
///////////////////////////////////////////////////////////////

#ifndef _KASCom_h_
#define _KASCom_h_
#include <Unknwn.h>
#include "KDllModule.h"
#include "KGetClassObject.h"





#define USE_DEFAULT_COM_PROC  KDllModule _Module; \
	STDAPI  DllCanUnloadNow(void){  \
		return _Module.DllCanUnloadNow();}  \
	STDAPI  DllGetClassObject(IN REFCLSID rclsid, IN REFIID riid, OUT LPVOID FAR* ppv){  \
		return _Module.DllGetClassObject( rclsid, riid, ppv );}


#define USE_DEFAULT_COM_PROCEX    USE_DEFAULT_COM_PROC \
	BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){ \
		if( ul_reason_for_call == DLL_PROCESS_ATTACH ) _Module.Init( hModule ); \
		return TRUE; }



#endif // _KASCom_h_