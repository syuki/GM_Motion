/**
	@file	snTextureManager.cpp
	@brief	テクスチャ管理？
	@date	2014/08/23
*/

/// Includes
#include "snTextureManager.h"

namespace snStudy{

/// Static variable member
TextureManager* TextureManager::originalPointer = NULL;

/**
@brief	コンストラクタ
@param	none
@return	none
@date	2014/08/23
*/
TextureManager::TextureManager(){
	textureNumCnt = 1;
}

/**
@brief	デストラクタ
@param	none
@return	none
@date	2014/08/23
*/
TextureManager::~TextureManager(){

}

/**
	@brief	テクスチャの管理番号取得
	@param	ファイルパス
	@return	none
	@date	2014/08/23
*/
unsigned long TextureManager::GetTextuneNum(char* filePath){
	// テクスチャがあるか確認
	if (!textureNameToNumList.count(filePath)){
		// テクスチャ作成 & 登録
		textureNameToNumList[filePath] = textureNumCnt;
		D3DXCreateTextureFromFile(RenderingManager::GetPointer()->GetDevice(), filePath, &textureList[textureNumCnt]);

		_ASSERT_EXPR(textureList[textureNumCnt], L"TextureManager::Texture load failed");
		textureNumCnt++;
	}
	return textureNameToNumList[filePath];
}

/**
	@brief	テクスチャ設定
	@param	テクスチャ番号
	@return	none
	@date	2014/08/23
*/
void TextureManager::SetTexture(unsigned long textureNum){
	RenderingManager::GetPointer()->GetDevice()->SetTexture(0, textureNum?textureList[textureNum]:NULL);
}

}

//=EOF=