#ifndef __CFLEVELSLAYER_H__
#define __CFLEVELSLAYER_H__
#include "cocos2d.h"

#include "LuaEngine.h"
extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "CFGameLayer.h"
#include "CFAbsLayer.h"
#include "lib/CFGameEngine.h"
#include "lib/CFConfiguration.h"

using namespace cocos2d;

/************************************************************************/
/* ¹Ø¿¨Ñ¡Ôñ¾ØÕó
/************************************************************************/
class CFLevelsLayer:public CFAbsLayer{
private:
    int charpter;
    int currentlevel;
    CCMenu* pMenu;
    void loadLevelScore(CFConfiguration* config, CCMenuItemSprite* item, int level);
protected:
    void onStartGame(CCObject* pSender);
    void onBackToMenu(CCObject* pSender);
    static int getCurrentLevel(int charpter);
    void onEnter();
public:
    CFLevelsLayer();
    virtual ~CFLevelsLayer();
    virtual bool init(int* num);
    LAYER_NODE_FUNC_PARAM(CFLevelsLayer,int*,num);
    static CCScene* initLevelsScene(int charpter);
};
#endif