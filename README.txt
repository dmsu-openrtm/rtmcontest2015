/** 著作権および免責事項 **/

著作権は埼玉大学設計工学研究室が保有しています．このソフトウェアを使用したことによって生じたすべての障害・損害・不具合等に関しては，一切の責任を負いません．各自の責任においてご使用ください．ライセンスに関しては，各々の上位ディレクトリを参照ください．

/** 更新履歴 **/
バージョン  更新日      更新内容                           
--------------------------------------------------------------------------------
1.0         2015/10/31  新規公開
1.0.1	    2015/11/2 　解説マニュアルに「ソースコード，ライブラリの引用・参照箇所」の追加
				        ライセンス条件の変更
				        OpenRTM TutorialにおけるScaraRobot部分の公開
1.1.0       2015/11/6   OpenRTM-aist C++のバージョンを1.1.0から1.1.1へ更新 (binおよびsrcをすべて再生成)
                        camera_para.datをArToolKit配布時の標準的なものに変更
                        "armarker_for_palletizing.pdf"を点対称・線対称にならないように変更，同時にpatternファイル (patt_r, patt_t, patt_m) を再生成
                        解説マニュアル等のマニュアルを更新
                        動画を視聴時間短縮のため早送り
1.2.0       2015/11/28  OpenRTM Tutorialとの互換性を保つためOpenRTM-aist C++ 1.1.0-RELEASE へダウングレード
                        ARマーカに用いたパレタイジングデモに関して，三菱電機株式会社MELFA-RV-3SDを追加(マニュアル，動画，RTC)
                        DetectArMarkerRTCにおいて，2値化の閾値をアクティブに変更可能に
                        "Start_OpenRTM_Tutorial.pdf"の更新
1.3.0       2015/12/13  VS_ASR_RTC，ScaraRobotControlRTC，ScaraRobotArRTCのサービスポートインタフェース名の変更
                        ロボット教育ツールの使い方に関するドキュメントを大幅更新，解説動画の追加
                        ARマーカに用いたパレタイジングデモのうち，スカラロボット部分をOpenRTM Tutorialへ実装
                        動画をYouTubeへ移動
                        国際ロボット展(iREX2015)関連資料の追加

----------
本パッケージは、RTミドルウエアコンテスト2015の応募作品である「USBメモリに搭載したポータブルRTM環境を用いたロボット教育ツール」をパッケージ化したものです．
はじめに，"OpenRTM_Tutorial\Start_OpenRTM_Tutorial.pdf"をお読みください．
詳細は各ディレクトリの"README.txt"を参照してください．

----------
ScaraRobotRTC1_0
  ├Document
  │  ├image
  │  ├01_StartOpenRTMTutorial.html
  │  ├01_StartOpenRTMTutorial.md
  │  ├02_Change_DriveName.html
  │  ├02_Change_DriveName.md
  │  ├11_Control_AcademicScaraRobot_by_RTC.pdf
  │  ├12_Control_MELFA_RV-3SD_by_RTC.pdf
  │  ├21_VS_ASR_RTC_manual.pdf
  │  ├22_ScaraRobotControlRTC_manual.pdf
  │  ├23_DetectArMarkerRTC_manual.pdf
  │  ├24_ScaraRobotArRTC_manual.pdf
  │  ├31_RTC_DevelopmentEnviroment_manual.pdf
  │  ├LICENSE.txt
  │  └README.txt
  ├iREX2015
  │  ├iREX2015_display.pdf
  │  ├iREX2015_handout.pdf
  │  └README.txt
  ├Movie
  │  ├
  │  └README.txt
  ├OpenRTM_Tutorial
  │  ├src
  │  │  ├demo
  │  │  └doc
  │  ├LICENSE.txt
  │  └README.txt
  ├RTC
  │  ├DetectArMarkerRTC
  │  │  ├bin
  │  │  │  ├Data
  │  │  │  │  ├camera_para.dat
  │  │  │  │  ├palletizing_pattern.pdf
  │  │  │  │  ├patt_m
  │  │  │  │  ├patt_r
  │  │  │  │  ├patt_t
  │  │  │  │  ├robot_calibration_grid.pdf
  │  │  │  │  └WDM_camera_flipV.xml
  │  │  │  ├detectarmarkerrtc.dll
  │  │  │  ├detectarmarkerrtc.exp
  │  │  │  ├detectarmarkerrtc.lib
  │  │  │  ├detectarmarkerrtccomp.exe
  │  │  │  ├DSVL.dll
  │  │  │  ├freeglut.dll
  │  │  │  ├libARvideo.dll
  │  │  │  ├msvcp71.dll
  │  │  │  └rtc.conf
  │  │  ├src
  │  │  └COPYING.txt
  │  ├DIOInterfaceModuleRTC
  │  │  ├bin
  │  │  │  ├diointerfacemodulertc.dll
  │  │  │  ├diointerfacemodulertc.exp
  │  │  │  ├diointerfacemodulertc.lib
  │  │  │  ├diointerfacemodulertccomp.exe
  │  │  │  └rtc.conf
  │  │  └LICENSE.txt
  │  ├RobotArmCommonInterfaceConvertRTC
  │  │  ├bin
  │  │  │  ├robotarmcommoninterfaceconvertrtc.dll
  │  │  │  ├robotarmcommoninterfaceconvertrtc.exp
  │  │  │  ├robotarmcommoninterfaceconvertrtc.lib
  │  │  │  ├robotarmcommoninterfaceconvertrtccomp.exe
  │  │  │  └rtc.conf
  │  │  └LICENSE.txt
  │  ├ScaraRobotArRTC
  │  │  ├bin
  │  │  │  ├rtc.conf
  │  │  │  ├scararobotarrtc.dll
  │  │  │  ├scararobotarrtc.exp
  │  │  │  ├scararobotarrtc.lib
  │  │  │  └scararobotarrtccomp.exe
  │  │  ├src
  │  │  └LICENSE.txt
  │  ├ScaraRobotControlRTC
  │  │  ├bin
  │  │  │  ├rtc.conf
  │  │  │  ├Sample.csv
  │  │  │  ├scararobotcontrolrtc.dll
  │  │  │  ├scararobotcontrolrtc.exp
  │  │  │  ├scararobotcontrolrtc.lib
  │  │  │  └scararobotcontrolrtccomp.exe
  │  │  ├src
  │  │  └LICENSE.txt
  │  ├VS_ASR_RTC
  │  │  ├bin
  │  │  │  ├rtc.conf
  │  │  │  ├SLABHIDDevice.dll
  │  │  │  ├SLABHIDtoUART.dll
  │  │  │  ├vs_asr_rtc.dll
  │  │  │  ├vs_asr_rtc.exp
  │  │  │  ├vs_asr_rtc.lib
  │  │  │  └vs_asr_rtccomp.exe
  │  │  ├src
  │  │  └LICENSE.txt
  │  └README.txt
  └README.txt
