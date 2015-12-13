echo CURRENT DIR=%cd%
set usbdrive=%~d0
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial\demo\ScaraRobotPalletizingDemo\tools

TIMEOUT /T 1

start armarker_for_palletizing.pdf

TIMEOUT /T 1
