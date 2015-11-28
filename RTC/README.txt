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
  │  │  ├detectarmarkerrtc.dll
  │  │  ├detectarmarkerrtc.exp
  │  │  ├detectarmarkerrtc.lib
  │  │  ├detectarmarkerrtccomp.exe
  │  │  ├DSVL.dll
  │  │  ├freeglut.dll
  │  │  ├libARvideo.dll
  │  │  └rtc.conf
  │  ├src
  │  └COPYING.txt
  ├DIOInterfaceModuleRTC
  │  ├bin
  │  │  ├diointerfacemodulertc.dll
  │  │  ├diointerfacemodulertc.exp
  │  │  ├diointerfacemodulertc.lib
  │  │  ├diointerfacemodulertccomp.exe
  │  │  └rtc.conf
  │  └LICENSE.txt
  ├RobotArmCommonInterfaceConvertRTC
  │  ├bin
  │  │  ├robotarmcommoninterfaceconvertrtc.dll
  │  │  ├robotarmcommoninterfaceconvertrtc.exp
  │  │  ├robotarmcommoninterfaceconvertrtc.lib
  │  │  ├robotarmcommoninterfaceconvertrtccomp.exe
  │  │  └rtc.conf
  │  └LICENSE.txt
  ├ScaraRobotArRTC
  │  ├bin
  │  │  ├rtc.conf
  │  │  ├scararobotarrtc.dll
  │  │  ├scararobotarrtc.exp
  │  │  ├scararobotarrtc.lib
  │  │  └scararobotarrtccomp.exe
  │  ├src
  │  └LICENSE.txt
  ├ScaraRobotControlRTC
  │  ├bin
  │  │  ├rtc.conf
  │  │  ├Sample.csv
  │  │  ├scararobotcontrolrtc.dll
  │  │  ├scararobotcontrolrtc.exp
  │  │  ├scararobotcontrolrtc.lib
  │  │  └scararobotcontrolrtccomp.exe
  │  ├src
  │  └LICENSE.txt
  ├VS_ASR_RTC
  │  ├bin
  │  │  ├rtc.conf
  │  │  ├SLABHIDDevice.dll
  │  │  ├SLABHIDtoUART.dll
  │  │  ├vs_asr_rtc.dll
  │  │  ├vs_asr_rtc.exp
  │  │  ├vs_asr_rtc.lib
  │  │  └vs_asr_rtccomp.exe
  │  ├src
  │  └LICENSE.txt
  └README.txt

srcディレクトリに配置されているファイルは，cmakeでビルドの後，Microsoft VC++でビルドすることで実行ファイルを再生成できます．ただし，一部RTCは追加のインクルードファイル・ライブラリが必要になります．

最終更新日：2015/11/28
