@echo off
IF NOT EXIST cminst.exe xcopy /Y /D ..\x64\Release\cminst.exe .
IF NOT EXIST ..\cmajor\bin mkdir ..\cmajor\bin
IF NOT EXIST ..\cmajor\lib mkdir ..\cmajor\lib
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
cminst -v ..\cmajor\projects\tools\soulcm\scm2html\bin\release\scm2html.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\soulcm\scmlg\bin\release\scmlg.exe ..\cmajor\bin
cminst -v ..\cmajor\projects\tools\soulcm\scmpg\bin\release\scmpg.exe ..\cmajor\bin
