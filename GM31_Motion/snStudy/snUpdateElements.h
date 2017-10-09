/// [UpdateElement]
/// Update element
// 20098 syuki nishida
// 140701
#pragma once
#ifndef _SN_UPDATE_ELEMENT_H_
#define _SN_UPDATE_ELEMENT_H_

/// Includes
#include <map>

namespace snStudy{

/// Class decralation
class UpdateElement{
	// 要素リスト
	static std::map<unsigned long, UpdateElement*> updateElementList;

	// リストのカウンタ
	static unsigned long listCnt;

	// 自分の識別子
	unsigned long myId;
protected:
	// コンストラクタ
	UpdateElement();

	// デストラクタ
	virtual ~UpdateElement();

	// 更新
	virtual void Update() = 0;
public:
	// 全部更新
	static void UpdateAll();
};

}

#endif
//=EOF=