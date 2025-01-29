include ../../Makefile.common

OBJECTS = attribute.obj content_handler.obj entity.obj xml_processor.obj

LIBRARY_NAME=cmajor.llvm.xml_processor.release.lib

SOURCE_PREFIX=../../../../rt/xml_processor

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
