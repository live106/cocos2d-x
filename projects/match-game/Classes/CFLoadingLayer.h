#ifndef __CFLOADINGLAYER_H__
#define __CFLOADINGLAYER_H__
#include "cocos2d.h"
#include "CFMenuLayer.h"
#include "lib/CFGameEngine.h"

using namespace cocos2d;

class CFLoadingLayer:public CCLayer
{
public:
    CFLoadingLayer(void);
    virtual ~CFLoadingLayer(void);
    virtual bool init();
    LAYER_NODE_FUNC(CFLoadingLayer);    
    static CCScene* initLoadingScene();
    void loadGame();
};
#endif
