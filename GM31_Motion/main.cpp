/**
@file	main.cpp
@brief	プログラムのエントリーポイント
@author	syuki nishida
@date 2014/08/21
*/

/// Memory leak check
#include "Input.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

/// Includes
#include <Windows.h>
#include "snStudy/snStudy.h"
#include "CMotionModel.h"

#pragma comment(lib,"winmm.lib")

/// Macros
#define SCREEN_SIZE_W	(800)
#define SCREEN_SIZE_H	(600)
#define WINDOW_NAME		"GM_Motion"

/// Global variable
DWORD	gFPSLastTime = timeGetTime();
DWORD	gExecLastTime = timeGetTime();
DWORD	gCurrentTime = timeGetTime();
DWORD	gFrameCount = 0;
int		gCountFPS = 0;

/**
@brief	プロシージャ
@param	hWnd
@param	メッセージ
@param	wParam
@param	lParam
@return	結果
@date	2014/08/21
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Message branch
	switch (uMsg) {

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**
@brief	エントリーポイント
@param	インスタンスハンドル
@param	前のインスタンスハンドル
@param	コマンドライン引数
@param	表示方法
@return	結果
@date	2014/08/21
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	// Leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Variable daclaration
	MSG msg;
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),				// Struct size
		CS_CLASSDC,						// Window style
		WndProc,						// Window procedure function pointer
		0,
		0,
		hInstance,						// Instance handle
		NULL,							// Icon handle
		LoadCursor(NULL, IDC_ARROW),	// Mouse cursor handle
		(HBRUSH)(COLOR_WINDOW),			// Background color
		NULL,							// Menu name
		WINDOW_NAME,					// Window class name
		NULL							// Mini icon
	};

	// レンダリングマネージャ初期化
	snStudy::RenderingManager::Initalize(hInstance, nCmdShow, wcex, SCREEN_SIZE_W, SCREEN_SIZE_H, WINDOW_NAME);

	// インプット
	DirectInputModuleInit(hInstance,snStudy::RenderingManager::GetPointer()->GetWindowHandle());
	DirectInputKeyboardInid();
	SetInputFlag(true);

	// ドローエレメント初期化
	snStudy::DrawElement::InitDecralation();

	/// Light
	D3DLIGHT9 light;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1,1,1,1);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &D3DXVECTOR3(-0.5f,-1,0.5f));
	snStudy::RenderingManager::GetPointer()->GetDevice()->SetLight(0, &light);
	snStudy::RenderingManager::GetPointer()->GetDevice()->LightEnable(0, TRUE);
	
	// ポリゴン
	snStudy::SimplePrimitivePolygon2D* poly = snStudy::SimplePrimitivePolygon2D::Create(500, 500, snStudy::DrawElement::PERTH);
	snStudy::SimplePrimitivePolygon2D::OriginalValue oriVal = poly->CopyOriginalValue(poly->AddDraw());

	snStudy::Matrix44f inMtx = inMtx.Identity();
	inMtx.RotatinX(snStudy::Pi/2);
	inMtx.Translation(-250, -15, 250);

	oriVal.eachValue->worldMtx = inMtx;
	oriVal.eachValue->textureNum = snStudy::TextureManager::GetPointer()->GetTextuneNum("data/TEXTURE/tex01.jpg");

	/// CMotionModel
	CMotionModel* model = CMotionModel::Create();
	
	/// Instantion game manager

	// Message loop
	for (;;){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			// If message is quit them break to loop
			if (msg.message == WM_QUIT) break;

			// If get to message them message handling
			else TranslateMessage(&msg), DispatchMessage(&msg);
		}
		else{
			//input->UpdateKeyboard();
			//Manager->AllUpdate();

			snStudy::RenderingManager::Step();
		}
	}

	// ばいばい
	model->Release();
	poly->Release();

	// インプット終了
	DirectInputModuleUninit();

	// テクスチャ終了
	snStudy::TextureManager::Release();

	// レンダラ終了
	snStudy::RenderingManager::Finalize();

	// Post processing to window
	UnregisterClass(WINDOW_NAME, hInstance);

	return (int)msg.wParam;
}


//=EOF=