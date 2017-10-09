/// [Input]
// Direct input finctions
// 20098 syuki nishida
// 140507

/// Includes
#include "Input.h"
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

/// ==============================Key============================== ///

#define REPEAT_COUNT		(30)

LPDIRECTINPUT8			gpDInput = NULL;	//DirectInputへのポインタ
LPDIRECTINPUTDEVICE8	gpDIDevKeyboard = NULL;	//DeviceObjectのポインタ
HWND					gHandle;
BYTE					gaKeyState[256];			//キーボードの押下状況
BYTE					gaKeyTrig[256];				//キーボードのトリガー状況
BYTE					gaKeyRelease[256];			//キーボードのリリース状況
BYTE					gaKeyRepeat[256];			//キーボードのリリース状況
int						gaKeyRepeatCnt[256];
bool					gInputFlg = true;

// DirectInput初期化
// 引数　：インスタンス　ハンドル
// 戻り値：結果
// 140110
HRESULT DirectInputModuleInit(HINSTANCE hInstance, HWND hWnd) {
	//変数宣言
	HRESULT hr;

	gHandle = hWnd;

	// DirectInputオブジェクトの作成
	if (gpDInput == NULL) {
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&gpDInput, NULL);
		if (FAILED(hr)) {
			//メッセージボックス
			MessageBox(hWnd, "DirectInputオブジェクトの作成に失敗しました。", "エラー", MB_OK);
			return hr;
		}
	}
	return S_OK;
}

// DirectInput終了
// 引数　：void
// 戻り値：void
// 140110
void DirectInputModuleUninit(void) {

	// DirectInputオブジェクトの開放
	if (gpDInput != NULL) {
		gpDInput->Release();
		gpDInput = NULL;
	}

	// デバイスオブジェクトの開放
	if (gpDIDevKeyboard != NULL) {
		gpDIDevKeyboard->Unacquire();
		gpDIDevKeyboard->Release();
		gpDIDevKeyboard = NULL;
	}
}

// キーボード初期化
// 引数　：void
// 戻り値：結果
// 140111
HRESULT DirectInputKeyboardInid(void) {
	//変数宣言
	HRESULT hr;

	//DirectInputがあるか
	if (!gpDInput) {
		return E_FAIL;
	}

	// デバイスオブジェクトを作成
	hr = gpDInput->CreateDevice(GUID_SysKeyboard, &gpDIDevKeyboard, NULL);
	if (FAILED(hr)) {
		//メッセージボックス	
		MessageBox(gHandle, "デバイスオブジェクトの作成に失敗しました。", "エラー", MB_OK);
		return hr;
	}

	// データフォーマットを設定
	hr = gpDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		//メッセージボックス
		MessageBox(gHandle, "データフォーマットの設定に失敗しました。", "エラー", MB_OK);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = gpDIDevKeyboard->SetCooperativeLevel(gHandle, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		//メッセージボックス
		MessageBox(gHandle, "調和モードの設定に失敗しました。", "エラー", MB_OK);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	gpDIDevKeyboard->Acquire();

	gpDInput->Release();
	gpDInput = NULL;


	return S_OK;
}

//「キーボードの更新処理」
// 引数　：void
// 戻り値：void
// 140110
void DirectInputKeyboardUpdate(void) {
	//変数宣言
	BYTE aKeyState[256];

	//情報取得
	if (SUCCEEDED(gpDIDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0]))) {
		//キー毎更新
		for (int nKey = 0; nKey<256; nKey++) {
			//キー情報作成
			gaKeyRepeat[nKey] =															//リピートのトリガー部分
			gaKeyTrig[nKey] = (aKeyState[nKey] ^ gaKeyState[nKey]) & aKeyState[nKey];		//トリガーキー
			gaKeyRelease[nKey] = (aKeyState[nKey] ^ gaKeyState[nKey]) & gaKeyState[nKey];	//リリースキー
			gaKeyState[nKey] = aKeyState[nKey];											//プレスキー

			//リピートキー
			if (gaKeyState[nKey] & 0x80){// プレスキーが押されてたら
				if (gaKeyRepeatCnt[nKey] > REPEAT_COUNT){// カウントがたまってたら
					gaKeyRepeat[nKey] |= 0x80;	// リピートキー作成
				}
				else {// たまってなかったら
					gaKeyRepeatCnt[nKey]++;		// カウントする
				}
			}
			else {//押されてなかったら
				gaKeyRepeatCnt[nKey] = 0;	//カウント初期化
			}
		}
	}
	else {
		gpDIDevKeyboard->Acquire();
	}
}


// キー情報取得
// 引数　：キー、モード
// 戻り値：t/f
// 140110
bool GetDirectInputKeyboardInfo(int Key, int Mode) {
	if (!gInputFlg){
		return false;
	}

	//モード分岐
	switch (Mode) {
	case DI_MODE_PRESS:
		return gaKeyState[Key] & 0x80 ? true : false;
	case DI_MODE_TRIGG:
		return gaKeyTrig[Key] & 0x80 ? true : false;
	case DI_MODE_REPEAT:
		return gaKeyRepeat[Key] & 0x80 ? true : false;
	case DI_MODE_RELEASE:
		return gaKeyRelease[Key] & 0x80 ? true : false;
	}
	return false;
}

// キーフラグ設定
// 引数　：フラグ
// 戻り値：none
// 140723
void	SetInputFlag(bool Flag){
	gInputFlg = Flag;
}


//=EOF=