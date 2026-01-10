@echo off
setlocal EnableDelayedExpansion

REM ================= ARGUMENTS =================
set BOARD=%1
set PORT=%2
set BAUD=%3
set FILE=%~4

if "%FILE%"=="" (
  echo Usage: upload ^<board^> ^<COMx^> ^<baud^> ^<file.ino^>
  exit /b 1
)

REM ================= RESOLVE PATH =================
for %%F in ("%FILE%") do (
  set FILE_ABS=%%~fF
  set SKETCH_NAME=%%~nF
)

if not exist "%FILE_ABS%" (
  echo File not found: %FILE_ABS%
  exit /b 1
)

REM ================= TEMP SKETCH =================
set TEMP_DIR=%TEMP%\arduino_upload_%RANDOM%
set SKETCH_DIR=%TEMP_DIR%\%SKETCH_NAME%

mkdir "%SKETCH_DIR%"
copy "%FILE_ABS%" "%SKETCH_DIR%\%SKETCH_NAME%.ino" >nul

echo Sketch     : %FILE_ABS%
echo Board      : arduino:avr:%BOARD%
echo Port       : %PORT%
echo Baudrate   : %BAUD%
echo.

REM ================= COMPILE =================
echo Compiling...
arduino-cli compile --fqbn arduino:avr:%BOARD% "%SKETCH_DIR%"
if errorlevel 1 (
  echo COMPILE FAILED
  goto cleanup
)

REM ================= UPLOAD =================
echo Uploading...
arduino-cli upload -p %PORT% --fqbn arduino:avr:%BOARD% "%SKETCH_DIR%"
if errorlevel 1 (
  echo UPLOAD FAILED
  goto cleanup
)

REM ================= SERIAL =================
echo.
echo Opening serial monitor (Ctrl+C to exit)...
arduino-cli monitor -p %PORT% -c baudrate=%BAUD%

:cleanup
rd /s /q "%TEMP_DIR%"
endlocal
