/**
	@file	snTextureManager.cpp
	@brief	�e�N�X�`���Ǘ��H
	@date	2014/08/23
*/

/// Includes
#include "snTextureManager.h"

namespace snStudy{

/// Static variable member
TextureManager* TextureManager::originalPointer = NULL;

/**
@brief	�R���X�g���N�^
@param	none
@return	none
@date	2014/08/23
*/
TextureManager::TextureManager(){
	textureNumCnt = 1;
}

/**
@brief	�f�X�g���N�^
@param	none
@return	none
@date	2014/08/23
*/
TextureManager::~TextureManager(){

}

/**
	@brief	�e�N�X�`���̊Ǘ��ԍ��擾
	@param	�t�@�C���p�X
	@return	none
	@date	2014/08/23
*/
unsigned long TextureManager::GetTextuneNum(char* filePath){
	// �e�N�X�`�������邩�m�F
	if (!textureNameToNumList.count(filePath)){
		// �e�N�X�`���쐬 & �o�^
		textureNameToNumList[filePath] = textureNumCnt;
		D3DXCreateTextureFromFile(RenderingManager::GetPointer()->GetDevice(), filePath, &textureList[textureNumCnt]);

		_ASSERT_EXPR(textureList[textureNumCnt], L"TextureManager::Texture load failed");
		textureNumCnt++;
	}
	return textureNameToNumList[filePath];
}

/**
	@brief	�e�N�X�`���ݒ�
	@param	�e�N�X�`���ԍ�
	@return	none
	@date	2014/08/23
*/
void TextureManager::SetTexture(unsigned long textureNum){
	RenderingManager::GetPointer()->GetDevice()->SetTexture(0, textureNum?textureList[textureNum]:NULL);
}

}

//=EOF=