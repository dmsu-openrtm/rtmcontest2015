/* 
 * "AR_Finder.cpp"
 * 
 * ���t�@�C�� (�C��BSD���C�Z���X)
 * "HMD��p����PT�J�����̃C���^�[�l�b�g���u����"
 * "AR_data_out": ARToolkit���g�p��AR�}�[�J�̎p���v�Z���A�o�� 
 * http://www.openrtm.org/openrtm/ja/project/contest2014_22
 * 
 */

#include "DetectArMarkerRTC.h"

using namespace std;

/* �J�����\�� */
//char	*vconf;		// ����̃J�����\�� (�� �֐�"AR_finder"���Ő錾 ��)
int	xsize, ysize;	// �E�B���h�E�T�C�Y
int	thresh   = 100;	// 臒l(0�`255)
int	count_ar = 0;	// �����t���[����

/* �J�����p�����[�^ */
//const char *cparam_name; //  (�� �֐�"AR_finder"���Ő錾 ��)
ARParam cparam;

/* �p�^�[���t�@�C�� */
#define	OBJ_NUM			3	// �g�p����}�[�J�[�̌�
#define	OBJ1_MARK_ID	1	// �}�[�J�[ID
#define	OBJ2_MARK_ID	2
#define	OBJ3_MARK_ID	3
const char *obj1_patt_name; // �p�^�[���t�@�C����
const char *obj2_patt_name;
const char *obj3_patt_name;
double obj_size;			// �p�^�[���̕� [mm]

/*OBJECT_T�\���̂ɂ��p�^�[���t�@�C���̈ꊇ�Ǘ�*/
typedef struct {
  const char	*patt_name;		// �p�^�[���t�@�C����
  int			patt_id;		// �p�^�[��ID
  int			mark_id;		// �}�[�J�[ID
  int			visible;		// ���o�t���O
  double		width;			// �p�^�[���̕� [mm]
  double		center[2];		// �p�^�[���̒��S���W
  double		trans[3][4];	// ���W�ϊ��s��
} OBJECT_T;
//-----

OBJECT_T	object[OBJ_NUM] = {
		{obj1_patt_name, -1, OBJ1_MARK_ID, 0, obj_size, {0.0,0.0}},
		{obj2_patt_name, -1, OBJ2_MARK_ID, 0, obj_size, {0.0,0.0}},
		{obj3_patt_name, -1, OBJ3_MARK_ID, 0, obj_size, {0.0,0.0}}
	};

// �֐��̃v���g�^�C�v�錾
static void init(void);	
static void mainloop(void);
void draw(int mid, double patt_trans[3][4]);
static void setupLighting(void);
static void setupMaterial(void);
static void cleanup(void);
static void keyEvent(unsigned char key, int x, int y);

char    *file_dir;
LPVOID  GParam;
ARParam	wparam;				// �J�����p�����[�^
bool    stop_flag=false;
DetectArMarkerRTC* GpDlg;

bool AR_finder(LPVOID pParam)
{
	// �ϐ��錾�E��`
	DetectArMarkerRTC* pDlg = (DetectArMarkerRTC*)pParam;
	GpDlg = pDlg;
	int argc=1;
	char* argv[]={"NULL"};

	// �R���t�B�M�����[�V�����l����̒l����
	// ����̃J�����\��
	int length = GpDlg -> m_VideoConf.length();
	char* vconf = new char[length+1];
	memcpy(vconf, GpDlg -> m_VideoConf.c_str(), length+1);
	// �J�����p�����[�^
	const char *cparam_name = GpDlg -> m_CameraPara.c_str();
	// �p�^�[���t�@�C��
	object[0].patt_name = GpDlg -> m_PatternName1.c_str();
	object[1].patt_name = GpDlg -> m_PatternName2.c_str();
	object[2].patt_name = GpDlg -> m_PatternName3.c_str();
	// �p�^�[����
	for(int i = 0; i < 3; i++) {
		object[i].width = GpDlg -> m_PatternWidth;
	}

//------------------------------------------------------------
//�@���C���֐�
//  ����������
//------------------------------------------------------------

	/* GLUT�̏����� */
	glutInit(&argc, argv);
	
	/* �r�f�I�f�o�C�X�̐ݒ� */
	if(arVideoOpen( vconf ) < 0) {
		return 1;
	}
	
	/* �E�B���h�E�T�C�Y�̎擾 */
	if(arVideoInqSize(&xsize, &ysize) < 0) {
		return 1;
	}
	std::cout<<"Image size (x,y) = ("<<xsize<<","<<ysize<<")"<<std::endl;

	/* �J�����p�����[�^�̓ǂݍ��� */
	if(arParamLoad(cparam_name, 1, &wparam) < 0) {
		std::cout<<"Camera parameter load error !!"<<std::endl;
		return 1;
	}
	
	/* �J�����p�����[�^�̏����� */
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	arInitCparam( &cparam );
	std::cout<<"*** Camera Parameter ***"<<std::endl;
	arParamDisp( &cparam );

	for(int i = 0; i < OBJ_NUM; i++){
		if((object[i].patt_id = arLoadPatt(object[i].patt_name)) < 0) {
			return 1;
		}
	}
	
	/* gsub���C�u�����̏����� */
	argInit( &cparam, 1.0, 0, 0, 0, 0 );

	/* �L���v�`���J�n */
	arVideoCapStart();

	/* ���C�����[�v�̌Ăяo�� */
	argMainLoop( NULL, keyEvent, mainloop );
	
	return 1;
}

