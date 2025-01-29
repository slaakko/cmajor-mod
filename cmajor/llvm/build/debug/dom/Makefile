include ../../Makefile.common

OBJECTS = attribute_node.obj axis.obj cdata_section.obj character_data.obj comment.obj \
document.obj document_fragment.obj element.obj entity_reference.obj error.obj index.obj \
node.obj node_operation.obj parent_node.obj processing_instruction.obj text.obj \
visitor.obj

LIBRARY_NAME=cmajor.llvm.dom.debug.lib

SOURCE_PREFIX=../../../../rt/dom

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
