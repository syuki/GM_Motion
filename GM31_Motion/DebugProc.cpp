/// [DebugProc.cpp]
/// �f�o�b�O���̃c�[���H
// AT12B241 25 ���c�A��
// 140111

///�C���N���[�h
#include "DebugProc.h"
#include <d3dx9.h>
#include <stdio.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

/// �}�N����`
#define FONT_SIZE_H	(18)
#define FONT_SIZE_W	(8)
#define BUFF_SIZE	(2048)

/// �O���[�o���ϐ�
LPD3DXFONT	gpD3DXDebugProcFont;
char		gaDebugStr[BUFF_SIZE];

//�u�f�o�b�O�\�������̏������v
// �����@�Fvoid
// �߂�l�F�������ǂ���
// 140111
HRESULT DebugProcInit(LPDIRECT3DDEVICE9 Device) {
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = Device;

	//�t�H���g�쐬
	D3DXCreateFont(pDevice, FONT_SIZE_H, FONT_SIZE_W, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &gpD3DXDebugProcFont);

	//�o�b�t�@������
	ZeroMemory(gaDebugStr, sizeof(char) * BUFF_SIZE);

	return S_OK;
}

//�u�f�o�b�O�\�������̏I�������v
// �����@�Fvoid
// �߂�l�Fvoid
// 140111
void DebugProcUninit(void) {
	if (gpD3DXDebugProcFont) {
		gpD3DXDebugProcFont->Release();
		gpD3DXDebugProcFont = NULL;
	}
}

//�u�f�o�b�O�\�������̍X�V�����v
// �����@�Fvoid
// �߂�l�Fvoid
// 140111
void DebugProcUpdate(void) {
	ZeroMemory(gaDebugStr, sizeof(char) * BUFF_SIZE);
}

//�u�f�o�b�O�\�������̕`�揈���v
// �����@�Fvoid
// �߂�l�Fvoid
// 140111
void DebugProcDraw(void) {
	RECT rect = { 0, 0, 800, 600 };
	gpD3DXDebugProcFont->DrawText(NULL, gaDebugStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x66, 0x66));
}

//�u�f�o�b�O�\���̓o�^�v
// �����@�F�����Ƃ��ϐ��Ƃ�
// �߂�l�Fvoid
// 140111
void PrintDebugProc(const char *format, ...) {
	//�ϐ��錾
	va_list arg;
	int done;
	char buf[BUFF_SIZE];

	va_start(arg, format);
	done = vsprintf_s(buf, format, arg);
	va_end(arg);
	strcat_s(gaDebugStr, buf);

}
//=EOF=