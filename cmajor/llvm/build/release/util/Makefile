include ../../Makefile.common

OBJECTS = ansi.obj binary_resource_ptr.obj \
binary_stream_reader.obj binary_stream_writer.obj buffered_stream.obj code_formatter.obj \
compression.obj deflate_stream.obj error.obj fiber.obj file_stream.obj \
init_done.obj memory_reader.obj memory_stream.obj memory_writer.obj \
path.obj rand.obj sha1.obj socket.obj stream.obj system.obj \
text_util.obj time.obj unicode.obj unicode_conversion.obj unicode_engine.obj uuid.obj zlib_interface.obj

LIBRARY_NAME=cmajor.llvm.util.release.lib

SOURCE_PREFIX=../../../../rt/util

LIBRARY_PREFIX=../../lib

CXXFLAGS += -std=c++20 -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -DNDEBUG -O2 -DUNICODE

CFLAGS += -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -DNDEBUG -O2

%.obj: $(SOURCE_PREFIX)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.obj: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	llvm-lib /out:$@ $(OBJECTS)

clean:
	$(RM) *.obj
