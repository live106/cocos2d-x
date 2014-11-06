#include "CFSlot.h"

/************************************************************************/
/* Slot实现，火柴锚点
/************************************************************************/
CFSlot::CFSlot(CFLevel* pLevel):pLevel(pLevel),mountMatchIndex(-1){

}

CFSlot::~CFSlot(){
}

void CFSlot::setIndex(int index){
    this->index = index;
}

int CFSlot::getIndex(){
    return this->index;
}


CFSlot* CFSlot::slotWithFrame(CFLevel* pLevel, CCSpriteFrame* pFrame)
{
    CFSlot* pSlot = new CFSlot(pLevel);
    if(pSlot && pSlot->initWithSpriteFrame(pFrame)){
        pSlot->autorelease();
        return pSlot;
    }
    CC_SAFE_DELETE(pSlot);
    return NULL;
}


void CFSlot::mountMatch(CFMatch* pMatch){
    if(this->hasMounted()){//如果本slot已经被其他的match绑定
        int index = pMatch->getBindSlot();
        CFSlot* pSlot = pMatch->getLevel()->getSlots()->getObjectAtIndex(index);//获取原slot
        pMatch->setRotation(pSlot->getRotation()-360);
        CCRotateBy* pRot = CCRotateBy::actionWithDuration(0.5, 360);
        CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, pSlot->getPosition());
        pMatch->runAction(CCSpawn::actions(pRot, pMove, NULL));
        pMatch->setBindSlot(pSlot->getIndex());
    }else{
        float rot = -pMatch->getRotation()+360+this->getRotation();
        pMatch->setRotation(this->getRotation()-360);
        CCRotateBy* pRot = CCRotateBy::actionWithDuration(0.5, 360);
        CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, this->getPosition());
        pMatch->runAction(CCSpawn::actions(pRot, pMove, NULL));
        pMatch->setBindSlot(this->getIndex());
        this->mountMatchIndex = pMatch->getIndex();
    }
}

void CFSlot::unmountMatch(){
    this->mountMatchIndex = -1;
}

bool CFSlot::hasMounted(){
    return this->mountMatchIndex != -1;
}

CFMatch* CFSlot::getMountedMatch(){
    if(!hasMounted()){
        return NULL;
    }
    CCMutableArray<CFMatch*>* matches = this->pLevel->getMatches();
    return matches->getObjectAtIndex(mountMatchIndex);
}

CFLevel* CFSlot::getLevel(){
    return this->pLevel;
}
