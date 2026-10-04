@echo off
REM ============================================================
REM  Build and run one .c file  (ask  dsh  if you forget)
REM
REM  Usage:  b exercises\W5-bit-ops.c
REM
REM  Uses -Werror: any warning counts as failure, so you cannot
REM  run code that still has warnings. That is the habit.
REM ============================================================

if "%~1"=="" (
    echo.
    echo Usage: b ^<path-to-c-file^>
    echo Example: b exercises\W5-bit-ops.c
    echo.
    exit /b 1
)

set "SRC=%~1"
set "OUT=%~dpn1.exe"

echo.
echo === Compile: %SRC% ===
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g "%SRC%" -o "%OUT%"

if errorlevel 1 (
    echo.
    echo [STOP] Fix the warnings/errors above first.
    exit /b 1
)

echo.
echo === Run ===
"%OUT%"

echo.
echo === Done ===