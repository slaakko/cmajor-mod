include ../../Makefile.common

OBJECTS = adler32.obj compress.obj crc32.obj deflate.obj gzclose.obj gzlib.obj gzread.obj gzwrite.obj inffast.obj inflate.obj infback.obj inftrees.obj trees.obj uncompr.obj zutil.obj

LIBRARY_NAME=z.lib

SOURCE_PREFIX=../../../../../ext/zlib-1.2.11

LIBRARY_PREFIX=../../lib

CFLAGS += -I$(SOURCE_PREFIX) -DNDEBUG -O2

%.obj: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	llvm-lib /out:$@ $(OBJECTS)

clean:
	$(RM) *.obj
