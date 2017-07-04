@echo off
SETLOCAL

set _ROOT=%CD%
pushd external\reshade
CALL "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
msbuild ReShade.sln /property:Configuration=Release "/property:Platform=32-bit"
msbuild ReShade.sln /property:Configuration=Release "/property:Platform=64-bit"
popd

if not exist build\64 mkdir build\64
pushd build\64
cmake --build . --config Release
popd

if not exist build\32 mkdir build\32
pushd build\32
cmake --build . --config Release
popd

ENDLOCAL
