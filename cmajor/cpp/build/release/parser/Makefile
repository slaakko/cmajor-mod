include ../../Makefile.common

OBJECTS = match.o value.o

LIBRARY_NAME=libcmajor.cpp.parser.release.a

SOURCE_PREFIX=../../../../rt/parser

LIBRARY_PREFIX=../../lib

CXXFLAGS += -std=c++20 -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -I$(BOOST_INCLUDE_DIR) -DNDEBUG -O2 -DUNICODE

CFLAGS += -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -DNDEBUG -O2

%.o: $(SOURCE_PREFIX)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.o: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

clean:
	$(RM) *.o
