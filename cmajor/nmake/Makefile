#	Makefile for Cmajor compiler & tools, system library, tool programs and examples.
#	Run with nmake from Visual Studio 2022 x64 Native Tools command prompt

all: makedirs zlibasm runtime unicode sys tools examples install

makedirs:
	makedirs.bat

zlibasm:
	cd ..\..\ext\zlib-1.2.11\contrib\masmx64 && bld_ml64.bat

compiler: 
	MSBuild -m -verbosity:minimal ..\..\cmajor-mod.sln -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\..\cmajor-mod.sln -p:Configuration="Release"
	cd ..\..\inst && inst.bat

runtime:
	cd ../llvm/build && mingw32-make -j
	cd ../llvm/build && mingw32-make install
	cd ../cpp/build && mingw32-make -j
	cd ../cpp/build && mingw32-make install

systemx:
	sxcmc -u -v ../system-x/system/System.cms
	sxcmc -u -v -c=release ../system-x/system/System.cms

vstask:	
	MSBuild -m -verbosity:minimal ..\task\task.sln -p:Configuration="Debug" -p:Platform="Any CPU"
	MSBuild -m -verbosity:minimal ..\task\task.sln -p:Configuration="Release" -p:Platform="Any CPU"
	
unicode:
	cd ..\..\inst && inst.bat
	cd ..\unicode && build.bat
	
sys:
	cd ..\..\inst && inst.bat
	cmc -rv ../masm/system/System.cms
	cmc -rv -c=release ../masm/system/System.cms
	cppcmc -rv ../masm/system/System.cms
	cppcmc -rv -c=release ../masm/system/System.cms
	masmcmc -rv ../masm/system/System.cms
	masmcmc -rv -c=release ../masm/system/System.cms

tools:
	cmc -rv ../projects/tools/soulcm/soulcm.cms
	cmc -rv -c=release ../projects/tools/soulcm/soulcm.cms
	
examples:
	cmc -rv ../projects/examples/examples.cms
	cmc -rv -c=release ../projects/examples/examples.cms

install:
	cd ..\..\inst && inst.bat
