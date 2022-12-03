@echo off
REM arduino-cli board listall
echo Customer Arduino Compiler v1.0


REM eg. c:\arduino\arduino_compile_run.bat mysketch
set fn=%1
set "File2Read=.\%fn%\%fn%.ino"
echo %File2Read%

setlocal EnableExtensions EnableDelayedExpansion
for /f "delims=" %%a in ('Type "%File2Read%"') do (
    set /a count+=1
    set "Line[!count!]=%%a"
)

SET arduino_fqbn=%Line[1]:~2%
rem echo %arduino_fqbn%

echo first line of file should contain fqbn, found %arduino_fqbn%
rem echo (expecting //arduino:avr:mega or //arduino:avr:uno)
echo.

FOR /F "tokens=* USEBACKQ" %%F IN (`arduino-cli board list`) DO (
SET var=%%F
)

echo %var%

for /f %%a in ("%var%") do set COMPORT=%%a

echo found board attached to port: %COMPORT%
echo.
echo compiling

@echo on
arduino-cli compile --fqbn "%arduino_fqbn%" %1

arduino-cli upload -p %COMPORT% --fqbn "%arduino_fqbn%" %1

@echo off 

