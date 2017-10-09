#include "CMotionModel.h"
#include "DebugProc.h"
#include "Input.h"

CMotionModel::CMotionModel(){
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 D3Device = snStudy::RenderingManager::GetPointer()->GetDevice();

	// ���f���ǂݍ���
	body	= CSceneX::Create(D3Device, "data/MODEL/body.x");
	head = CSceneX::Create(D3Device, "data/MODEL/head.x");
	arm_r = CSceneX::Create(D3Device, "data/MODEL/arm_r.x");
	arm_l	= CSceneX::Create(D3Device, "data/MODEL/arm_l.x");
	hand_r	= CSceneX::Create(D3Device, "data/MODEL/hand_r.x");
	hand_l	= CSceneX::Create(D3Device, "data/MODEL/hand_l.x");
	leg_r	= CSceneX::Create(D3Device, "data/MODEL/leg_r.x");
	leg_l	= CSceneX::Create(D3Device, "data/MODEL/leg_l.x");
	foot_r	= CSceneX::Create(D3Device, "data/MODEL/foot_r.x");
	foot_l	= CSceneX::Create(D3Device, "data/MODEL/foot_l.x");

	// �e�q
	head->SetParentMtxPtr(body->GetMyMtxPtr());
	arm_r->SetParentMtxPtr(body->GetMyMtxPtr());
	arm_l->SetParentMtxPtr(body->GetMyMtxPtr());
	leg_r->SetParentMtxPtr(body->GetMyMtxPtr());
	leg_l->SetParentMtxPtr(body->GetMyMtxPtr());
	hand_r->SetParentMtxPtr(arm_r->GetMyMtxPtr());
	hand_l->SetParentMtxPtr(arm_l->GetMyMtxPtr());
	foot_r->SetParentMtxPtr(leg_r->GetMyMtxPtr());
	foot_l->SetParentMtxPtr(leg_l->GetMyMtxPtr());

	// �o�C���h�|�[�Y
	head->SetPosition(snStudy::Vector3f(0,15,0));
	arm_r->SetPosition(snStudy::Vector3f(-7, 12, 0));
	arm_l->SetPosition(snStudy::Vector3f(7, 12, 0));
	hand_r->SetPosition(snStudy::Vector3f(-10, 0, 0));
	hand_l->SetPosition(snStudy::Vector3f(10, 0, 0));
	leg_r->SetPosition(snStudy::Vector3f(-5, 0, 0));
	leg_l->SetPosition(snStudy::Vector3f(5, 0, 0));
	foot_r->SetPosition(snStudy::Vector3f(0, -10, 0));
	foot_l ->SetPosition(snStudy::Vector3f(0, -10, 0));

	dbgFlg = false;
	frameCnt = 0;
	rot = 0;
	gravity = 0;

	stopAnime.LoadPauseFromFile("Stop00.p", 20);
	stopAnime.LoadPauseFromFile("Stop01.p", 20);
	warkAnime.LoadPauseFromFile("Wark00.p", 15);
	warkAnime.LoadPauseFromFile("Wark01.p", 15);
	punchAnime.LoadPauseFromFile("Punch00.p", 7);
	punchAnime.LoadPauseFromFile("Punch01.p", 5);
	punchAnime.LoadPauseFromFile("Punch02.p", 10);
	damageAnime.LoadPauseFromFile("Damage00.p",10);
	damageAnime.LoadPauseFromFile("Damage01.p",10);
	damageAnime.LoadPauseFromFile("Damage02.p",5);

	snStudy::Vector3f* rotArr;

	rotArr = stopAnime.GetPauseForTime(0, true);

	body->SetRotate(rotArr[0]);
	head->SetRotate(rotArr[1]);
	arm_r->SetRotate(rotArr[2]);
	arm_l->SetRotate(rotArr[3]);
	hand_r->SetRotate(rotArr[4]);
	hand_l->SetRotate(rotArr[5]);
	leg_r->SetRotate(rotArr[6]);
	leg_l->SetRotate(rotArr[7]);
	foot_r->SetRotate(rotArr[8]);
	foot_l->SetRotate(rotArr[9]);

	delete[] rotArr;
}

