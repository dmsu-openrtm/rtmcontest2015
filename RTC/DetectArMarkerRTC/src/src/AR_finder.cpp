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

using namespace std;

/* カメラ構成 */
//char	*vconf;		// 既定のカメラ構成 (※ 関数"AR_finder"内で宣言 ※)
int	xsize, ysize;	// ウィンドウサイズ
int	thresh   = 100;	// 閾値(0～255)
int	count_ar = 0;	// 処理フレーム数

/* カメラパラメータ */
//const char *cparam_name; //  (※ 関数"AR_finder"内で宣言 ※)
ARParam cparam;

/* パターンファイル */
#define	OBJ_NUM			3	// 使用するマーカーの個数
#define	OBJ1_MARK_ID	1	// マーカーID
#define	OBJ2_MARK_ID	2
#define	OBJ3_MARK_ID	3
const char *obj1_patt_name; // パターンファイル名
const char *obj2_patt_name;
const char *obj3_patt_name;
double obj_size;			// パターンの幅 [mm]

/*OBJECT_T構造体によるパターンファイルの一括管理*/
typedef struct {
  const char	*patt_name;		// パターンファイル名
  int			patt_id;		// パターンID
  int			mark_id;		// マーカーID
  int			visible;		// 検出フラグ
  double		width;			// パターンの幅 [mm]
  double		center[2];		// パターンの中心座標
  double		trans[3][4];	// 座標変換行列
} OBJECT_T;
//-----

OBJECT_T	object[OBJ_NUM] = {
		{obj1_patt_name, -1, OBJ1_MARK_ID, 0, obj_size, {0.0,0.0}},
		{obj2_patt_name, -1, OBJ2_MARK_ID, 0, obj_size, {0.0,0.0}},
		{obj3_patt_name, -1, OBJ3_MARK_ID, 0, obj_size, {0.0,0.0}}
	};

// 関数のプロトタイプ宣言
static void init(void);	
static void mainloop(void);
void draw(int mid, double patt_trans[3][4]);
static void setupLighting(void);
static void setupMaterial(void);
static void cleanup(void);
static void keyEvent(unsigned char key, int x, int y);

char    *file_dir;
LPVOID  GParam;
ARParam	wparam;				// カメラパラメータ
bool    stop_flag=false;
DetectArMarkerRTC* GpDlg;

bool AR_finder(LPVOID pParam)
{
	// 変数宣言・定義
	DetectArMarkerRTC* pDlg = (DetectArMarkerRTC*)pParam;
	GpDlg = pDlg;
	int argc=1;
	char* argv[]={"NULL"};

	// コンフィギュレーション値からの値を代入
	// 既定のカメラ構成
	int length = GpDlg -> m_VideoConf.length();
	char* vconf = new char[length+1];
	memcpy(vconf, GpDlg -> m_VideoConf.c_str(), length+1);
	// カメラパラメータ
	const char *cparam_name = GpDlg -> m_CameraPara.c_str();
	// パターンファイル
	object[0].patt_name = GpDlg -> m_PatternName1.c_str();
	object[1].patt_name = GpDlg -> m_PatternName2.c_str();
	object[2].patt_name = GpDlg -> m_PatternName3.c_str();
	// パターン幅
	for(int i = 0; i < 3; i++) {
		object[i].width = GpDlg -> m_PatternWidth;
	}

//------------------------------------------------------------
//　メイン関数
//  初期化処理
//------------------------------------------------------------

	/* GLUTの初期化 */
	glutInit(&argc, argv);
	
	/* ビデオデバイスの設定 */
	if(arVideoOpen( vconf ) < 0) {
		return 1;
	}
	
	/* ウィンドウサイズの取得 */
	if(arVideoInqSize(&xsize, &ysize) < 0) {
		return 1;
	}
	std::cout<<"Image size (x,y) = ("<<xsize<<","<<ysize<<")"<<std::endl;

	/* カメラパラメータの読み込み */
	if(arParamLoad(cparam_name, 1, &wparam) < 0) {
		std::cout<<"Camera parameter load error !!"<<std::endl;
		return 1;
	}
	
	/* カメラパラメータの初期化 */
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	arInitCparam( &cparam );
	std::cout<<"*** Camera Parameter ***"<<std::endl;
	arParamDisp( &cparam );

	for(int i = 0; i < OBJ_NUM; i++){
		if((object[i].patt_id = arLoadPatt(object[i].patt_name)) < 0) {
			return 1;
		}
	}
	
	/* gsubライブラリの初期化 */
	argInit( &cparam, 1.0, 0, 0, 0, 0 );

	/* キャプチャ開始 */
	arVideoCapStart();

	/* メインループの呼び出し */
	argMainLoop( NULL, keyEvent, mainloop );
	
	return 1;
}

