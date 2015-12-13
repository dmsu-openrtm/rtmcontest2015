set usbdrive=%~d0
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo\ScaraRobotDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

REM Connect RTC's Service Port
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc:ManipulatorCommonInterface_Common 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc:ManipulatorCommonInterface_Common
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc:ManipulatorCommonInterface_Middle 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc:ManipulatorCommonInterface_Middle

REM Activate RTC
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotControlRTC0.rtc
