// -*- C++ -*-
/*!
 * @file  DetectArMarkerRTC.h
 * @brief ModuleDescription
 * @date  $Date$
 *
 * $Id$
 */

#ifndef DETECTARMARKERRTC_H
#define DETECTARMARKERRTC_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#include <GL/freeglut.h>

#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/video.h>

#include <math.h>
#include <io.h>
#include <iostream>
#include <iomanip>
#include <fcntl.h>

bool AR_finder(LPVOID pParam);
static void init(void);	
static void mainloop(void);
void draw(int mid, double patt_trans[3][4]);
static void setupLighting(void);
static void setupMaterial(void);
static void cleanup(void);
static void keyEvent(unsigned char key, int x, int y);

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class DetectArMarkerRTC
 * @brief ModuleDescription
 *
 */
class DetectArMarkerRTC
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  DetectArMarkerRTC(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~DetectArMarkerRTC();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 //protected: // "AR_finder.cpp"からコンフィギュレーション，データポートにアクセスできるように変更
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  VideoConf
   * - DefaultValue: Data/WDM_camera_flipV.xml
   */
  std::string m_VideoConf;
  /*!
   * 
   * - Name:  CameraPara
   * - DefaultValue: Data/camera_para.dat
   */
  std::string m_CameraPara;
  /*!
   * 
   * - Name:  PatternName1
   * - DefaultValue: Data/patt_r
   */
  std::string m_PatternName1;
  /*!
   * 
   * - Name:  PatternName2
   * - DefaultValue: Data/patt_t
   */
  std::string m_PatternName2;
  /*!
   * 
   * - Name:  PatternName3
   * - DefaultValue: Data/patt_m
   */
  std::string m_PatternName3;
  /*!
   * 
   * - Name:  PatternWidth
   * - DefaultValue: 0.017
   */
  double m_PatternWidth;
  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  TimedDoubleSeq m_Coord;
  /*!
   */
  OutPort<TimedDoubleSeq> m_CoordOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

  typedef struct {
	  const char	*patt_name;		// パターンファイル名
	  int			patt_id;		// パターンID
	  int			mark_id;		// マーカーID
	  int			visible;		// 検出フラグ
	  double		width;			// パターンの幅 [mm]
	  double		center[2];		// パターンの中心座標
	  double		trans[3][4];	// 座標変換行列
  } OBJECT_T;
  
  OBJECT_T object[3];

  int thresh; // 閾値(0～255)

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void DetectArMarkerRTCInit(RTC::Manager* manager);
};

#endif // DETECTARMARKERRTC_H
