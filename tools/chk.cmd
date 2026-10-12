@echo off
REM ============================================================
REM  chk - Check warnings only, do not run
REM
REM  Usage:  chk exercises\W5-bit-ops.c
REM
REM  Three lessons baked into this file:
REM    1. gcc exits 0 even when it only warns, so we cannot use
REM       errorlevel. Count the "warning:" lines ourselves.
REM    2. NEVER do `echo %%L | findstr ...` on diagnostics.
REM       gcc output often contains & | < > (e.g. rb_put(&rb, c));
REM       echo would let cmd treat those as command separators,
REM       producing garbage and wrong counts.
REM       Use `find /v ""` instead - it prints the file verbatim
REM       and the pipe carries raw bytes, not parsed arguments.
REM    3. Do NOT use delayed expansion (!VAR!) because source code
REM       lines containing ! would break. Plain %VAR% is enough.
REM
REM  NOTE: This file must stay pure ASCII. cmd.exe reads .cmd files
REM        using the local ANSI codepage (GBK on zh-CN Windows), so
REM        UTF-8 Chinese comments become garbage and break the script.
REM ============================================================

if "%~1"=="" (
    echo.
    echo Usage: chk ^<path-to-c-file^>
    echo Example: chk exercises\W5-bit-ops.c
    echo.
    exit /b 1
)

set "SRC=%~1"
set "LOG=%TEMP%\chk_out.txt"

echo.
echo === Check: %SRC% ===
echo.

gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only "%SRC%" 2>"%LOG%"

REM ---------- print all diagnostics verbatim ----------
find /v "" "%LOG%"

REM ---------- count warning / error lines ----------
set WCOUNT=0
set ECOUNT=0
for /f %%C in ('findstr /c:"warning:" "%LOG%" ^| find /c /v ""') do set WCOUNT=%%C
for /f %%C in ('findstr /c:"error:" "%LOG%" ^| find /c /v ""') do set ECOUNT=%%C

del /q "%LOG%" 2>nul

echo.
if not "%ECOUNT%"=="0" (
    echo [Result] %ECOUNT% errors, %WCOUNT% warnings
) else if not "%WCOUNT%"=="0" (
    echo [Result] %WCOUNT% warnings, need to fix
) else (
    echo [Result] clean, zero warnings
)
