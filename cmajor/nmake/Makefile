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
	install_vstask.bat
	
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

cmsx:
	cmc -u -v -st -bt=1 ../projects/cmsx/cmsx.cms
	cmc -u -v -c=release -st -bt=1 ../projects/cmsx/cmsx.cms

xmlSerializationTests:
	cmc -u -v -bt=1 ../projects/xmlSerializationTests/xmlSerializationTests.cms
	cmc -u -v -bt=1 -c=release ../projects/xmlSerializationTests/xmlSerializationTests.cms

preinstallation_check:
	MSBuild -m -verbosity:minimal ..\installation\checkcmajor\checkcmajor.vcxproj -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\installation\checkcmajor\checkcmajor.vcxproj -p:Configuration="Release"

plain_package:
	wingstall -v --create-package ..\..\setup\plain\cmajor.xml
	
plain_setup:	
	wingstall -v --make-setup ..\..\setup\plain\cmajor.bin
	MSBuild -m -verbosity:minimal ..\..\setup\plain\program\setup.vcxproj -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\..\setup\plain\program\setup.vcxproj -p:Configuration="Release"

deflate_package:
	wingstall -v --create-package ..\..\setup\deflate\cmajor.xml
	
deflate_setup:	
	wingstall -v --make-setup ..\..\setup\deflate\cmajor.bin
	MSBuild -m -verbosity:minimal ..\..\setup\deflate\program\setup.vcxproj -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\..\setup\deflate\program\setup.vcxproj -p:Configuration="Release"

bzip2_package:
	wingstall -v --create-package ..\..\setup\bzip2\cmajor.xml
	
bzip2_setup:	
	wingstall -v --make-setup ..\..\setup\bzip2\cmajor.bin
	MSBuild -m -verbosity:minimal ..\..\setup\bzip2\program\setup.vcxproj -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\..\setup\bzip2\program\setup.vcxproj -p:Configuration="Release"
	
packages: preinstallation_check plain_package deflate_package bzip2_package
	
setups: plain_setup deflate_setup bzip2_setup
	cminst -v ..\..\setup\plain\program\x64\Debug\setupd.exe ..\..\setup\plain\bin
	cminst -v ..\..\setup\plain\program\x64\Release\setup.exe ..\..\setup\plain\bin
	cminst -v ..\..\setup\deflate\program\x64\Debug\setupd.exe ..\..\setup\deflate\bin
	cminst -v ..\..\setup\deflate\program\x64\Release\setup.exe ..\..\setup\deflate\bin
	cminst -v ..\..\setup\bzip2\program\x64\Debug\setupd.exe ..\..\setup\bzip2\bin
	cminst -v ..\..\setup\bzip2\program\x64\Release\setup.exe ..\..\setup\bzip2\bin

packages_and_setups: packages setups

olddevenv:
	MSBuild -m -verbosity:minimal ..\..\devenv\devenv.sln -p:Configuration="Debug"
	MSBuild -m -verbosity:minimal ..\..\devenv\devenv.sln -p:Configuration="Release"
	
install:
	cd ..\..\inst && inst.bat
