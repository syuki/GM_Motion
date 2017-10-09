/// [DebugProc.cpp]
/// デバッグ時のツール？
// AT12B241 25 西田柊希
// 140111

///インクルード
#include "DebugProc.h"
#include <d3dx9.h>
#include <stdio.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

/// マクロ定義
#define FONT_SIZE_H	(18)
#define FONT_SIZE_W	(8)
#define BUFF_SIZE	(2048)

/// グローバル変数
LPD3DXFONT	gpD3DXDebugProcFont;
char		gaDebugStr[BUFF_SIZE];

//「デバッグ表示処理の初期化」
// 引数　：void
// 戻り値：成功かどうか
// 140111
HRESULT DebugProcInit(LPDIRECT3DDEVICE9 Device) {
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = Device;

	//フォント作成
	D3DXCreateFont(pDevice, FONT_SIZE_H, FONT_SIZE_W, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &gpD3DXDebugProcFont);

	//バッファ初期化
	ZeroMemory(gaDebugStr, sizeof(char) * BUFF_SIZE);

	return S_OK;
}

//「デバッグ表示処理の終了処理」
// 引数　：void
// 戻り値：void
// 140111
void DebugProcUninit(void) {
	if (gpD3DXDebugProcFont) {
		gpD3DXDebugProcFont->Release();
		gpD3DXDebugProcFont = NULL;
	}
}

//「デバッグ表示処理の更新処理」
// 引数　：void
// 戻り値：void
// 140111
void DebugProcUpdate(void) {
	ZeroMemory(gaDebugStr, sizeof(char) * BUFF_SIZE);
}

//「デバッグ表示処理の描画処理」
// 引数　：void
// 戻り値：void
// 140111
void DebugProcDraw(void) {
	RECT rect = { 0, 0, 800, 600 };
	gpD3DXDebugProcFont->DrawText(NULL, gaDebugStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x66, 0x66));
}

//「デバッグ表示の登録」
// 引数　：文字とか変数とか
// 戻り値：void
// 140111
void PrintDebugProc(const char *format, ...) {
	//変数宣言
	va_list arg;
	int done;
	char buf[BUFF_SIZE];

	va_start(arg, format);
	done = vsprintf_s(buf, format, arg);
	va_end(arg);
	strcat_s(gaDebugStr, buf);

}
//=EOF=