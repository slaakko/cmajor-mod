@echo off
IF NOT EXIST ..\cmajor\bin mkdir ..\cmajor\bin
IF NOT EXIST ..\cmajor\lib mkdir ..\cmajor\lib
IF NOT EXIST ..\cmajor\rc mkdir ..\cmajor\rc
IF NOT EXIST ..\rc mkdir ..\rc
xcopy /Y ..\x64\Debug\*.exe ..\cmajor\bin
xcopy /Y ..\x64\Release\*.exe ..\cmajor\bin
xcopy /Y ..\x64\Debug\*.lib ..\cmajor\lib
xcopy /Y ..\x64\Release\*.lib ..\cmajor\lib
xcopy /Y ..\cmajor\lexer\*.classmap.compressed ..\rc
xcopy /Y ..\cmajor\lexer\*.classmap.rc ..\cmajor\rc
xcopy /Y ..\soul\rex\*.classmap.compressed ..\cmajor\rc
xcopy /Y ..\soul\rex\*.classmap.rc ..\cmajor\rc
xcopy /Y ..\soul\xml\xpath\soul.xml.xpath.lexer.classmap.compressed ..\cmajor\rc
xcopy /Y ..\soul\xml\xpath\soul.xml.xpath.lexer.classmap.rc ..\cmajor\rc
IF EXIST ..\cmajor\projects\tools\soulcm\scmlg\bin\llvm\release\2\scmlg.exe xcopy /Y ..\cmajor\projects\tools\soulcm\scmlg\bin\llvm\release\2\scmlg.exe ..\cmajor\bin
IF EXIST ..\cmajor\projects\tools\soulcm\scmpg\bin\llvm\release\2\scmpg.exe xcopy /Y ..\cmajor\projects\tools\soulcm\scmpg\bin\llvm\release\2\scmpg.exe ..\cmajor\bin
iF EXIST ..\cmajor\projects\tools\ref2html\bin\llvm\release\2\ref2html.exe xcopy /Y ..\cmajor\projects\tools\ref2html\bin\llvm\release\2\ref2html.exe ..\cmajor\bin
IF EXIST ..\cmajor\projects\tools\soulcm\cm2html\bin\llvm\release\2\cm2html.exe xcopy /Y ..\cmajor\projects\tools\soulcm\cm2html\bin\llvm\release\2\cm2html.exe ..\cmajor\bin
IF EXIST ..\cmajor\projects\tools\soulcm\soul2html\bin\llvm\release\2\soul2html.exe xcopy /Y ..\cmajor\projects\tools\soulcm\soul2html\bin\llvm\release\2\soul2html.exe ..\cmajor\bin
IF EXIST ..\cmajor\projects\tools\txt2html\bin\llvm\release\2\txt2html.exe xcopy /Y ..\cmajor\projects\tools\txt2html\bin\llvm\release\2\txt2html.exe ..\cmajor\bin
IF EXIST ..\cmajor\projects\tools\spring\bin\llvm\release\2\spring.exe xcopy /Y ..\cmajor\projects\tools\spring\bin\llvm\release\2\spring.exe ..\cmajor\bin
IF EXIST ..\cmajor\projects\tools\compiler\cmcmc\bin\masm\release\2\cmcmc.exe xcopy /Y ..\cmajor\projects\tools\compiler\cmcmc\bin\masm\release\2\cmcmc.exe ..\cmajor\bin
