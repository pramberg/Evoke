@echo off
pushd %~dp0\..\
call ThirdParty\Premake\Windows\premake5.exe vs2019
popd
timeout 5