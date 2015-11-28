/* 
 * "AR_Finder.cpp"
 * 
 * 元ファイル (修正BSDライセンス)
 * "HMDを用いたPTカメラのインターネット遠隔操作"
 * "AR_data_out": ARToolkitを使用しARマーカの姿勢計算し、出力 
 * http://www.openrtm.org/openrtm/ja/project/contest2014_22
 * 
 */

#include "DetectArMarkerRTC.h"

DetectArMarkerRTC* GpDlg;
bool ReturnFlag = 0;

bool AR_finder(LPVOID pParam)
{
	// 
	GpDlg = (DetectArMarkerRTC*)pParam;

	// 構造体初期化
	GpDlg -> object[0].patt_name = GpDlg -> m_PatternName1.c_str();	// パターンファイル名
	GpDlg -> object[1].patt_name = GpDlg -> m_PatternName2.c_str();
	GpDlg -> object[2].patt_name = GpDlg -> m_PatternName3.c_str();
	for(int i = 0; i < 3; i++) {
		GpDlg -> object[i].patt_id = -1;							// パターンID
		GpDlg -> object[i].mark_id = i + 1;							// マーカID
		GpDlg -> object[i].visible = 0;								// 検出フラグ
		GpDlg -> object[i].width = GpDlg -> m_PatternWidth * 1000;	// パターン幅 [mm]
		for(int j = 0; j < 3; j++) {								// パターン中心座標
			GpDlg -> object[i].center[j] = 0.0;
		}
	}

	// 閾値初期化
	GpDlg -> thresh = 100;

	// GLUT初期化
	int argc = 1;
	char* argv[] = {"NULL"};
	glutInit(&argc, argv);
	
	// ビデオデバイス設定
	int length = GpDlg -> m_VideoConf.length();
	char* vconf = new char[length+1];
	memcpy(vconf, GpDlg -> m_VideoConf.c_str(), length+1);
	if(arVideoOpen(vconf) < 0) {
		std::cout<<"Error: Video device setting"<<std::endl;
		return 1;
	}
	
	// ウィンドウサイズ取得
	int	xsize, ysize;
	if(arVideoInqSize(&xsize, &ysize) < 0) {
		std::cout<<"Error: Window size getting"<<std::endl;
		return 1;
	}
	std::cout<<"Image size (x,y) = ("<<xsize<<","<<ysize<<")"<<std::endl;

	// カメラパラメータ読み込み
	const char *cparam_name = GpDlg -> m_CameraPara.c_str();
	ARParam	wparam;
	if(arParamLoad(cparam_name, 1, &wparam) < 0) {
		std::cout<<"Error: Camera parameter loading"<<std::endl;
		return 1;
	}

	// カメラパラメータ初期化
	ARParam cparam;
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	arInitCparam( &cparam );
	std::cout<<"*** Camera Parameter ***"<<std::endl;
	arParamDisp( &cparam );

	for(int i = 0; i < 3; i++){
		if((GpDlg -> object[i].patt_id = arLoadPatt(GpDlg -> object[i].patt_name)) < 0) {
			std::cout<<"Error: Camera parameter initializing"<<std::endl;
			return 1;
		}
	}
	
	// gsubライブラリ初期化
	argInit(&cparam, 1.0, 0, 0, 0, 0);

	// キャプチャ開始
	arVideoCapStart();

	// メインループ呼び出し
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	argMainLoop(NULL, keyEvent, mainloop);

	return ReturnFlag;
}

