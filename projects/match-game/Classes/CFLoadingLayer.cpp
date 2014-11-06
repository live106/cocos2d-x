#include "CFLoadingLayer.h"


CFLoadingLayer::CFLoadingLayer(void)
{
}


CFLoadingLayer::~CFLoadingLayer(void)
{
}

bool CFLoadingLayer::init(){
    if(!CCLayer::init()){
        return false;
    }
    CCSize winsize = CFGameEngine::getWinSize();
    CCSprite* pLoading = CCSprite::spriteWithFile("img/loading/loading.png");
    pLoading->setPosition(ccp(winsize.width/2,winsize.height/2));
    this->addChild(pLoading);
    CCCallFunc* pCallFunc = CCCallFunc::actionWithTarget(this, SEL_CallFunc(&CFLoadingLayer::loadGame));
    CCDelayTime* pDelayTime = CCDelayTime::actionWithDuration(1);
    CCAction* pSeq = CCSequence::actions(pDelayTime, pCallFunc, NULL);
    this->runAction(pSeq);
    return true;
}

void CFLoadingLayer::loadGame(){
    CCScene* menuScene = CFMenuLayer::initMenuScene();
    CFGameEngine::getDirector()->replaceScene(menuScene);
}

CCScene* CFLoadingLayer::initLoadingScene(){
    CCScene* mScene = CCScene::node();
    CFLoadingLayer* pLoadingLayer = CFLoadingLayer::node();
    mScene->addChild(pLoadingLayer);
    //pMenuLayer->showLoadings();
    return mScene;
}