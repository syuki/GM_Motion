/// [UpdateElement]
/// Update element
// 20098 syuki nishida
// 140701
#pragma once
#ifndef _SN_UPDATE_ELEMENT_H_
#define _SN_UPDATE_ELEMENT_H_

/// Includes
#include <map>

namespace snStudy{

/// Class decralation
class UpdateElement{
	// �v�f���X�g
	static std::map<unsigned long, UpdateElement*> updateElementList;

	// ���X�g�̃J�E���^
	static unsigned long listCnt;

	// �����̎��ʎq
	unsigned long myId;
protected:
	// �R���X�g���N�^
	UpdateElement();

	// �f�X�g���N�^
	virtual ~UpdateElement();

	// �X�V
	virtual void Update() = 0;
public:
	// �S���X�V
	static void UpdateAll();
};

}

#endif
//=EOF=