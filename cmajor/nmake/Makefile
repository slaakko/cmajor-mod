#	Makefile for Cmajor compiler & tools, system library, tool programs and examples.
#	Run with nmake from Visual Studio 2022 x64 Native Tools command prompt

#	Note: before running this Makefile for all targets you should:
#	(1) Create a bin directory under the cmajor directory and add the cmajor/bin directory to the PATH.
#	(2) Create environment variable CMAJOR_ROOT and set it to point to the cmajor directory.
#	(3) Run 'nmake zlibasm' from Visual Studio 2022 x64 Native Tools command prompt.
#	(4) Build the compiler using Visual Studio 2022 IDE by opening the cmajor-mod.sln
#		and building for Debug/x64 and Release/x64 configurations.
#		I have used Visual Studio 2022 Community Edition Version 17.11.4.

all: makedirs zlibasm runtime unicode sys tools examples install

makedirs:
	makedirs.bat

zlibasm:
	echo zlibasm:
	cd ..\..\ext\zlib-1.2.11\contrib\masmx64 && bld_ml64.bat

compiler: 
#	this rule does not succeed, see note at the top of this file for resolution.
	echo compiler:
	MSBuild -m -verbosity:minimal ..\..\cmajor-mod.sln -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\..\cmajor-mod.sln -p:Configuration="Release"
	cd ..\..\inst && inst.bat

runtime:
	echo runtime:
	cd ../llvm/build && mingw32-make -j
	cd ../llvm/build && mingw32-make install
	cd ../cpp/build && mingw32-make -j
	cd ../cpp/build && mingw32-make install

systemx:
	echo systemx:
	sxcmc -u -v ../system-x/system/System.cms
	sxcmc -u -v -c=release ../system-x/system/System.cms

vstask:	
	echo vstask:
	MSBuild -m -verbosity:minimal ..\task\task.sln -p:Configuration="Debug" -p:Platform="Any CPU"
	MSBuild -m -verbosity:minimal ..\task\task.sln -p:Configuration="Release" -p:Platform="Any CPU"
	
unicode:
	echo unicode:
	cd ..\..\inst && inst.bat
	cd ..\unicode && build.bat
	
sys:
	echo sys:
	cd ..\..\inst && inst.bat
	cmc -v ../system/System.Windows.cms
	cmc -v -c=release ../system/System.Windows.cms
	cppcmc -v ../system/System.Windows.cms
	cppcmc -v -c=release ../system/System.Windows.cms
	masmcmc -v ../system/System.Windows.cms
	masmcmc -v -c=release ../system/System.Windows.cms

tools:
	echo tools:
	cmc -v ../projects/tools/soulcm/soulcm.cms
	cmc -v -c=release ../projects/tools/soulcm/soulcm.cms
	cmc -v ../projects/tools/spring/spring.cms
	cmc -v -c=release ../projects/tools/spring/spring.cms
	masmcmc -v ../projects/tools/compiler/compiler.cms
	masmcmc -v -c=release ../projects/tools/compiler/compiler.cms
	
examples:
	echo examples:
	cmc -sv ../projects/examples/examples.cms
	cmc -sv -c=release ../projects/examples/examples.cms

install:
	cd ..\..\inst && inst.bat
