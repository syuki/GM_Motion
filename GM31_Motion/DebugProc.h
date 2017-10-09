/// [DebugProc.h]
/// デバッグ時のツール？
// AT12B241 25 西田柊希
// 140111

///インクルード
#include <Windows.h>
#include <d3d9.h>

///プロトタイプ宣言

//「デバッグ表示処理の初期化」
// 引数　：void
// 戻り値：成功かどうか
HRESULT DebugProcInit(LPDIRECT3DDEVICE9 Device);

//「デバッグ表示処理の終了処理」
// 引数　：void
// 戻り値：void
void DebugProcUninit(void);

//「デバッグ表示処理の更新処理」
// 引数　：void
// 戻り値：void
void DebugProcUpdate(void);

//「デバッグ表示処理の描画処理」
// 引数　：void
// 戻り値：void
void DebugProcDraw(void);

//「デバッグ表示の登録」
// 引数　：文字とか変数とか
// 戻り値：void
void PrintDebugProc(const char *format, ...);

//=EOF=