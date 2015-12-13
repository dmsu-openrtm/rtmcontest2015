echo CURRENT DIR=%cd%
set usbdrive=%~d0
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial\misc\ARToolKit

TIMEOUT /T 1

start calib_cpara.pdf

TIMEOUT /T 1
