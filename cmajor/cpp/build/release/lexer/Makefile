include ../../Makefile.common

OBJECTS = base.o classmap.o concepts.o error.o \
file_map.o keyword.o lexeme.o lexer.o lexer_class.o \
lexing_util.o parsing_log.o token.o token_parser.o \
trivial_lexer.o variables.o xml_parsing_log.o

LIBRARY_NAME=libcmajor.cpp.lexer.release.a

SOURCE_PREFIX=../../../../rt/lexer

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
