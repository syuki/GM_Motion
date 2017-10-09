/**
@file	snSimplePrimitivePolygon2D.cpp
@brief	2D�|���S��
@author	syuki nishida
@date	2014/08/22
*/

/// Includes
#include "snSimplePrimitivePolygon2D.h"
#include "snMath.h"

namespace snStudy{

/**
	@brief	�R���X�g���N�^
	@param	none
	@return	none
*/
SimplePrimitivePolygon2D::SimplePrimitivePolygon2D(float width, float height, LIST_TYPE dimension) :DrawElement(4, 4, dimension){
	// �O�p�`
	SetPrimitiveType(TRIANGLESTRIP);

	// �@��
	float nor = 1;

	if (dimension == PERTH){
		height = -height;
		nor = -nor;
	}

	// ���_
	Vector3f vertex[4] = {
		{ 0,		0 , 0 },
		{ width,	0 , 0 },
		{ 0,		height , 0 },
		{ width,	height , 0 },
	};
	SetVertex(vertex, 4);

	// �@��
	Vector3f normal[4] = {
		{ 0, 0, nor },
		{ 0, 0, nor },
		{ 0, 0, nor },
		{ 0, 0, nor },
	};
	SetNormal(normal, 4);

	// ���_�F
	Color1ul diffuse[4] = {
		{ Color4f(1, 1, 1, 1) },
		{ Color4f(1, 1, 1, 1) },
		{ Color4f(1, 1, 1, 1) },
		{ Color4f(1, 1, 1, 1) },
	};
	SetDiffuse(diffuse, 4);

	// �e�N�X�R�[�h
	Vector2f texcoord[4] = {
		Vector2f(0,0),
		Vector2f(1,0),
		Vector2f(0,1),
		Vector2f(1,1),
	};
	SetTexcoord(texcoord, 4);

	// �C���f�b�N�X
	DWORD index[4] = {
		0, 1, 2, 3
	};
	SetIndex(index, 4);

	// �v���~�e�B�u��
	SetPolygonCnt(2);
}

/**
	@brief	�f�X�g���N�^
	@param	none
	@return	none
*/
SimplePrimitivePolygon2D::~SimplePrimitivePolygon2D(){
	for (std::map<unsigned long, OriginalValue>::iterator it = originalValueList.begin(); it != originalValueList.end(); it++){
		delete it->second.texCoord[0];
		delete it->second.texCoord[1];
		delete it->second.texCoord[2];
		delete it->second.texCoord[3];
		delete it->second.alphaValue;
	}
}

/**
	@brief	�X�V
	@param	none
	@return	none
*/
void SimplePrimitivePolygon2D::Update(){

}

/**
	@brief	�`��
	@param	none
	@return	none
*/
void SimplePrimitivePolygon2D::Draw(){

	// �`��
	DrawElement::Draw();
}

/**
	@brief	�X�̐ݒ�p�֐�
	@param	���X�g��ID
	@return	none
	@date	2014/08/23
*/
void SimplePrimitivePolygon2D::OriginalApply(unsigned long num){
	// ���_�F
	float difAlpha = *(originalValueList[num].alphaValue);
	Color1ul diffuse[4] = {
			{ Color4f(1, 1, 1, difAlpha) },
			{ Color4f(1, 1, 1, difAlpha) },
			{ Color4f(1, 1, 1, difAlpha) },
			{ Color4f(1, 1, 1, difAlpha) },
	};
	SetDiffuse(diffuse, 4);

	// �e�N�X�R�[�h
	Vector2f texcoord[4] = {
		*(originalValueList[num].texCoord[0]),
		*(originalValueList[num].texCoord[1]),
		*(originalValueList[num].texCoord[2]),
		*(originalValueList[num].texCoord[3]),
	};
	SetTexcoord(texcoord, 4);
}

/**
@brief	�`�敨�ǉ�
@param	�s��ւ̃|�C���^ddddd
@return ID
@date	2014/08/23
*/
unsigned long SimplePrimitivePolygon2D::AddDraw(){
	// �o�b�t�@
	OriginalValue originalValueBuf;

	// �e�N�X�R�[�h�ݒ�
	originalValueBuf.texCoord[0] = new Vector2f(0, 0);
	originalValueBuf.texCoord[1] = new Vector2f(1, 0);
	originalValueBuf.texCoord[2] = new Vector2f(0, 1);
	originalValueBuf.texCoord[3] = new Vector2f(1, 1);

	// �A���t�@�l�ݒ�
	originalValueBuf.alphaValue = new float(1);

	// ID�擾 & �s��o�^
	int id = DrawElement::AddDraw(&(originalValueBuf.eachValue));

	// �I���W�i���v�f�̃��X�g��
	originalValueList[id] = originalValueBuf;

	return id;
}


}

//=EOF=