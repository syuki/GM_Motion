/**
	@file	CAnimation
	@brief	アニメーションオブジェクト
	@author	syuki nishida
	@date	今日
*/
#pragma once
#ifndef _C_ANIMATION_H_
#define _C_ANIMATION_H_

/// Includes
#include <vector>
#include <snStudy.h>

/// Class
class CAnimation{
	struct pauseData{
		snStudy::Vector3f* pauseArr;
		unsigned long frameTime;
	};

	std::vector<pauseData> pauseList;
	unsigned long oneLoopFrame;


public:
	CAnimation();
	virtual ~CAnimation();

	void LoadPauseFromFile(char* FileName, unsigned long FrameTime);
	snStudy::Vector3f* GetPauseForTime(unsigned long Frame, bool loop);
};
#endif
//=EOF=