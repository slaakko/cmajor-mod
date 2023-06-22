include Makefile.common

ifeq ($(PLATFORM),windows)

all:
	$(MAKE) -C static/debug
	$(MAKE) -C shared/debug
	$(MAKE) -C static/release
	$(MAKE) -C shared/release
	
clean:
	$(RM) /Q bin\*.*
	$(RM) /Q lib\*.*
	$(MAKE) -C static/debug clean
	$(MAKE) -C shared/debug clean
	$(MAKE) -C static/release clean
	$(MAKE) -C shared/release clean
	
else

all:
	$(MAKE) -C static/debug
	$(MAKE) -C static/release

clean:
	$(RM) bin/*
	$(RM) lib/*
	$(MAKE) -C static/debug clean
	$(MAKE) -C static/release clean

endif

install: $(PLATFORM)	
	
windows:
	$(CP) lib\*.a ..\..\lib
	$(CP) bin\* ..\..\bin
	
linux:
	$(CP) lib/*.a ../../lib
