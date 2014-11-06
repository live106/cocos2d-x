#ifndef __CFMENULAYER_H__
#define __CFMENULAYER_H__
#include "cocos2d.h"
#include "lib/CFGameEngine.h"
#include "CFCharpterLayer.h"
#include "CFAbsLayer.h"
using namespace cocos2d;

/************************************************************************/
/* 主菜单场景图层
/************************************************************************/
class CFMenuLayer :public CFAbsLayer
{
private:
    CCScene* pMenuScene;
    CCSprite* pMenuTree;
protected:
    virtual void onStartMenu(CCObject* pSender);
    virtual void onCloseMenu(CCObject* pSender);
public:
    CFMenuLayer();
    virtual ~CFMenuLayer();
    virtual bool init();
    LAYER_NODE_FUNC(CFMenuLayer);
    static CCScene* initMenuScene(void);
    CCSprite* createMenuTree();
    //主菜单的动作
    void showMenus();
    void hideMenus();
    void toCharpterScene();
};

#endif