//------------------------------------------------------------
//　メインループ処理
//------------------------------------------------------------
static void mainloop(void)
{
	ARUint8			*dataPtr;			// カメラ画像
	ARMarkerInfo	*marker_info;		// マーカー検出用の情報
	int				marker_num;			// マーカーらしき部分の個数
	double			wmat1[3][4] = {};	// 変換行列
	double			wmat2[3][4] = {};
	double			wmat3[3][4] = {};

	/* カメラ画像の取得 */
	if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ){
		arUtilSleep(2);
		return;
	}
		
	if( count_ar == 0 ) arUtilTimerReset();
	count_ar++;

	/* キャプチャした画像の表示 */
	argDrawMode2D();
	argDispImage( dataPtr, 0, 0 );

	/* マーカーの検出 */
	if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
		cleanup();
		exit(0);
	}

	/* 次の画像をキャプチャ */
	arVideoCapNext();

	/* 3次元オブジェクトの描画準備 */
	argDrawMode3D();
	argDraw3dCamera(0, 0);

	/* デプス・バッファの初期化 */
	glClearDepth(1.0);				// デプス・バッファの消去値
	glClear(GL_DEPTH_BUFFER_BIT);	// デプス・バッファの初期化

	/* マーカーの一致度を判定 */
	for (int i = 0; i < OBJ_NUM; i++){
		int k = -1;
		for(int j = 0; j < marker_num; j++ ){
			if( object[i].patt_id == marker_info[j].id ){
				if( k == -1 ) k = j;
				else if( marker_info[k].cf < marker_info[j].cf ) k = j;
			}
		}

		/* マーカーが見つからなかったとき */
		if( k == -1 ){
			object[i].visible = 0;
			continue;
		}

		/* 座標変換行列を取得 */
		if( object[i].visible == 0 ){
			// １フレームを使って座標変換行列を取得
			arGetTransMat(&marker_info[k],
							object[i].center, 
							object[i].width, 
							object[i].trans);
  		}else{
			// 前のフレームを使って座標変換行列を取得
			arGetTransMatCont(&marker_info[k], 
								object[i].trans,
								object[i].center, 
								object[i].width, 
								object[i].trans);
  		}
		object[i].visible = 1;

		/* 3次元オブジェクトの描画処理 */
		draw(object[i].mark_id,object[i].trans);
	}

	/* バッファの内容を出力 */
	argSwapBuffers();

	/* マーカーが見つかったとき */
	if(object[0].visible > 0 || object[1].visible> 0 || object[2].visible> 0){
		//どのマーカーが見つかっているか判別
		int mflag = 0;

		for(int flag_num = 0; flag_num < OBJ_NUM; flag_num++) { 
			if(object[flag_num].visible	== 1) { 
				mflag = flag_num;
				if(flag_num == 0){ 
					memcpy(wmat1, object[flag_num].trans, sizeof(object[flag_num].trans));	//マーカー1の座標系でカメラの位置を取得
				}
				if(flag_num == 1){ 
					memcpy(wmat2, object[flag_num].trans, sizeof(object[flag_num].trans));	//マーカー2の座標系でカメラの位置を取得
				}
				if(flag_num == 2){ 
					memcpy(wmat3, object[flag_num].trans, sizeof(object[flag_num].trans));	//マーカー3の座標系でカメラの位置を取得
				}
			}
		}
	}
	
	/* 位置座標をOutPortへ出力する */

	// Sequenceの長さ
	GpDlg -> m_Coord.data.length(9);
		
	// マーカ1の位置座標(x, y, z)
	GpDlg -> m_Coord.data[0] = wmat1[0][3];
	GpDlg -> m_Coord.data[1] = wmat1[2][3];
	GpDlg -> m_Coord.data[2] = wmat1[1][3];

	// マーカ2の位置座標(x, y, z)
	GpDlg -> m_Coord.data[3] = wmat2[0][3];
	GpDlg -> m_Coord.data[4] = wmat2[2][3];
	GpDlg -> m_Coord.data[5] = wmat2[1][3];

	// マーカ3の位置座標(x, y, z)
	GpDlg -> m_Coord.data[6] = wmat3[0][3];
	GpDlg -> m_Coord.data[7] = wmat3[2][3];
	GpDlg -> m_Coord.data[8] = wmat3[1][3];

	// OutPort更新
	GpDlg -> m_CoordOut.write();

	// 出力
	printf("%.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f\n", GpDlg -> m_Coord.data[0], GpDlg -> m_Coord.data[1], GpDlg -> m_Coord.data[2], GpDlg -> m_Coord.data[3], GpDlg -> m_Coord.data[4], GpDlg -> m_Coord.data[5], GpDlg -> m_Coord.data[6], GpDlg -> m_Coord.data[7], GpDlg -> m_Coord.data[8]);	
}

