@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo   Student Help Desk Management System - Windows Build
echo ========================================================

rem Check if GCC is available
where gcc >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo [INFO] Found GCC compiler. Compiling with GCC...
    echo.
    echo Compiling main application: helpdesk.exe
    gcc -std=c99 -Wall -Wextra -Iinclude src/main.c src/request.c src/priority_queue.c src/stack.c -o helpdesk.exe
    if !ERRORLEVEL! NEQ 0 (
        echo [ERROR] Failed to compile helpdesk.exe
        exit /b 1
    )

    echo Compiling Priority Queue test suite: test_priority_queue.exe
    gcc -std=c99 -Wall -Wextra -Iinclude tests/test_priority_queue.c src/priority_queue.c src/request.c -o test_priority_queue.exe
    if !ERRORLEVEL! NEQ 0 (
        echo [ERROR] Failed to compile test_priority_queue.exe
        exit /b 1
    )

    echo Compiling Stack test suite: test_stack.exe
    gcc -std=c99 -Wall -Wextra -Iinclude tests/test_stack.c src/stack.c -o test_stack.exe
    if !ERRORLEVEL! NEQ 0 (
        echo [ERROR] Failed to compile test_stack.exe
        exit /b 1
    )

    echo.
    echo [SUCCESS] GCC build completed successfully!
    goto check_run_tests
)

rem Check if cl.exe is already in PATH
where cl >nul 2>nul
if %ERRORLEVEL% EQU 0 goto run_msvc_build

rem Attempt to locate and initialize MSVC environment if vcvars64.bat exists
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    call "%ProgramFiles(x86)%\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
    where cl >nul 2>nul
    if !ERRORLEVEL! EQU 0 goto run_msvc_build
)

echo [ERROR] Neither GCC nor MSVC was found in your PATH.
echo Please install MinGW-w64 or run from a Visual Studio Developer Command Prompt.
exit /b 1

:run_msvc_build
echo [INFO] GCC not found in PATH. Using MSVC cl.exe...
echo.
echo Compiling main application: helpdesk.exe
cl /nologo /W4 /Iinclude src\main.c src\request.c src\priority_queue.c src\stack.c /Fe:helpdesk.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] MSVC failed to compile helpdesk.exe
    exit /b 1
)

echo Compiling Priority Queue test suite: test_priority_queue.exe
cl /nologo /W4 /Iinclude tests\test_priority_queue.c src\priority_queue.c src\request.c /Fe:test_priority_queue.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] MSVC failed to compile test_priority_queue.exe
    exit /b 1
)

echo Compiling Stack test suite: test_stack.exe
cl /nologo /W4 /Iinclude tests\test_stack.c src\stack.c /Fe:test_stack.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] MSVC failed to compile test_stack.exe
    exit /b 1
)

del /q *.obj 2>nul
echo.
echo [SUCCESS] MSVC build completed successfully!
goto check_run_tests

:check_run_tests
if "%1"=="test" (
    echo.
    echo Running automated unit tests...
    echo.
    test_priority_queue.exe
    if %ERRORLEVEL% NEQ 0 exit /b 1
    test_stack.exe
    if %ERRORLEVEL% NEQ 0 exit /b 1
)

echo.
echo Build finished. To run the application, execute: helpdesk.exe
echo To run tests, execute: build.bat test
