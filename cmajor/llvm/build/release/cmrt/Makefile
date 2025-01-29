include ../../Makefile.common

OBJECTS = ascii.obj classes.obj condition_variable.obj debug.obj \
directory.obj env.obj error.obj execute.obj fiber.obj init_done.obj \
io.obj lex.obj math.obj memory.obj mutex.obj process.obj random.obj \
socket.obj strings.obj thread.obj time.obj trace.obj unit_test.obj \
util.obj zlib.obj

LIBRARY_NAME=cmajor.llvm.cmrt.release.lib

SOURCE_PREFIX=../../../../rt/cmrt

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
