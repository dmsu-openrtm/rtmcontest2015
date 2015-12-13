echo CURRENT DIR=%cd%
set usbdrive=%~d0
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\

cd openrtm_tutorial\demo\ScaraRobotPalletizingDemo\bin
set OMNI_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1\omniORB\4.1.5
set RTM_ROOT=%usbdrive%\openrtm_tutorial\misc\OpenRTM-aist\1.1
set PATH=%PATH%;%RTM_ROOT%\runtime;%OMNI_ROOT%\bin\x86_win32;%usbdrive%\demo\ScaraRobotPalletizingDemo\bin

echo CURRENT DIR=%cd%
echo PATH=%PATH%

TIMEOUT /T 1

start detectarmarkerrtccomp.exe
start scararobotarrtccomp.exe
start vs_asr_rtccomp.exe

TIMEOUT /T 1
