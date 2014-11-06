#ifndef __CFABSLAYER_H__
#define __CFABSLAYER_H__
#include "lib/CFGameEngine.h"
#include "lib/CFSprite.h"

using namespace cocos2d;

/************************************************************************/
/* ������Ϸ������չ����Ϸ�����г���ʹ��ͬ���ı���                          */
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
    //��Ϸ��ÿ����ͬ�ĳ���������ͬһ�ױ�����ֻ��Ϊ��ͬ�ĳ������ò�ͬ����ɫ����
    void setColorPlan();
};
#endif

