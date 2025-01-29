include ../../Makefile.common

ifeq ($(PLATFORM),windows)
OBJECTS = context.o evaluate.o expr.o function.o object.o token_parser.o xpath.lexer.o xpath.parser.o xpath_rules.o 
else
OBJECTS = context.o evaluate.o expr.o function.o object.o token_parser.o xpath.lexer.o xpath.parser.o xpath_rules.o
endif

LIBRARY_NAME=libcmajor.cpp.xpath.debug.a

SOURCE_PREFIX=../../../../rt/xpath

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
