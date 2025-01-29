include ../../Makefile.common

ifeq ($(PLATFORM),windows)
OBJECTS = ansi.o binary_resource_ptr.o \
binary_stream_reader.o binary_stream_writer.o buffered_stream.o code_formatter.o \
compression.o deflate_stream.o error.o fiber.o file_stream.o \
init_done.o memory_reader.o memory_stream.o memory_writer.o \
multiprecision.o path.o process.o rand.o sha1.o socket.o stream.o system.o \
text_util.o time.o unicode.o unicode_conversion.o unicode_engine.o uuid.o zlib_interface.o
else
OBJECTS = ansi.o binary_stream_reader.o binary_stream_writer.o buffered_stream.o code_formatter.o \
compression.o deflate_stream.o error.o file_stream.o \
init_done.o memory_reader.o memory_stream.o memory_writer.o \
multiprecision.o path.o process.o rand.o sha1.o socket.o stream.o system.o \
text_util.o time.o unicode.o unicode_conversion.o unicode_engine.o uuid.o zlib_interface.o
endif

LIBRARY_NAME=libcmajor.cpp.util.debug.a

SOURCE_PREFIX=../../../../rt/util

LIBRARY_PREFIX=../../lib

CXXFLAGS += -std=c++20 -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -I$(BOOST_INCLUDE_DIR) -g -O0 -DUNICODE

CFLAGS += -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -g -O0

%.o: $(SOURCE_PREFIX)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.o: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

clean:
	$(RM) *.o
