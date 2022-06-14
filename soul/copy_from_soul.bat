@echo off
xcopy /F /Y ..\..\soul-mod\soul\ast\*.ixx ast\
xcopy /F /Y ..\..\soul-mod\soul\ast\*.cpp ast\

xcopy /F /Y ..\..\soul-mod\soul\lexer\*.ixx lexer\
xcopy /F /Y ..\..\soul-mod\soul\lexer\*.cpp lexer\

xcopy /F /Y ..\..\soul-mod\soul\parser\*.ixx parser\
xcopy /F /Y ..\..\soul-mod\soul\parser\*.cpp parser\

xcopy /F /Y ..\..\soul-mod\soul\xml\dom\*.ixx xml\dom\
xcopy /F /Y ..\..\soul-mod\soul\xml\dom\*.cpp xml\dom\

xcopy /F /Y ..\..\soul-mod\soul\xml\dom_parser\*.ixx xml\dom_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\dom_parser\*.cpp xml\dom_parser\

xcopy /F /Y ..\..\soul-mod\soul\xml\processor\*.ixx xml\processor\
xcopy /F /Y ..\..\soul-mod\soul\xml\processor\*.cpp xml\processor\

xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.ixx xml\xml_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.cpp xml\xml_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.parser xml\xml_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.spg xml\xml_parser\
