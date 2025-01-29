include ../../Makefile.common

OBJECTS = base.obj classmap.obj concepts.obj error.obj \
file_map.obj keyword.obj lexeme.obj lexer.obj lexer_class.obj \
lexing_util.obj parsing_log.obj token.obj token_parser.obj \
trivial_lexer.obj variables.obj xml_parsing_log.obj

LIBRARY_NAME=cmajor.llvm.lexer.release.lib

SOURCE_PREFIX=../../../../rt/lexer

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
