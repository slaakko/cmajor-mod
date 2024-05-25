#	makefile for building the C++ runtime library of the Cmajor compiler with LLVM backend (cmc)
#	run from Visual Studio x64 native tools command prompt with 'mingw32-make' 
#	can be run parallelly by giving the -j option to the mingw32-make

include Makefile.common

all:
	make_lib_dir.bat
	$(MAKE) -C debug
	$(MAKE) -C release
	$(MAKE) -C resource
	
clean:
	$(RM) /Q lib\*.*
	$(MAKE) -C debug clean
	$(MAKE) -C release clean
	

install: 
	$(CP) lib\*.* ..\..\lib
