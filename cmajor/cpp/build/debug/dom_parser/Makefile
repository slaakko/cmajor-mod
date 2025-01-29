include ../../Makefile.common

OBJECTS = document_handler.o dom_parser.o 

LIBRARY_NAME=libcmajor.cpp.dom_parser.debug.a

SOURCE_PREFIX=../../../../rt/dom_parser

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
