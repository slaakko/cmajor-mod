include ../../Makefile.common

OBJECTS = attribute_node.o axis.o cdata_section.o character_data.o comment.o \
document.o document_fragment.o element.o entity_reference.o error.o index.o \
node.o node_operation.o parent_node.o processing_instruction.o text.o \
visitor.o

LIBRARY_NAME=libcmajor.cpp.dom.release.a

SOURCE_PREFIX=../../../../rt/dom

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
