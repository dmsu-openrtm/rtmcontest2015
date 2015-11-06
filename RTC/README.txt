本ディレクトリには，RTCを実行するために必要なファイル，およびソースコードが配置されています．
各RTCの使用方法については，"..\Documentation"に配置されているドキュメントを参照してください．

----------
RTC
  ├DetectArMarkerRTC
  │  ├bin
  │  │  ├Data
  │  │  ｜  ├armarker_for_palletizing.pdf
  │  │  ｜  ├camera_para.dat
  │  │  ｜  ├patt_m
  │  │  ｜  ├patt_r
  │  │  ｜  ├patt_t
  │  │  ｜  ├robot_calibration_grid.pdf
  │  │  ｜  └WDM_camera_flipV.xml
  │  │  ├DetectArMarkerRTC.dll
  │  │  ├DetectArMarkerRTC.exp
  │  │  ├DetectArMarkerRTC.lib
  │  │  ├DetectArMarkerRTCComp.exe
  │  │  ├DetectArMarkerRTCComp.exp
  │  │  ├DetectArMarkerRTCComp.lib
  │  │  ├DSVL.dll
  │  │  ├libARvideo.dll
  │  │  └rtc.conf
  │  ├src
  │  └COPYING.txt
  ├ScaraRobotArRTC
  │  ├bin
  │  │  ├rtc.conf
  │  │  ├ScaraRobotArRTC.dll
  │  │  ├ScaraRobotArRTC.exp
  │  │  ├ScaraRobotArRTC.lib
  │  │  ├ScaraRobotArRTCComp.exe
  │  │  ├ScaraRobotArRTCComp.exp
  │  │  └ScaraRobotArRTCComp.lib
  │  ├src
  │  └LICENSE.txt
  ├ScaraRobotControlRTC
  │  ├bin
  │  │  ├rtc.conf
  │  │  ├Sample.csv
  │  │  ├ScaraRobotControlRTC.dll
  │  │  ├ScaraRobotControlRTC.exp
  │  │  ├ScaraRobotControlRTC.lib
  │  │  ├ScaraRobotControlRTCComp.exe
  │  │  ├ScaraRobotControlRTCComp.exp
  │  │  └ScaraRobotControlRTCComp.lib
  │  ├src
  │  └LICENSE.txt
  ├VS_ASR_RTC
  │  ├bin
  │  │  ├rtc.conf
  │  │  ├SLABHIDDevice.dll
  │  │  ├SLABHIDtoUART.dll
  │  │  ├VS_ASR_RTC.dll
  │  │  ├VS_ASR_RTC.exp
  │  │  ├VS_ASR_RTC.lib
  │  │  ├VS_ASR_RTCComp.exe
  │  │  ├VS_ASR_RTCComp.exp
  │  │  └VS_ASR_RTCComp.lib
  │  ├src
  │  └LICENSE.txt
  └README.txt

sourceディレクトリに配置されているファイルは，cmakeでビルドの後，Microsoft VC++でビルドすることで実行ファイルを再生成できます．ただし，一部RTCは追加のインクルードファイル・ライブラリが必要になります．

最終更新日：2015/11/2
