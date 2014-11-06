#include "CFMatch.h"

/************************************************************************/
/* Match实现，火柴棍
/************************************************************************/

CFMatch::CFMatch(CFLevel* pLevel):bindSlotIndex(-1),pLevel(pLevel){
}

CFMatch::~CFMatch(){

}

CCParticleGalaxy* CFMatch::getFireParticle(){
    CCParticleGalaxy* m_emitter = CCParticleGalaxy::node();
    m_emitter->retain();
    m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("texture/fire.png") );
    CCSize size = this->getContentSize();
    m_emitter->setPosition(ccp(0, size.height));
    m_emitter->setTag(0);
    m_emitter->setLife(1);
    return m_emitter;
}

void CFMatch::setBindSlot(int index){
    this->bindSlotIndex = index;
}

int CFMatch::getBindSlot(){
    return this->bindSlotIndex;
}

void CFMatch::setIndex(int index){
    this->index = index;
}

int CFMatch::getIndex(){
    return this->index;
}

CFLevel* CFMatch::getLevel(){
    return this->pLevel;
}


//获取可移动的火柴
CFMatch* CFMatch::matchDragableWithFrame(CFLevel* pLevel, CCSpriteFrame* pFrame)
{
    CFMatch* pMatch = new CFDragableMatch(pLevel);
    if(pMatch && pMatch->initWithSpriteFrame(pFrame)){
        pMatch->autorelease();
        return pMatch;
    }
    CC_SAFE_DELETE(pMatch);
    return NULL;
}

//获取可消除的火柴
CFMatch* CFMatch::matchRemoveableWithFrame(CFLevel* pLevel, CCSpriteFrame* pFrame){
    CFMatch* pMatch = new CFRemoveableMatch(pLevel);
    if(pMatch && pMatch->initWithSpriteFrame(pFrame)){
        pMatch->autorelease();
        return pMatch;
    }
    CC_SAFE_DELETE(pMatch);
    return NULL;
}


/**
* 可移动的火柴
*/
CFDragableMatch::CFDragableMatch(CFLevel* pLevel):CFMatch(pLevel){

}

CFDragableMatch::~CFDragableMatch(){

}

bool CFDragableMatch::resetAction(){
    int slotid = this->getBindSlot();
    CFSlot* pSlot = this->pLevel->getSlots()->getObjectAtIndex(slotid);
    pSlot->mountMatch(this);
    return true;
}

bool CFDragableMatch::cfTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(!this->pLevel){//||!this->pLevel->isEnable()
        return false;
    }
    //this->getFireParticle();
    this->getParent()->reorderChild(this, 39);
    this->runAction(CCRotateBy::actionWithDuration(0.1,45));
    this->pLevel->showSlots();
    return true;
}

void CFDragableMatch::cfTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->locationInView( touch->view() );
    touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
    setPosition( CCPointMake(touchPoint.x, touchPoint.y) );
}

void CFDragableMatch::cfTouchEnded(CCTouch* touch, CCEvent* event)
{
    this->cfTouchMoved(touch, event);
    CFLevel* pLevel = this->getLevel();
    CFSlot* toslot = pLevel->pointInFirstSlot(this->getPosition());
    if(toslot && !toslot->hasMounted()){
        int fromslotid = this->getBindSlot();
        int toslotid = toslot->getIndex();
        pLevel->doStep(fromslotid,toslotid);
    }else{//松开触摸时，没有与任何slot相交，将火柴退回原slot
        this->resetAction();
    }
    this->removeChildByTag(0, true);
    this->pLevel->hideSlots();
}


void CFDragableMatch::cfTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    this->removeChildByTag(0, true);
}

/**
* 可删除的火柴
*/
CFRemoveableMatch::CFRemoveableMatch(CFLevel* pLevel):CFMatch(pLevel){
}

CFRemoveableMatch::~CFRemoveableMatch(){

}

bool CFRemoveableMatch::resetAction(){
    //int slotid = this->getBindSlot();
    //CFSlot* pSlot = this->pLevel->getSlots()->getObjectAtIndex(slotid);
    //pSlot->mountMatch(this);
    this->setIsVisible(true);
    return true;
}

bool CFRemoveableMatch::cfTouchBegan(CCTouch* touch, CCEvent* event)
{
    this->beginpos = touch->locationInView( touch->view() );
    return true;
}

void CFRemoveableMatch::cfTouchMoved(CCTouch* touch, CCEvent* event)
{
    //CCPoint touchPoint = touch->locationInView( touch->view() );
    //touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
    //setPosition( CCPointMake(touchPoint.x, touchPoint.y) );
}

void CFRemoveableMatch::cfTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->locationInView( touch->view() );
    if(CCPoint::CCPointEqualToPoint(touchPoint, beginpos)){
        //隐藏火柴，slot号和match号一致
        pLevel->doStep(this->getIndex(), this->getIndex());
    }else{
        this->resetAction();
    }
}


void CFRemoveableMatch::cfTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    cfTouchEnded(pTouch, pEvent);
}