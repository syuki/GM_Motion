/**
	@file	snRenderingManager.cpp
	@brief	�`��̊�b������S��
	@author	syuki nishida
	@date	2014/08/21
*/

/// Includes
#include "snRenderingManager.h"
#include "snUpdateElements.h"
#include "snDrawElements.h"
#include "../DebugProc.h"
#include "../Input.h"

/// Macros
#define TARGET_FRAME_UPDATE	(60)
#define TARGET_FRAME_DRAW	(60)

/// Namespace
namespace snStudy{

/// Static variable member
RenderingManager* RenderingManager::originalPointer = nullptr;	// Singleton point
DWORD RenderingManager::updateFrameCnt	= 0;	// �t���[���J�E���^
DWORD RenderingManager::updateLastTime	= 0;	// �ŏI�X�V����
DWORD RenderingManager::updateRateCnt	= 0;	// �t���[�����[�g�J�E���^
DWORD RenderingManager::updateFrameRate	= 0;	// �t���[�����[�g
DWORD RenderingManager::drawFrameCnt	= 0;	// �t���[���J�E���^
DWORD RenderingManager::drawLastTime	= 0;	// �ŏI�X�V����
DWORD RenderingManager::drawRateCnt		= 0;	// �t���[�����[�g�J�E���^
DWORD RenderingManager::drawFrameRate	= 0;	// �t���[�����[�g
DWORD RenderingManager::fpsLastTime		= 0;	// �t���[�����[�g�v�Z�ŏI����
DWORD RenderingManager::updateTargetFrame	= TARGET_FRAME_UPDATE;
DWORD RenderingManager::drawTargetFrame		= TARGET_FRAME_DRAW;

/// Constructer
// Argumebts :
// ReturnVal : none
// 140701
RenderingManager::RenderingManager(const HINSTANCE &hInstance, const int &CmdShow, const WNDCLASSEX &Wcex, const unsigned short &Width, const unsigned short &Height, const char* WindowName){
	// valiable regist
	windowWidth = Width;
	windowHeight = Height;
	hinstance = hInstance;
	wcex = Wcex;
	cmdShow = CmdShow;
	windowName = new char[strlen(WindowName) + 1];
	strcpy_s(windowName, strlen(WindowName) + 1, WindowName);

	projectionNear = 0.1f;
	projectionFar = 1000;
	perthFov = (D3DX_PI / 4);
}

/// Destructer
// Arguments : none
// ReturnVal : none
// 140701
RenderingManager::~RenderingManager(){
	// call to uninit
	UninitDirectX();
	UninitWindow();
	delete[] windowName;
}


/// Initalize
// Arguments :
// ReturnVal : t/f
// 140625
bool RenderingManager::Initalize(const HINSTANCE &hInstance, const int &CmdShow, const WNDCLASSEX &Wcex, const unsigned short &Width, const unsigned short &Height, const char* WindowName){

	if (originalPointer){ delete originalPointer; }

	// instation and init
	originalPointer = new RenderingManager(hInstance, CmdShow, Wcex, Width, Height, WindowName);
	originalPointer->InitWindow();
	originalPointer->InitDirectX();

	//
	DebugProcInit(originalPointer->D3Device);
	//

	return true;
}

/// Finalize
// Argumebts : none
// ReturnVal : t/f
// 140701
bool RenderingManager::Finalize(){
	// delete instance
	if (originalPointer){
		delete originalPointer;
		return true;
	}

	//
	DebugProcUninit();
	//

	return false;
}

/// Step
// Arguments :
// ReturnVal :
// 140701
bool RenderingManager::Step(){

	// Count frame
	DWORD nowTime = timeGetTime();

	// Frame check update
	if (nowTime - updateLastTime > (1000 / updateTargetFrame)){
		// Update
		UpdateElement::UpdateAll();

		DirectInputKeyboardUpdate();

		// Rate count
		updateRateCnt++;
		updateLastTime = nowTime;
	}

	// Frame check draw
	if (nowTime - drawLastTime > (1000 / drawTargetFrame)){
		// Draw
		originalPointer->DrawDirectX();

		// Rate count
		drawRateCnt++;
		drawLastTime = nowTime;
	}

	// calc FPS
	if (nowTime - fpsLastTime > 1000){
		updateFrameRate = updateRateCnt;
		drawFrameRate = drawRateCnt;
		updateRateCnt = drawRateCnt = 0;
		fpsLastTime = nowTime;
	}

	return true;
}

/// Init window
// Arguments : none
// ReturnVal : none
// 140701
void RenderingManager::InitWindow(){

	// Register window class and create window
	RegisterClassEx(&wcex);
	hwnd = CreateWindowEx(0,
		wcex.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		windowHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		hinstance,
		NULL);

	// Display to window
	ShowWindow(hwnd, cmdShow);
	UpdateWindow(hwnd);
}

/// Uninit window
// Argumebts : none
// ReturnVal : none
// 140701
void RenderingManager::UninitWindow(){
	// release window
	if (hwnd){
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	// release regist class
	if (hinstance){
		UnregisterClass(wcex.lpszClassName, hinstance);
		hinstance = NULL;
		memset(&wcex, 0, sizeof(WNDCLASSEX));
	}
}

/// Init DirectX
// Argumebts :
// ReturnVal : t/f
// 140701
bool RenderingManager::InitDirectX(){
	// Variable daclaration
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3D9 pD3D;
	bool windowMode = true;

	// Create to Direct3D object
	if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		MessageBox(hwnd, "Failed to create Direct3D object", "error", MB_OK);
		return false;
	}

	// Get to current display mode
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		MessageBox(hwnd, "Failed to set current display mode", "error", MB_OK);
		return false;
	}

