根目录Build:
1．目录删除文件列表
	名称: deldir.list
	用途：构建中指定要删除目录的文件列表接口
   格式：filename
　 参数：filename：相对库目录要删除的目录
　 备注：对于增量编译，最好设好删除相对工程目录下的release目录
   例子:
{{{
Build
Src
Product/win
 }}}
2．指定要编译工程的列表：solutionfile.list
　 格式：filename[,argname[,toolname]]
　 说明：
     filename：相对库目录的工程文件/解决方案文件，
　　 argname：编译参数，默认/t:build /p:Configuration=Release参数
　　 toolname：编译工具，默认msbuild.exe工具
   备注：不同编译参数的首行不得省略argname参数
　　　　 不同编译工具的首行不得省略toolname参数
   参考：Build\solutionfile.list
3．文件签名文件列表
	名称: signfile.list
	用途: 构建中指定为文件签名的文件列表接口
　 格式：filename[,filetype][,filenote]
　 参数：
　　filename: 相对库目录要打数字签名的文件
     filetype: 支持三种签名方式:PE(正常打法,类型为文件的扩展名)/SYS(基于visat环境的驱动文件sys，类型为文件的扩展名)/VSA(基于visat环境以上的安全中心文件,必须在vista系统操作才生效，类型为VSA)
    filenote: 签名的描述信息,如有中文则文本文件需存为utf-8格式
	备注：不同扩展名的文件首行不得省略filetype参数值
	例子:
{{{
aaa.dll,dll
a\bbb.dll
a\ccc.exe,exe
}}}
4．外部数据文件列表
	名称: getdata.list
	用途：构建中指定要copy的文件列表接口 
   格式：srcpath,dstpath
   参数：srcpath：相对dubabin根目录的相对路径
	dstpath：相对svn仓库根目录的相对路径
   备注： 
   例子:
{{{
DubaTest\KIS\SelfData\makesetup_installfile,product\win32\
DubaTest\KIS\SelfData\miniksg2010,product\win32\miniksg\
 }}}
5. 指定要修改rc文件的列表：verfilelist.list
   格式：filename,type,mmr,comp,same
   说明：
       filename：相对库目录的rc2文件
       type：文件类型,0:vc工程;1:c#工程
       mmr：定义rc2文件中ProductVersion字段值的前两位或前三位,形式如1.0,1.0.1.根据版本号的格式定义.
       comp:定义rc文件中VALUE "CompanyName"的值,不需要加引号
       same:定义rc文件中FileVersion与ProductVersion是否取同值.0同值,非0不同值.
   备注：三个参数不得为空,逗号隔开
   例子:
{{{
include\buildcfg\aaa\VersionNo.rc2,0,1.0
include\buildcfg\bbb\VersionNo.rc2,0,3.0
include\buildcfg\bbb\VersionNo.cs,1,1.0
include\buildcfg\VersionNo.rc2,0,2.0.1,Kingsoft Corporation,0
include\buildcfg\VersionNo.rc2,0,3.7,Kingsoft Corporation,1
 }}}