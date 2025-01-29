include ../../Makefile.common

OBJECTS = ascii.o classes.o condition_variable.o debug.o \
directory.o env.o error.o execute.o fiber.o init_done.o \
io.o lex.o math.o memory.o mutex.o process.o random.o \
socket.o strings.o thread.o time.o trace.o unit_test.o \
util.o zlib.o 

LIBRARY_NAME=libcmajor.cpp.cmrt.release.a

SOURCE_PREFIX=../../../../rt/cmrt

LIBRARY_PREFIX=../../lib

CXXFLAGS += -std=c++20 -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -I$(BOOST_INCLUDE_DIR) -O2 -DUNICODE -DNDEBUG

CFLAGS += -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -O2 -DNDEBUG

%.o: $(SOURCE_PREFIX)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.o: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

clean:
	$(RM) *.o