//------------------------------------------------------------
//�@���C�����[�v����
//------------------------------------------------------------
static void mainloop(void)
{
	ARUint8			*dataPtr;			// �J�����摜
	ARMarkerInfo	*marker_info;		// �}�[�J�[���o�p�̏��
	int				marker_num;			// �}�[�J�[�炵�������̌�
	double			wmat1[3][4] = {};	// �ϊ��s��
	double			wmat2[3][4] = {};
	double			wmat3[3][4] = {};

	/* �J�����摜�̎擾 */
	if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ){
		arUtilSleep(2);
		return;
	}
		
	if( count_ar == 0 ) arUtilTimerReset();
	count_ar++;

	/* �L���v�`�������摜�̕\�� */
	argDrawMode2D();
	argDispImage( dataPtr, 0, 0 );

	/* �}�[�J�[�̌��o */
	if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
		cleanup();
		exit(0);
	}

	/* ���̉摜���L���v�`�� */
	arVideoCapNext();

	/* 3�����I�u�W�F�N�g�̕`�揀�� */
	argDrawMode3D();
	argDraw3dCamera(0, 0);

	/* �f�v�X�E�o�b�t�@�̏����� */
	glClearDepth(1.0);				// �f�v�X�E�o�b�t�@�̏����l
	glClear(GL_DEPTH_BUFFER_BIT);	// �f�v�X�E�o�b�t�@�̏�����

	/* �}�[�J�[�̈�v�x�𔻒� */
	for (int i = 0; i < OBJ_NUM; i++){
		int k = -1;
		for(int j = 0; j < marker_num; j++ ){
			if( object[i].patt_id == marker_info[j].id ){
				if( k == -1 ) k = j;
				else if( marker_info[k].cf < marker_info[j].cf ) k = j;
			}
		}

		/* �}�[�J�[��������Ȃ������Ƃ� */
		if( k == -1 ){
			object[i].visible = 0;
			continue;
		}

		/* ���W�ϊ��s����擾 */
		if( object[i].visible == 0 ){
			// �P�t���[�����g���č��W�ϊ��s����擾
			arGetTransMat(&marker_info[k],
							object[i].center, 
							object[i].width, 
							object[i].trans);
  		}else{
			// �O�̃t���[�����g���č��W�ϊ��s����擾
			arGetTransMatCont(&marker_info[k], 
								object[i].trans,
								object[i].center, 
								object[i].width, 
								object[i].trans);
  		}
		object[i].visible = 1;

		/* 3�����I�u�W�F�N�g�̕`�揈�� */
		draw(object[i].mark_id,object[i].trans);
	}

	/* �o�b�t�@�̓��e���o�� */
	argSwapBuffers();

	/* �}�[�J�[�����������Ƃ� */
	if(object[0].visible > 0 || object[1].visible> 0 || object[2].visible> 0){
		//�ǂ̃}�[�J�[���������Ă��邩����
		int mflag = 0;

		for(int flag_num = 0; flag_num < OBJ_NUM; flag_num++) { 
			if(object[flag_num].visible	== 1) { 
				mflag = flag_num;
				if(flag_num == 0){ 
					memcpy(wmat1, object[flag_num].trans, sizeof(object[flag_num].trans));	//�}�[�J�[1�̍��W�n�ŃJ�����̈ʒu���擾
				}
				if(flag_num == 1){ 
					memcpy(wmat2, object[flag_num].trans, sizeof(object[flag_num].trans));	//�}�[�J�[2�̍��W�n�ŃJ�����̈ʒu���擾
				}
				if(flag_num == 2){ 
					memcpy(wmat3, object[flag_num].trans, sizeof(object[flag_num].trans));	//�}�[�J�[3�̍��W�n�ŃJ�����̈ʒu���擾
				}
			}
		}
	}
	
	/* �ʒu���W��OutPort�֏o�͂��� */

	// Sequence�̒���
	GpDlg -> m_Coord.data.length(9);
		
	// �}�[�J1�̈ʒu���W(x, y, z)
	GpDlg -> m_Coord.data[0] = wmat1[0][3];
	GpDlg -> m_Coord.data[1] = wmat1[2][3];
	GpDlg -> m_Coord.data[2] = wmat1[1][3];

	// �}�[�J2�̈ʒu���W(x, y, z)
	GpDlg -> m_Coord.data[3] = wmat2[0][3];
	GpDlg -> m_Coord.data[4] = wmat2[2][3];
	GpDlg -> m_Coord.data[5] = wmat2[1][3];

	// �}�[�J3�̈ʒu���W(x, y, z)
	GpDlg -> m_Coord.data[6] = wmat3[0][3];
	GpDlg -> m_Coord.data[7] = wmat3[2][3];
	GpDlg -> m_Coord.data[8] = wmat3[1][3];

	// OutPort�X�V
	GpDlg -> m_CoordOut.write();

	// �o��
	printf("%.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f\n", GpDlg -> m_Coord.data[0], GpDlg -> m_Coord.data[1], GpDlg -> m_Coord.data[2], GpDlg -> m_Coord.data[3], GpDlg -> m_Coord.data[4], GpDlg -> m_Coord.data[5], GpDlg -> m_Coord.data[6], GpDlg -> m_Coord.data[7], GpDlg -> m_Coord.data[8]);	
}

