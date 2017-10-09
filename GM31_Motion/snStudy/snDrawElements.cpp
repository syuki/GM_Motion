/// [DrawElement]
/// Drawing element
// 20098 syuki nishida
// 140701


/// Includes
#include "snDrawElements.h"
#include "snTextureManager.h"

//
#include "../CSceneX.h"
//

namespace snStudy{

/// Static variavle member
std::map<unsigned long, DrawElement*> DrawElement::orthoElementList;
std::map<unsigned long, DrawElement*> DrawElement::perthElementList;
unsigned long DrawElement::listCnt = 0;
IDirect3DVertexDeclaration9* DrawElement::vtxDeclaration = NULL;

/// Init decralation
// Arguments : none
// ReturnVal : none
// 140702
void DrawElement::InitDecralation(){
	if (vtxDeclaration){ return; }
	// Element set
	D3DVERTEXELEMENT9 vtxElem[] = {
		// stream offset type tesse usage usageindex
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 2, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
	};
	RenderingManager::GetPointer()->GetDevice()->CreateVertexDeclaration(vtxElem, &vtxDeclaration);
}

/// Constructer
// Arguments :
// ReturnVal : none
// 140701
DrawElement::DrawElement(const unsigned long &VertexCnt, const unsigned long &IndexCnt, const LIST_TYPE &ListType){

	myNum = listCnt;
	listType = ListType;

	switch (listType){
	case ORTHO:
		orthoElementList[myNum] = this;
		break;
	case PERTH:
		perthElementList[myNum] = this;
		break;
	};
	listCnt++;

	primitiveType = D3DPT_LINELIST;
	vertexCnt = VertexCnt;
	indexCnt = IndexCnt;
	polygonCnt = IndexCnt / 3;
	eachValueCnt = 0;
	
	if (FAILED(RenderingManager::GetPointer()->GetDevice()->
		CreateVertexBuffer(sizeof(Vector3f)*vertexCnt, D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_MANAGED, &vtxBuff, NULL))) {
		MessageBox(RenderingManager::GetPointer()->GetWindowHandle(), "Failed to create vertex buffer", "error", MB_OK);
		return;
	}
	if (FAILED(RenderingManager::GetPointer()->GetDevice()->
		CreateVertexBuffer(sizeof(Vector3f)*vertexCnt, D3DUSAGE_WRITEONLY, D3DFVF_NORMAL, D3DPOOL_MANAGED, &norBuff, NULL))) {
		MessageBox(RenderingManager::GetPointer()->GetWindowHandle(), "Failed to create vertex buffer", "error", MB_OK);
		return;
	}
	if (FAILED(RenderingManager::GetPointer()->GetDevice()->
		CreateVertexBuffer(sizeof(Color1ul)*vertexCnt, D3DUSAGE_WRITEONLY, D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &clrBuff, NULL))) {
		MessageBox(RenderingManager::GetPointer()->GetWindowHandle(), "Failed to create vertex buffer", "error", MB_OK);
		return;
	}
	if (FAILED(RenderingManager::GetPointer()->GetDevice()->
		CreateVertexBuffer(sizeof(Vector2f)*vertexCnt, D3DUSAGE_WRITEONLY, D3DFVF_TEX1, D3DPOOL_MANAGED, &texBuff, NULL))) {
		MessageBox(RenderingManager::GetPointer()->GetWindowHandle(), "Failed to create vertex buffer", "error", MB_OK);
		return;
	}
	if (FAILED(RenderingManager::GetPointer()->GetDevice()->
		CreateIndexBuffer(sizeof(DWORD)*indexCnt, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &idxBuff, NULL))){
		MessageBox(RenderingManager::GetPointer()->GetWindowHandle(), "Failed to create index buffer", "error", MB_OK);
		return;
	}
}

/// Destructer
// Arguments :
// ReturnVal : none
// 140701
DrawElement::~DrawElement(){

	switch (listType){
	case ORTHO:
		orthoElementList.erase(myNum);
		break;

	case PERTH:
		perthElementList.erase(myNum);
		break;
	};
}

/// Draw
// Arguments :
// ReturnVal :
// 140702
void DrawElement::Draw(){
	// デバイス取得
	LPDIRECT3DDEVICE9 device = RenderingManager::GetPointer()->GetDevice();
	
	// 頂点フォーマット設定
	device->SetVertexDeclaration(vtxDeclaration);

	// バッファ適応
	ApplyBuffer(ALL);


	// 持ってる行列を全部描画
	for (std::map<unsigned long, EachValue>::iterator it = eachValueList.begin(); it != eachValueList.end(); ++it){
		// 個々の最低限の要素適応
		device->SetTransform(D3DTS_WORLD, &D3DXMATRIX(it->second.worldMtx));
		TextureManager::GetPointer()->SetTexture(it->second.textureNum);

		// 個々の個別の要素適応
		OriginalApply(it->first);
		device->DrawIndexedPrimitive(primitiveType, 0, 0, vertexCnt, 0, polygonCnt);
	}
}

/**
	@brief	頂点バッファ適応
	@param	適用するバッファタイプ
	@return	none
*/
void DrawElement::ApplyBuffer(BUFFER_TYPE type){
	LPDIRECT3DDEVICE9 device = RenderingManager::GetPointer()->GetDevice();

	if (type & VERTEX){
		device->SetStreamSource(0, vtxBuff, 0, sizeof(Vector3f));
	}

	if (type & NORMAL){
		device->SetStreamSource(1, norBuff, 0, sizeof(Vector3f));
	}

	if (type & COLOR){
		device->SetStreamSource(2, clrBuff, 0, sizeof(Color1ul));
	}

	if (type & TEXCOORD){
		device->SetStreamSource(3, texBuff, 0, sizeof(Vector2f));
	}

	if (type & INDEX){
		device->SetIndices(idxBuff);
	}
}

/// Set vertex
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetVertex(Vector3f* Data, const unsigned long &Count){
	Vector3f* vtx;
	vtxBuff->Lock(0, 0, (void**)&vtx, 0);
	memcpy_s(vtx, sizeof(Vector3f)*vertexCnt, Data, sizeof(Vector3f) * (Count ? Count : vertexCnt));
	vtxBuff->Unlock();
}

/// Set Normal
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetNormal(Vector3f* Data, const unsigned long &Count){
	Vector3f* vtx;
	norBuff->Lock(0, 0, (void**)&vtx, 0);
	memcpy_s(vtx, sizeof(Vector3f)*vertexCnt, Data, sizeof(Vector3f) * (Count ? Count : vertexCnt));
	norBuff->Unlock();
}

/// Set diffuse
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetDiffuse(Color1ul* Data, const unsigned long &Count){
	Color1ul* vtx;
	clrBuff->Lock(0, 0, (void**)&vtx, 0);
	memcpy_s(vtx, sizeof(Color1ul)*vertexCnt, Data, sizeof(Color1ul) * (Count ? Count : vertexCnt));
	clrBuff->Unlock();
}

/// Set texcoord
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetTexcoord(Vector2f* Data, const unsigned long &Count){
	Vector2f* vtx;
	texBuff->Lock(0, 0, (void**)&vtx, 0);
	memcpy_s(vtx, sizeof(Vector2f)*vertexCnt, Data, sizeof(Vector2f) * (Count ? Count : vertexCnt));
	texBuff->Unlock();
}

/// Set index
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetIndex(DWORD* Data, const unsigned long &Count){
	DWORD* vtx;
	idxBuff->Lock(0, 0, (void**)&vtx, 0);
	memcpy_s(vtx, sizeof(DWORD)*indexCnt, Data, sizeof(DWORD) * (Count ? Count : indexCnt));
	idxBuff->Unlock();
}

/// Set matrix
// Arguments :
// ReturnVal :
// 140701
//void DrawElement::SetMatrix(const Matrix44f &Data){
//	transMtx = Data;
//}

/// Set polygon count
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetPolygonCnt(const unsigned long &Count){
	polygonCnt = Count;
}

/// Set primitive type
// Arguments :
// ReturnVal :
// 140701
void DrawElement::SetPrimitiveType(const PRIMITIVE_TYPE &Type){
	switch (Type){
	case POINTLIST:
		primitiveType = D3DPT_POINTLIST;
		break;
	case LINELIST:
		primitiveType = D3DPT_LINELIST;
		break;
	case LINESTRIP:
		primitiveType = D3DPT_LINESTRIP;
		break;
	case TRIANGLELIST:
		primitiveType = D3DPT_TRIANGLELIST;
		break;
	case TRIANGLESTRIP:
		primitiveType = D3DPT_TRIANGLESTRIP;
		break;
	default:
		primitiveType = D3DPT_TRIANGLELIST;
		break;
	};
}

/**
	@brief	描画物追加
	@param	個々要素へのポインタ
	@return ID
	@date	2014/08/23
*/
unsigned long DrawElement::AddDraw(EachValue** pointer){
	// ID確定
	int id = eachValueCnt;

	// 行列初期化
	eachValueList[id].worldMtx = Matrix44f().Identity();
	eachValueList[id].textureNum = 0;

	// ポインタ渡す
	*pointer = &(eachValueList[id]);
	eachValueCnt++;

	return id;
}


/// Draw all
// Arguments :
// ReturnVal : none
// 140701
/* Draw all macro*/
#define DRAW_ALL(LIST)	for (std::map<unsigned long, DrawElement*>::iterator itElement = LIST.begin(); itElement != LIST.end(); ++itElement){itElement->second->Draw();}
void DrawElement::DrawAll(){

	// Drawing perth primitive
	RenderingManager::GetPointer()->SetPerthProjection();
	DRAW_ALL(perthElementList);

	for (std::vector<CSceneX*>::iterator itElement = CSceneX::sceneXList.begin(); itElement != CSceneX::sceneXList.end(); ++itElement){
		(*itElement)->Draw();
	}

	// Drawing ortho primitive
	RenderingManager::GetPointer()->SetOrthoProjection();
	DRAW_ALL(orthoElementList);
}

}

//=EOF=