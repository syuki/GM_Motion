/**
	@file	snSimplePrimitivePolygon2D.h
	@brief	2D�|���S��
	@author	syuki nishida
	@date	2014/08/22
*/
#pragma once
#ifndef _SN_SIMPLE_PRIMITIVE_POLYGON_2D_H_
#define _SN_SIMPLE_PRIMITIVE_POLYGON_2D_H_

/// Incluudes
#include <map>
#include "snUpdateElements.h"
#include "snDrawElements.h"

namespace snStudy{

/// Class decalation
class SimplePrimitivePolygon2D : public DrawElement, public UpdateElement{

public:
	//==�\����==//

	// �I���W�i���v�f
	struct OriginalValue{
		// �X�̗v�f
		EachValue* eachValue;

		// �A���t�@�l
		float* alphaValue;

		// �e�N�X�R�[�h
		Vector2f* texCoord[4];
	};

private:

	//==�ϐ�==//

	std::map<unsigned long, OriginalValue> originalValueList;
	
	//==�֐�==//

	// �R���X�g���N�^
	SimplePrimitivePolygon2D(float width, float height, LIST_TYPE dimension);

	// �f�X�g���N�^
	virtual ~SimplePrimitivePolygon2D();

	// �X�V
	virtual void Update();

	// �`��
	virtual void Draw();

	// �͂��߂ɓǂ�ł���
	static void InitDecralation();
	// �h���E����
	static void DrawAll();

	// �X�̐ݒ�p�֐�
	virtual void OriginalApply(unsigned long num);
public:
	static SimplePrimitivePolygon2D* Create(float width, float height, LIST_TYPE dimension = ORTHO){ return new SimplePrimitivePolygon2D(width, height, dimension); }
	void Release(){
		delete this;
	}

	// �`�敨�ǉ�
	virtual unsigned long AddDraw();

	// �X�̗v�f�̎擾
	OriginalValue CopyOriginalValue(unsigned long id){
		// ���X�g�ɂȂ���
		_ASSERT(originalValueList.count(id));
		return originalValueList[id];
	}
};

}

#endif // _SN_SIMPLE_PRIMITIVE_POLYGON_2D_H_

//=EOF=