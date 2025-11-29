@echo off
setlocal enabledelayedexpansion

set OPTIMIZATION_LEVEL=-Os

rmdir /s /q bin 2>nul

rem ===== Windows payloads (built with linux triples, but PLATFORM_WINDOWS macros) =====
call toolchain.bat  bin\windows i386-unknown-linux-gnu ^
                    windows_i386  ^
                    src\*.c ^
                    -DARCHITECTURE_I386 ^
                    -DPLATFORM_WINDOWS ^
                    -DPLATFORM_WINDOWS_I386 ^
                    %OPTIMIZATION_LEVEL% ^
                    -fshort-wchar 
if errorlevel 1 goto :end

call toolchain.bat  bin\windows x86_64-unknown-linux-gnu ^
                    windows_amd64 ^
                    src\*.c ^
                    -DARCHITECTURE_AMD64 ^
                    -DPLATFORM_WINDOWS ^
                    -DPLATFORM_WINDOWS_AMD64 ^
                    %OPTIMIZATION_LEVEL% ^
                    -fshort-wchar
if errorlevel 1 goto :end

call toolchain.bat  bin\windows thumbv7a-none-linux-gnueabi ^
                    windows_arm32 ^
                    src\*.c ^
                    -DARCHITECTURE_ARM32 ^
                    -DPLATFORM_WINDOWS ^
                    -DPLATFORM_WINDOWS_ARM32 ^
                    %OPTIMIZATION_LEVEL% ^
                    -fshort-wchar

call toolchain.bat  bin\windows aarch64-unknown-linux-gnu ^
                    windows_arm64 ^
                    src\*.c ^
                    -DARCHITECTURE_ARM64 ^
                    -DPLATFORM_WINDOWS ^
                    -DPLATFORM_WINDOWS_ARM64 ^
                    %OPTIMIZATION_LEVEL% ^
                    -fshort-wchar
if errorlevel 1 goto :end

rem ===== Linux payloads =====
call toolchain.bat  bin\linux i386-unknown-linux-gnu ^
                    linux_i386 ^
                    src\*.c ^
                    -DARCHITECTURE_I386 ^
                    -DPLATFORM_LINUX ^
                    -DPLATFORM_LINUX_I386 ^
                    %OPTIMIZATION_LEVEL%
if errorlevel 1 goto :end

call toolchain.bat  bin\linux x86_64-unknown-linux-gnu ^
                    linux_amd64 ^
                    src\*.c ^
                    -DARCHITECTURE_AMD64 ^
                    -DPLATFORM_LINUX ^
                    -DPLATFORM_LINUX_AMD64 ^
                    %OPTIMIZATION_LEVEL%
if errorlevel 1 goto :end

call toolchain.bat  bin\linux aarch64-unknown-linux-gnu ^
                    linux_arm64 ^
                    src\*.c ^
                    -DARCHITECTURE_ARM64 ^
                    -DPLATFORM_LINUX ^
                    -DPLATFORM_LINUX_ARM64 ^
                    %OPTIMIZATION_LEVEL%
if errorlevel 1 goto :end

:end
endlocal