CMotionModel::~CMotionModel(){
	FILE* fp;
	fopen_s(&fp,"Pause.p","wb");
	if (!fp){
		_asm int 3
	}

	snStudy::Vector3f rotArr[11];
	rotArr[0] = body->GetRotate();
	rotArr[1] = head->GetRotate();
	rotArr[2] = arm_r->GetRotate();
	rotArr[3] = arm_l->GetRotate();
	rotArr[4] = hand_r->GetRotate();
	rotArr[5] = hand_l->GetRotate();
	rotArr[6] = leg_r->GetRotate();
	rotArr[7] = leg_l->GetRotate();
	rotArr[8] = foot_r->GetRotate();
	rotArr[9] = foot_l->GetRotate();
	rotArr[10] = snStudy::Vector3f(0, body->GetPosition().y, 0);

	fwrite(rotArr, sizeof(snStudy::Vector3f), 11, fp);
	fclose(fp);

	// �΂��΂�
	delete head;
	delete body;
	delete arm_r;
	delete arm_l;
	delete hand_r;
	delete hand_l;
	delete leg_r;
	delete leg_l;
	delete foot_r;
	delete foot_l;
}

void CMotionModel::Update(){
	if (GetDirectInputKeyboardInfo(DIK_F1, DI_MODE_TRIGG)){
		dbgFlg ^= 1;	
	}

	if (dbgFlg){
		EditUpdate();
	}else{
		// �t���[���J�E���^���Z
		frameCnt++;
		PrintDebugProc("frame = %d\n", frameCnt);

		snStudy::Vector3f* rotArr = nullptr;
		snStudy::Vector3f pos = body->GetPosition();
		snStudy::Vector3f vector = snStudy::Vector3f(0,0,0);
		unsigned char keyMask = 0;
		static float nowRot = 0;
		ANIME_TYPE nowAnimeType = oldAnimeType;
		static bool uniqueAnimeFlg = false;

		// �L�[�}�X�N�쐬
		if (GetDirectInputKeyboardInfo(DIK_W, DI_MODE_PRESS)){
			keyMask |= 1;
		}
		if (GetDirectInputKeyboardInfo(DIK_A, DI_MODE_PRESS)){
			keyMask |= 2;
		}
		if (GetDirectInputKeyboardInfo(DIK_S, DI_MODE_PRESS)){
			keyMask |= 4;
		}
		if (GetDirectInputKeyboardInfo(DIK_D, DI_MODE_PRESS)){
			keyMask |= 8;
		}
		if (GetDirectInputKeyboardInfo(DIK_SPACE, DI_MODE_TRIGG)){
			if (gravity == 0){
				gravity = 8;
			}
		}
		if (GetDirectInputKeyboardInfo(DIK_1, DI_MODE_TRIGG)){
			uniqueAnimeFlg = true;
			nowAnimeType = PUNCH;
		}
		if (GetDirectInputKeyboardInfo(DIK_0, DI_MODE_TRIGG)){
			uniqueAnimeFlg = true;
			nowAnimeType = DAMAGE;
		}

		// �L�[����
		if (keyMask & 1){
			if (rot < 0){
				rot += snStudy::Pi * 2;
			}
			nowRot = snStudy::Pi;
			if (keyMask & 2){
				nowRot = snStudy::Pi * 0.75f;
			}
			if (keyMask & 8){
				rot -= snStudy::Pi * 2;
				nowRot = -snStudy::Pi * 0.75f;
			}
		}
		else if (keyMask & 4){
			nowRot = 0;
			if (keyMask & 2){
				nowRot += snStudy::Pi / 4;
			}
			if (keyMask & 8){
				nowRot -= snStudy::Pi / 4;
			}
		}
		else if (keyMask & 2){
			nowRot = snStudy::Pi / 2;
		}
		else if (keyMask & 8){
			if (rot > 1){
				rot -= snStudy::Pi * 2;
			}
			nowRot = -snStudy::Pi / 2;
		}

		PrintDebugProc("nowRot = %f\n", nowRot);
		PrintDebugProc("Rot = %f\n", rot);

		rot += (nowRot - rot)*0.4f;;

		if (keyMask){
			pos.x -= sinf(rot) * 5.f;
			pos.z -= cosf(rot) * 5.f;
			if (!uniqueAnimeFlg){
				nowAnimeType = WARK;
			}
		}else{
			if (!uniqueAnimeFlg){
				nowAnimeType = STOP;
			}
		}

		if (nowAnimeType != oldAnimeType){
			frameCnt = 0;
			oldAnimeType = nowAnimeType;
		}

		if (uniqueAnimeFlg){
			switch (oldAnimeType){
			case PUNCH:
				rotArr = punchAnime.GetPauseForTime(frameCnt, false);
				break;

			case DAMAGE:
				rotArr = damageAnime.GetPauseForTime(frameCnt, false);
				break;
			}
			if (!rotArr){
				uniqueAnimeFlg = false;
				rotArr = stopAnime.GetPauseForTime(0, true);
				nowAnimeType = STOP;
			}
		}else{
			switch (nowAnimeType){
			case STOP:
				rotArr = stopAnime.GetPauseForTime(frameCnt, true);
				break;

			case WARK:
				rotArr = warkAnime.GetPauseForTime(frameCnt, true);
				break;
			}
		}

		gravity -= MOTION_CHARA_GRAVITY;

		rotArr[0].y += rot;
		pos.y += gravity;
		if (pos.y < 0){
			pos.y = 0;
			gravity = 0;
		}
		pos.y += rotArr[10].y;
		body->SetRotate(rotArr[0]);
		head->SetRotate(rotArr[1]);
		arm_r->SetRotate(rotArr[2]);
		arm_l->SetRotate(rotArr[3]);
		hand_r->SetRotate(rotArr[4]);
		hand_l->SetRotate(rotArr[5]);
		leg_r->SetRotate(rotArr[6]);
		leg_l->SetRotate(rotArr[7]);
		foot_r->SetRotate(rotArr[8]);
		foot_l->SetRotate(rotArr[9]);
		body->SetPosition(pos);

		delete[] rotArr;
	}
}

