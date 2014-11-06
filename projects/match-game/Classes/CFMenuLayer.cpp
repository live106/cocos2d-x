#include "CFMenuLayer.h"

/************************************************************************/
/* MenuLayerÊµÏÖ
/************************************************************************/

CFMenuLayer::CFMenuLayer():pMenuTree(NULL){

}

CFMenuLayer::~CFMenuLayer(){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->removeSpriteFramesFromFile("img/game/monkey.plist");
}

bool CFMenuLayer::init(){
    if ( !CFAbsLayer::init() )
    {
        return false;
    }
    pMenuTree = this->createMenuTree();
    this->addChild(pMenuTree);
    return true;
};

CCSprite* CFMenuLayer::createMenuTree(){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("img/menu/menus.plist");
    CCSpriteBatchNode* pMenusPush = CCSpriteBatchNode::batchNodeWithFile("img/menu/menus.png");
    this->addChild(pMenusPush);

    CCSprite* pMenuTree = CCSprite::spriteWithSpriteFrameName("menutree.png");
    CCSize csize = pMenuTree->getContentSize();
    int height = csize.height;
    pMenuTree->setPosition(ccp(winsize.width/2,winsize.height/2));
    CCSize treesize = pMenuTree->getContentSize();
    CCSprite* pPlay = CCSprite::spriteWithSpriteFrameName("play.png");
    CCMenuItem* pPlayItem = CCMenuItemSprite::itemFromNormalSprite(pPlay, NULL, NULL, this, TO_SEL_MENUHANDLER(CFMenuLayer::onStartMenu));
    pPlayItem->setTag(0);
    pPlayItem->setPosition(ccp(101+50,366-400));

    CCSprite* pArciv = CCSprite::spriteWithSpriteFrameName("achievement.png");
    CCMenuItem* pArcivItem = CCMenuItemSprite::itemFromNormalSprite(pArciv, NULL, NULL, this, TO_SEL_MENUHANDLER(CFMenuLayer::onStartMenu));
    pArcivItem->setTag(1);
    pArcivItem->setPosition(ccp(20+50,130-400));

    CCSprite* pCredit = CCSprite::spriteWithSpriteFrameName("credit.png");
    CCMenuItem* pCreditItem = CCMenuItemSprite::itemFromNormalSprite(pCredit, NULL, NULL, this, TO_SEL_MENUHANDLER(CFMenuLayer::onStartMenu));
    pCreditItem->setTag(2);
    pCreditItem->setPosition(ccp(185+50,190-400));

    CCSprite* pOption = CCSprite::spriteWithSpriteFrameName("option.png");
    CCMenuItem* pOptionItem = CCMenuItemSprite::itemFromNormalSprite(pOption, NULL, NULL, this, TO_SEL_MENUHANDLER(CFMenuLayer::onStartMenu));
    pOptionItem->setTag(3);
    pOptionItem->setPosition(ccp(29+50,230-400));

    CCSprite* pRank = CCSprite::spriteWithSpriteFrameName("rank.png");
    CCMenuItem* pRankItem = CCMenuItemSprite::itemFromNormalSprite(pRank, NULL, NULL, this, TO_SEL_MENUHANDLER(CFMenuLayer::onStartMenu));
    pRankItem->setTag(4);
    pRankItem->setPosition(ccp(233+50,117-400));
    
    CCSprite* pStory = CCSprite::spriteWithSpriteFrameName("story.png");
    CCMenuItem* pStoryItem = CCMenuItemSprite::itemFromNormalSprite(pStory, NULL, NULL, this, TO_SEL_MENUHANDLER(CFMenuLayer::onStartMenu));
    pStoryItem->setTag(5);
    pStoryItem->setPosition(ccp(189+50,285-400));

    CCMenu* pMenu = CCMenu::menuWithItems(pPlayItem,pArcivItem, pCreditItem, pOptionItem, pRankItem,pStoryItem, NULL);
    pMenu->setPosition(ccp(0,0));
    pMenu->setTag(0);
    pMenuTree->addChild(pMenu, 40);
    return pMenuTree;
}

void CFMenuLayer::onStartMenu(CCObject* pSender){
    this->hideMenus();
    CCCallFunc* pCallFunc = CCCallFunc::actionWithTarget(this, SEL_CallFunc(&CFMenuLayer::toCharpterScene));
    CCDelayTime* pDelayTime = CCDelayTime::actionWithDuration(1);
    CCAction* pSeq = CCSequence::actions(pDelayTime, pCallFunc, NULL);
    this->runAction(pSeq);
};

void CFMenuLayer::toCharpterScene(){
    CCScene* pCharpters = CFCharpterLayer::initScene();
    CFGameEngine::getDirector()->pushScene(pCharpters);
}

void CFMenuLayer::onCloseMenu(CCObject* pSender){
    // "close" menu item clicked
    CFGameEngine::getDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

};

CCScene* CFMenuLayer::initMenuScene(){
    CCScene* mScene = CCScene::node();
    CFMenuLayer* pMenuLayer = CFMenuLayer::node();
    mScene->addChild(pMenuLayer);
    pMenuLayer->showMenus();
    return mScene;
}

void CFMenuLayer::showMenus(){
    if(!this->pMenuTree){
        return;
    }
    CCSize winsize = CFGameEngine::getWinSize();
    CCMenu* pMenu = (CCMenu*) pMenuTree->getChildByTag(0);
    for(int i = 0; i < 6;i++){
        CCMenuItem* pItem = (CCMenuItem*)pMenu->getChildByTag(i);
        CCPoint itempos = pItem->getPosition();
        CCMoveTo* pMoveTo = CCMoveTo::actionWithDuration(0.1*i, ccp(itempos.x, itempos.y + winsize.height/2));
        pItem->runAction(pMoveTo);
    }
}

void CFMenuLayer::hideMenus(){
    if(!this->pMenuTree){
        return;
    }
    CCSize winsize = CFGameEngine::getWinSize();
    CCMenu* pMenu = (CCMenu*) pMenuTree->getChildByTag(0);
    for(int i = 0; i < 6;i++){
        CCMenuItem* pItem = (CCMenuItem*)pMenu->getChildByTag(i);
        CCPoint itempos = pItem->getPosition();
        CCMoveTo* pMoveTo = CCMoveTo::actionWithDuration(0.3*i, ccp(itempos.x, itempos.y-winsize.height/2));
        pItem->runAction(pMoveTo);
    }
}
