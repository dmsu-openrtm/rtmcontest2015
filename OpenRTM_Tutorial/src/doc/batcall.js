/**
* Call .bat file under %DRIVE_HOME%\demo
* @param {String} bat_name   Name of the .bat file. If the file is in subdirectory 
                  under `demo` folder, double-backslash '\\' needs to be used to separate
                  directories. 

                  Eg. If the .bat is at %DRIVE_HOME%/demo/demofolder-a/sample.bat, 
                      you pass demofolder-a\\sample.bat
* @return None
*/
function callbat_relativepath(bat_name) {
  var objFileSys = new ActiveXObject("Scripting.FileSystemObject");
  var objFile = objFileSys.GetFile(location.pathname.slice(1));
  var driveLetter = location.pathname.slice(1, 3);
  var path = objFile.ParentFolder + "\\..\\..\\..\\demo\\" + bat_name;
  var obj = new ActiveXObject("WScript.Shell");
  //alert(path);  // Comment in this life for debugging
  obj.Run(path + " " + driveLetter + " 2809"); // 2 values in double-quote represent arguments to the command passed.
}

function call_rtmnamerserver() {
    callbat_relativepath("rtm-naming.bat");
}

function call_rtseditor() {
    callbat_relativepath("RTSE.bat");
}

function call_OpencvRtmDemo_0() {
    callbat_relativepath("OpencvRtmDemo\\0_StartDemo.bat");
}

/* Added by Saitama Univ. Design Lab. */

function call_ScaraRobotDemo_1() {
    callbat_relativepath("ScaraRobotDemo\\1_StartDemo.bat");
}

function call_ScaraRobotDemo_2() {
    callbat_relativepath("ScaraRobotDemo\\2_CallCsv.bat");
}

function call_ScaraRobotDemo_8() {
    callbat_relativepath("ScaraRobotDemo\\8_ConnectRTC.bat");
}

function call_ScaraRobotDemo_11() {
    callbat_relativepath("ScaraRobotDemo\\11_DemoExit.bat");
}

function call_ScaraRobotPallet_StartRTC() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\StartRTC.bat");
}

function call_ScaraRobotPallet_ConnectAndActivateRTC() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\ConnectAndActivateRTC.bat");
}

function call_ScaraRobotPallet_Open_ArmarkerPdf() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\Open_ArmarkerPdf.bat");
}

function call_ScaraRobotPallet_Open_CalibrationGridPdf() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\Open_CalibrationGridPdf.bat");
}

function call_ScaraRobotPallet_Open_CalibDistPdf() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\Open_CalibDistPdf.bat");
}

function call_ScaraRobotPallet_Open_CalibCparaPdf() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\Open_CalibCparaPdf.bat");
}

function call_ScaraRobotPallet_Start_CalibCparam() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\Start_CalibCparam.bat");
}

function call_ScaraRobotPallet_Start_CalibDist() {
    callbat_relativepath("ScaraRobotPalletizingDemo\\Start_CalibDist.bat");
}
