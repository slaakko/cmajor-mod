@echo off
pushd cm.lexer
call build.bat
popd
pushd cm.parser
call build.bat
popd
pushd common.parser
call build.bat
popd
pushd re.lexer
call build.bat
popd
pushd re.parser
call build.bat
popd
pushd scmlg.parser
call build.bat
popd
pushd scmpg.parser
call build.bat
popd