	// Set to device presentation parametor
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = windowWidth;			// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = windowHeight;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;			// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = windowMode;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	// Window mode
	if (windowMode){
		d3dpp.FullScreen_RefreshRateInHz = 0;								// Refresh rate
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// Interval
	}

	// Full screen mode
	else{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// Refresh rate
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// Interval
	}

	// Create to device object
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &D3Device))){
		// Failed to initiarize
		MessageBox(hwnd, "Failed to create device object", "error", MB_OK);
		return false;
	}

	// Set to render state parametor
	D3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	D3Device->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	D3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	D3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	D3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// Set to sampler state parametor
	D3Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	D3Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	D3Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	D3Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// Set to texture state parametor
	D3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	D3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	D3Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// Release to Direct3D object
	if (pD3D != NULL){ pD3D->Release(); pD3D = NULL; }

	return S_OK;
}

/// Uninit DirectX
// Arguments : none
// ReturnVal : none
// 140701
void RenderingManager::UninitDirectX(){
	// release device
	if (D3Device){
		D3Device->Release();
		D3Device = NULL;
	}
}

/// Draw DirectX
// Arguments : none
// ReturnVal : none
// 140701
void RenderingManager::DrawDirectX(){
	///////////////////////
	Matrix44f view;
	Vector3f eyePos = Vector3f(0, 100, -300);
	Vector3f atPos = 0;
	Vector3f upVec = Vector3f(0, 1, 0);

	// View
	view.Identity();
	view.EyeAtL(eyePos, atPos, upVec);
	D3Device->SetTransform(D3DTS_VIEW, &D3DMATRIX(view));
	///////////////////////

	// Clear to back buffer and z buffer
	D3Device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(50, 0, 50, 0), 1.0f, 0);

	// Begin draw
	if (SUCCEEDED(D3Device->BeginScene())) {
		// Draw to all scene
		DrawElement::DrawAll();

		//
		DebugProcDraw();
		//

		// End draw
		D3Device->EndScene();
	}

	//
	DebugProcUpdate();
	//

	// Frip the back buffer
	D3Device->Present(NULL, NULL, NULL, NULL);
}


/// Set ortho proj
// Arguments : none
// ReturnVal : none
// 140709
void RenderingManager::SetOrthoProjection(){
	// make and set ortho matrix
	Matrix44f ortho;
	RenderingManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	ortho.OrthoScreenL(windowWidth, windowHeight, projectionNear, projectionFar);
	D3Device->SetTransform(D3DTS_PROJECTION, &D3DXMATRIX(ortho));
}

/// Set perth proj
// Arguments : none
// ReturnVal : none
// 140709
void RenderingManager::SetPerthProjection(){
	// make and set perth matrix
	Matrix44f perth;
	RenderingManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	perth.PerthFovL(perthFov, ((float)windowWidth / windowHeight), projectionNear, projectionFar);
	D3Device->SetTransform(D3DTS_PROJECTION, &D3DXMATRIX(perth));
}

bool RenderingManager::OpenFileCommon(char* FileType, char* FileExt, char* Title,char** FilePath){
	OPENFILENAME ofn = { 0 };
	char strCustom[256] = "Before files\0*.*\0\0";
	char filePath[256] = { 0 };
	char fillter[256] = { 0 };

	// �t�B���^�[�쐬
	sprintf_s(fillter, 256, "%s (*.%s)\n*.%s;.%s\nAll files (*.*)\n*.*\n\0", FileType, FileExt, FileExt, FileExt);
	for (int i = 0; fillter[i]!='\0'; i++){
		if (fillter[i] == '\n'){
			fillter[i] = '\0';
		}
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance;
	ofn.lpstrFilter = fillter;
	ofn.lpstrCustomFilter;
	ofn.nMaxCustFilter;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle;
	ofn.nMaxFileTitle;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.nFileOffset;
	ofn.nFileExtension;
	ofn.lpstrDefExt;
	ofn.lCustData;
	ofn.lpfnHook;
	ofn.lpTemplateName;

	// File open
	if (GetOpenFileName(&ofn)){
		// ����
		strcpy_s(*FilePath,256,filePath);
		return true;
	}
	return false;
}

bool RenderingManager::SaveFileCommon(char* FileType, char* FileExt, char* Title, char** FilePath){
	OPENFILENAME ofn = { 0 };
	char fillter[256] = { 0 };
	char filePath[256] = { 0 };

	// �t�B���^�[�쐬
	sprintf_s(fillter, 256, "%s (*.%s)\n*.%s;.%s\nAll files (*.*)\n*.*\n\0", FileType, FileExt, FileExt, FileExt);
	for (int i = 0; fillter[i] != '\0'; i++){
		if (fillter[i] == '\n'){
			fillter[i] = '\0';
		}
	}

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = fillter;
	ofn.lpstrFile = filePath;
	ofn.lpstrFileTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = Title;
	ofn.lpstrDefExt;

	// ���[�Ղ�
	if (GetSaveFileName(&ofn)){
		// ����
		char ext[256] = ".";
		strcat_s<256>(ext,FileExt);
		if (!strstr(filePath, ext)){
			strcat_s<256>(filePath, ext);
		}
		strcpy_s(*FilePath, 256, filePath);
		return true;
	}
	return false;
}


}
//=EOF=