# 	makefile for building the C++ runtime library for the Cmajor compiler with C++ backend (cppcmc)
#	on windows run with 'mingw32-make' from command prompt
#	can be run parallelly by giving the -j option to the mingw32-make

include Makefile.common

ifeq ($(PLATFORM),windows)

all:
	make_lib_dir.bat
	$(MAKE) -C debug
	$(MAKE) -C release
	$(MAKE) -C resource
	$(MAKE) -C resource install
	
clean:
	$(RM) /Q lib\*.*
	$(MAKE) -C debug clean
	$(MAKE) -C release clean
	
else

all:
	$(MAKE) -C debug
	$(MAKE) -C release
	$(MAKE) -C resource
	$(MAKE) -C resource install

clean:
	$(RM) bin/*
	$(RM) lib/*
	$(MAKE) -C debug clean
	$(MAKE) -C release clean
	$(MAKE) -C resource clean

endif

install: $(PLATFORM)	
	
windows:
	$(CP) lib\*.* ..\..\lib
	
linux:
	$(CP) lib/*.* ../../lib
