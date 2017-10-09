#ifndef _C_MOTION_MODEL_H_
#define _C_MOTION_MODEL_H_

#include "CSceneX.h"
#include "snStudy/snUpdateElements.h"
#include "CAnimatiom.h"

#define MOTION_CHARA_GRAVITY 0.5f;

class CMotionModel : public snStudy::UpdateElement{
	enum ANIME_TYPE{
		STOP,
		WARK,
		PUNCH,
		DAMAGE,
	};

	CMotionModel();
	virtual ~CMotionModel();

	virtual void Update();

	void EditUpdate();

	CSceneX* head;
	CSceneX* body;
	CSceneX* arm_r;
	CSceneX* arm_l;
	CSceneX* hand_r;
	CSceneX* hand_l;
	CSceneX* leg_r;
	CSceneX* leg_l;
	CSceneX* foot_r;
	CSceneX* foot_l;

	bool dbgFlg;

	float rot;
	float gravity;

	unsigned long frameCnt;
	CAnimation stopAnime;
	CAnimation warkAnime;
	CAnimation punchAnime;
	CAnimation damageAnime;
	ANIME_TYPE oldAnimeType;

public:
	static CMotionModel* Create();
	void Release(){
		delete this;
	}
};

#endif
