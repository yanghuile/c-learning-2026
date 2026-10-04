@echo off
setlocal enabledelayedexpansion
REM ============================================================
REM  Check warnings only, do not run
REM  Usage:  chk exercises\W5-bit-ops.c
REM
REM  Note: gcc returns exit code 0 even when it only warns, so we
REM        count the "warning:" lines ourselves instead of relying
REM        on errorlevel.
REM ============================================================

if "%~1"=="" (
    echo.
    echo Usage: chk ^<path-to-c-file^>
    echo Example: chk exercises\W5-bit-ops.c
    echo.
    exit /b 1
)

echo.
echo === Check: %~1 ===
set "LOG=%TEMP%\chk_out.txt"
gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only "%~1" 2>"%LOG%"

set /a WCOUNT=0
set /a ECOUNT=0
for /f "usebackq delims=" %%L in ("%LOG%") do (
    echo %%L
    echo %%L | findstr /c:"warning:" >nul && set /a WCOUNT+=1
    echo %%L | findstr /c:"error:" >nul && set /a ECOUNT+=1
)
del /q "%LOG%" 2>nul

echo.
if !ECOUNT! GTR 0 (
    echo [Result] !ECOUNT! errors, !WCOUNT! warnings
) else if !WCOUNT! GTR 0 (
    echo [Result] !WCOUNT! warnings, need to fix
) else (
    echo [Result] clean, zero warnings
)