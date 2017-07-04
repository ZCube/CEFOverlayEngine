mkdir dist\module\32
xcopy /hrkys /exclude:exclude_files_2.txt bin\32\Release\loader.* dist\module\loader32_mod.*
xcopy /hrkys /exclude:exclude_files_2.txt bin\32\Release\CefOverlay.* dist\module\32\overlay_mod.*
xcopy /hrkys external\reshade\bin\Win32\Release\ReShade32.dll dist\module\d3d9.*

mkdir dist\module\64
xcopy /hrkys /exclude:exclude_files_2.txt bin\64\Release\loader.* dist\module\loader64_mod.*
xcopy /hrkys /exclude:exclude_files_2.txt bin\64\Release\CefOverlay.* dist\module\64\overlay_mod.*
xcopy /hrkys external\reshade\bin\x64\Release\ReShade64.dll dist\module\dxgi.*

mkdir dist\runtime\32
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\32\Resources\* dist\runtime\32
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\32\Release\* dist\runtime\32

mkdir dist\runtime\64
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\64\Resources\* dist\runtime\64
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\64\Release\* dist\runtime\64

mkdir dist\with_runtime\32
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\32\Resources\* dist\with_runtime\32
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\32\Release\* dist\with_runtime\32
xcopy /hrkys /exclude:exclude_files_2.txt bin\32\Release\loader.* dist\with_runtime\loader32_mod.*
xcopy /hrkys /exclude:exclude_files_2.txt bin\32\Release\CefOverlay.* dist\with_runtime\32\overlay_mod.*
xcopy /hrkys external\reshade\bin\Win32\Release\ReShade32.dll dist\with_runtime\d3d9.*

mkdir dist\with_runtime\64
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\64\Resources\* dist\with_runtime\64
xcopy /hrkys /exclude:exclude_files_2.txt prebuilt\cef\64\Release\* dist\with_runtime\64
xcopy /hrkys /exclude:exclude_files_2.txt bin\64\Release\loader.* dist\with_runtime\loader64_mod.*
xcopy /hrkys /exclude:exclude_files_2.txt bin\64\Release\CefOverlay.* dist\with_runtime\64\overlay_mod.*
xcopy /hrkys external\reshade\bin\x64\Release\ReShade64.dll dist\with_runtime\dxgi.*
