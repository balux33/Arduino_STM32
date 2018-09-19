@echo off
rem: Note %~dp0 get path of this batch file
rem: Need to change drive if My Documents is on a drive other than C:
set driverLetter=%~dp0
set driverLetter=%driverLetter:~0,2%
%driverLetter%

cd %~dp0
cd ..\..\..\..\..\..\..
if exist driver_installed.txt (
    echo "driver already installed"
) else (
    echo "driver isn't installed, installing driver.."
	@echo "This file help Arduino_STM32 board to check driver installation state, delete it if you want to reinstall driver in next uplpad" > driver_installed.txt
	cd %~dp0
	cd ..\..
	cd drivers\win
	call "install_drivers.bat"
)



cd %~dp0
java -jar maple_loader.jar %1 %2 %3 %4 %5 %6 %7 %8 %9

for /l %%x in (1, 1, 40) do (
  ping -w 50 -n 1 192.0.2.1 > nul
  mode %1 > nul
  if ERRORLEVEL 0 goto comPortFound
)

echo timeout waiting for %1 serial

:comPortFound
