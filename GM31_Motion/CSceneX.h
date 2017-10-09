#pragma once
#ifndef _C_SCENE_X_H_
#define _C_SCENE_X_H_

#include "snStudy/snStudy.h"
#include <vector>

class CSceneX : public snStudy::UpdateElement{
	// デバイス
	LPDIRECT3DDEVICE9	mpD3DDevice;

	// バッファ
	LPD3DXBUFFER	mBuff;
	DWORD			mMatNum;
	LPD3DXMESH		mMesh;

	// 情報
	snStudy::Vector3f mPos;
	snStudy::Vector3f mRot;
	snStudy::Vector3f mScl;
	snStudy::Matrix44f mMyMtx;
	snStudy::Matrix44f* mPaMtx;

public:
	CSceneX(LPDIRECT3DDEVICE9 D3Device, char* Model);
	virtual ~CSceneX();

	virtual void Update();
	void Draw();
	static CSceneX* Create(LPDIRECT3DDEVICE9 D3Device, char* Model);

	void SetPosition(snStudy::Vector3f Pos){ mPos = Pos; }
	void SetRotate(snStudy::Vector3f Rot){ mRot = Rot; }
	void SetParentMtxPtr(snStudy::Matrix44f* Mtx){ mPaMtx = Mtx; }

	snStudy::Vector3f GetPosition(void){ return mPos; }
	snStudy::Vector3f GetRotate(void){ return mRot; }
	snStudy::Matrix44f* GetMyMtxPtr(void){ return &mMyMtx; }


	static std::vector<CSceneX*> sceneXList;
};

#endif