include ../../Makefile.common

OBJECTS = context.obj evaluate.obj expr.obj function.obj object.obj token_parser.obj xpath.lexer.obj xpath.parser.obj xpath_rules.obj

LIBRARY_NAME=cmajor.llvm.xpath.release.lib

SOURCE_PREFIX=../../../../rt/xpath

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
