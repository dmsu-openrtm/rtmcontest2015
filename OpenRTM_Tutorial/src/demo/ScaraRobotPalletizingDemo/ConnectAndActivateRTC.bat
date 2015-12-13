set usbdrive=%~d0
%usbdrive%   REM Changing directory to the USB drive.
cd openrtm_tutorial\demo\ScaraRobotPalletizingDemo
echo Moved to DIR=%cd%

for /f "usebackq" %%h in (`hostname`) do set HOSTNAME=%%h

REM Set RTC's Configuration
call ..\Apps\rtshell\rtconf 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotArRTC0.rtc set RobotName SCARA
call ..\Apps\rtshell\rtconf 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotArRTC0.rtc set Speed 40

REM Connect RTC's Service Port
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc:ManipulatorCommonInterface_Common 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotArRTC0.rtc:ManipulatorCommonInterface_Common
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc:ManipulatorCommonInterface_Middle 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotArRTC0.rtc:ManipulatorCommonInterface_Middle

REM Connect RTC's Data Port
call ..\Apps\rtshell\rtcon 127.0.0.1/%HOSTNAME%.host_cxt/DetectArMarkerRTC0.rtc:Coord 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotArRTC0.rtc:Coord

REM Activate RTC
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/VS_ASR_RTC0.rtc
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/DetectArMarkerRTC0.rtc
TIMEOUT /T 3 /NOBREAK
call ..\Apps\rtshell\rtact 127.0.0.1/%HOSTNAME%.host_cxt/ScaraRobotArRTC0.rtc
