/**
	@file	snTextureManager.h
	@brief	�e�N�X�`���Ǘ��H
	@date	2014/08/23
*/
#pragma once
#ifndef _SN_TEXTURE_MANAGER_H_
#define _SN_TEXTURE_MANAGER_H_

/// Includes
#include <map>
#include <string>
#include "snRenderingManager.h"

namespace snStudy{

/// Class
class TextureManager{

	//====�ϐ�====//

	// �����|�C���^
	static TextureManager* originalPointer;

	// �e�N�X�`�����ƊǗ��ԍ��ƍ����X�g
	std::map<std::string, unsigned long> textureNameToNumList;

	// �e�N�X�`�����X�g
	std::map<unsigned long, LPDIRECT3DTEXTURE9> textureList;

	// �Ǘ��ԍ��J�E���^
	unsigned long textureNumCnt;

	//====�֐�====//

	// �R���X�g���N�^
	TextureManager();

	// �f�X�g���N�^
	virtual ~TextureManager();
public:

	// �|�C���^�擾
	static TextureManager* GetPointer(){
		if (!originalPointer){
			originalPointer = new TextureManager();
		}
		return originalPointer;
	}
	
	static void Release(){
		if (originalPointer){
			delete originalPointer;
		}
	}

	// �e�N�X�`���̊Ǘ��ԍ��擾
	unsigned long GetTextuneNum(char* filePath);

	// �e�N�X�`���Z�b�g
	void SetTexture(unsigned long textureNum);
};

}

#endif // _SN_TEXTURE_MANAGER_H_
//=EOF=