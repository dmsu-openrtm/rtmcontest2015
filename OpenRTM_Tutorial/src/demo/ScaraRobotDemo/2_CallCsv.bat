echo CURRENT DIR=%cd%
REM set usbdrive=%0
set usbdrive=%~d0
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial\demo\ScaraRobotDemo\bin

TIMEOUT /T 1

start Sample.csv

TIMEOUT /T 1
