// -*- C++ -*-
/*!
 * @file  ScaraRobotControlRTC.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */
#define _USE_MATH_DEFINES

#include "ScaraRobotControlRTC.h"
#include <conio.h>

// Module specification
// <rtc-template block="module_spec">
static const char* scararobotcontrolrtc_spec[] =
  {
    "implementation_id", "ScaraRobotControlRTC",
    "type_name",         "ScaraRobotControlRTC",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "Saitama Univ. Design Lab.",
    "category",          "Industrial Robot",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.FilePass", "Sample.csv",
    "conf.default.Speed", "30",
    "conf.default.BaseOffsetX", "0.0",
    "conf.default.BaseOffsetY", "0.0",
    // Widget
    "conf.__widget__.FilePass", "text",
    "conf.__widget__.Speed", "text",
    "conf.__widget__.BaseOffsetX", "text",
    "conf.__widget__.BaseOffsetY", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ScaraRobotControlRTC::ScaraRobotControlRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ManipulatorCommonInterface_CommonPort("ManipulatorCommonInterface_Common"),
    m_ManipulatorCommonInterface_MiddlePort("ManipulatorCommonInterface_Middle")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ScaraRobotControlRTC::~ScaraRobotControlRTC()
{
}



RTC::ReturnCode_t ScaraRobotControlRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  m_ManipulatorCommonInterface_CommonPort.registerConsumer("ManipulatorCommonInterface_Common", "JARA_ARM::ManipulatorCommonInterface_Common", m_ManipulatorCommonInterface_Common);
  m_ManipulatorCommonInterface_MiddlePort.registerConsumer("ManipulatorCommonInterface_Middle", "JARA_ARM::ManipulatorCommonInterface_Middle", m_ManipulatorCommonInterface_Middle);
  
  // Set CORBA Service Ports
  addPort(m_ManipulatorCommonInterface_CommonPort);
  addPort(m_ManipulatorCommonInterface_MiddlePort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("FilePass", m_FilePass, "Sample.csv");
  bindParameter("Speed", m_Speed, "30");
  bindParameter("BaseOffsetX", m_BaseOffsetX, "0.0");
  bindParameter("BaseOffsetY", m_BaseOffsetY, "0.0");
  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ScaraRobotControlRTC::onActivated(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"           Activate           "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;


	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"           Execute            "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;

	//実行の確認
	std::cout<<"Input Command ( s:Start, e: Exit )"<<std::endl;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaraRobotControlRTC::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"          Deactivate          "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaraRobotControlRTC::onExecute(RTC::UniqueId ec_id)
{
	char* FilePass;
	FILE * fp;
	int len;
	char c;
	char buf[200];
	int ret;
	
	c = ' ';

	//キー入力待ち
	if (kbhit()) c = getch();

	switch(c){
	case 's':
	
		//ベースオフセットの設定
		std::cout<<"Set Base Offset"<<std::endl<<std::endl;
		JARA_ARM::HgMatrix offset;

		//1列目
		offset[0][0]=1.0;
		offset[1][0]=0.0;
		offset[2][0]=0.0;
			
		//2列目
		offset[0][1]=0.0;
		offset[1][1]=1.0;
		offset[2][1]=0.0;
			
		//3列目
		offset[0][2]=0.0;
		offset[1][2]=0.0;
		offset[2][2]=1.0;
			
		//4列目
		offset[0][3]=m_BaseOffsetX;//[m]
		offset[1][3]=m_BaseOffsetY;//[m]
		offset[2][3]=0.0;

		m_rid=m_ManipulatorCommonInterface_Middle->setBaseOffset(offset);
		if(m_rid->id != 0){//Error
			std::cout<<"Set Base Offset is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		//動作速度設定
		std::cout<<"Set Speed"<<std::endl<<std::endl;
		if((m_Speed <= 0)||(m_Speed > 100)){
			std::cout<<"Value is Wrong"<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}
		
		m_rid=m_ManipulatorCommonInterface_Middle->setSpeedCartesian(m_Speed);
		if(m_rid->id != 0){//Error
			std::cout<<"Set Speed Cartesian is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		m_rid=m_ManipulatorCommonInterface_Middle->setSpeedJoint(m_Speed);
		if(m_rid->id != 0){//Error
			std::cout<<"Set Speed Joint is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		//ファイルの読み込み
		len = m_FilePass.length();
		FilePass = new char[len+1];
		memcpy(FilePass,m_FilePass.c_str(),len+1);
		fp=fopen( FilePass , "r");
		if( fp == NULL ) {
			std::cout<<"File Pass is Wrong"<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		//ループ
		//1行ごとに読み込み、コマンド実行
		while(fgets( buf,200, fp ) != NULL ){
			
			ret = sscanf( buf, "%[^\n,],%lf,%lf,%lf,%lf" , m_command , &m_Buf[0] ,&m_Buf[1] , &m_Buf[2] ,&m_Buf[3] );

			if(ret =! 5){
				return RTC::RTC_ERROR;
			}
			
			if(ActCommand() != RTC::RTC_OK){
				return RTC::RTC_ERROR;
			}

		}

		//ファイルクローズ
		fclose( fp );

		//実行の確認
		std::cout<<"Input Command ( s:Start, e: Exit )"<<std::endl;

		break;


	case 'e'://Esc
			std::cout<<std::endl<<"Exit"<<std::endl<<"Ending program"<<std::endl;
			
			deactivate(ec_id);

	break;
	default:
		break;
	}

  return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaraRobotControlRTC::onAborting(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"            Error             "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotControlRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t ScaraRobotControlRTC::ActCommand()
{
	if(strcmp(m_command,"SERVO_OFF") == 0){//サーボOFF
		
		std::cout<<"ServoOFF"<<std::endl;
		m_rid=m_ManipulatorCommonInterface_Common->servoOFF();
		if(m_rid->id != 0){//Error
			std::cout<<"ServoOFF is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(1000);

	}else if(strcmp(m_command,"SERVO_ON") == 0){//サーボON

		std::cout<<"ServoON"<<std::endl;
		m_rid=m_ManipulatorCommonInterface_Common->servoON();
		if(m_rid->id != 0){//Error
			std::cout<<"ServoON is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(1000);

	}else if(strcmp(m_command,"HAND_CLOSE") == 0){//ハンドクローズ

		std::cout<<"Close Gripper"<<std::endl;
		m_rid=m_ManipulatorCommonInterface_Middle->closeGripper();
		if(m_rid->id != 0){//Error
			std::cout<<"Close Gripper is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(120000/m_Speed);

	}else if(strcmp(m_command,"HAND_MOV") == 0){//ハンド動作

		std::cout<<"Move Gripper"<<std::endl;
		if((m_Buf[0] < 0)||(m_Buf[0] > 100)){
			std::cout<<"Value is Wrong"<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}
		m_rid=m_ManipulatorCommonInterface_Middle->moveGripper((JARA_ARM::ULONG)m_Buf[0]);
		if(m_rid->id != 0){//Error
			std::cout<<"Move Gripper is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(120000/m_Speed);

	}else if(strcmp(m_command,"CMVS") == 0){//直交座標系の直線補間(絶対指令)

		std::cout<<"Move Linear Cartesian Abs"<<std::endl;

		JARA_ARM::CarPosWithElbow pos;
		m_Buf[0] = m_Buf[0];//X[m]
		m_Buf[1] = m_Buf[1];//Y[m]
		m_Buf[2] = m_Buf[2];//Z[m]
		m_Buf[3]=m_Buf[3];//C[rad]

		//姿勢はX->Y->Z回転での値
		//1列目
		pos.carPos[0][0]=cos(m_Buf[3]);
		pos.carPos[1][0]=sin(m_Buf[3]);
		pos.carPos[2][0]=0.0;
			
		//2列目
		pos.carPos[0][1]=-sin(m_Buf[3]);
		pos.carPos[1][1]=cos(m_Buf[3]);
		pos.carPos[2][1]=0.0;
			
		//3列目
		pos.carPos[0][2]=0.0;
		pos.carPos[1][2]=0.0;
		pos.carPos[2][2]=1.0;
			
		//4列目
		pos.carPos[0][3]=m_Buf[0];
		pos.carPos[1][3]=m_Buf[1];
		pos.carPos[2][3]=m_Buf[2];

		m_rid=m_ManipulatorCommonInterface_Middle->moveLinearCartesianAbs(pos);
		if(m_rid->id != 0){//Error
			std::cout<<"Move Linear Cartesian Abs is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(180000/m_Speed);

	}else if(strcmp(m_command,"CMOV") == 0){//PTP補間動作(直交座標系)

		std::cout<<"Move PTP Cartesian Abs"<<std::endl;

		JARA_ARM::CarPosWithElbow pos;
		m_Buf[0] = m_Buf[0];//X[m]
		m_Buf[1] = m_Buf[1];//Y[m]
		m_Buf[2] = m_Buf[2];//Z[m]
		m_Buf[3]=m_Buf[3];//C[rad]

		//姿勢はX->Y->Z回転での値
		//1列目
		pos.carPos[0][0]=cos(m_Buf[3]);
		pos.carPos[1][0]=sin(m_Buf[3]);
		pos.carPos[2][0]=0.0;
			
		//2列目
		pos.carPos[0][1]=-sin(m_Buf[3]);
		pos.carPos[1][1]=cos(m_Buf[3]);
		pos.carPos[2][1]=0.0;
			
		//3列目
		pos.carPos[0][2]=0.0;
		pos.carPos[1][2]=0.0;
		pos.carPos[2][2]=1.0;
			
		//4列目
		pos.carPos[0][3]=m_Buf[0];
		pos.carPos[1][3]=m_Buf[1];
		pos.carPos[2][3]=m_Buf[2];

		m_rid=m_ManipulatorCommonInterface_Middle->movePTPCartesianAbs(pos);
		if(m_rid->id != 0){//Error
			std::cout<<"Move PTP Cartesian Abs is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(180000/m_Speed);

	}else if(strcmp(m_command,"JMOV") == 0){//関節座標系の直線補間(関節・絶対指令)

		std::cout<<"Move PTP Joint Abs"<<std::endl;

		JARA_ARM::JointPos jnt;
		jnt.length(4);
		jnt[0]=m_Buf[0];//[rad]
		jnt[1]=m_Buf[1];//[rad]
		jnt[2]=m_Buf[2];//[m]
		jnt[3]=m_Buf[3];//[rad]

		m_rid=m_ManipulatorCommonInterface_Middle->movePTPJointAbs(jnt);
		if(m_rid->id != 0){//Error
			std::cout<<"Move PTP Joint Abs is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(180000/m_Speed);

	}else if(strcmp(m_command,"HAND_OPEN") == 0){//ハンドオープン

		std::cout<<"Open Gripper"<<std::endl;
		m_rid=m_ManipulatorCommonInterface_Middle->openGripper();
		if(m_rid->id != 0){//Error
			std::cout<<"Open Gripper is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(120000/m_Speed);

	}else{//定義されていないコマンド

		std::cout<<"Command :【"<<m_command<<"】 is NOT IMPLEMENTED"<<std::endl;
		return RTC::RTC_ERROR;

	}
	
	return RTC::RTC_OK;
}


extern "C"
{
 
  void ScaraRobotControlRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(scararobotcontrolrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<ScaraRobotControlRTC>,
                             RTC::Delete<ScaraRobotControlRTC>);
  }
  
};


