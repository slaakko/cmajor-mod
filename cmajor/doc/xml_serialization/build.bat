@echo off
pushd ref
call build.bat
popd
ref2html -v xml_serialization.ref
