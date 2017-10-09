/**
@file	snSimplePrimitivePolygon2D.cpp
@brief	2Dポリゴン
@author	syuki nishida
@date	2014/08/22
*/

/// Includes
#include "snSimplePrimitivePolygon2D.h"
#include "snMath.h"

namespace snStudy{

/**
	@brief	コンストラクタ
	@param	none
	@return	none
*/
SimplePrimitivePolygon2D::SimplePrimitivePolygon2D(float width, float height, LIST_TYPE dimension) :DrawElement(4, 4, dimension){
	// 三角形
	SetPrimitiveType(TRIANGLESTRIP);

	// 法線
	float nor = 1;

	if (dimension == PERTH){
		height = -height;
		nor = -nor;
	}

	// 頂点
	Vector3f vertex[4] = {
		{ 0,		0 , 0 },
		{ width,	0 , 0 },
		{ 0,		height , 0 },
		{ width,	height , 0 },
	};
	SetVertex(vertex, 4);

	// 法線
	Vector3f normal[4] = {
		{ 0, 0, nor },
		{ 0, 0, nor },
		{ 0, 0, nor },
		{ 0, 0, nor },
	};
	SetNormal(normal, 4);

	// 頂点色
	Color1ul diffuse[4] = {
		{ Color4f(1, 1, 1, 1) },
		{ Color4f(1, 1, 1, 1) },
		{ Color4f(1, 1, 1, 1) },
		{ Color4f(1, 1, 1, 1) },
	};
	SetDiffuse(diffuse, 4);

	// テクスコード
	Vector2f texcoord[4] = {
		Vector2f(0,0),
		Vector2f(1,0),
		Vector2f(0,1),
		Vector2f(1,1),
	};
	SetTexcoord(texcoord, 4);

	// インデックス
	DWORD index[4] = {
		0, 1, 2, 3
	};
	SetIndex(index, 4);

	// プリミティブ数
	SetPolygonCnt(2);
}

/**
	@brief	デストラクタ
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
	@brief	更新
	@param	none
	@return	none
*/
void SimplePrimitivePolygon2D::Update(){

}

/**
	@brief	描画
	@param	none
	@return	none
*/
void SimplePrimitivePolygon2D::Draw(){

	// 描画
	DrawElement::Draw();
}

/**
	@brief	個々の設定用関数
	@param	リストのID
	@return	none
	@date	2014/08/23
*/
void SimplePrimitivePolygon2D::OriginalApply(unsigned long num){
	// 頂点色
	float difAlpha = *(originalValueList[num].alphaValue);
	Color1ul diffuse[4] = {
			{ Color4f(1, 1, 1, difAlpha) },
			{ Color4f(1, 1, 1, difAlpha) },
			{ Color4f(1, 1, 1, difAlpha) },
			{ Color4f(1, 1, 1, difAlpha) },
	};
	SetDiffuse(diffuse, 4);

	// テクスコード
	Vector2f texcoord[4] = {
		*(originalValueList[num].texCoord[0]),
		*(originalValueList[num].texCoord[1]),
		*(originalValueList[num].texCoord[2]),
		*(originalValueList[num].texCoord[3]),
	};
	SetTexcoord(texcoord, 4);
}

/**
@brief	描画物追加
@param	行列へのポインタddddd
@return ID
@date	2014/08/23
*/
unsigned long SimplePrimitivePolygon2D::AddDraw(){
	// バッファ
	OriginalValue originalValueBuf;

	// テクスコード設定
	originalValueBuf.texCoord[0] = new Vector2f(0, 0);
	originalValueBuf.texCoord[1] = new Vector2f(1, 0);
	originalValueBuf.texCoord[2] = new Vector2f(0, 1);
	originalValueBuf.texCoord[3] = new Vector2f(1, 1);

	// アルファ値設定
	originalValueBuf.alphaValue = new float(1);

	// ID取得 & 行列登録
	int id = DrawElement::AddDraw(&(originalValueBuf.eachValue));

	// オリジナル要素のリストへ
	originalValueList[id] = originalValueBuf;

	return id;
}


}

//=EOF=