/**
	@file	snTextureManager.h
	@brief	テクスチャ管理？
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

	//====変数====//

	// 自分ポインタ
	static TextureManager* originalPointer;

	// テクスチャ名と管理番号照合リスト
	std::map<std::string, unsigned long> textureNameToNumList;

	// テクスチャリスト
	std::map<unsigned long, LPDIRECT3DTEXTURE9> textureList;

	// 管理番号カウンタ
	unsigned long textureNumCnt;

	//====関数====//

	// コンストラクタ
	TextureManager();

	// デストラクタ
	virtual ~TextureManager();
public:

	// ポインタ取得
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

	// テクスチャの管理番号取得
	unsigned long GetTextuneNum(char* filePath);

	// テクスチャセット
	void SetTexture(unsigned long textureNum);
};

}

#endif // _SN_TEXTURE_MANAGER_H_
//=EOF=