@echo off
sxcmc -rv System.cms
sxcmc -rv -c=release System.cms
pushd Main
call build.bat
popd

