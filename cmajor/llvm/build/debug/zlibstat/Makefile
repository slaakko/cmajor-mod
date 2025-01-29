include ../../Makefile.common

OBJECTS = adler32.obj compress.obj crc32.obj deflate.obj gzclose.obj gzlib.obj gzread.obj \
       gzwrite.obj infback.obj inflate.obj inftrees.obj inffast.obj trees.obj uncompr.obj zutil.obj

LIBRARY_NAME=zd.lib

SOURCE_PREFIX=../../../../../ext/zlib-1.2.11

LIBRARY_PREFIX=../../lib

CFLAGS += -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -g -O0

%.obj: $(SOURCE_PREFIX)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.obj: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	llvm-lib /out:$@ $(OBJECTS)

clean:
	$(RM) *.obj
