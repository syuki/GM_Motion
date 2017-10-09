/**
@file	CAnimation
@brief	アニメーションオブジェクト
@author	syuki nishida
@date	今日
*/

/// Includes
#include "CAnimatiom.h"
#include "DebugProc.h"

/**
@brief	コンストラクタ
@param	none
@return	none
@date	2014/09/10
*/
CAnimation::CAnimation(){
	oneLoopFrame = 0;
}

/**
@brief	デストラクタ
@param	none
@return	none
@date	2014/09/10
*/
CAnimation::~CAnimation(){
	for (std::vector<pauseData>::iterator it = pauseList.begin(); it != pauseList.end(); it++){
		delete[] (*it).pauseArr;
	}
}

/**
@brief	ポーズ読む
@param	ファイル名
@return	none
@date	2014/09/10
*/
void CAnimation::LoadPauseFromFile(char* FileName, unsigned long FrameTime){
	FILE* fp;
	fopen_s(&fp, FileName, "rb");
	if (fp){
		pauseData nowData;
		snStudy::Vector3f* rotArr = new snStudy::Vector3f[11]();
		fread(rotArr, sizeof(snStudy::Vector3f), 11, fp);
		fclose(fp);

		nowData.pauseArr = rotArr;
		nowData.frameTime = FrameTime;

		pauseList.push_back(nowData);

		oneLoopFrame += FrameTime;
	}

	return;
}

/**
@brief	このフレームのポーズを取得
@param	フレーム
@return	ポーズ
@date	2014/09/10
*/
snStudy::Vector3f* CAnimation::GetPauseForTime(unsigned long Frame, bool loop){

	float buf = Frame / oneLoopFrame;
	if (buf > 0 && !loop){
		return false;
	}

	Frame %= oneLoopFrame;
	unsigned long pauseNum = 0;
	unsigned long frameNum = Frame;
	pauseData nowPause;
	pauseData nextPause;

	for(std::vector<pauseData>::iterator it = pauseList.begin();it != pauseList.end();it++){
		if (frameNum > it->frameTime){
			frameNum -= it->frameTime;
			pauseNum++;
		}else{
			break;
		}
	}

	snStudy::Vector3f* retPause = new snStudy::Vector3f[11]();

	PrintDebugProc("pauseNum = %d\n", pauseNum);
	PrintDebugProc("frameNum = %d\n", frameNum);

	nowPause = pauseList.at(pauseNum);
	if(pauseNum + 1 == pauseList.size()){
		nextPause = pauseList.at(0);
	}else{
		nextPause = pauseList.at(pauseNum+1);
	}

	float per = float(frameNum) / nowPause.frameTime;

	for (int i = 0; i < 11; i++){
		retPause[i] = nowPause.pauseArr[i].Lerp(nextPause.pauseArr[i], float(per));
	}

	return retPause;
}

//=EOF=