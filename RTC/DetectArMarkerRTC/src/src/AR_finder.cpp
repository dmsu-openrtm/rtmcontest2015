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

DetectArMarkerRTC* GpDlg;
bool ReturnFlag = 0;

bool AR_finder(LPVOID pParam)
{
	// 
	GpDlg = (DetectArMarkerRTC*)pParam;

	// �\���̏�����
	GpDlg -> object[0].patt_name = GpDlg -> m_PatternName1.c_str();	// �p�^�[���t�@�C����
	GpDlg -> object[1].patt_name = GpDlg -> m_PatternName2.c_str();
	GpDlg -> object[2].patt_name = GpDlg -> m_PatternName3.c_str();
	for(int i = 0; i < 3; i++) {
		GpDlg -> object[i].patt_id = -1;							// �p�^�[��ID
		GpDlg -> object[i].mark_id = i + 1;							// �}�[�JID
		GpDlg -> object[i].visible = 0;								// ���o�t���O
		GpDlg -> object[i].width = GpDlg -> m_PatternWidth * 1000;	// �p�^�[���� [mm]
		for(int j = 0; j < 3; j++) {								// �p�^�[�����S���W
			GpDlg -> object[i].center[j] = 0.0;
		}
	}

	// 臒l������
	GpDlg -> thresh = 100;

	// GLUT������
	int argc = 1;
	char* argv[] = {"NULL"};
	glutInit(&argc, argv);
	
	// �r�f�I�f�o�C�X�ݒ�
	int length = GpDlg -> m_VideoConf.length();
	char* vconf = new char[length+1];
	memcpy(vconf, GpDlg -> m_VideoConf.c_str(), length+1);
	if(arVideoOpen(vconf) < 0) {
		std::cout<<"Error: Video device setting"<<std::endl;
		return 1;
	}
	
	// �E�B���h�E�T�C�Y�擾
	int	xsize, ysize;
	if(arVideoInqSize(&xsize, &ysize) < 0) {
		std::cout<<"Error: Window size getting"<<std::endl;
		return 1;
	}
	std::cout<<"Image size (x,y) = ("<<xsize<<","<<ysize<<")"<<std::endl;

	// �J�����p�����[�^�ǂݍ���
	const char *cparam_name = GpDlg -> m_CameraPara.c_str();
	ARParam	wparam;
	if(arParamLoad(cparam_name, 1, &wparam) < 0) {
		std::cout<<"Error: Camera parameter loading"<<std::endl;
		return 1;
	}

	// �J�����p�����[�^������
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
	
	// gsub���C�u����������
	argInit(&cparam, 1.0, 0, 0, 0, 0);

	// �L���v�`���J�n
	arVideoCapStart();

	// ���C�����[�v�Ăяo��
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	argMainLoop(NULL, keyEvent, mainloop);

	return ReturnFlag;
}

// Main loop
static void mainloop(void)
{
	//
	
	// �J�����摜�̎擾
	ARUint8 *dataPtr;
	if((dataPtr = (ARUint8 *)arVideoGetImage()) == NULL) {
		arUtilSleep(2);
		return;
	}

	// �����t���[����
	//int	count_ar = 0;	
	//if(count_ar == 0) {
	//	arUtilTimerReset();
	//}
	//count_ar++;

	// �L���v�`�������摜�̕\��
	argDrawMode2D();
	argDispImage(dataPtr, 0, 0);

	// �}�[�J���o
	ARMarkerInfo *marker_info; // �}�[�J���o�p�̏��
	int marker_num; // �}�[�J�[�炵�������̌�
	if(arDetectMarker(dataPtr, GpDlg -> thresh, &marker_info, &marker_num) < 0) {
		cleanup();
		std::cout<<"Error: Marker detecting"<<std::endl;
		ReturnFlag = 1;
		glutLeaveMainLoop();
	}
	
	// ���̉摜���L���v�`��
	arVideoCapNext();
	
	// 3�����I�u�W�F�N�g�̕`�揀��
	argDrawMode3D();
	argDraw3dCamera(0, 0);
	
	// �f�v�X�E�o�b�t�@�̏�����
	glClearDepth(1.0);				// �f�v�X�E�o�b�t�@�̏����l
	glClear(GL_DEPTH_BUFFER_BIT);	// �f�v�X�E�o�b�t�@�̏�����

	// �}�[�J�[�̈�v�x����
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

		// �}�[�J��������Ȃ������Ƃ�
		if(k == -1) {
			GpDlg -> object[i].visible = 0;
			continue;
		}

		// ���W�ϊ��s����擾
		if(GpDlg -> object[i].visible == 0) {
			// 1�t���[�����g���č��W�ϊ��s����擾
			arGetTransMat(&marker_info[k],GpDlg -> object[i].center, GpDlg -> object[i].width, GpDlg -> object[i].trans);
		}
		else{
			// �O�̃t���[�����g���č��W�ϊ��s����擾
			arGetTransMatCont(&marker_info[k], GpDlg -> object[i].trans, GpDlg -> object[i].center, GpDlg -> object[i].width, GpDlg -> object[i].trans);
  		}
		GpDlg -> object[i].visible = 1;

		// 3�����I�u�W�F�N�g�̕`�揈��
		draw(GpDlg -> object[i].mark_id, GpDlg -> object[i].trans);
	}

	// �o�b�t�@�̓��e���o��
	argSwapBuffers();

	// �}�[�J�����������Ƃ�
	double wmat1[3][4] = {}; // �ϊ��s��
	double wmat2[3][4] = {};
	double wmat3[3][4] = {};
	if(GpDlg -> object[0].visible > 0 || GpDlg -> object[1].visible> 0 || GpDlg -> object[2].visible> 0){
		//�ǂ̃}�[�J���������Ă��邩����
		int mflag = 0;
		for(int flag_num = 0; flag_num < 3; flag_num++) { 
			if(GpDlg -> object[flag_num].visible == 1) { 
				mflag = flag_num;
				if(flag_num == 0){ 
					memcpy(wmat1, GpDlg -> object[flag_num].trans, sizeof(GpDlg -> object[flag_num].trans)); //�}�[�J1�̍��W�n�ŃJ�����̈ʒu���擾
				}
				if(flag_num == 1){ 
					memcpy(wmat2, GpDlg -> object[flag_num].trans, sizeof(GpDlg -> object[flag_num].trans)); //�}�[�J2�̍��W�n�ŃJ�����̈ʒu���擾
				}
				if(flag_num == 2){ 
					memcpy(wmat3, GpDlg -> object[flag_num].trans, sizeof(GpDlg -> object[flag_num].trans)); //�}�[�J3�̍��W�n�ŃJ�����̈ʒu���擾
				}
			}
		}
	}
	
	// �}�[�J�ʒu���W��OutPort�֏o�͂���
	GpDlg -> m_Coord.data[0] = wmat1[0][3] / 1000; // �}�[�J1�̈ʒu���W(x, y, z) [m]
	GpDlg -> m_Coord.data[1] = wmat1[2][3] / 1000;
	GpDlg -> m_Coord.data[2] = wmat1[1][3] / 1000;
	GpDlg -> m_Coord.data[3] = wmat2[0][3] / 1000; // �}�[�J2�̈ʒu���W(x, y, z) [m]
	GpDlg -> m_Coord.data[4] = wmat2[2][3] / 1000;
	GpDlg -> m_Coord.data[5] = wmat2[1][3] / 1000;
	GpDlg -> m_Coord.data[6] = wmat3[0][3] / 1000; // �}�[�J3�̈ʒu���W(x, y, z) [m]
	GpDlg -> m_Coord.data[7] = wmat3[2][3] / 1000;
	GpDlg -> m_Coord.data[8] = wmat3[1][3] / 1000;

	// OutPort�X�V
	GpDlg -> m_CoordOut.write();

	// �o��
	std::cout<<"("<<std::setprecision(1)<<std::fixed
		     <<wmat1[0][3]<<", "<<wmat1[2][3]<<", "<<wmat1[1][3]<<", "
			 <<wmat2[0][3]<<", "<<wmat2[2][3]<<", "<<wmat2[1][3]<<", "
			 <<wmat3[0][3]<<", "<<wmat3[2][3]<<", "<<wmat3[1][3]<<") [mm]"<<std::endl;
}

