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
    // ��ȡ������Director�࣬Director���ǵ��ݣ�������Ϸ��������Ϊ
    static CCDirector* getDirector();
    static void initDirector(ccDeviceOrientation orientation, bool showFps, float animationInterval);
    static CCSize getWinSize();
    // ��ȡSprite�����࣬һ�������п����ж�����飬����һ������¾���һ������*/
    static CCTexture2D* newTexture(char* pszFilename);
    // ��ȡ�����Ķ�ά����ṹ
    static CCRect newRect(float x, float y, float width, float height);
    static CCPoint newPoint(float x, float y);
    static CCSize newSize(float width, float height);

    // Ϊͼ������Ϸ����е�����˵��
    static bool addTitle(CCLayer* layer, const char* text, const char* fontname, float fontsize);

    // Ϊͼ����Ӳ˵���Ŀ���˵�����������ͼƬ����ƽʱ״̬�������״̬
    static bool addMenu(CCLayer* layer, const char* normalpic, const char* selectpic, CCPoint position, SEL_MenuHandler handler);

    // Ϊͼ����Ӿ���
    static bool addSprite(CCLayer* layer, CCSprite* sprite);
    // ��������
    static void runWithScene(CCScene* scene);
    // �л�����
    static void switchScene(CCScene* from, CCScene* to);

    //�ű��������
    static void setScriptEngine(CCScriptEngineProtocol* engine);
    static CCScriptEngineProtocol* getScriptEngine();
    static void executeScript(const char* src);

    //��������
    static void setB2World(int width, int height);

    //��ȡ�����ļ�

};

/************************************************************************/
/* TODO: ������������֧�ֵ�CCSprite
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