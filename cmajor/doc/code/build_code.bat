@echo off
cmc -rv code.cms
cppcmc -rv code.cms
masmcmc -rv code.cms
cmcmc -rv code.cms
pushd solution
call build.bat
popd
pushd xml
call build.bat
popd
pushd xml_serialization
call build.bat
popd
