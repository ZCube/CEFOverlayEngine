@echo off
SETLOCAL
CALL "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
set _ROOT=%CD%

pushd prebuilt
py -3 init_prebuilt.py
popd

if not exist build\64 mkdir build\64
pushd build\64
cmake -G "Visual Studio 15 2017 Win64" %_ROOT% ^
	"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=%_ROOT%/bin/64" ^
	"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=%_ROOT%/lib/64" ^
	"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=%_ROOT%/bin/64"
popd

if not exist build\32 mkdir build\32
pushd build\32
cmake -G "Visual Studio 15 2017" %_ROOT% ^
	"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=%_ROOT%/bin/32" ^
	"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=%_ROOT%/lib/32" ^
	"-DCMAKE_RUNTIME_OUTPUT_DIRECTORY:PATH=%_ROOT%/bin/32"
popd

ENDLOCAL
