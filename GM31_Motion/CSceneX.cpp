#include "CSceneX.h"

std::vector<CSceneX*> CSceneX::sceneXList;

CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 D3Device, char* Model){
	// Variable daclaration
	CSceneX* newData;

	// Instantion and initialize
	newData = new CSceneX(D3Device,Model);

	return newData;
}


CSceneX::CSceneX(LPDIRECT3DDEVICE9 D3Device, char* Model){
	mpD3DDevice = D3Device;

	if (FAILED(D3DXLoadMeshFromX(Model, D3DXMESH_SYSTEMMEM, mpD3DDevice, NULL, &mBuff, NULL, &mMatNum, &mMesh))){
		_ASSERT_EXPR(mBuff, L"CSceneX::Model load failed");
		return;
	}

	mPos = mRot = snStudy::Vector3f(0, 0, 0);
	mScl = snStudy::Vector3f(1, 1, 1);
	mPaMtx = NULL;

	sceneXList.push_back(this);
}

CSceneX::~CSceneX(){
	if (mBuff != NULL){
		(mBuff)->Release(); (mBuff) = NULL;
	}
	if (mMesh != NULL){
		(mMesh)->Release(); (mMesh) = NULL;
	}
}

void CSceneX::Update(){
	D3DXMATRIX buf,work;

	// Maxrix transformation
	D3DXMatrixIdentity(&buf);

	D3DXMatrixScaling(&work, mScl.x, mScl.y, mScl.z);
	D3DXMatrixMultiply(&buf, &buf, &work);

	D3DXMatrixRotationZ(&work, mRot.z);
	D3DXMatrixMultiply(&buf, &buf, &work);

	D3DXMatrixRotationX(&work, mRot.x);
	D3DXMatrixMultiply(&buf, &buf, &work);

	D3DXMatrixRotationY(&work, mRot.y);
	D3DXMatrixMultiply(&buf, &buf, &work);

	D3DXMatrixTranslation(&work, mPos.x, mPos.y, mPos.z);
	D3DXMatrixMultiply(&buf, &buf, &work);

	if (mPaMtx){
		D3DXMatrixMultiply(&buf, &D3DXMATRIX(buf), &D3DXMATRIX(*mPaMtx));
	}

	mMyMtx = buf;
}

void CSceneX::Draw(){
	// Variavle declaration
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pD3DMat;


	mpD3DDevice->SetTransform(D3DTS_WORLD, &D3DXMATRIX(mMyMtx));

	// Get the materials
	mpD3DDevice->GetMaterial(&matDef);					// Default
	pD3DMat = (D3DXMATERIAL*)mBuff->GetBufferPointer();	// Draw target

	// Drawing material
	for (UINT nCntMat = 0; nCntMat<mMatNum; nCntMat++){
		mpD3DDevice->SetMaterial(&pD3DMat[nCntMat].MatD3D);
		mpD3DDevice->SetTexture(0, NULL);
		mMesh->DrawSubset(nCntMat);
	}

	// Returned default material
	mpD3DDevice->SetMaterial(&matDef);

}