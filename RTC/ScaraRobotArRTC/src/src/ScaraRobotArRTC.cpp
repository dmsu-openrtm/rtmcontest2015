// -*- C++ -*-
/*!
 * @file  ScaraRobotArRTC.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */
#define _USE_MATH_DEFINES

#include "ScaraRobotArRTC.h"

// Module specification
// <rtc-template block="module_spec">
static const char* scararobotarrtc_spec[] =
  {
    "implementation_id", "ScaraRobotArRTC",
    "type_name",         "ScaraRobotArRTC",
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
    "conf.default.Speed", "30",
    "conf.default.BaseOffsetX", "0.0",
    "conf.default.BaseOffsetY", "0.0",
    "conf.default.BaseOffsetZ", "0.0",
    "conf.default.RobotName", "ROBOT_NAME",
	"conf.default.CoordMaxX", "0.04",
	"conf.default.CoordMaxY", "0.07",
    // Widget
    "conf.__widget__.Speed", "text",
    "conf.__widget__.BaseOffsetX", "text",
    "conf.__widget__.BaseOffsetY", "text",
    "conf.__widget__.BaseOffsetZ", "text",
    "conf.__widget__.RobotName", "text",
	"conf.__widget__.CoordMaxX", "text",
	"conf.__widget__.CoordMaxY", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ScaraRobotArRTC::ScaraRobotArRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_CoordIn("Coord", m_Coord),
    m_digitalOutputOut("digitalOutput", m_digitalOutput),
    m_ManipulatorCommonInterface_CommonPort("ManipulatorCommonInterface_Common"),
    m_ManipulatorCommonInterface_MiddlePort("ManipulatorCommonInterface_Middle")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ScaraRobotArRTC::~ScaraRobotArRTC()
{
}



RTC::ReturnCode_t ScaraRobotArRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Coord", m_CoordIn);
  
  // Set OutPort buffer
  addOutPort("digitalOutput", m_digitalOutputOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  m_ManipulatorCommonInterface_CommonPort.registerConsumer("JARA_ARM_ManipulatorCommonInterface_Common", "JARA_ARM::ManipulatorCommonInterface_Common", m_JARA_ARM_ManipulatorCommonInterface_Common);
  m_ManipulatorCommonInterface_MiddlePort.registerConsumer("JARA_ARM_ManipulatorCommonInterface_Middle", "JARA_ARM::ManipulatorCommonInterface_Middle", m_JARA_ARM_ManipulatorCommonInterface_Middle);
  
  // Set CORBA Service Ports
  addPort(m_ManipulatorCommonInterface_CommonPort);
  addPort(m_ManipulatorCommonInterface_MiddlePort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("Speed", m_Speed, "30");
  bindParameter("BaseOffsetX", m_BaseOffsetX, "0.0");
  bindParameter("BaseOffsetY", m_BaseOffsetY, "0.0");
  bindParameter("BaseOffsetZ", m_BaseOffsetZ, "0.0");
  bindParameter("RobotName", m_RobotName, "ROBOT_NAME");
  bindParameter("CoordMaxX", m_CoordMaxX, "0.04");
  bindParameter("CoordMaxY", m_CoordMaxY, "0.07");
  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ScaraRobotArRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotArRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotArRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ScaraRobotArRTC::onActivated(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"           Activate           "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;

	// 変数の初期化
	m_Coord.data.length(9);
	m_digitalOutput.data.length(2);

	// 動作速度設定
	std::cout<<"Set Speed"<<std::endl<<std::endl;
	if((m_Speed <= 0) || (m_Speed > 100)) { // Error
		std::cout<<"Value is Wrong"<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}
		
	m_rid = m_JARA_ARM_ManipulatorCommonInterface_Middle->setSpeedCartesian(m_Speed);
	if(m_rid -> id != 0) { // Error
		std::cout<<"Set Speed Cartesian is Failure"<<std::endl;
		std::cout<<m_rid -> comment<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}

	m_rid = m_JARA_ARM_ManipulatorCommonInterface_Middle->setSpeedJoint(m_Speed);
	if(m_rid -> id != 0) { // Error
		std::cout<<"Set Speed Joint is Failure"<<std::endl;
		std::cout<<m_rid -> comment<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}

	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"        Set BaseOffset        "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;
	
	// サーボON
	strcpy(m_command, "SERVO_ON");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	// アームを初期位置へ移動
	if(m_RobotName == "SCARA") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = 0.005;	// [m]
			m_Buf[3] = 0;		// [rad]
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = M_PI/2;	// [rad]
			m_Buf[3] = 0;		// [rad]
			m_Buf[4] = M_PI/2;	// [rad]
			m_Buf[5] = 0;		// [rad]
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}
	strcpy(m_command, "JMOV");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	// ハンドを閉じる
	strcpy(m_command, "HAND_CLOSE");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}
		
	// サーボOFF
	strcpy(m_command, "SERVO_OFF");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	// ベースオフセット初期化
	offset[0][0] = 1.0; offset[0][1] = 0.0; offset[0][2] = 0.0; offset[0][3] = 0.0;
	offset[1][0] = 0.0; offset[1][1] = 1.0; offset[1][2] = 0.0; offset[1][3] = 0.0;
	offset[2][0] = 0.0; offset[2][1] = 0.0; offset[2][2] = 1.0; offset[2][3] = 0.0;
	m_rid = m_JARA_ARM_ManipulatorCommonInterface_Middle -> setBaseOffset(offset);
	if(m_rid -> id != 0) { // Error
		std::cout<<"Set Base Offset is Failure"<<std::endl;
		std::cout<<m_rid -> comment<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}

	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"(1) Move the robot hand to the point of origin manually."<<std::endl;
	std::cout<<"(2) Return 's' and 'Enter' key. If you want to exit, return 'e' and 'Enter' key."<<std::endl<<std::endl;

	// ユーザによる設定待機
	char c[1];
	while(1){
		std::cin>>c;
		if(strcmp(c,"e") == 0) {
			//this->get_context(0)->deactivate_component(this->getObjRef());
			this->deactivate(ec_id);
			return RTC::RTC_OK;
		}else if(strcmp(c,"s") == 0){
			break;
		}
	}

	// サーボON
	strcpy(m_command, "SERVO_ON");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}
		
	// ユーザによる設定位置取得
	m_JARA_ARM_ManipulatorCommonInterface_Middle->getFeedbackPosCartesian(m_CarPos);
		
	// ベースオフセット設定
	offset[0][0] = 1.0; offset[0][1] = 0.0; offset[0][2] = 0.0;
	offset[1][0] = 0.0; offset[1][1] = 1.0; offset[1][2] = 0.0;
	offset[2][0] = 0.0; offset[2][1] = 0.0; offset[2][2] = 1.0;
	// 4th column
	offset[0][3] = m_BaseOffsetX + m_CarPos.carPos[0][3];
	offset[1][3] = m_BaseOffsetY + m_CarPos.carPos[1][3];
	if(m_RobotName == "SCARA") {
		offset[2][3] = m_BaseOffsetZ;
	}
	else if(m_RobotName == "MITSUBISHI") {
		offset[2][3] = m_BaseOffsetZ + m_CarPos.carPos[2][3];
	}
	else {
		std::cout<<"Unknown ROBOT_NAME"<<std::endl;
		return RTC::RTC_ERROR;
	}
	m_rid = m_JARA_ARM_ManipulatorCommonInterface_Middle -> setBaseOffset(offset);
	if(m_rid -> id != 0) { // Error
		std::cout<<"Set Base Offset is Failure"<<std::endl;
		std::cout<<m_rid -> comment<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}
	
	// 設定ベースオフセット値の表示
	std::cout<<"BaseOffsetX = "<<offset[0][3]<<std::endl;
	std::cout<<"BaseOffsetY = "<<offset[1][3]<<std::endl;
	std::cout<<"BaseOffsetZ = "<<offset[2][3]<<std::endl<<std::endl;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::cout<<std::endl<<"***************************"<<std::endl;
	std::cout<<"        Calibration        "<<std::endl;
	std::cout<<"***************************"<<std::endl<<std::endl;

	// アームを初期位置へ移動
	if(m_RobotName == "SCARA") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = 0.06;	// [m]
			m_Buf[3] = 0;		// [rad]
	}
	else if(m_RobotName == "MITSUBISHI") {
		m_Buf[0] = 0;		// [rad]
		m_Buf[1] = 0;		// [rad]
		m_Buf[2] = M_PI/2;	// [rad]
		m_Buf[3] = 0;		// [rad]
		m_Buf[4] = M_PI/2;	// [rad]
		m_Buf[5] = 0;		// [rad]
	}
	else {
		std::cout<<"Unknown ROBOT_NAME"<<std::endl;
		return RTC::RTC_ERROR;
	}
	strcpy(m_command, "JMOV");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	// サーボOFF
	strcpy(m_command, "SERVO_OFF");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"Take center and four corners."<<std::endl<<std::endl;

	// calib_mat 初期化
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 3; j++) {
			calib_mat[i][j] = 0;
		}
	}

	// マーカ値取得
	for(int i = 0; i < 5; i++) {
		std::cout<<std::endl<<"******************************"<<std::endl;
		std::cout<<"********** Step "<<i + 1<<" **********"<<std::endl;
			
		switch(i) {
		case 0:
			std::cout<<std::endl<<"    |   "<<std::endl;
			std::cout<<" x  |   "<<std::endl;
			std::cout<<"<---*---"<<std::endl;
			std::cout<<"    |   "<<std::endl;
			std::cout<<"   y|   "<<std::endl;
			std::cout<<"    v   "<<std::endl;
			break;
		case 1:
			std::cout<<std::endl<<"    |  *"<<std::endl;
			std::cout<<" x  |   "<<std::endl;
			std::cout<<"<---+---"<<std::endl;
			std::cout<<"    |   "<<std::endl;
			std::cout<<"   y|   "<<std::endl;
			std::cout<<"    v   "<<std::endl;
			break;
		case 2:
			std::cout<<std::endl<<" *  |   "<<std::endl;
			std::cout<<" x  |   "<<std::endl;
			std::cout<<"<---+---"<<std::endl;
			std::cout<<"    |   "<<std::endl;
			std::cout<<"   y|   "<<std::endl;
			std::cout<<"    v   "<<std::endl;
			break;
		case 3:
			std::cout<<std::endl<<"    |   "<<std::endl;
			std::cout<<" x  |   "<<std::endl;
			std::cout<<"<---+---"<<std::endl;
			std::cout<<"    |   "<<std::endl;
			std::cout<<" * y|   "<<std::endl;
			std::cout<<"    v   "<<std::endl;
			break;
		case 4:
			std::cout<<std::endl<<"    |   "<<std::endl;
			std::cout<<" x  |   "<<std::endl;
			std::cout<<"<---+---"<<std::endl;
			std::cout<<"    |   "<<std::endl;
			std::cout<<"   y|  *"<<std::endl;
			std::cout<<"    v   "<<std::endl;
			break;
		default:
			break;
		}
		
		std::cout<<std::endl<<"(1) Put 'R' marker."<<std::endl;
		std::cout<<"(2) Press 'Enter' key."<<std::endl;

		// マーカが置かれるまで待機
		getch();
		Sleep(1000);
		
		// 座標取得
		for(int j = 0; j < 10; j++) {
			Sleep(100);
			m_CoordIn.read();
			for(int k = 0; k < 3; k++) {
				calib_mat[i][k] = calib_mat[i][k] + m_Coord.data[k];
			}
		}

		// 移動平均値導出
		for(int j = 0; j < 3; j++) {
			calib_mat[i][j] = calib_mat[i][j] / 10;
		}
	}

	// "Detect_AR_Markers"から取得した値の表示
	std::cout<<std::endl;
	for(int i = 0; i < 5; i++ ) {
		std::cout<<calib_mat[i][0]<<" "<<calib_mat[i][1]<<" "<<calib_mat[i][2]<<std::endl;
	}
	std::cout<<std::endl;


	// 平面方程式の導出
	// (最小二乗法を利用した近似解)
	// 
	// a * x + b * y + c * z + d = 0
	// c = -1.0
		
	double lea_squ[9] = {};
	for(int i = 0; i < 5; i++) {
			lea_squ[0] = lea_squ[0] + calib_mat[i][0] * calib_mat[i][0]; // a(0)
			lea_squ[1] = lea_squ[1] + calib_mat[i][1] * calib_mat[i][1]; // b(1)
			lea_squ[2] = lea_squ[2] + calib_mat[i][0] * calib_mat[i][1]; // c(2)
			lea_squ[3] = lea_squ[3] + calib_mat[i][0] * calib_mat[i][2]; // d(3)
			lea_squ[4] = lea_squ[4] + calib_mat[i][1] * calib_mat[i][2]; // e(4)
			lea_squ[5] = lea_squ[5] + calib_mat[i][0];					 // f(5)
			lea_squ[6] = lea_squ[6] + calib_mat[i][1];					 // g(6)
			lea_squ[7] = lea_squ[7] + calib_mat[i][2];					 // h(7)
	}
	lea_squ[8] = 5; // n(8)
		
	double plane_mat[3][4];
	plane_mat[0][0] = lea_squ[0]; // a(0)
	plane_mat[0][1] = lea_squ[2]; // c(2)
	plane_mat[0][2] = lea_squ[5]; // f(5)
	plane_mat[0][3] = lea_squ[3]; // d(3)
	plane_mat[1][0] = lea_squ[2]; // c(2)
	plane_mat[1][1] = lea_squ[1]; // b(1)
	plane_mat[1][2] = lea_squ[6]; // g(6)
	plane_mat[1][3] = lea_squ[4]; // e(4)
	plane_mat[2][0] = lea_squ[5]; // f(5)
	plane_mat[2][1] = lea_squ[6]; // g(6)
	plane_mat[2][2] = lea_squ[8]; // n(8)
	plane_mat[2][3] = lea_squ[7]; // h(7)

	// ガウス消去法
	Gauss3(plane_mat);

	// 係数行列への代入
	plane_coef[0] = plane_mat[0][3];
	plane_coef[1] = plane_mat[1][3];
	plane_coef[2] = -1.0;
	plane_coef[3] = plane_mat[2][3];
		
	// 結果の表示
	std::cout<<std::endl<<"a = "<<plane_coef[0]<<", b = "<<plane_coef[1]<<", c = "<<plane_coef[2]<<", d = "<<plane_coef[3]<<std::endl;

	// 座標変換行列の導出
	double wmat_x[4][5];
	double wmat_y[4][5];

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			wmat_x[i][j] = calib_mat[i][j];
		}
	}
	for(int i = 0; i < 4; i++) {
		wmat_x[i][3] = 1.0;
	}
	wmat_x[0][4] = 0.0;
	wmat_x[1][4] = -m_CoordMaxX;
	wmat_x[2][4] = m_CoordMaxX;
	wmat_x[3][4] = m_CoordMaxX;
		
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) {
			wmat_y[i][j] = calib_mat[i][j];
		}
	}
	for(int i = 0; i < 4; i++) {
		wmat_y[i][3] = 1.0;
	}
	wmat_y[0][4] = 0.0;
	wmat_y[1][4] = -m_CoordMaxY;
	wmat_y[2][4] = -m_CoordMaxY;
	wmat_y[3][4] = m_CoordMaxY;
		
	// ガウス消去法
	Gauss4(wmat_x);
	Gauss4(wmat_y);
		
	// 座標変換行列への代入
	for(int i = 0; i < 4; i++) {
		tmat[0][i] = wmat_x[i][4];
		tmat[1][i] = wmat_y[i][4];
	}

	// 3rd column
	for(int i = 0; i < 4; i++) {
		tmat[2][i] = 0;
	}

	// 4th column
	for(int i = 0; i < 3; i++) {
		tmat[3][i] = 0;
	}
	tmat[3][3] = 1;
	
	std::cout<<std::endl<<"Transformation matrix"<<std::endl;
	std::cout<<"( "<<tmat[0][0]<<" "<<tmat[0][1]<<" "<<tmat[0][2]<<" "<<tmat[0][3]<<" )"<<std::endl;
	std::cout<<"( "<<tmat[1][0]<<" "<<tmat[1][1]<<" "<<tmat[1][2]<<" "<<tmat[1][3]<<" )"<<std::endl;
	std::cout<<"( "<<tmat[2][0]<<" "<<tmat[2][1]<<" "<<tmat[2][2]<<" "<<tmat[2][3]<<" )"<<std::endl;
	std::cout<<"( "<<tmat[3][0]<<" "<<tmat[3][1]<<" "<<tmat[3][2]<<" "<<tmat[3][3]<<" )"<<std::endl;

	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"(1) Put 'R', 'T' and 'M' marker."<<std::endl;
	std::cout<<"(2) Return 's' and 'Enter' key. If you want to exit, return 'e' and 'Enter' key."<<std::endl<<std::endl;

	// ユーザによる設定待機
	while(1){
		std::cin>>c;
		if(strcmp(c,"e") == 0) {
			//this->get_context(0)->deactivate_component(this->getObjRef());
			this->deactivate(ec_id);
			return RTC::RTC_OK;
		}else if(strcmp(c,"s") == 0){
			break;
		}
	}

	double coord[3][3];
	double dis[3][3];
	
	// coord 初期化
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			coord[i][j] = 0;
		}
	}
	
	// 座標取得
	for(int i = 0; i < 10; i++) {
		Sleep(100);
		m_CoordIn.read();
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				coord[k][j] = coord[k][j] + m_Coord.data[j + 3 * k];
			}
		}
	}

	// 移動平均値
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			coord[i][j] = coord[i][j] / 10;
		}
	}
		
	// 点と平面の距離 (z値の導出)
	for(int i = 0; i < 3; i++) {
		dis[i][2] = fabs(plane_coef[0] * coord[i][0] + plane_coef[1] * coord[i][1] + plane_coef[2] * coord[i][2] + plane_coef[3]) / sqrt(plane_coef[0] * plane_coef[0] + plane_coef[1] * plane_coef[1] + plane_coef[2] * plane_coef[2]);
	}

	double insec_k[3];
	double insec[3][3];

	// 点から平面に下ろした垂線と平面との交点の導出
	for(int i = 0; i < 3; i++) {
		insec_k[i] = -(plane_coef[0] * coord[i][0] + plane_coef[1] * coord[i][1] + plane_coef[2] * coord[i][2] + plane_coef[3]) / (plane_coef[0] * plane_coef[0] + plane_coef[1] * plane_coef[1] + plane_coef[2] * plane_coef[2]);
	}
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			insec[j][i] = coord[j][i] + insec_k[j] * plane_coef[i];
		}
	}
		
	// 交点を座標変換行列に代入 (x値，y値の導出)
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
		dis[j][i] = tmat[i][0] * insec[j][0] + tmat[i][1] * insec[j][1] + tmat[i][2] * insec[j][2] + tmat[i][3] * 1;
		}
	}

	// 座標の表示
	std::cout<<"Marker R ( "<<dis[0][0] * 1000<<", "<<dis[0][1] * 1000<<", "<<dis[0][2] * 1000<<" )"<<std::endl;
	std::cout<<"Marker T ( "<<dis[1][0] * 1000<<", "<<dis[1][1] * 1000<<", "<<dis[1][2] * 1000<<" )"<<std::endl;
	std::cout<<"Marker M ( "<<dis[2][0] * 1000<<", "<<dis[2][1] * 1000<<", "<<dis[2][2] * 1000<<" )"<<std::endl;

	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"(1) Check value."<<std::endl;
	std::cout<<"(2) Value is alomost correct, return 's' and 'Enter' key. If not, return 'e' and 'Enter' key to exit."<<std::endl<<std::endl;

	// ユーザによる設定待機
	while(1){
		std::cin>>c;
		if(strcmp(c,"e") == 0) {
			//this->get_context(0)->deactivate_component(this->getObjRef());
			this->deactivate(ec_id);
			return RTC::RTC_OK;
		}else if(strcmp(c,"s") == 0){
			break;
		}
	}

	// サーボON
	strcpy(m_command, "SERVO_ON");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"           Execute            "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;
	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaraRobotArRTC::onDeactivated(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"          Deactivate          "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;
	
	// ベースオフセット初期化
	offset[0][0] = 1.0; offset[0][1] = 0.0; offset[0][2] = 0.0; offset[0][3] = 0.0;
	offset[1][0] = 0.0; offset[1][1] = 1.0; offset[1][2] = 0.0; offset[1][3] = 0.0;
	offset[2][0] = 0.0; offset[2][1] = 0.0; offset[2][2] = 1.0; offset[2][3] = 0.0;
	m_rid = m_JARA_ARM_ManipulatorCommonInterface_Middle -> setBaseOffset(offset);
	if(m_rid -> id != 0) { // Error
		std::cout<<"Set Base Offset is Failure"<<std::endl;
		std::cout<<m_rid -> comment<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}

	// サーボOFF
	strcpy(m_command, "SERVO_OFF");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaraRobotArRTC::onExecute(RTC::UniqueId ec_id)
{
	double coord[3][3];
	double dis[3][3];
	double insec_k[3];
	double insec[3][3];
	double dis_mat[3][3];
	
	/**************************************/
	/********** キューブ積み上げ **********/
	/**************************************/
	
	if(GetAsyncKeyState(VK_ESCAPE)) {
		std::cout<<"Exit loop and deactivate RTC"<<std::endl;
		std::cout<<"Please wait..."<<std::endl;
		this->get_context(0)->deactivate_component(this->getObjRef());
		return RTC::RTC_OK;
	}

	// 目標位置
	double goal_mat[3][3];
	goal_mat[0][0] = -0.012;	// マーカM
	goal_mat[0][1] = 0.0;
	goal_mat[0][2] = 0.005;
	goal_mat[1][0] = 0.012;		// マーカT
	goal_mat[1][1] = 0.0;
	goal_mat[1][2] = 0.005;
	goal_mat[2][0] = 0.0;		// マーカR
	goal_mat[2][1] = 0.0;
	goal_mat[2][2] = 0.025;
	
	for(int i = 0; i < 3; i++) {

		// (0-1)アームを初期位置へ移動
		if(m_RobotName == "SCARA") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = 0.06;	// [m]
			m_Buf[3] = 0;		// [rad]
			
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = M_PI/2;	// [rad]
			m_Buf[3] = 0;		// [rad]
			m_Buf[4] = M_PI/2;	// [rad]
			m_Buf[5] = 0;		// [rad]
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}
		strcpy(m_command, "JMOV");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (0-2)ハンドを開いておく
		if(m_RobotName == "MITSUBISHI") {
			strcpy(m_command, "HAND_CLOSE");
			if(ActCommand() != RTC::RTC_OK) {
				return RTC::RTC_ERROR;
			}
		}
		strcpy(m_command, "HAND_OPEN");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// coord 初期化
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				coord[j][k] = 0;
			}
		}
	
		// 座標取得
		for(int j = 0; j < 10; j++) {
			Sleep(100);
			m_CoordIn.read();
			for(int k = 0; k < 3; k++) {
				for(int l = 0; l < 3; l++) {
					coord[l][k] = coord[l][k] + m_Coord.data[k + 3 * l]; // [m]
				}
			}
		}

		// 移動平均値
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				coord[j][k] = coord[j][k] / 10;
			}
		}
		
		// 点と平面の距離 (z値の導出)
		for(int j = 0; j < 3; j++) {
			dis[j][2] = fabs(plane_coef[0] * coord[j][0] + plane_coef[1] * coord[j][1] + plane_coef[2] * coord[j][2] + plane_coef[3]) / sqrt(plane_coef[0] * plane_coef[0] + plane_coef[1] * plane_coef[1] + plane_coef[2] * plane_coef[2]);
		}


		// 点から平面に下ろした垂線と平面との交点の導出
		for(int j = 0; j < 3; j++) {
			insec_k[j] = -(plane_coef[0] * coord[j][0] + plane_coef[1] * coord[j][1] + plane_coef[2] * coord[j][2] + plane_coef[3]) / (plane_coef[0] * plane_coef[0] + plane_coef[1] * plane_coef[1] + plane_coef[2] * plane_coef[2]);
		}
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				insec[k][j] = coord[k][j] + insec_k[k] * plane_coef[j];
			}
		}
		
		// 交点を座標変換行列に代入 (x値，y値の導出)
		for(int j = 0; j < 2; j++) {
			for(int k = 0; k < 3; k++) {
			dis[k][j] = tmat[j][0] * insec[k][0] + tmat[j][1] * insec[k][1] + tmat[j][2] * insec[k][2] + tmat[j][3] * 1;
			}
		}

		// マーカMが1番目
		for(int j = 0; j < 3; j++) {
		dis_mat[0][j] = dis[2][j];
		}
		// マーカTが2番目
		for(int j = 0; j < 3; j++) {
		dis_mat[1][j] = dis[1][j];
		}
		// マーカRが3番目
		for(int j = 0; j < 3; j++) {
		dis_mat[2][j] = dis[0][j];
		}

		// ソフトリミット判定 (2015/11/20追加)
		if(i == 0) { // マーカM
			if(dis_mat[0][0] < -(m_CoordMaxX + 0.01) || dis_mat[0][0] > m_CoordMaxX + 0.01) { // x値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
			if(dis_mat[0][1] < -(m_CoordMaxY + 0.01) || dis_mat[0][1] > m_CoordMaxY + 0.01) { // y値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else if(i == 1) { // マーカT
			if(dis_mat[1][0] < -(m_CoordMaxX + 0.01) || dis_mat[1][0] > m_CoordMaxX + 0.01) { // x値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
			if(dis_mat[1][1] < -(m_CoordMaxY + 0.01) || dis_mat[1][1] > m_CoordMaxY + 0.01) { // y値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else if(i == 2) { // マーカR
			if(dis_mat[2][0] < -(m_CoordMaxX + 0.01) || dis_mat[2][0] > m_CoordMaxX + 0.01) { // x値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
			if(dis_mat[2][1] < -(m_CoordMaxY + 0.01) || dis_mat[2][1] > m_CoordMaxY + 0.01) { // y値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else {
			return RTC::RTC_ERROR;
		}

		// (1-1)マーカ位置まで近づく動作
		m_Buf[0] = dis_mat[i][0];	// X[m]
		m_Buf[1] = dis_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		strcpy(m_command, "CMVS");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
				
		// (1-2)マーカ位置で下がる動作
		m_Buf[0] = dis_mat[i][0];	// X[m]
		m_Buf[1] = dis_mat[i][1];	// Y[m]
		m_Buf[2] = 0.005;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		strcpy(m_command, "CMOV");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-3)マーカ位置でつかむ動作
		if(m_RobotName == "SCARA") {
			m_Buf[0] = 40;
			strcpy(m_command, "HAND_MOV");
			if(ActCommand() != RTC::RTC_OK) {
				return RTC::RTC_ERROR;
			}
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_digitalOutput.data[0] = 1;
			m_digitalOutput.data[1] = 1;
			m_digitalOutputOut.write();
			Sleep(200);
			m_digitalOutput.data[0] = 0;
			m_digitalOutput.data[1] = 0;
			m_digitalOutputOut.write();
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}
		
		// (1-4)マーカ位置で上がる動作
		m_Buf[0] = dis_mat[i][0];	// X[m]
		m_Buf[1] = dis_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		strcpy(m_command, "CMOV");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-5)目標位置へ移動
		m_Buf[0] = goal_mat[i][0];	// X[m]
		m_Buf[1] = goal_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		strcpy(m_command, "CMVS");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-6)目標位置で下がる動作
		m_Buf[0] = goal_mat[i][0];	// X[m]
		m_Buf[1] = goal_mat[i][1];	// Y[m]
		m_Buf[2] = goal_mat[i][2];	// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		strcpy(m_command, "CMOV");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
			
		// (1-7)目標位置で離す動作
		strcpy(m_command, "HAND_OPEN");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-8)目標位置で上がる動作
		m_Buf[0] = goal_mat[i][0];	// X[m]
		m_Buf[1] = goal_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		strcpy(m_command, "CMOV");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
	}

	/**********************************/
	/********** キューブ崩し **********/
	/**********************************/
	
	if(GetAsyncKeyState(VK_ESCAPE)) {
		std::cout<<"Exit loop and deactivate RTC"<<std::endl;
		std::cout<<"Please wait..."<<std::endl;
		this->get_context(0)->deactivate_component(this->getObjRef());
		return RTC::RTC_OK;
	}

	// 目標位置
	goal_mat[0][0] = 0.030;
	goal_mat[0][1] = -0.050;
	goal_mat[0][2] = 0.005;
	goal_mat[1][0] = -0.040;
	goal_mat[1][1] = 0.040;
	goal_mat[1][2] = 0.005;
	goal_mat[2][0] = 0.030;
	goal_mat[2][1] = 0.060;
	goal_mat[2][2] = 0.005;
			
	for(int i = 0; i < 3; i++) {

		// (0-1)アームを初期位置へ移動
		if(m_RobotName == "SCARA") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = 0.06;	// [m]
			m_Buf[3] = 0;		// [rad]
			
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_Buf[0] = 0;		// [rad]
			m_Buf[1] = 0;		// [rad]
			m_Buf[2] = M_PI/2;	// [rad]
			m_Buf[3] = 0;		// [rad]
			m_Buf[4] = M_PI/2;	// [rad]
			m_Buf[5] = 0;		// [rad]
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}
		strcpy(m_command, "JMOV");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
	
		// (0-2)ハンドを開いておく
		if(m_RobotName == "MITSUBISHI") {
			strcpy(m_command, "HAND_CLOSE");
			if(ActCommand() != RTC::RTC_OK) {
				return RTC::RTC_ERROR;
			}
		}
		strcpy(m_command, "HAND_OPEN");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// coord 初期化
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				coord[j][k] = 0;
			}
		}
	
		// 座標取得
		for(int j = 0; j < 10; j++) {
			Sleep(100);
			m_CoordIn.read();
			for(int k = 0; k < 3; k++) {
				for(int l = 0; l < 3; l++) {
					coord[l][k] = coord[l][k] + m_Coord.data[k + 3 * l]; // [m]
				}
			}
		}

		// 移動平均値
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				coord[j][k] = coord[j][k] / 10;
			}
		}
		
		// 点と平面の距離 (z値の導出)
		for(int j = 0; j < 3; j++) {
			dis[j][2] = fabs(plane_coef[0] * coord[j][0] + plane_coef[1] * coord[j][1] + plane_coef[2] * coord[j][2] + plane_coef[3]) / sqrt(plane_coef[0] * plane_coef[0] + plane_coef[1] * plane_coef[1] + plane_coef[2] * plane_coef[2]);
		}


		// 点から平面に下ろした垂線と平面との交点の導出
		for(int j = 0; j < 3; j++) {
			insec_k[j] = -(plane_coef[0] * coord[j][0] + plane_coef[1] * coord[j][1] + plane_coef[2] * coord[j][2] + plane_coef[3]) / (plane_coef[0] * plane_coef[0] + plane_coef[1] * plane_coef[1] + plane_coef[2] * plane_coef[2]);
		}
		for(int j = 0; j < 3; j++) {
			for(int k = 0; k < 3; k++) {
				insec[k][j] = coord[k][j] + insec_k[k] * plane_coef[j];
			}
		}
		
		// 交点を座標変換行列に代入 (x値，y値の導出)
		for(int j = 0; j < 2; j++) {
			for(int k = 0; k < 3; k++) {
			dis[k][j] = tmat[j][0] * insec[k][0] + tmat[j][1] * insec[k][1] + tmat[j][2] * insec[k][2] + tmat[j][3] * 1;
			}
		}
	
		// マーカRが1番目
		for(int j = 0; j < 3; j++) {
			dis_mat[0][j] = dis[0][j];
		}
		// マーカTが2番目
		for(int j = 0; j < 3; j++) {
			dis_mat[1][j] = dis[1][j];
		}
		// マーカMが3番目
		for(int j = 0; j < 3; j++) {
			dis_mat[2][j] = dis[2][j];
		}

		// ソフトリミット判定 (2015/11/20追加)
		if(i == 0) { // マーカR
			if(dis_mat[0][0] < -(m_CoordMaxX + 0.01) || dis_mat[0][0] > m_CoordMaxX + 0.01) { // x値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
			if(dis_mat[0][1] < -(m_CoordMaxY + 0.01) || dis_mat[0][1] > m_CoordMaxY + 0.01) { // y値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else if(i == 1) { // マーカT
			if(dis_mat[1][0] < -(m_CoordMaxX + 0.01) || dis_mat[1][0] > m_CoordMaxX + 0.01) { // x値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
			if(dis_mat[1][1] < -(m_CoordMaxY + 0.01) || dis_mat[1][1] > m_CoordMaxY + 0.01) { // y値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else if(i == 2) { // マーカM
			if(dis_mat[2][0] < -(m_CoordMaxX + 0.01) || dis_mat[2][0] > m_CoordMaxX + 0.01) { // x値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
			if(dis_mat[2][1] < -(m_CoordMaxY + 0.01) || dis_mat[2][1] > m_CoordMaxY + 0.01) { // y値
				std::cout<<"Cartesian soft limits over!!"<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else {
			return RTC::RTC_ERROR;
		}

		// (1-1)マーカ位置まで近づく動作
		strcpy(m_command, "CMVS");
		m_Buf[0] = dis_mat[i][0];	// X[m]
		m_Buf[1] = dis_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
				
		// (1-2)マーカ位置で下がる動作
		strcpy(m_command, "CMOV");
		m_Buf[0] = dis_mat[i][0];			// X[m]
		m_Buf[1] = dis_mat[i][1];			// Y[m]
		if(i == 0) {
			m_Buf[2] = 0.030;	// Z[m]
		}
		else {
			m_Buf[2] = 0.005;	// Z[m]
		}
		m_Buf[3] = 0.0;						// C[rad]
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-3)マーカ位置でつかむ動作
		strcpy(m_command, "HAND_MOV");
		if(m_RobotName == "SCARA") {
			m_Buf[0] = 40;
			strcpy(m_command, "HAND_MOV");
			if(ActCommand() != RTC::RTC_OK) {
				return RTC::RTC_ERROR;
			}
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_digitalOutput.data[0] = 1;
			m_digitalOutput.data[1] = 1;
			m_digitalOutputOut.write();
			Sleep(200);
			m_digitalOutput.data[0] = 0;
			m_digitalOutput.data[1] = 0;
			m_digitalOutputOut.write();
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}

		// (1-4)マーカ位置で上がる動作
		strcpy(m_command, "CMOV");
		m_Buf[0] = dis_mat[i][0];	// X[m]
		m_Buf[1] = dis_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-5)目標位置へ移動
		strcpy(m_command, "CMVS");
		m_Buf[0] = goal_mat[i][0];	// X[m]
		m_Buf[1] = goal_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-6)目標位置で下がる動作
		strcpy(m_command, "CMOV");
		m_Buf[0] = goal_mat[i][0];	// X[m]
		m_Buf[1] = goal_mat[i][1];	// Y[m]
		m_Buf[2] = 0.01;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
			
		// (1-7)目標位置で離す動作
		strcpy(m_command, "HAND_OPEN");
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}

		// (1-8)目標位置で上がる動作
		strcpy(m_command, "CMOV");
		m_Buf[0] = goal_mat[i][0];	// X[m]
		m_Buf[1] = goal_mat[i][1];	// Y[m]
		m_Buf[2] = 0.06;			// Z[m]
		m_Buf[3] = 0.0;				// C[rad]
		if(ActCommand() != RTC::RTC_OK) {
			return RTC::RTC_ERROR;
		}
	}
	
	return RTC::RTC_OK;
}


RTC::ReturnCode_t ScaraRobotArRTC::onAborting(RTC::UniqueId ec_id)
{
	std::cout<<std::endl<<"******************************"<<std::endl;
	std::cout<<"            Error             "<<std::endl;
	std::cout<<"******************************"<<std::endl<<std::endl;
	
	// ベースオフセット初期化
	offset[0][0] = 1.0; offset[0][1] = 0.0; offset[0][2] = 0.0; offset[0][3] = 0.0;
	offset[1][0] = 0.0; offset[1][1] = 1.0; offset[1][2] = 0.0; offset[1][3] = 0.0;
	offset[2][0] = 0.0; offset[2][1] = 0.0; offset[2][2] = 1.0; offset[2][3] = 0.0;
	m_rid = m_JARA_ARM_ManipulatorCommonInterface_Middle -> setBaseOffset(offset);
	if(m_rid -> id != 0) { // Error
		std::cout<<"Set Base Offset is Failure"<<std::endl;
		std::cout<<m_rid -> comment<<std::endl<<std::endl;
		return RTC::RTC_ERROR;
	}

	// サーボOFF
	strcpy(m_command, "SERVO_OFF");
	if(ActCommand() != RTC::RTC_OK) {
		return RTC::RTC_ERROR;
	}

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ScaraRobotArRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotArRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotArRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ScaraRobotArRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



RTC::ReturnCode_t ScaraRobotArRTC::Gauss3(double wmat[][4])
{
	int n = 3;
	int max;
	double	pivot, temp;

	// 前進消去
	for(int i = 0; i < n - 1; i++) { 
		max = i;
		for(int j = i + 1; j < n; j++) { 
			if(fabs(wmat[j][i]) > fabs(wmat[max][i]) ) {
				max = j;
			}
		}
		if(i != max) {
			for(int k = i; k < n + 1; k++){ 
				temp = wmat[i][k];
				wmat[i][k] = wmat[max][k];
				wmat[max][k] = temp;
			}
		}
		for(int j = i + 1; j < n; j++) {
			pivot = wmat[j][i] / wmat[i][i];
			for(int k = i; k < n + 1; k++) {
				wmat[j][k] -= pivot * wmat[i][k];
			}
		}
	}
	// 後進代入
	for(int i = n - 1; i >= 0; i--) {
		pivot = wmat[i][n] / wmat[i][i];
		for(int j = 0; j < i; j++) {
			wmat[j][n] -= pivot * wmat[j][i];
		}
		wmat[i][n] = pivot;
	}

	return RTC::RTC_OK;
}

RTC::ReturnCode_t ScaraRobotArRTC::Gauss4(double wmat[][5])
{
	int n = 4;
	int max;
	double	pivot, temp;

	// 前進消去
	for(int i = 0; i < n - 1; i++) { 
		max = i;
		for(int j = i + 1; j < n; j++) { 
			if(fabs(wmat[j][i]) > fabs(wmat[max][i]) ) {
				max = j;
			}
		}
		if(i != max) {
			for(int k = i; k < n + 1; k++){ 
				temp = wmat[i][k];
				wmat[i][k] = wmat[max][k];
				wmat[max][k] = temp;
			}
		}
		for(int j= i + 1; j < n; j++){ 
			pivot = wmat[j][i] / wmat[i][i];
			for(int k = i; k < n+1; k++){ 
				wmat[j][k] -= pivot * wmat[i][k];
			}
		}
	}
	// 後進代入
	for(int i = n - 1; i >= 0; i--) {
		pivot = wmat[i][n] / wmat[i][i];
		for(int j = 0; j < i; j++) {
			wmat[j][n] -= pivot * wmat[j][i];
		}
		wmat[i][n] = pivot;
	}
	
	return RTC::RTC_OK;
}




RTC::ReturnCode_t ScaraRobotArRTC::ActCommand()
{
	if(strcmp(m_command,"SERVO_OFF") == 0){//サーボOFF
		
		std::cout<<"ServoOFF"<<std::endl;
		m_rid=m_JARA_ARM_ManipulatorCommonInterface_Common->servoOFF();
		if(m_rid->id != 0){//Error
			std::cout<<"ServoOFF is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(1000);

	}else if(strcmp(m_command,"SERVO_ON") == 0){//サーボON

		std::cout<<"ServoON"<<std::endl;
		m_rid=m_JARA_ARM_ManipulatorCommonInterface_Common->servoON();
		if(m_rid->id != 0){//Error
			std::cout<<"ServoON is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(1000);

	}else if(strcmp(m_command,"HAND_CLOSE") == 0){//ハンドクローズ

		//std::cout<<"Close Gripper"<<std::endl;
		if(m_RobotName == "SCARA") {
		
			m_rid=m_JARA_ARM_ManipulatorCommonInterface_Middle->closeGripper();
			if(m_rid->id != 0){//Error
				std::cout<<"Close Gripper is Failure"<<std::endl;
				std::cout<<m_rid->comment<<std::endl<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_digitalOutput.data[0] = 1;
			m_digitalOutput.data[1] = 1;
			m_digitalOutputOut.write();
			Sleep(350);
			m_digitalOutput.data[0] = 0;
			m_digitalOutput.data[1] = 0;
			m_digitalOutputOut.write();
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(120000/m_Speed-1000);

	}else if(strcmp(m_command,"HAND_MOV") == 0){//ハンド動作

		//std::cout<<"Move Gripper"<<std::endl;
		if((m_Buf[0] < 0)||(m_Buf[0] > 100)){
			std::cout<<"Value is Wrong"<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}
		m_rid=m_JARA_ARM_ManipulatorCommonInterface_Middle->moveGripper((JARA_ARM::ULONG)m_Buf[0]);
		if(m_rid->id != 0){//Error
			std::cout<<"Move Gripper is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(120000/m_Speed-1000);

	}else if(strcmp(m_command,"CMVS") == 0){//直交座標系の直線補間(絶対指令)

		//std::cout<<"Move Linear Cartesian Abs"<<std::endl;

		JARA_ARM::CarPosWithElbow pos;
		m_Buf[0] = m_Buf[0];//X[m]
		m_Buf[1] = m_Buf[1];//Y[m]
		m_Buf[2] = m_Buf[2];//Z[m]
		m_Buf[3]=m_Buf[3];//C[rad]

		if(m_RobotName == "SCARA") {
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
		}
		else if(m_RobotName == "MITSUBISHI") {
			//姿勢はX->Y->Z回転での値
			//1列目
			pos.carPos[0][0]=-cos(m_Buf[3]);
			pos.carPos[1][0]=-sin(m_Buf[3]);
			pos.carPos[2][0]=0.0;
			
			//2列目
			pos.carPos[0][1]=-sin(m_Buf[3]);
			pos.carPos[1][1]=cos(m_Buf[3]);
			pos.carPos[2][1]=0.0;
			
			//3列目
			pos.carPos[0][2]=0.0;
			pos.carPos[1][2]=0.0;
			pos.carPos[2][2]=-1.0;
			
			//4列目
			pos.carPos[0][3]=m_Buf[0];
			pos.carPos[1][3]=m_Buf[1];
			pos.carPos[2][3]=m_Buf[2];

			pos.structFlag = 7;
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}

		m_rid=m_JARA_ARM_ManipulatorCommonInterface_Middle->moveLinearCartesianAbs(pos);
		if(m_rid->id != 0){//Error
			std::cout<<"Move Linear Cartesian Abs is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(180000/m_Speed-1000);

	}else if(strcmp(m_command,"CMOV") == 0){//PTP補間動作(直交座標系)

		//std::cout<<"Move PTP Cartesian Abs"<<std::endl;

		JARA_ARM::CarPosWithElbow pos;
		m_Buf[0] = m_Buf[0];//X[m]
		m_Buf[1] = m_Buf[1];//Y[m]
		m_Buf[2] = m_Buf[2];//Z[m]
		m_Buf[3]=m_Buf[3];//C[rad]

		if(m_RobotName == "SCARA") {
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
		}
		else if(m_RobotName == "MITSUBISHI") {
			//姿勢はX->Y->Z回転での値
			//1列目
			pos.carPos[0][0]=-cos(m_Buf[3]);
			pos.carPos[1][0]=-sin(m_Buf[3]);
			pos.carPos[2][0]=0.0;
			
			//2列目
			pos.carPos[0][1]=-sin(m_Buf[3]);
			pos.carPos[1][1]=cos(m_Buf[3]);
			pos.carPos[2][1]=0.0;
			
			//3列目
			pos.carPos[0][2]=0.0;
			pos.carPos[1][2]=0.0;
			pos.carPos[2][2]=-1.0;
			
			//4列目
			pos.carPos[0][3]=m_Buf[0];
			pos.carPos[1][3]=m_Buf[1];
			pos.carPos[2][3]=m_Buf[2];

			pos.structFlag = 7;
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}

		m_rid=m_JARA_ARM_ManipulatorCommonInterface_Middle->movePTPCartesianAbs(pos);
		if(m_rid->id != 0){//Error
			std::cout<<"Move PTP Cartesian Abs is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(180000/m_Speed-1000);

	}else if(strcmp(m_command,"JMOV") == 0){//関節座標系の直線補間(関節・絶対指令)

		//std::cout<<"Move PTP Joint Abs"<<std::endl;

		JARA_ARM::JointPos jnt;

		if(m_RobotName == "SCARA") {
			jnt.length(4);
			jnt[0]=m_Buf[0];//[rad]
			jnt[1]=m_Buf[1];//[rad]
			jnt[2]=m_Buf[2];//[m]
			jnt[3]=m_Buf[3];//[rad]
		}
		else if(m_RobotName == "MITSUBISHI") {
			jnt.length(6);
			jnt[0]=m_Buf[0];//[rad]
			jnt[1]=m_Buf[1];//[rad]
			jnt[2]=m_Buf[2];//[rad]
			jnt[3]=m_Buf[3];//[rad]
			jnt[4]=m_Buf[4];//[rad]
			jnt[5]=m_Buf[5];//[rad]
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}

		m_rid=m_JARA_ARM_ManipulatorCommonInterface_Middle->movePTPJointAbs(jnt);
		if(m_rid->id != 0){//Error
			std::cout<<"Move PTP Joint Abs is Failure"<<std::endl;
			std::cout<<m_rid->comment<<std::endl<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(180000/m_Speed-1000);

	}else if(strcmp(m_command,"HAND_OPEN") == 0){//ハンドオープン

		//std::cout<<"Open Gripper"<<std::endl;
		if(m_RobotName == "SCARA") {
			m_rid=m_JARA_ARM_ManipulatorCommonInterface_Middle->openGripper();
			if(m_rid->id != 0){//Error
				std::cout<<"Open Gripper is Failure"<<std::endl;
				std::cout<<m_rid->comment<<std::endl<<std::endl;
				return RTC::RTC_ERROR;
			}
		}
		else if(m_RobotName == "MITSUBISHI") {
			m_digitalOutput.data[0] = 1;
			m_digitalOutput.data[1] = 0;
			m_digitalOutputOut.write();
			Sleep(200);
			m_digitalOutput.data[0] = 0;
			m_digitalOutput.data[1] = 0;
			m_digitalOutputOut.write();
		}
		else {
			std::cout<<"Unknown ROBOT_NAME"<<std::endl;
			return RTC::RTC_ERROR;
		}

		Sleep(120000/m_Speed-1000);

	}else{//定義されていないコマンド

		std::cout<<"Command :【"<<m_command<<"】 is NOT IMPLEMENTED"<<std::endl;
		return RTC::RTC_ERROR;

	}
	
	return RTC::RTC_OK;
}



extern "C"
{
 
  void ScaraRobotArRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(scararobotarrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<ScaraRobotArRTC>,
                             RTC::Delete<ScaraRobotArRTC>);
  }
  
};


