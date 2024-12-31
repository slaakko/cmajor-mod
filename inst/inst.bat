@echo off
IF NOT EXIST cminst.exe xcopy /Y /D ..\x64\Release\cminst.exe .
IF NOT EXIST ..\cmajor\bin mkdir ..\cmajor\bin
IF NOT EXIST ..\cmajor\lib mkdir ..\cmajor\lib
IF NOT EXIST ..\cmajor\rc mkdir ..\cmajor\rc
IF NOT EXIST ..\rc mkdir ..\rc
cminst -v ..\x64\Debug\*.exe ..\cmajor\bin
cminst -v ..\x64\Release\*.exe ..\cmajor\bin
cminst -v ..\x64\Debug\*.dll ..\cmajor\bin
cminst -v ..\x64\Release\*.dll ..\cmajor\bin
cminst -v ..\x64\Debug\*.lib ..\cmajor\lib
cminst -v ..\x64\Release\*.lib ..\cmajor\lib
cminst -v ..\cmajor\lexer\*.classmap.compressed ..\rc
cminst -v ..\cmajor\lexer\*.classmap.rc ..\cmajor\rc
cminst -v ..\soul\rex\*.classmap.compressed ..\cmajor\rc
cminst -v ..\soul\rex\*.classmap.rc ..\cmajor\rc
cminst -v ..\soul\xml\xpath\soul.xml.xpath.lexer.classmap.compressed ..\cmajor\rc
cminst -v ..\soul\xml\xpath\soul.xml.xpath.lexer.classmap.rc ..\cmajor\rc
cminst -v ..\cmajor\projects\tools\soulcm\scmlg\bin\llvm\release\2\scmlg.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\soulcm\scmpg\bin\llvm\release\2\scmpg.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\ref2html\bin\llvm\release\2\ref2html.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\soulcm\cm2html\bin\llvm\release\2\cm2html.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\soulcm\soul2html\bin\llvm\release\2\soul2html.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\txt2html\bin\llvm\release\2\txt2html.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\compiler\cmcmc\bin\llvm\release\2\cmcmc.exe ..\cmajor\bin
