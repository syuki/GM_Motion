/// [UpdateElement]
/// Update element
// 20098 syuki nishida
// 140701

/// Includes
#include "snUpdateElements.h"

namespace snStudy{

/// Static variavle member
std::map<unsigned long, UpdateElement*> UpdateElement::updateElementList;
unsigned long UpdateElement::listCnt = 0;

/// Constructer
// Arguments :
// ReturnVal : none
// 140701
UpdateElement::UpdateElement(){
	myId = listCnt;
	
	updateElementList[myId] = this;

	listCnt++;
}

/// Destructer
// Arguments :
// ReturnVal : none
// 140701
UpdateElement::~UpdateElement(){
	updateElementList.erase(myId);
}

/// Update all
// Arguments :
// ReturnVal : none
// 140701
void UpdateElement::UpdateAll(){
	for (std::map<unsigned long, UpdateElement*>::iterator itElement = updateElementList.begin(); itElement != updateElementList.end(); ++itElement){
		(*itElement).second->Update();
	}
}

}
//=EOF=