@echo off
xcopy /F /Y ..\..\soul-mod\soul\ast\*.ixx ast\
xcopy /F /Y ..\..\soul-mod\soul\ast\*.cpp ast\

xcopy /F /Y ..\..\soul-mod\soul\lexer\*.ixx lexer\
xcopy /F /Y ..\..\soul-mod\soul\lexer\*.cpp lexer\

xcopy /F /Y ..\..\soul-mod\soul\parser\*.ixx parser\
xcopy /F /Y ..\..\soul-mod\soul\parser\*.cpp parser\

xcopy /F /Y ..\..\soul-mod\soul\xml\dom\*.ixx xml\dom\
xcopy /F /Y ..\..\soul-mod\soul\xml\dom\*.cpp xml\dom\
