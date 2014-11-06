#ifndef __CFGAMEENGINE_H__
#define __CFGAMEENGINE_H__
#include "cocos2d.h"
#include "CFLuaConfig.h"
#include "Box2D.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

using namespace cocos2d;

#define TO_SEL_MENUHANDLER(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define PTM_RATIO 32

class CFGameEngine
{
public:
    // 获取单例的Director类，Director就是导演，主管游戏的整体行为
    static CCDirector* getDirector();
    static void initDirector(ccDeviceOrientation orientation, bool showFps, float animationInterval);
    static CCSize getWinSize();
    // 获取Sprite精灵类，一个场景中可以有多个精灵，例如一个马里奥就是一个精灵*/
    static CCTexture2D* newTexture(char* pszFilename);
    // 获取基本的二维坐标结构
    static CCRect newRect(float x, float y, float width, float height);
    static CCPoint newPoint(float x, float y);
    static CCSize newSize(float width, float height);

    // 为图层添加上方居中的文字说明
    static bool addTitle(CCLayer* layer, const char* text, const char* fontname, float fontsize);

    // 为图层添加菜单条目，菜单可以有两个图片表明平时状态，点击后状态
    static bool addMenu(CCLayer* layer, const char* normalpic, const char* selectpic, CCPoint position, SEL_MenuHandler handler);

    // 为图层添加精灵
    static bool addSprite(CCLayer* layer, CCSprite* sprite);
    // 启动场景
    static void runWithScene(CCScene* scene);
    // 切换场景
    static void switchScene(CCScene* from, CCScene* to);

    //脚本引擎管理
    static void setScriptEngine(CCScriptEngineProtocol* engine);
    static CCScriptEngineProtocol* getScriptEngine();
    static void executeScript(const char* src);

    //物理引擎
    static void setB2World(int width, int height);

    //获取配置文件

};

/************************************************************************/
/* TODO: 加入物理引擎支持的CCSprite
/************************************************************************/
class B2Sprite:public b2Body, public CCSprite{
    //    pSprite->setPosition(ccp(x,y));
    //    CFGameEngine::addSprite(this, pSprite);
    //    b2BodyDef matchBodyDef;
    //    matchBodyDef.type = b2_dynamicBody;
    //    matchBodyDef.position.Set(x/PTM_RATIO,y/PTM_RATIO);
    //    matchBodyDef.userData = pSprite;
    //    b2Body* pMatchBody = pWorld->CreateBody(&matchBodyDef);
    //    b2PolygonShape matchShape;
    //    matchShape.SetAsBox(pSprite->getContentSize().width/PTM_RATIO/2, pSprite->getContentSize().width/PTM_RATIO/2);
    //    b2FixtureDef matchFixtureDef;
    //    matchFixtureDef.shape = &matchShape;
    //    matchFixtureDef.density = 10.0f;
    //    matchFixtureDef.friction = 0.4f;
    //    matchFixtureDef.restitution = 0.1f;
    //    pMatchBody->CreateFixture(&matchFixtureDef);
    //    return pSprite;
};
#endif // endif