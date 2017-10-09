/**
	@file	snRenderingManager.h
	@brief	•`‰æ‚ÌŠî‘b‚ ‚½‚è’S“–
	@author	syuki nishida
	@date	2014/08/21
*/
#pragma once
#ifndef _SN_RENDERING_MANAGER_H_
#define _SN_RENDERING_MANAGER_H_

/// Includes
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

/// Link liblary
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// Memory
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

/// Namespace
namespace snStudy{

	/// Class
	class RenderingManager{
		RenderingManager(const HINSTANCE &hInstance, const int &CmdShow, const WNDCLASSEX &Wcex, const unsigned short &Width, const unsigned short &Height, const char* WindowName);
		virtual ~RenderingManager();
		static RenderingManager* originalPointer;

		// Window related variable
		HWND hwnd;
		HINSTANCE hinstance;
		WNDCLASSEX wcex;
		int cmdShow;
		char* windowName;
		unsigned short windowWidth;
		unsigned short windowHeight;

		// Window related function
		void InitWindow();
		void UninitWindow();

		// DirectX related variable
		LPDIRECT3DDEVICE9	D3Device;
		float projectionNear;
		float projectionFar;
		float perthFov;

		// DirectX related funstion
		bool InitDirectX();
		void UninitDirectX();
		void DrawDirectX();
		void DrawingFunctions();

		// Frame late related
		static DWORD updateFrameCnt;	// update frame counter
		static DWORD updateLastTime;	// update last get frame
		static DWORD updateTargetFrame;	// update target frame rate
		static DWORD updateRateCnt;		// update measure frame rate
		static DWORD updateFrameRate;	// update calc frame rate

		static DWORD drawFrameCnt;		// draw frame counter
		static DWORD drawLastTime;		// draw last get frame
		static DWORD drawTargetFrame;	// draw target frame rate
		static DWORD drawRateCnt;		// draw measure frame rate
		static DWORD drawFrameRate;		// draw calc frame rate

		static DWORD fpsLastTime;

	public:
		// Function is called from outside
		static bool Initalize(const HINSTANCE &hInstance, const int &CmdShow, const WNDCLASSEX &Wcex, const unsigned short &Width, const unsigned short &Height, const char *WindowName);
		static bool Finalize();
		static bool Step();
		static RenderingManager* GetPointer(void){ return originalPointer; }

		// Function is called from a pointer
		LPDIRECT3DDEVICE9 GetDevice(){ return D3Device; }
		HWND GetWindowHandle(){ return hwnd; }
		void SetProjectionNear(const float &Near){ projectionNear = Near; }
		void SetProjectionFar(const float &Far){ projectionFar = Far; }
		void SetPerthFov(const float &Fov){ perthFov = Fov; }
		void SetOrthoProjection();
		void SetPerthProjection();
		bool OpenFileCommon(char* FileType, char* FileExt, char* Title, char** FilePath);
		bool SaveFileCommon(char* FileType, char* FileExt, char* Title, char** FilePath);

	};
}

#endif // _SN_RENDERING_MANAGER_H_
//=EOF=