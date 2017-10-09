/// [Input]
// Direct input finctions
// 20098 syuki nishida
// 140507
#ifndef _INPUT_H_
#define _INPUT_H_

/// Includes
#define DIRECTINPUT_VERSION (0x0800)    // åxçêëŒçÙóp
#include <dinput.h>
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

HRESULT	DirectInputModuleInit(HINSTANCE hInstance, HWND hWnd);
void	DirectInputModuleUninit(void);
HRESULT DirectInputKeyboardInid(void);
void	DirectInputKeyboardUpdate(void);
bool	GetDirectInputKeyboardInfo(int Key, int Mode);
void	SetInputFlag(bool Flag);
enum {
	DI_MODE_PRESS,
	DI_MODE_TRIGG,
	DI_MODE_REPEAT,
	DI_MODE_RELEASE,
	DI_MODE_MAX,
};

#endif // ifndef

//=EOF=