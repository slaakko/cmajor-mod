include ../../Makefile.common

OBJECTS = xml_document.parser.obj xml_parser.obj xml_parser_rules.obj

LIBRARY_NAME=cmajor.llvm.xml_parser.debug.lib

SOURCE_PREFIX=../../../../rt/xml_parser

LIBRARY_PREFIX=../../lib

CXXFLAGS += -std=c++20 -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -g -O0 -DUNICODE

CFLAGS += -I$(SOURCE_PREFIX)/../.. -I$(SOURCE_PREFIX)/.. -I$(SOURCE_PREFIX) -g -O0

%.obj: $(SOURCE_PREFIX)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.obj: $(SOURCE_PREFIX)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(LIBRARY_PREFIX)/$(LIBRARY_NAME): $(OBJECTS)
	llvm-lib /out:$@ $(OBJECTS)

clean:
	$(RM) *.obj