void draw(int mid,double patt_trans[3][4])
{
	double gl_para[16];						// ARToolkit����ϊ�����OpenGL�̍s��
    
	// 3�����I�u�W�F�N�g�̕`�揀��
	argDrawMode3D();
	argDraw3dCamera(0, 0);
    
	// �B�ʏ���
	glClearDepth(1.0);						// �f�v�X�E�o�b�t�@�̏����l
	glClear(GL_DEPTH_BUFFER_BIT);			// �f�v�X�E�o�b�t�@�̏�����
	glEnable(GL_DEPTH_TEST);				// �B�ʏ����E�L��
	glDepthFunc(GL_LEQUAL);					// �f�v�X�E�e�X�g
    
	// ���W�ϊ��s��̓ǂݍ���
	argConvGlpara(patt_trans, gl_para);		// ARToolkit -> OpenGL
	glMatrixMode(GL_MODELVIEW);				// �s��ϊ����[�h�E���f���r���[
	glLoadMatrixd(gl_para);					// �ǂݍ��ލs����w��

	// ���C�e�B���O
	setupLighting();						// ���C�g�̒�`
	glEnable(GL_LIGHTING);					// ���C�e�B���O�E�L��
	glEnable(GL_LIGHT0);					// ���C�g0�E�I��
	glEnable(GL_LIGHT1);					// ���C�g1�E�I��

	// �I�u�W�F�N�g�̍ގ�
	setupMaterial();

	// 3�����I�u�W�F�N�g�̕`��
	glTranslatef(0.0, 0.0, -10.0);			// �I�u�W�F�N�g�̕��s�ړ�
	glutWireCube(17.0);						// �\���b�h�L���[�u��`��
		
	// �I������
	glDisable(GL_LIGHTING);					// ���C�e�B���O�E����
	glDisable(GL_DEPTH_TEST);				// �f�v�X�E�e�X�g�E����
}

static void setupLighting(void)
{
	// ���C�g�̒�`
	GLfloat	lt0_position[] = {100.0, -200.0, 200.0, 0.0};	// ���C�g0�̈ʒu
	GLfloat	lt0_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          ����
	GLfloat	lt0_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          �g�U��
	//
	GLfloat	lt1_position[] = {-100.0, 200.0, 200.0, 0.0};	// ���C�g1�̈ʒu
	GLfloat	lt1_ambient[]  = {0.1, 0.1, 0.1, 1.0};			//          ����
	GLfloat	lt1_diffuse[]  = {0.8, 0.8, 0.8, 1.0};			//          �g�U��

	// ���C�g�̐ݒ�
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
	// �I�u�W�F�N�g�̍ގ�
	GLfloat	mat_ambient[]   = {0.0, 0.0, 1.0, 1.0};		// �ގ��̊���
	GLfloat	mat_specular[]  = {0.0, 0.0, 1.0, 1.0};		// ���ʌ�
	GLfloat	mat_shininess[] = {50.0};					// ���ʌW��
	//
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

static void cleanup(void)
{
	// �I������
	arVideoCapStop();	// �L���v�`�����~
	arVideoClose();		// �r�f�I�f�o�C�X���I��
	argCleanup();		// ARToolkit�̏I������
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
