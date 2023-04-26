@echo off
xcopy /F /Y ..\..\soul-mod\soul\ast\*.cppm ast\
xcopy /F /Y ..\..\soul-mod\soul\ast\*.cpp ast\

xcopy /F /Y ..\..\soul-mod\soul\lexer\*.cppm lexer\
xcopy /F /Y ..\..\soul-mod\soul\lexer\*.cpp lexer\

xcopy /F /Y ..\..\soul-mod\soul\parser\*.cppm parser\
xcopy /F /Y ..\..\soul-mod\soul\parser\*.cpp parser\

xcopy /F /Y ..\..\soul-mod\soul\xml\dom\*.cppm xml\dom\
xcopy /F /Y ..\..\soul-mod\soul\xml\dom\*.cpp xml\dom\

xcopy /F /Y ..\..\soul-mod\soul\xml\dom_parser\*.cppm xml\dom_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\dom_parser\*.cpp xml\dom_parser\

xcopy /F /Y ..\..\soul-mod\soul\xml\processor\*.cppm xml\processor\
xcopy /F /Y ..\..\soul-mod\soul\xml\processor\*.cpp xml\processor\

xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.cppm xml\xml_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.cpp xml\xml_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.parser xml\xml_parser\
xcopy /F /Y ..\..\soul-mod\soul\xml\xml_parser\*.spg xml\xml_parser\
