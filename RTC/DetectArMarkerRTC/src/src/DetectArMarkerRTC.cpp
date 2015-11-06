// -*- C++ -*-
/*!
 * @file  DetectArMarkerRTC.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "DetectArMarkerRTC.h"

// Module specification
// <rtc-template block="module_spec">
static const char* detectarmarkerrtc_spec[] =
  {
    "implementation_id", "DetectArMarkerRTC",
    "type_name",         "DetectArMarkerRTC",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "Saitama Univ. Design Lab.",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.VideoConf", "Data/WDM_camera_flipV.xml",
    "conf.default.CameraPara", "Data/camera_para.dat",
    "conf.default.PatternName1", "Data/patt_r",
    "conf.default.PatternName2", "Data/patt_t",
    "conf.default.PatternName3", "Data/patt_m",
    "conf.default.PatternWidth", "17.0",
    // Widget
    "conf.__widget__.VideoConf", "text",
    "conf.__widget__.CameraPara", "text",
    "conf.__widget__.PatternName1", "text",
    "conf.__widget__.PatternName2", "text",
    "conf.__widget__.PatternName3", "text",
    "conf.__widget__.PatternWidth", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
DetectArMarkerRTC::DetectArMarkerRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_CoordOut("Coord", m_Coord)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
DetectArMarkerRTC::~DetectArMarkerRTC()
{
}



RTC::ReturnCode_t DetectArMarkerRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("Coord", m_CoordOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("VideoConf", m_VideoConf, "Data/WDM_camera_flipV.xml");
  bindParameter("CameraPara", m_CameraPara, "Data/camera_para.dat");
  bindParameter("PatternName1", m_PatternName1, "Data/patt_r");
  bindParameter("PatternName2", m_PatternName2, "Data/patt_t");
  bindParameter("PatternName3", m_PatternName3, "Data/patt_m");
  bindParameter("PatternWidth", m_PatternWidth, "17.0");
  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DetectArMarkerRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DetectArMarkerRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DetectArMarkerRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t DetectArMarkerRTC::onActivated(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"*********************************"<<std::endl;
	std::cout<<"********** onActivated **********"<<std::endl;
	std::cout<<"*********************************"<<std::endl;

	std::cout<<std::endl<<"*******************************"<<std::endl;
	std::cout<<"********** onExecute **********"<<std::endl;
	std::cout<<"*******************************"<<std::endl;

	return RTC::RTC_OK;
}


RTC::ReturnCode_t DetectArMarkerRTC::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"***********************************"<<std::endl;
	std::cout<<"********** onDeactivated **********"<<std::endl;
	std::cout<<"***********************************"<<std::endl;
	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t DetectArMarkerRTC::onExecute(RTC::UniqueId ec_id)
{
	if(AR_finder(this))
	{
		return RTC::RTC_ERROR;
	}
	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t DetectArMarkerRTC::onAborting(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"***************************"<<std::endl;
	std::cout<<"********** Error **********"<<std::endl;
	std::cout<<"***************************"<<std::endl;
	
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DetectArMarkerRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DetectArMarkerRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DetectArMarkerRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DetectArMarkerRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void DetectArMarkerRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(detectarmarkerrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<DetectArMarkerRTC>,
                             RTC::Delete<DetectArMarkerRTC>);
  }
  
};


