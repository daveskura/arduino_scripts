REM arduino-cli board listall
set "fn=MyFirstSketch"
rem set "File2Read=.\%fn%\%fn%.ino"

setlocal EnableExtensions EnableDelayedExpansion

for /f "delims=" %%a in ('Type "%File2Read%"') do (
    set /a count+=1
    set "Line[!count!]=%%a"
)
SET arduino_fqbn=%Line[1]:~2%
echo %arduino_fqbn%

pause