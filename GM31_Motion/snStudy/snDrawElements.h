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
	// プリミティブタイプ
	enum PRIMITIVE_TYPE {
		POINTLIST,
		LINELIST,
		LINESTRIP,
		TRIANGLELIST,
		TRIANGLESTRIP,
	};

public:
	// 描画のリストのタイプ
	enum LIST_TYPE {
		ORTHO,
		PERTH,
		LIST_TYPE_MAX,
	};

protected:
	// バッファの種類
	enum BUFFER_TYPE{
		VERTEX		= 0x01,
		NORMAL		= 0x02,
		COLOR		= 0x04,
		TEXCOORD	= 0x08,
		INDEX		= 0x10,
		ALL = VERTEX|NORMAL|COLOR|TEXCOORD|INDEX,
	};

	// 個々の要素構造体
	struct EachValue{
		Matrix44f worldMtx;
		unsigned long textureNum;
	};
private:
	// オルソー用リスト
	static std::map<unsigned long, DrawElement*> orthoElementList;

	// パースペクティブ用リスト
	static std::map<unsigned long, DrawElement*> perthElementList;

	// リストの登録数カウンタ
	static unsigned long listCnt;

	// デクラレーション
	static IDirect3DVertexDeclaration9* vtxDeclaration;

	// 自分の識別子
	unsigned long myNum;

	// バーテクスバッファ
	LPDIRECT3DVERTEXBUFFER9	vtxBuff;
	LPDIRECT3DVERTEXBUFFER9	norBuff;
	LPDIRECT3DVERTEXBUFFER9	clrBuff;
	LPDIRECT3DVERTEXBUFFER9	texBuff;
	LPDIRECT3DINDEXBUFFER9	idxBuff;

	// プリミティブタイプ
	D3DPRIMITIVETYPE primitiveType;

	// 描画タイプ
	LIST_TYPE listType;

	// 要素の個数
	unsigned long vertexCnt;
	unsigned long indexCnt;
	unsigned long polygonCnt;

	// 描画時の行列
	std::map<unsigned long, EachValue> eachValueList;
	unsigned long eachValueCnt;

protected:
	// コンストラクタ
	DrawElement(const unsigned long &VertexCnt, const unsigned long &IndexCnt, const LIST_TYPE &ListType);

	// デストラクタ
	virtual ~DrawElement();

	// 描画
	virtual void Draw();

	// 描画準備
	void ApplyBuffer(BUFFER_TYPE type);

	// セッター
	void SetPrimitiveType(const PRIMITIVE_TYPE &Type);
	void SetPolygonCnt(const unsigned long &Count);
	void SetVertex(Vector3f* Data, const unsigned long &Count);
	void SetNormal(Vector3f* Data, const unsigned long &Count);
	void SetDiffuse(Color1ul* Data, const unsigned long &Count);
	void SetTexcoord(Vector2f* Data, const unsigned long &Count);
	void SetIndex(DWORD* Data, const unsigned long &Count);
	//void SetMatrix(const Matrix44f &Data);

	// 個々に設定したいバッファがある場合にこの関数にかけば描画前に呼ばれる
	virtual void OriginalApply(unsigned long num) = 0;
public:
	// はじめに読んでおく
	static void InitDecralation();
	// ドロウする
	static void DrawAll();

	// 描画物追加
	virtual unsigned long AddDraw(EachValue** pointer);
};
}
#endif
//=EOF=