/************************************************************************/
/*CFGameEngine ClassFoo's Game Engine 游戏引擎类，提供获取各种实例的static方法
/************************************************************************/
#include "CFGameEngine.h"

void CFGameEngine::initDirector(ccDeviceOrientation orientation, bool showFps, float animationInterval){
    do{
        CCDirector *pDirector = getDirector();
        CC_BREAK_IF(!pDirector);
        pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());
        // sets landscape mode
        pDirector->setDeviceOrientation(orientation);
        // turn on display FPS
        pDirector->setDisplayFPS(showFps);
        // set FPS. the default value is 1.0/60 if you don't call this
        pDirector->setAnimationInterval(animationInterval);
        pDirector->setProjection(CCDirectorProjection2D);
    }while(0);
}

void CFGameEngine::runWithScene(CCScene* scene){
    do{
        CCDirector* pDirector = getDirector();
        CC_BREAK_IF(!pDirector);
        pDirector->runWithScene(scene);
    }while(0);
}

CCDirector* CFGameEngine::getDirector(){
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->retain();
    return pDirector;
}

CCSize CFGameEngine::getWinSize(){
    CCSize winsize = getDirector()->getWinSize();
    return winsize;
}

CCTexture2D* CFGameEngine::newTexture(char* pszFilename){
    assert(pszFilename != NULL);
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
    return pTexture;
}

CCRect CFGameEngine::newRect(float x, float y, float width, float height){
    return CCRectMake(x, y, width, height);
}

CCPoint CFGameEngine::newPoint(float x, float y){
    return CCPointMake(x,y);
}

CCSize CFGameEngine::newSize(float width, float height){
    return CCSizeMake(width, height);
}

bool CFGameEngine::addTitle(CCLayer* layer, const char* text, const char* fontname, float fontsize){
    // Create a label and initialize with string "Hello World".
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString(text, fontname, fontsize);
    if(! pLabel){
        return false;
    }
    // Get window size and place the label upper. 
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    pLabel->setPosition(ccp(size.width / 2, size.height - 20));

    // Add the label to HelloWorld layer as a child layer.
    layer->addChild(pLabel, 1);
    return true;
}

bool CFGameEngine::addMenu(CCLayer* layer, const char* normalpic, const char* selectpic, CCPoint pos, SEL_MenuHandler handler){
    // 1. Add a menu item with "X" image, which is clicked to quit the program.
    // Create a "close" menu item with close icon, it's an auto release object.
    CCMenuItemImage *pStartItem = CCMenuItemImage::itemFromNormalImage(
        normalpic, 
        selectpic,
        layer,
        handler);
    if(!pStartItem){
        return false;
    }
    pStartItem->setPosition(ccp(0, 0));
    // Create a menu with the "close" menu item, it's an auto release object.
    CCMenu* pMenuStart = CCMenu::menuWithItems(pStartItem, NULL);
    pMenuStart->setPosition(CCPointMake(pos.x, pos.y));
    if(! pMenuStart){
        return false;
    }

    // Add the menu to HelloWorld layer as a child layer.
    layer->addChild(pMenuStart, 1);
    return true;
}

bool CFGameEngine::addSprite(CCLayer* layer, CCSprite* sprite){
    layer->addChild(sprite);
    return true;
}

CCScriptEngineProtocol* CFGameEngine::getScriptEngine(){
  return CCScriptEngineManager::sharedScriptEngineManager()->getScriptEngine();
}

void CFGameEngine::setScriptEngine(CCScriptEngineProtocol* engine){
  CCScriptEngineManager::sharedScriptEngineManager()->setScriptEngine(engine);
}

void CFGameEngine::executeScript(const char* src){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    unsigned long size;
    char *pFileContent = (char*)CCFileUtils::getFileData(src, "r", &size);

    if (pFileContent)
    {
        // copy the file contents and add '\0' at the end, or the lua parser can not parse it
        char *pCodes = new char[size + 1];
        pCodes[size] = '\0';
        memcpy(pCodes, pFileContent, size);
        delete[] pFileContent;
        CFGameEngine::getScriptEngine()->executeString(pCodes);
        delete []pCodes;
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    string path = CCFileUtils::fullPathFromRelativePath(src);
    CFGameEngine::getScriptEngine()->executeScriptFile(path.c_str());
#endif 
}
/**
void CFGameEngine::setB2World(int width, int height){
    //初始化屏幕显示区域为Box2d的物理背景区域
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    bool doSleep = false;
    b2World* pWorld = new b2World(gravity, doSleep);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0,0);
    b2Body* pGroundBody = pWorld->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    b2FixtureDef groundBoxDef;
    groundBoxDef.shape = &groundBox;
    //groundBox.SetAsEdge(b2Vec2(0,0), b2Vec2(width/PTM_RATIO, 0));
    b2Fixture* pBottomFixture = pGroundBody->CreateFixture(&groundBoxDef);
    //groundBox.SetAsEdge(b2Vec2(0, 0), b2Vec2(0, height/PTM_RATIO));
    b2Fixture* pLeftFixture = pGroundBody->CreateFixture(&groundBoxDef);
    //groundBox.SetAsEdge(b2Vec2(width/PTM_RATIO, 0), b2Vec2(width/PTM_RATIO, height/PTM_RATIO));
    b2Fixture* pRightFixture = pGroundBody->CreateFixture(&groundBoxDef);
    //groundBox.SetAsEdge(b2Vec2(0, height/PTM_RATIO), b2Vec2(width/PTM_RATIO, height/PTM_RATIO));
    b2Fixture* pTopFixture = pGroundBody->CreateFixture(&groundBoxDef);
}
*/


