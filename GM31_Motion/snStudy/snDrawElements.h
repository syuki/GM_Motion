/// [DrawElement]
/// Drawing element
// 20098 syuki nishida
// 140701
#pragma once
#ifndef _SN_DRAW_ELEMENT_H_
#define _SN_DRAW_ELEMENT_H_

/// Includes
#include <map>
#include "snRenderingManager.h"
#include "snMath.h"

namespace snStudy{

/// Class decralation
class DrawElement{
protected:
	// �v���~�e�B�u�^�C�v
	enum PRIMITIVE_TYPE {
		POINTLIST,
		LINELIST,
		LINESTRIP,
		TRIANGLELIST,
		TRIANGLESTRIP,
	};

public:
	// �`��̃��X�g�̃^�C�v
	enum LIST_TYPE {
		ORTHO,
		PERTH,
		LIST_TYPE_MAX,
	};

protected:
	// �o�b�t�@�̎��
	enum BUFFER_TYPE{
		VERTEX		= 0x01,
		NORMAL		= 0x02,
		COLOR		= 0x04,
		TEXCOORD	= 0x08,
		INDEX		= 0x10,
		ALL = VERTEX|NORMAL|COLOR|TEXCOORD|INDEX,
	};

	// �X�̗v�f�\����
	struct EachValue{
		Matrix44f worldMtx;
		unsigned long textureNum;
	};
private:
	// �I���\�[�p���X�g
	static std::map<unsigned long, DrawElement*> orthoElementList;

	// �p�[�X�y�N�e�B�u�p���X�g
	static std::map<unsigned long, DrawElement*> perthElementList;

	// ���X�g�̓o�^���J�E���^
	static unsigned long listCnt;

	// �f�N�����[�V����
	static IDirect3DVertexDeclaration9* vtxDeclaration;

	// �����̎��ʎq
	unsigned long myNum;

	// �o�[�e�N�X�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9	vtxBuff;
	LPDIRECT3DVERTEXBUFFER9	norBuff;
	LPDIRECT3DVERTEXBUFFER9	clrBuff;
	LPDIRECT3DVERTEXBUFFER9	texBuff;
	LPDIRECT3DINDEXBUFFER9	idxBuff;

	// �v���~�e�B�u�^�C�v
	D3DPRIMITIVETYPE primitiveType;

	// �`��^�C�v
	LIST_TYPE listType;

	// �v�f�̌�
	unsigned long vertexCnt;
	unsigned long indexCnt;
	unsigned long polygonCnt;

	// �`�掞�̍s��
	std::map<unsigned long, EachValue> eachValueList;
	unsigned long eachValueCnt;

protected:
	// �R���X�g���N�^
	DrawElement(const unsigned long &VertexCnt, const unsigned long &IndexCnt, const LIST_TYPE &ListType);

	// �f�X�g���N�^
	virtual ~DrawElement();

	// �`��
	virtual void Draw();

	// �`�揀��
	void ApplyBuffer(BUFFER_TYPE type);

	// �Z�b�^�[
	void SetPrimitiveType(const PRIMITIVE_TYPE &Type);
	void SetPolygonCnt(const unsigned long &Count);
	void SetVertex(Vector3f* Data, const unsigned long &Count);
	void SetNormal(Vector3f* Data, const unsigned long &Count);
	void SetDiffuse(Color1ul* Data, const unsigned long &Count);
	void SetTexcoord(Vector2f* Data, const unsigned long &Count);
	void SetIndex(DWORD* Data, const unsigned long &Count);
	//void SetMatrix(const Matrix44f &Data);

	// �X�ɐݒ肵�����o�b�t�@������ꍇ�ɂ��̊֐��ɂ����Ε`��O�ɌĂ΂��
	virtual void OriginalApply(unsigned long num) = 0;
public:
	// �͂��߂ɓǂ�ł���
	static void InitDecralation();
	// �h���E����
	static void DrawAll();

	// �`�敨�ǉ�
	virtual unsigned long AddDraw(EachValue** pointer);
};
}
#endif
//=EOF=