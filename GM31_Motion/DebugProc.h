/// [DebugProc.h]
/// �f�o�b�O���̃c�[���H
// AT12B241 25 ���c�A��
// 140111

///�C���N���[�h
#include <Windows.h>
#include <d3d9.h>

///�v���g�^�C�v�錾

//�u�f�o�b�O�\�������̏������v
// �����@�Fvoid
// �߂�l�F�������ǂ���
HRESULT DebugProcInit(LPDIRECT3DDEVICE9 Device);

//�u�f�o�b�O�\�������̏I�������v
// �����@�Fvoid
// �߂�l�Fvoid
void DebugProcUninit(void);

//�u�f�o�b�O�\�������̍X�V�����v
// �����@�Fvoid
// �߂�l�Fvoid
void DebugProcUpdate(void);

//�u�f�o�b�O�\�������̕`�揈���v
// �����@�Fvoid
// �߂�l�Fvoid
void DebugProcDraw(void);

//�u�f�o�b�O�\���̓o�^�v
// �����@�F�����Ƃ��ϐ��Ƃ�
// �߂�l�Fvoid
void PrintDebugProc(const char *format, ...);

//=EOF=