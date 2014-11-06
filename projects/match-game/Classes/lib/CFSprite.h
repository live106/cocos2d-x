#ifndef __CFSPRITE_H__
#define __CFSPRITE_H__

#include "cocos2d.h"

using namespace cocos2d;

typedef enum tagCFTouchableState
{
    stateGrabbed,
    stateUngrabbed
} CFTouchableState; 

/************************************************************************/
/* 这个Sprite将屏蔽其覆盖的其他Sprite，使得其他Sprite不接受事件            */
/************************************************************************/
class CFUnTouchableSprite:public CCSprite, public CCTargetedTouchDelegate

{
public:
    CFUnTouchableSprite(void);
    virtual ~CFUnTouchableSprite(void);
    virtual void onEnter();
    virtual void onExit();
    virtual CCRect rect();//子类实现
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent *event);
};

/************************************************************************/
/* 包装了触摸事件的Sprite                                                */
/************************************************************************/
class CFTouchableSprite:public CCSprite, public CCTargetedTouchDelegate{
private:
    CFTouchableState t_state;
public:
    CFTouchableSprite(void);
    virtual ~CFTouchableSprite(void);
    virtual void onEnter();
    virtual void onExit();
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchMoved(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void ccTouchCancelled(CCTouch *touch, CCEvent *event);
    virtual CCRect rect();
    virtual bool cfTouchBegan(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchMoved(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchEnded(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchCancelled(CCTouch *touch, CCEvent *event) = 0;
};

/************************************************************************/
/* 通过一个点和一个矩形，以点位中心，矩形为边界，显示辐射条纹的Sprite       */
/************************************************************************/
class CFRadiationSprite:public CCSprite{
private:
    bool start;
    bool warning;
    CCSize size;
    void draw();
    void setWarningColors(ccColor4F* colors);
    void setNormalColors(ccColor4F* colors);
public:
    CFRadiationSprite();
    virtual ~CFRadiationSprite(void);
    virtual bool init(CCSize size);
    LAYER_NODE_FUNC_PARAM(CFRadiationSprite,CCSize, size);    
    void startAnimation();
    void stopAnimation();
    void startWarning(float time);
    void stopWarning();
};

/************************************************************************/
/* 一组水平波纹模拟Sprite                                                */
/************************************************************************/
class CFHorizonWaterLinesSprite:public CCSprite{
private:
    bool start;
    bool warning;
    CCSize size;
    void draw();
    ccColor4F getWarningColors();
    ccColor4F getNormalColors();
public:
    CFHorizonWaterLinesSprite();
    virtual ~CFHorizonWaterLinesSprite(void);
    virtual bool init(CCSize size);
    LAYER_NODE_FUNC_PARAM(CFHorizonWaterLinesSprite, CCSize, size);
    void startAnimation();
    void stopAnimation();
};
#endif
