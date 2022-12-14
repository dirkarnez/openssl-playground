REM run as Administrator
@echo off

cd /d %~dp0
set DOWNLOADS_DIR=%USERPROFILE%\Downloads
set DOWNLOADS_DIR_LINUX=%DOWNLOAD_DIR:\=/%
@REM SET PATH=^
@REM %DOWNLOADS_DIR%\PortableGit\bin;^
@REM %DOWNLOADS_DIR%\x86_64-8.1.0-release-posix-seh-rt_v6-rev0;^
@REM %DOWNLOADS_DIR%\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\bin;^
@REM %DOWNLOADS_DIR%\cmake-3.22.2-windows-x86_64\bin;^

set PATH=^
D:\Softwares\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\mingw64;^
D:\Softwares\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\mingw64\bin;^
D:\Softwares\cmake-3.23.0-rc1-windows-x86_64\bin;

set current_directory=%~dp0
set current_directory_linux=%current_directory:\=/%

rmdir build

cmake.exe -G"MinGW Makefiles" ^
-DOpenSSL_DIR="%current_directory_linux%/cpk-cmake/external/openssl-v3.0.5-x86_64-posix-seh-rev0-8.1.0" ^
-DCMAKE_BUILD_TYPE=Debug ^
-B./build &&^
cd build &&^
cmake --build . &&^
echo "Successful build"
pause