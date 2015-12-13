rem echo CURRENT DIR=%cd%
rem set usbdrive=%1
rem if NOT DEFINED usbdrive set usbdrive=F:
rem cd /d %usbdrive%\

rem for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

REM set usbdrive=%1
REM %usbdrive%   REM Changing directory to the USB drive.
set usbdrive=%~d0
if NOT DEFINED usbdrive set usbdrive=F:
cd /d %usbdrive%\
cd openrtm_tutorial\demo\ScaraRobotDemo\bin
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc
call ..\Apps\rtshell\rtdeact 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc


call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc
call ..\Apps\rtshell\rtexit 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc

rem taskkill /F /IM vs_asr_rtccomp.exe
rem taskkill /F /IM scararobotcontrolrtccomp.exe

