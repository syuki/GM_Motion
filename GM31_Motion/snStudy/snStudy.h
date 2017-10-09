/**
	@file	snStudy.h
	@brief	•×‹­—p‚Ì‚¢‚ë‚¢‚ë
	@author	syuki nishida
	@date	2014/08/21
*/
#pragma once
#ifndef _SN_STUDY_H_
#define _SN_STUDY_H_

/// Include

#include "snRenderingManager.h"
#include "snMath.h"
#include "snUpdateElements.h"
#include "snDrawElements.h"
#include "snSimplePrimitivePolygon2D.h"
#include "snTextureManager.h"

/// Memory leak check
#include <crtdbg.h>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

#endif
//=EOF=