// Main loop
static void mainloop(void)
{
	//
	
	// カメラ画像の取得
	ARUint8 *dataPtr;
	if((dataPtr = (ARUint8 *)arVideoGetImage()) == NULL) {
		arUtilSleep(2);
		return;
	}

	// 処理フレーム数
	//int	count_ar = 0;	
	//if(count_ar == 0) {
	//	arUtilTimerReset();
	//}
	//count_ar++;

	// キャプチャした画像の表示
	argDrawMode2D();
	argDispImage(dataPtr, 0, 0);

	// マーカ検出
	ARMarkerInfo *marker_info; // マーカ検出用の情報
	int marker_num; // マーカーらしき部分の個数
	if(arDetectMarker(dataPtr, GpDlg -> thresh, &marker_info, &marker_num) < 0) {
		cleanup();
		std::cout<<"Error: Marker detecting"<<std::endl;
		ReturnFlag = 1;
		glutLeaveMainLoop();
	}
	
	// 次の画像をキャプチャ
	arVideoCapNext();
	
	// 3次元オブジェクトの描画準備
	argDrawMode3D();
	argDraw3dCamera(0, 0);
	
	// デプス・バッファの初期化
	glClearDepth(1.0);				// デプス・バッファの消去値
	glClear(GL_DEPTH_BUFFER_BIT);	// デプス・バッファの初期化

	// マーカーの一致度判定
	for(int i = 0; i < 3; i++) {
		int k = -1;
		for(int j = 0; j < marker_num; j++) {
			if(GpDlg -> object[i].patt_id == marker_info[j].id) {
				if(k == -1) {
					k = j;
				}
				else if(marker_info[k].cf < marker_info[j].cf) {
					k = j;
				}
			}
		}

		// マーカが見つからなかったとき
		if(k == -1) {
			GpDlg -> object[i].visible = 0;
			continue;
		}

		// 座標変換行列を取得
		if(GpDlg -> object[i].visible == 0) {
			// 1フレームを使って座標変換行列を取得
			arGetTransMat(&marker_info[k],GpDlg -> object[i].center, GpDlg -> object[i].width, GpDlg -> object[i].trans);
		}
		else{
			// 前のフレームを使って座標変換行列を取得
			arGetTransMatCont(&marker_info[k], GpDlg -> object[i].trans, GpDlg -> object[i].center, GpDlg -> object[i].width, GpDlg -> object[i].trans);
  		}
		GpDlg -> object[i].visible = 1;

		// 3次元オブジェクトの描画処理
		draw(GpDlg -> object[i].mark_id, GpDlg -> object[i].trans);
	}

	// バッファの内容を出力
	argSwapBuffers();

	// マーカが見つかったとき
	double wmat1[3][4] = {}; // 変換行列
	double wmat2[3][4] = {};
	double wmat3[3][4] = {};
	if(GpDlg -> object[0].visible > 0 || GpDlg -> object[1].visible> 0 || GpDlg -> object[2].visible> 0){
		//どのマーカが見つかっているか判別
		int mflag = 0;
		for(int flag_num = 0; flag_num < 3; flag_num++) { 
			if(GpDlg -> object[flag_num].visible == 1) { 
				mflag = flag_num;
				if(flag_num == 0){ 
					memcpy(wmat1, GpDlg -> object[flag_num].trans, sizeof(GpDlg -> object[flag_num].trans)); //マーカ1の座標系でカメラの位置を取得
				}
				if(flag_num == 1){ 
					memcpy(wmat2, GpDlg -> object[flag_num].trans, sizeof(GpDlg -> object[flag_num].trans)); //マーカ2の座標系でカメラの位置を取得
				}
				if(flag_num == 2){ 
					memcpy(wmat3, GpDlg -> object[flag_num].trans, sizeof(GpDlg -> object[flag_num].trans)); //マーカ3の座標系でカメラの位置を取得
				}
			}
		}
	}
	
	// マーカ位置座標をOutPortへ出力する
	GpDlg -> m_Coord.data[0] = wmat1[0][3] / 1000; // マーカ1の位置座標(x, y, z) [m]
	GpDlg -> m_Coord.data[1] = wmat1[2][3] / 1000;
	GpDlg -> m_Coord.data[2] = wmat1[1][3] / 1000;
	GpDlg -> m_Coord.data[3] = wmat2[0][3] / 1000; // マーカ2の位置座標(x, y, z) [m]
	GpDlg -> m_Coord.data[4] = wmat2[2][3] / 1000;
	GpDlg -> m_Coord.data[5] = wmat2[1][3] / 1000;
	GpDlg -> m_Coord.data[6] = wmat3[0][3] / 1000; // マーカ3の位置座標(x, y, z) [m]
	GpDlg -> m_Coord.data[7] = wmat3[2][3] / 1000;
	GpDlg -> m_Coord.data[8] = wmat3[1][3] / 1000;

	// OutPort更新
	GpDlg -> m_CoordOut.write();

	// 出力
	std::cout<<"("<<std::setprecision(1)<<std::fixed
		     <<wmat1[0][3]<<", "<<wmat1[2][3]<<", "<<wmat1[1][3]<<", "
			 <<wmat2[0][3]<<", "<<wmat2[2][3]<<", "<<wmat2[1][3]<<", "
			 <<wmat3[0][3]<<", "<<wmat3[2][3]<<", "<<wmat3[1][3]<<") [mm]"<<std::endl;
}

