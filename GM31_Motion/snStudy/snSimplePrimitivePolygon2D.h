/**
	@file	snSimplePrimitivePolygon2D.h
	@brief	2Dポリゴン
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
	//==構造体==//

	// オリジナル要素
	struct OriginalValue{
		// 個々の要素
		EachValue* eachValue;

		// アルファ値
		float* alphaValue;

		// テクスコード
		Vector2f* texCoord[4];
	};

private:

	//==変数==//

	std::map<unsigned long, OriginalValue> originalValueList;
	
	//==関数==//

	// コンストラクタ
	SimplePrimitivePolygon2D(float width, float height, LIST_TYPE dimension);

	// デストラクタ
	virtual ~SimplePrimitivePolygon2D();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// はじめに読んでおく
	static void InitDecralation();
	// ドロウする
	static void DrawAll();

	// 個々の設定用関数
	virtual void OriginalApply(unsigned long num);
public:
	static SimplePrimitivePolygon2D* Create(float width, float height, LIST_TYPE dimension = ORTHO){ return new SimplePrimitivePolygon2D(width, height, dimension); }
	void Release(){
		delete this;
	}

	// 描画物追加
	virtual unsigned long AddDraw();

	// 個々の要素の取得
	OriginalValue CopyOriginalValue(unsigned long id){
		// リストにないよ
		_ASSERT(originalValueList.count(id));
		return originalValueList[id];
	}
};

}

#endif // _SN_SIMPLE_PRIMITIVE_POLYGON_2D_H_

//=EOF=