void CMotionModel::EditUpdate(){
	// �J�[�\���ϐ�
	static int cursor = 0;
	// �p�x�i�[�p�ϐ�
	snStudy::Vector3f rotArr[11];

	// �p�x�擾
	rotArr[0] = body->GetRotate();
	rotArr[1] = head->GetRotate();
	rotArr[2] = arm_r->GetRotate();
	rotArr[3] = arm_l->GetRotate();
	rotArr[4] = hand_r->GetRotate();
	rotArr[5] = hand_l->GetRotate();
	rotArr[6] = leg_r->GetRotate();
	rotArr[7] = leg_l->GetRotate();
	rotArr[8] = foot_r->GetRotate();
	rotArr[9] = foot_l->GetRotate();
	rotArr[10] = body->GetPosition();

	// �|�[�X�J��
	static bool openFlg = false;
	if (openFlg){
		char *filePath = new char[256];

		if (GetDirectInputKeyboardInfo(DIK_RSHIFT, DI_MODE_PRESS)){
			if (snStudy::RenderingManager::GetPointer()->SaveFileCommon("Pause", "p", "�|�[�Y��ۑ�", &filePath)){
				FILE* fp;
				snStudy::Vector3f rotArr[11];

				fopen_s(&fp, filePath, "wb");
				if (!fp){
					_asm int 3
				}

				rotArr[0] = body->GetRotate();
				rotArr[1] = head->GetRotate();
				rotArr[2] = arm_r->GetRotate();
				rotArr[3] = arm_l->GetRotate();
				rotArr[4] = hand_r->GetRotate();
				rotArr[5] = hand_l->GetRotate();
				rotArr[6] = leg_r->GetRotate();
				rotArr[7] = leg_l->GetRotate();
				rotArr[8] = foot_r->GetRotate();
				rotArr[9] = foot_l->GetRotate();
				rotArr[10] = snStudy::Vector3f(0, body->GetPosition().y, 0);

				fwrite(rotArr, sizeof(snStudy::Vector3f), 11, fp);
				fclose(fp);
			}

		}else{
			if (snStudy::RenderingManager::GetPointer()->OpenFileCommon("Pause", "p", "�|�[�Y���J��", &filePath)){
				FILE* fp;
				fopen_s(&fp, filePath, "rb");
				if (fp){
					fread(rotArr, sizeof(snStudy::Vector3f), 11, fp);
					fclose(fp);
				}

			}
		}
		delete filePath;
		openFlg = false;
	}

	if (GetDirectInputKeyboardInfo(DIK_O, DI_MODE_RELEASE)){
		openFlg = true;
	}

	// �J�[�\���ړ�
	if (GetDirectInputKeyboardInfo(DIK_UP, DI_MODE_TRIGG)){
		if (cursor == 0){
			cursor = 9;
		}
		else{
			cursor--;
		}
	}
	if (GetDirectInputKeyboardInfo(DIK_DOWN, DI_MODE_TRIGG)){
		if (cursor == 9){
			cursor = 0;
		}
		else{
			cursor++;
		}
	}

	// �p�[�c�����ʗp������
	char* partsStr[11] = {
		"�́@\0",
		"���@\0",
		"�E�r\0",
		"�E��\0",
		"���r\0",
		"����\0",
		"�E�r\0",
		"�E��\0",
		"���r\0",
		"����\0",
		"�̈�\0"
	};

	// ���j���[�\��
	PrintDebugProc("\n"
		"������������������������������������������\n"
		"��[��][��]�F�p�[�c�I���@�@�@�@�@�@�@�@�@��\n"
		"������������������������������������������\n"
		"��[�s][�f]�F�w����]�@[�a]�F�w�����Z�b�g��\n"
		"��[�x][�g]�F�w����]�@[�m]�F�x�����Z�b�g��\n"
		"��[��][�i]�F�w����]�@[�l]�F�y�����Z�b�g��\n"
		"��[�u][�v]�F�̏㉺�@�@[��]�F�̃��Z�b�g�@��\n"
		"��[�d�m�s�d�q]�F�S�����Z�b�g�@�@�@�@�@�@��\n"
		"������������������������������������������\n"
		);
	for (int i = 0; i < 11; i++){
		if (cursor == i){
			PrintDebugProc("����");
		}
		else{
			PrintDebugProc("����");
		}
		PrintDebugProc("%s�F(%+6.5f, %+6.5f, %+6.5f)��\n", partsStr[i], rotArr[i].x, rotArr[i].y, rotArr[i].z);
	}
	PrintDebugProc("������������������������������������������\n");

	// X������
	if (GetDirectInputKeyboardInfo(DIK_T, DI_MODE_PRESS)){
		rotArr[cursor].x += 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_G, DI_MODE_PRESS)){
		rotArr[cursor].x -= 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_B, DI_MODE_PRESS)){
		rotArr[cursor].x = 0.f;
	}

	// Y������
	if (GetDirectInputKeyboardInfo(DIK_Y, DI_MODE_PRESS)){
		rotArr[cursor].y += 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_H, DI_MODE_PRESS)){
		rotArr[cursor].y -= 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_N, DI_MODE_PRESS)){
		rotArr[cursor].y = 0.f;
	}

	// Z������
	if (GetDirectInputKeyboardInfo(DIK_U, DI_MODE_PRESS)){
		rotArr[cursor].z += 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_J, DI_MODE_PRESS)){
		rotArr[cursor].z -= 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_M, DI_MODE_PRESS)){
		rotArr[cursor].z = 0.f;
	}

	// �̑���
	if (GetDirectInputKeyboardInfo(DIK_LBRACKET, DI_MODE_PRESS)){
		rotArr[10].y += 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_RBRACKET, DI_MODE_PRESS)){
		rotArr[10].y -= 0.03146f;
	}
	if (GetDirectInputKeyboardInfo(DIK_BACKSLASH, DI_MODE_PRESS)){
		rotArr[10].x = 0.f;
		rotArr[10].y = 0.f;
		rotArr[10].z = 0.f;
	}

	// �S���S�p�[�c���Z�b�g
	if (GetDirectInputKeyboardInfo(DIK_RETURN, DI_MODE_PRESS)){
		for (int i = 0; i < 10; i++){
			rotArr[i] = 0.f;
		}
		rotArr[10].y = 0;
	}

	// �l�K��
	body->SetRotate(rotArr[0]);
	head->SetRotate(rotArr[1]);
	arm_r->SetRotate(rotArr[2]);
	arm_l->SetRotate(rotArr[3]);
	hand_r->SetRotate(rotArr[4]);
	hand_l->SetRotate(rotArr[5]);
	leg_r->SetRotate(rotArr[6]);
	leg_l->SetRotate(rotArr[7]);
	foot_r->SetRotate(rotArr[8]);
	foot_l->SetRotate(rotArr[9]);
	body->SetPosition(rotArr[10]);
}

CMotionModel* CMotionModel::Create(){
	return new CMotionModel();
}