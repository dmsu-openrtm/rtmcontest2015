// -*- C++ -*-
/*!
 * @file  ScaraRobotArRTC.h
 * @brief ModuleDescription
 * @date  $Date$
 *
 * $Id$
 */

#ifndef SCARAROBOTARRTC_H
#define SCARAROBOTARRTC_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <conio.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">
#include "ManipulatorCommonInterface_CommonStub.h"
#include "ManipulatorCommonInterface_MiddleLevelStub.h"

// </rtc-template>

using namespace RTC;

/*!
 * @class ScaraRobotArRTC
 * @brief ModuleDescription
 *
 */
class ScaraRobotArRTC
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  ScaraRobotArRTC(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~ScaraRobotArRTC();

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


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  Speed
   * - DefaultValue: 30
   */
  int m_Speed;
  /*!
   * 
   * - Name:  BaseOffsetX
   * - DefaultValue: 0.0
   */
  double m_BaseOffsetX;
  /*!
   * 
   * - Name:  BaseOffsetY
   * - DefaultValue: 0.0
   */
  double m_BaseOffsetY;
  /*!
   * 
   * - Name:  BaseOffsetZ
   * - DefaultValue: 0.0
   */
  double m_BaseOffsetZ;
  /*!
   * 
   * - Name:  RobotName
   * - DefaultValue: ROBOT_NAME
   */
  std::string m_RobotName;
  /*!
   * 
   * - Name:  CoordMaxX
   * - DefaultValue: 0.04
   */
  double m_CoordMaxX;
  /*!
   * 
   * - Name:  CoordMaxY
   * - DefaultValue: 0.07
   */
  double m_CoordMaxY;
  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  TimedDoubleSeq m_Coord;
  /*!
   */
  InPort<TimedDoubleSeq> m_CoordIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  TimedBooleanSeq m_digitalOutput;
  /*!
   */
  OutPort<TimedBooleanSeq> m_digitalOutputOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  /*!
   */
  RTC::CorbaPort m_ManipulatorCommonInterface_CommonPort;
  /*!
   */
  RTC::CorbaPort m_ManipulatorCommonInterface_MiddlePort;
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  /*!
   */
  RTC::CorbaConsumer<JARA_ARM::ManipulatorCommonInterface_Common> m_JARA_ARM_ManipulatorCommonInterface_Common;
  /*!
   */
  RTC::CorbaConsumer<JARA_ARM::ManipulatorCommonInterface_Middle> m_JARA_ARM_ManipulatorCommonInterface_Middle;
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>
	 JARA_ARM::CarPosWithElbow m_CarPos;
	 JARA_ARM::HgMatrix offset;
	 JARA_ARM::RETURN_ID_var m_rid;
	 
	 RTC::ReturnCode_t ActCommand();
	 RTC::ReturnCode_t Gauss3(double wmat[][4]);
	 RTC::ReturnCode_t Gauss4(double wmat[][5]);
	 
	 char m_command[100];
	 double calib_mat[5][3];
	 double m_Buf[10];
	 double plane_coef[4];
	 double tmat[4][4];

};


extern "C"
{
  DLL_EXPORT void ScaraRobotArRTCInit(RTC::Manager* manager);
};

#endif // SCARAROBOTARRTC_H
