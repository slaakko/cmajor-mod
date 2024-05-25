include ../Makefile.common

soul.xml.xpath.lexer.classmap.res: ../../../rt/xpath/soul.xml.xpath.lexer.classmap.rc
	llvm-rc ../../../rt/xpath/soul.xml.xpath.lexer.classmap.rc
	$(CP) ..\..\..\rt\xpath\soul.xml.xpath.lexer.classmap.res ..\lib

clean:
	$(RM) *.res