void draw(int mid,double patt_trans[3][4])
{
	double gl_para[16];						// ARToolkitから変換したOpenGLの行列
    
	// 3次元オブジェクトの描画準備
	argDrawMode3D();
	argDraw3dCamera(0, 0);
    
	// 隠面消去
	glClearDepth(1.0);						// デプス・バッファの消去値
	glClear(GL_DEPTH_BUFFER_BIT);			// デプス・バッファの初期化
	glEnable(GL_DEPTH_TEST);				// 隠面消去・有効
	glDepthFunc(GL_LEQUAL);					// デプス・テスト
    
	// 座標変換行列の読み込み
	argConvGlpara(patt_trans, gl_para);		// ARToolkit -> OpenGL
	glMatrixMode(GL_MODELVIEW);				// 行列変換モード・モデルビュー
	glLoadMatrixd(gl_para);					// 読み込む行列を指定

	// ライティング
	setupLighting();						// ライトの定義
	glEnable(GL_LIGHTING);					// ライティング・有効
	glEnable(GL_LIGHT0);					// ライト0・オン
	glEnable(GL_LIGHT1);					// ライト1・オン

	// オブジェクトの材質
	setupMaterial();

	// 3次元オブジェクトの描画
	glTranslatef(0.0, 0.0, -10.0);			// オブジェクトの平行移動
	glutWireCube(17.0);						// ソリッドキューブを描画
		
	// 終了処理
	glDisable(GL_LIGHTING);					// ライティング・無効
	glDisable(GL_DEPTH_TEST);				// デプス・テスト・無効
}

static void setupLighting(void)
{
	// ライトの定義
	GLfloat	lt0_position[] = {100.0, -200.0, 200.0, 0.0};	// ライト0の位置
	GLfloat	lt0_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          環境光
	GLfloat	lt0_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          拡散光
	//
	GLfloat	lt1_position[] = {-100.0, 200.0, 200.0, 0.0};	// ライト1の位置
	GLfloat	lt1_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          環境光
	GLfloat	lt1_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          拡散光

	// ライトの設定
	glLightfv(GL_LIGHT0, GL_POSITION, lt0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lt0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lt0_diffuse);
	//
	glLightfv(GL_LIGHT1, GL_POSITION, lt1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lt1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lt1_diffuse);
}

static void setupMaterial(void)
{
	// オブジェクトの材質
	GLfloat	mat_ambient[]   = {0.0, 0.0, 1.0, 1.0};		// 材質の環境光
	GLfloat	mat_specular[]  = {0.0, 0.0, 1.0, 1.0};		// 鏡面光
	GLfloat	mat_shininess[] = {50.0};					// 鏡面係数
	//
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

static void cleanup(void)
{
	// 終了処理
	arVideoCapStop();	// キャプチャを停止
	arVideoClose();		// ビデオデバイスを終了
	argCleanup();		// ARToolkitの終了処理
}

static void keyEvent( unsigned char key, int x, int y)
{
	switch(key) {
	case 0x1b:
		std::cout<<std::endl<<key<<std::endl<<std::endl;
		std::cout<<std::endl<<"Waiting..."<<std::endl<<std::endl;
		cleanup();
		glutLeaveMainLoop();
		break;
	case 'p':
	case 'P':
		if(GpDlg -> thresh < 255) {
			//std::cout<<"Change ImageThreshold from "<<GpDlg -> thresh<<" to "<<GpDlg -> thresh + 1<<std::endl;
			GpDlg -> thresh = GpDlg -> thresh + 1;
		}
		else {
			//std::cout<<"Not change ImageThreshold (Now max 255)"<<std::endl;
		}
		break;
	case 'm':
	case 'M':
		if(GpDlg -> thresh > 0) {
			//std::cout<<"Change ImageThreshold from "<<GpDlg -> thresh<<" to "<<GpDlg -> thresh - 1<<std::endl;
			GpDlg -> thresh = GpDlg -> thresh - 1;
		}
		else {
			//std::cout<<"Not change ImageThreshold (Now min 0)"<<std::endl;
		}
		break;
	case 'n':
	case 'N':
		std::cout<<"Change ImageThreshold is "<<GpDlg -> thresh<<std::endl;
		Sleep(1000);
		break;
	default:
		break;
	}
}
