#ifndef __CFLAYER_H__
#define __CFLAYER_H__
#include "cocos2d.h"
using namespace cocos2d;

/**
* 可响应拖拽事件的CCLayer包装
*/
class CFLayer:public CCLayer
{
public:
    CFLayer(void);
    virtual ~CFLayer(void);
    virtual bool init();
    LAYER_NODE_FUNC(CFLayer);
};

/**
* 可响应拖拽事件的CCLayer包装
*/
class CFDragableLayer:public CFLayer,public CCTargetedTouchDelegate
{
public:
    CFDragableLayer(void);
    virtual ~CFDragableLayer(void);
    virtual bool init();
    LAYER_NODE_FUNC(CFDragableLayer);
};
#endif
