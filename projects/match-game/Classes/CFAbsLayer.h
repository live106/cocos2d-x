#ifndef __CFABSLAYER_H__
#define __CFABSLAYER_H__
#include "lib/CFGameEngine.h"
#include "lib/CFSprite.h"

using namespace cocos2d;

/************************************************************************/
/* 抽象游戏场景扩展，游戏中所有场景使用同样的背景                          */
/************************************************************************/
class CFAbsLayer:public CCLayer
{
private:
    CCSprite* pMountains;
    CCSprite* pStone;
    CCSprite* pLeftTree;
    CCSprite* pRightTree;
protected:
    CCSpriteFrameCache* pFrameCache;
    CCSize winsize;
    CFRadiationSprite* pBackGround;
    CFHorizonWaterLinesSprite* pWaterLine;
public:
    CFAbsLayer(void);
    virtual ~CFAbsLayer(void);
    virtual bool init();
    LAYER_NODE_FUNC(CFAbsLayer);
    //游戏中每个不同的场景都是用同一套背景，只是为不同的场景设置不同的颜色方案
    void setColorPlan();
};
#endif

