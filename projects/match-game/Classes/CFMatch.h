#ifndef __CFMATCH_H__
#define __CFMATCH_H__

#include "cocos2d.h"
#include "CFSlot.h"
#include "CFLevelLoader.h"
#include "lib/CFSprite.h"

using namespace cocos2d;

class CFMatch;
class CFDragableMatch;
class CFRemoveableMatch;
class CFLevel;
class CFGameLayer;
/************************************************************************/
/* »ð²ñ
/************************************************************************/
class CFMatch:public CFTouchableSprite{
protected:
    CFLevel* pLevel;
    int bindSlotIndex;
    int index;
public:
    CFMatch(CFLevel* pLevel);
    virtual ~CFMatch();
    CCParticleGalaxy* getFireParticle();
    void setBindSlot(int index);
    int getBindSlot();
    void setIndex(int index);
    int getIndex();
    virtual bool resetAction() = 0;
    virtual bool cfTouchBegan(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchMoved(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchEnded(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) = 0;
    CFLevel* getLevel();
    static CFMatch* matchDragableWithFrame(CFLevel* pLevel, CCSpriteFrame* pFrame);
    static CFMatch* matchRemoveableWithFrame(CFLevel* pLevel, CCSpriteFrame* pFrame);
};

class CFDragableMatch:public CFMatch{
public:
    CFDragableMatch(CFLevel* pLevel);
    virtual ~CFDragableMatch();
    virtual bool resetAction();
    virtual bool cfTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void cfTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void cfTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void cfTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

class CFRemoveableMatch:public CFMatch{
private:
    CCPoint beginpos;
public:
    CFRemoveableMatch(CFLevel* pLevel);
    virtual ~CFRemoveableMatch();
    virtual bool resetAction();
    virtual bool cfTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void cfTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void cfTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void cfTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

#endif