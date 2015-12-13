echo CURRENT DIR=%cd%
set usbdrive=%~d0
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial\demo\ScaraRobotPalletizingDemo\tools

TIMEOUT /T 1

start robot_calibration_grid.pdf

TIMEOUT /T 1