//------------------------------------------------------------
//�@3�����I�u�W�F�N�g�̕`�揈��
//------------------------------------------------------------
void draw(int mid,double patt_trans[3][4])
{
	double	gl_para[16];					// ARToolkit����ϊ�����OpenGL�̍s��
    
	/* 3�����I�u�W�F�N�g�̕`�揀�� */
	argDrawMode3D();
	argDraw3dCamera(0, 0);
    
	/* �B�ʏ��� */
	glClearDepth(1.0);						// �f�v�X�E�o�b�t�@�̏����l
	glClear(GL_DEPTH_BUFFER_BIT);			// �f�v�X�E�o�b�t�@�̏�����
	glEnable(GL_DEPTH_TEST);				// �B�ʏ����E�L��
	glDepthFunc(GL_LEQUAL);					// �f�v�X�E�e�X�g
    
	/* ���W�ϊ��s��̓ǂݍ��� */
	argConvGlpara(patt_trans, gl_para);		// ARToolkit -> OpenGL
	glMatrixMode(GL_MODELVIEW);				// �s��ϊ����[�h�E���f���r���[
	glLoadMatrixd(gl_para);					// �ǂݍ��ލs����w��

	/* ���C�e�B���O */
	setupLighting();						// ���C�g�̒�`
	glEnable(GL_LIGHTING);					// ���C�e�B���O�E�L��
	glEnable(GL_LIGHT0);					// ���C�g0�E�I��
	glEnable(GL_LIGHT1);					// ���C�g1�E�I��

	/* �I�u�W�F�N�g�̍ގ� */
	setupMaterial();

	/* 3�����I�u�W�F�N�g�̕`�� */
	glTranslatef(0.0, 0.0, -10.0);			// �I�u�W�F�N�g�̕��s�ړ�
	glutWireCube(17.0);						// �\���b�h�L���[�u��`��
		
	/* �I������ */
	glDisable(GL_LIGHTING);					// ���C�e�B���O�E����
	glDisable(GL_DEPTH_TEST);				// �f�v�X�E�e�X�g�E����
}
//------------------------------------------------------------
//�@���C�e�B���O
//------------------------------------------------------------
static void setupLighting(void)
{
	/* ���C�g�̒�` */
	GLfloat	lt0_position[] = {100.0, -200.0, 200.0, 0.0};	// ���C�g0�̈ʒu
	GLfloat	lt0_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          ����
	GLfloat	lt0_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          �g�U��
	//
	GLfloat	lt1_position[] = {-100.0, 200.0, 200.0, 0.0};	// ���C�g1�̈ʒu
	GLfloat	lt1_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          ����
	GLfloat	lt1_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          �g�U��

	/* ���C�g�̐ݒ� */
	glLightfv(GL_LIGHT0, GL_POSITION, lt0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lt0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lt0_diffuse);
	//
	glLightfv(GL_LIGHT1, GL_POSITION, lt1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lt1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lt1_diffuse);
}
//------------------------------------------------------------
//�@�I�u�W�F�N�g�̍ގ�
//------------------------------------------------------------
static void setupMaterial(void)
{
	/* �I�u�W�F�N�g�̍ގ� */
	GLfloat	mat_ambient[]   = {0.0, 0.0, 1.0, 1.0};		// �ގ��̊���
	GLfloat	mat_specular[]  = {0.0, 0.0, 1.0, 1.0};		// ���ʌ�
	GLfloat	mat_shininess[] = {50.0};					// ���ʌW��
	//
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
//------------------------------------------------------------
//�@�I������
//------------------------------------------------------------
static void cleanup(void)
{
	/* �I������ */
	arVideoCapStop();	// �L���v�`�����~
	arVideoClose();		// �r�f�I�f�o�C�X���I��
	argCleanup();		// ARToolkit�̏I������
}
//------------------------------------------------------------
//�@�L�[�C�x���g����
//------------------------------------------------------------
static void keyEvent( unsigned char key, int x, int y)
{
	/* ESC�L�[�ŏI�� */
	if( key == 0x1b ) {
		cleanup();
		exit(0);
	}

}
