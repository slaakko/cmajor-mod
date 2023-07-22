include ../Makefile.common

all:
	windres ../../xpath/soul.xml.xpath.lexer.classmap.rc -o soul.xml.xpath.lexer.classmap.o
	ld --dll -o soul.xml.xpath.lexer.classmap.dll soul.xml.xpath.lexer.classmap.o
	$(CP) soul.xml.xpath.lexer.classmap.dll ..\bin