//------------------------------------------------------------
//　3次元オブジェクトの描画処理
//------------------------------------------------------------
void draw(int mid,double patt_trans[3][4])
{
	double	gl_para[16];					// ARToolkitから変換したOpenGLの行列
    
	/* 3次元オブジェクトの描画準備 */
	argDrawMode3D();
	argDraw3dCamera(0, 0);
    
	/* 隠面消去 */
	glClearDepth(1.0);						// デプス・バッファの消去値
	glClear(GL_DEPTH_BUFFER_BIT);			// デプス・バッファの初期化
	glEnable(GL_DEPTH_TEST);				// 隠面消去・有効
	glDepthFunc(GL_LEQUAL);					// デプス・テスト
    
	/* 座標変換行列の読み込み */
	argConvGlpara(patt_trans, gl_para);		// ARToolkit -> OpenGL
	glMatrixMode(GL_MODELVIEW);				// 行列変換モード・モデルビュー
	glLoadMatrixd(gl_para);					// 読み込む行列を指定

	/* ライティング */
	setupLighting();						// ライトの定義
	glEnable(GL_LIGHTING);					// ライティング・有効
	glEnable(GL_LIGHT0);					// ライト0・オン
	glEnable(GL_LIGHT1);					// ライト1・オン

	/* オブジェクトの材質 */
	setupMaterial();

	/* 3次元オブジェクトの描画 */
	glTranslatef(0.0, 0.0, -10.0);			// オブジェクトの平行移動
	glutWireCube(17.0);						// ソリッドキューブを描画
		
	/* 終了処理 */
	glDisable(GL_LIGHTING);					// ライティング・無効
	glDisable(GL_DEPTH_TEST);				// デプス・テスト・無効
}
//------------------------------------------------------------
//　ライティング
//------------------------------------------------------------
static void setupLighting(void)
{
	/* ライトの定義 */
	GLfloat	lt0_position[] = {100.0, -200.0, 200.0, 0.0};	// ライト0の位置
	GLfloat	lt0_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          環境光
	GLfloat	lt0_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          拡散光
	//
	GLfloat	lt1_position[] = {-100.0, 200.0, 200.0, 0.0};	// ライト1の位置
	GLfloat	lt1_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          環境光
	GLfloat	lt1_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          拡散光

	/* ライトの設定 */
	glLightfv(GL_LIGHT0, GL_POSITION, lt0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lt0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lt0_diffuse);
	//
	glLightfv(GL_LIGHT1, GL_POSITION, lt1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lt1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lt1_diffuse);
}
//------------------------------------------------------------
//　オブジェクトの材質
//------------------------------------------------------------
static void setupMaterial(void)
{
	/* オブジェクトの材質 */
	GLfloat	mat_ambient[]   = {0.0, 0.0, 1.0, 1.0};		// 材質の環境光
	GLfloat	mat_specular[]  = {0.0, 0.0, 1.0, 1.0};		// 鏡面光
	GLfloat	mat_shininess[] = {50.0};					// 鏡面係数
	//
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
//------------------------------------------------------------
//　終了処理
//------------------------------------------------------------
static void cleanup(void)
{
	/* 終了処理 */
	arVideoCapStop();	// キャプチャを停止
	arVideoClose();		// ビデオデバイスを終了
	argCleanup();		// ARToolkitの終了処理
}
//------------------------------------------------------------
//　キーイベント処理
//------------------------------------------------------------
static void keyEvent( unsigned char key, int x, int y)
{
	/* ESCキーで終了 */
	if( key == 0x1b ) {
		cleanup();
		exit(0);
	}

}
