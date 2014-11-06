#include "CFGameLayer.h"
#include "CFMatchGame.h"
#include "lib/CFConfiguration.h"
/************************************************************************/
/* 提示信息框                                                            */
/************************************************************************/
CFTipBoardSprite::CFTipBoardSprite():pLabel(NULL),pLabelBoard(NULL){
}

CFTipBoardSprite::~CFTipBoardSprite(void){
}

bool CFTipBoardSprite::init(){
    if(!CCSprite::init()){
        return false;
    }
    return true;
}

void CFTipBoardSprite::showTips(CFGameLayer* pLayer, const char* tips, float time){
    if(!pLabelBoard){
        CCSize winsize = CFGameEngine::getWinSize();
        pLabelBoard = CCSprite::spriteWithSpriteFrameName("level_title.png");
        CCSize consize = pLabelBoard->getContentSize();
        pLabelBoard->setPosition(ccp(winsize.width/2, winsize.height - consize.height/2));
        pLayer->addChild(pLabelBoard);
    }
    if(pLabel){
        pLabel->removeFromParentAndCleanup(true);
        pLabel = NULL;
    }
    pLabel = CCLabelBMFont::labelWithString(tips,"fonts/fonts.fnt");
    CCSize consize = pLabelBoard->getContentSize();
    pLabel->setPosition(ccp(consize.width/2, consize.height/2));
    this->pLabelBoard->addChild(pLabel);
}

void CFTipBoardSprite::hideTips(CFGameLayer* pLayer){
    if(this->pLabel){
        this->removeChild(this->pLabel, true);
        pLabel = NULL;
    }
}

/************************************************************************/
/* 暂停信息框
/************************************************************************/
PauseBoardSprite::PauseBoardSprite():pBoardWood(NULL){
    
}

PauseBoardSprite::~PauseBoardSprite(){

}

bool PauseBoardSprite::init(){
    if(!CFUnTouchableSprite::init()){
        return false;
    }
    //if(this->initWithTexture(rt->getSprite()->getTexture())){
    //    return true;
    //}
    return true;
}

bool PauseBoardSprite::initBoardWood(CFGameLayer* pLayer){
    CCSize winsize = CFGameEngine::getWinSize();
    CCRenderTexture *rt = CCRenderTexture::renderTextureWithWidthAndHeight(winsize.width,winsize.height);
    ccColor4B color = {0,0, 0, 200};
    ccColor4F fcolor = ccc4FFromccc4B(color);
    rt->beginWithClear(fcolor.r,fcolor.g,fcolor.b,fcolor.a);
    rt->end();
    this->initWithTexture(rt->getSprite()->getTexture());

    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("img/game/pauseboard.plist");
    CCSpriteBatchNode* pSpriteSheet = CCSpriteBatchNode::batchNodeWithFile("img/game/pauseboard.png");
    this->addChild(pSpriteSheet);
    
    pBoardWood = CCSprite::spriteWithSpriteFrameName("pause_board.png");
    pBoardWood->setPosition(ccp(winsize.width/2,winsize.height/2));
    //继续游戏菜单
    CCMenuItemSprite *pContinue = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("resume_normal.png"),CCSprite::spriteWithSpriteFrameName("resume_selected.png"),pLayer, TO_SEL_MENUHANDLER(CFGameLayer::onResumeMenu));
    pContinue->setPosition(ccp(200,200));
    //重置游戏菜单
    CCMenuItemSprite* pRetry = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("retry_normal.png"), CCSprite::spriteWithSpriteFrameName("retry_selected.png"), pLayer, TO_SEL_MENUHANDLER(CFGameLayer::onResetMenu));
    pRetry->setPosition(ccp(300, 200));
    //返回关卡菜单
    CCMenuItemSprite* pBackToLevels = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("home_normal.png"), CCSprite::spriteWithSpriteFrameName("home_selected.png"), pLayer, TO_SEL_MENUHANDLER(CFGameLayer::onBackToLevelsMenu));
    pBackToLevels->setPosition(ccp(400,200));
    //静音菜单
    CCMenuItemSprite* pMute = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("mute_normal.png"), CCSprite::spriteWithSpriteFrameName("mute_selected.png"));
    pMute->setTag(0);
    CCMenuItemSprite* pUnMute = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("unmute_normal.png"), CCSprite::spriteWithSpriteFrameName("unmute_selected.png"));
    pUnMute->setTag(1);
    CCMenuItemToggle* pSound = CCMenuItemToggle::itemWithTarget(pLayer, TO_SEL_MENUHANDLER(CFGameLayer::onMuteSound), pMute, pUnMute, NULL);
    pSound->setPosition(ccp(300,100));
    if(CFMatchGame::isSoundMuted()){
        pSound->setSelectedIndex(0);
    }
    CCMenu* pMenu = CCMenu::menuWithItems(pContinue, pRetry, pBackToLevels, pSound, NULL);
    pMenu->setPosition(ccp(0,0));
    pBoardWood->addChild(pMenu);
    this->addChild(pBoardWood, 20);
    return true;
}

bool PauseBoardSprite::doPause(CFGameLayer* pLayer){
    if(!this->pBoardWood){
        this->initBoardWood(pLayer);
    }
    CCSize winsize = CFGameEngine::getWinSize();
    CCAction* sequence1 = CCSequence::actions(CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height/2)),CCFadeIn::actionWithDuration(1),  NULL);
    this->runAction(sequence1);
    CCAction* sequence2 = CCSequence::actions(CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height*2)),CCMoveTo::actionWithDuration(0.5, ccp(winsize.width/2, winsize.height/2)), NULL);
    pBoardWood->runAction(sequence2);
    return true;
}

bool PauseBoardSprite::doResume(CFGameLayer* pLayer){
    if(!this->pBoardWood){
        this->initBoardWood(pLayer);
    }
    CCSize winsize = CFGameEngine::getWinSize();
    CCAction* sequence2 = CCSequence::actions(CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height/2)),CCMoveTo::actionWithDuration(0.5, ccp(winsize.width/2, winsize.height*2)),NULL);
    pBoardWood->runAction(sequence2);
    CCAction* sequence1 = CCSequence::actions(CCFadeOut::actionWithDuration(1),CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height*2)),  NULL);
    this->runAction(sequence1);
    return true;
}

/************************************************************************/
/* 通关信息框
/************************************************************************/

SuccessBoardSprite::SuccessBoardSprite():pBoardStone(NULL){

}

SuccessBoardSprite::~SuccessBoardSprite(){

}

bool SuccessBoardSprite::init(){
    if(!CFUnTouchableSprite::init()){
        return false;
    }
    CCSize winsize = CFGameEngine::getWinSize();
    return true;
}

bool SuccessBoardSprite::initBoardStone(CFGameLayer* pLayer){
    CCSize winsize = CFGameEngine::getWinSize();
    CCRenderTexture *rt = CCRenderTexture::renderTextureWithWidthAndHeight(winsize.width,winsize.height);
    ccColor4B color = {0,0, 0, 100};
    ccColor4F fcolor = ccc4FFromccc4B(color);
    rt->beginWithClear(fcolor.r,fcolor.g,fcolor.b,fcolor.a);
    rt->end();
    this->initWithTexture(rt->getSprite()->getTexture());


    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("img/game/successboard.plist");
    CCSpriteBatchNode* pSpriteSheet = CCSpriteBatchNode::batchNodeWithFile("img/game/successboard.png");
    this->addChild(pSpriteSheet);

    pBoardStone = CCSprite::spriteWithSpriteFrameName("success_board.png");
    pBoardStone->setPosition(ccp(winsize.width/2,winsize.height/2));
    this->addChild(pBoardStone, 20);

    CCMenuItemSprite* pNext = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("next_normal.png"), CCSprite::spriteWithSpriteFrameName("next_selected.png"), pLayer, TO_SEL_MENUHANDLER(CFGameLayer::onNextMenu));
    pNext->setPosition(ccp(-winsize.width/4, -winsize.height/4));
    CCMenuItemSprite* pBackToLevels = CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("home_normal.png"), CCSprite::spriteWithSpriteFrameName("home_selected.png"), pLayer, TO_SEL_MENUHANDLER(CFGameLayer::onBackToLevelsMenu));
    pBackToLevels->setPosition(ccp(winsize.width/4, -winsize.height/4));
    CCMenu* pMenu = CCMenu::menuWithItems(pNext, pBackToLevels, NULL);
    pMenu->setPosition(ccp(winsize.width/2, winsize.height/2));
    this->addChild(pMenu, 30);

    CCSize stonesize = this->pBoardStone->getContentSize();
    for(int i = 0;i < 5;i++){
        CCSprite* pSlot = CCSprite::spriteWithSpriteFrameName("star_slot.png");
        pSlot->setPosition(ccp(100+i*50, stonesize.height/2));
        this->pBoardStone->addChild(pSlot,30);
        CCSprite* pStar = CCSprite::spriteWithSpriteFrameName("star.png");
        pStar->setPosition(ccp(100+i*50, stonesize.height/2));
        pStar->setTag(i);
        pStar->setIsVisible(false);
        this->pBoardStone->addChild(pStar,30);
    }
    return true;
}


bool SuccessBoardSprite::doShow(CFGameLayer* pLayer, int score){
    if(!this->pBoardStone&&!this->initBoardStone(pLayer)){
        throw exception("can not init Success Board!");
    }
    CCSize winsize = CFGameEngine::getWinSize();
    CCAction* sequence = CCSequence::actions(CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height/2)),CCFadeIn::actionWithDuration(1),  NULL);
    this->runAction(sequence);
    int starnum = score/20;
    for(int i = 0;i < starnum;i++){
        CCNode* star = this->pBoardStone->getChildByTag(i);
        if(!star){
            continue;
        }
        star->setIsVisible(true);
    }
    for(int i = starnum;i<5;i++){
        CCNode* star = this->pBoardStone->getChildByTag(i);
        if(!star){
            continue;
        }
        star->setIsVisible(false);
    }
    //CCFiniteTimeAction* pShaky = CCShaky3D::actionWithRange(1, true, ccg(100,100), 1);
    //CCFiniteTimeAction* pRev= pShaky->reverse();
    //pLayer->runAction(CCSequence::actions(pShaky, pRev, NULL));
    return false;
}

bool SuccessBoardSprite::doHide(CFGameLayer* pLayer){
    if(!this->pBoardStone&&!this->initBoardStone(pLayer)){
        throw exception("can not init Success Board!");
    }
    CCSize winsize = CFGameEngine::getWinSize();
    
    CCAction* sequence = CCSequence::actions(CCPlace::actionWithPosition(ccp(winsize.width/2, winsize.height*2)),CCFadeIn::actionWithDuration(1),  NULL);
    this->runAction(sequence);
    return true;
}

/************************************************************************/
/* GameLayer实现                                                        */
/************************************************************************/

CFGameLayer::CFGameLayer():pLevelLoader(NULL), pPause(NULL), pPauseBoard(NULL),pTimer(NULL),score(100){

};

CFGameLayer::~CFGameLayer(){
	if(pLevelLoader){
	    delete pLevelLoader;
		pLevelLoader = NULL;
	}
};

bool CFGameLayer::init(){
    if (!CFAbsLayer::init())
    {
        return false;
    }
    float winwidth = winsize.width;
    float winheight = winsize.height;
    //加载button title
    pFrameCache->addSpriteFramesWithFile("img/gamelayer/button_title.plist");
    CCSpriteBatchNode* pButtonTitle = CCSpriteBatchNode::batchNodeWithFile("img/gamelayer/button_title.png");
    this->addChild(pButtonTitle);

    //顶部小树
    CCSprite* pLeftTree = CCSprite::spriteWithSpriteFrameName("lefttree.png");
    CCSize ltreeSize = pLeftTree->getContentSize();
    pLeftTree->setPosition(ccp(ltreeSize.width/2, winheight - ltreeSize.height/2));
    this->addChild(pLeftTree, 20);

    //暂停按钮
    CCSprite* pUnselected = CCSprite::spriteWithSpriteFrameName("pause_normal.png");
    CCSprite* pSelected = CCSprite::spriteWithSpriteFrameName("pause_selected.png");
    this->pPause = CCMenuItemSprite::itemFromNormalSprite(pUnselected, pSelected);
    CCMenu* pMenu = CCMenu::menuWithItem(pPause);
    pMenu->setPosition(ccp(0,0));
    CCSize pauseSize = pPause->getContentSize();
    pPause->setPosition(ccp(ltreeSize.width/2, winheight - ltreeSize.height/2 - 20));
    pPause->initWithTarget(this, TO_SEL_MENUHANDLER(CFGameLayer::onPauseMenu));
    this->addChild(pMenu, 20);

    //暂停面板
    this->pPauseBoard = PauseBoardSprite::node();
    this->pPauseBoard->setPosition(ccp(winwidth*2,winheight*2));
    this->addChild(this->pPauseBoard, 40);

    //成功面板
    this->pSuccessBoard = SuccessBoardSprite::node();
    this->pSuccessBoard->setPosition(ccp(winwidth*2, winheight*2));
    this->addChild(this->pSuccessBoard, 40);

    //提示信息框
    this->pTips = CFTipBoardSprite::node();
    this->pTips->setPosition(ccp(winwidth/2, winheight-pTips->getContentSize().height));
    this->addChild(this->pTips,40);

    //分数栏
    this->pScoreBoard = CFScoreBoard::node(10000);
    CCSize sbsize = this->pScoreBoard->getContentSize();
    this->pScoreBoard->setPosition(ccp(winwidth-sbsize.width-50, winheight-sbsize.height-50));
    this->addChild(this->pScoreBoard, 40);

    this->scheduleUpdate();
    return true;
};

void CFGameLayer::showTips(const char* tips, float during){
    this->pTips->showTips(this, tips, during);
}

void CFGameLayer::hideTips(){
    this->pTips->hideTips(this);
}


void CFGameLayer::update(ccTime dt){
    //if ( this->numberOfRunningActions() == 0 && this->getGrid() != NULL)
    //    this->setGrid(NULL);
};

CCScene* CFGameLayer::initGameScene(int charpter, int level){
    CCScene* pScene = CCScene::node();
    CFGameLayer* pGameLayer = CFGameLayer::node();
    pGameLayer->loadLevel(charpter, level);
    pScene->addChild(pGameLayer);
    return pScene;
}

void CFGameLayer::loadLevel(int charpter, int level){
    this->charpter = charpter;
    this->level = level;
    char charpterstr[3];
    sprintf(charpterstr, "%d", charpter);
    char levelstr[3];
    sprintf(levelstr, "%d", level);
    string luafile;
    luafile.append("lua/charpter").append(charpterstr).append("/level").append(levelstr).append(".lua");
    this->levelfile = luafile;
    if(!this->pLevelLoader){
        this->pLevelLoader = new CFLevelLoader(this);
    }
    this->pLevelLoader->loadLevel(this->levelfile.c_str());
    CFLevel* pLevel = this->pLevelLoader->getCurrentLevel();
    if(pLevel){
        const char* levelname = pLevel->getLevelName();
        this->showTips(levelname, 100);
    }else{
        this->showTips("error loading level!", 100);
    }

};

//回到游戏，将暂停面板隐藏
void CFGameLayer::onResumeMenu(CCObject* pSender){
    this->pPauseBoard->doResume(this);
}


//暂停游戏，显示暂停面板
void CFGameLayer::onPauseMenu(CCObject* pSender){
    this->pPauseBoard->doPause(this);
}


void CFGameLayer::onBackToLevelsMenu(CCObject* pSender){
    CFGameEngine::getDirector()->popScene();
}


void CFGameLayer::onReturnMenu(CCObject* pSender){
    CFGameEngine::getDirector()->popScene();
};

void CFGameLayer::onNextMenu(CCObject* pSender){
    CCLog("next");
	this->loadLevel(charpter, level + 1);
	this->pSuccessBoard->doHide(this);
};

void CFGameLayer::onSubmitMenu(CCObject* pSender){
    if(this->pLevelLoader->getCurrentLevel()->checkSuccess()){
        CCLog("Game Passed");
    }else{
        CCLog("Game Failed");
    }
};

void CFGameLayer::onDelMenu(CCObject* pSender){

};

void CFGameLayer::onRuleMenu(CCObject* pSender){

};

void CFGameLayer::onResetMenu(CCObject* pSender){
    this->pLevelLoader->getCurrentLevel()->resetSteps();
};

void CFGameLayer::onForwardMenu(CCObject* pSender){
    this->pLevelLoader->getCurrentLevel()->redoStep();
}

void CFGameLayer::onBackwardMenu(CCObject* pSender){
    this->pLevelLoader->getCurrentLevel()->undoStep();
}

void CFGameLayer::onMuteSound(CCObject* pSender){
    if(CFMatchGame::isSoundMuted){
        CFMatchGame::unMuteSound();
    }else{
        CFMatchGame::muteSound();
    }
}

void CFGameLayer::onGameSuccess(){
    CFConfiguration* config = new CFConfiguration();
    char* progkey = new char[25];
    sprintf(progkey, "charpter_%d_progress", this->charpter);
    char* scorekey = new char[30];
    sprintf(scorekey, "charpter_%d_level_%d_score", this->charpter, this->level);
    int progress = config->getInt(progkey, -1);
    if(this->level<progress){
        config->setInt(scorekey, this->score);
    }else{
        config->setInt(progkey, level+1);
        config->setInt(scorekey, this->score);
    }
    config->writeToFile();
    delete[] progkey;
    delete[] scorekey;
    delete config;
    //显示成功积分牌
    if(!this->pSuccessBoard){
        this->pSuccessBoard = SuccessBoardSprite::node();
    }
    this->pSuccessBoard->doShow(this, this->score);
}

void CFGameLayer::onGameFailed(){
    CCSize winsize = CFGameEngine::getWinSize();
    CCLabelBMFont* pLabel = CCLabelBMFont::labelWithString("Wrong Move!", "fonts/fonts.fnt");
    pLabel->setPosition(ccp(winsize.width/2, winsize.height/2-100));
    this->addChild(pLabel);
    this->pBackGround->startWarning(0.3);
    CCCallFunc* pCallFunc = CCCallFunc::actionWithTarget(this->pLevelLoader->getCurrentLevel(), SEL_CallFunc(&CFLevel::undoStep));
    CCDelayTime* pDelayTime = CCDelayTime::actionWithDuration(0.3);
    CCAction* pSeq = CCSequence::actions(pDelayTime, pCallFunc, NULL);
    this->runAction(pSeq);
    if(this->score>20){
        this->score = this->score-10;
    }
}

CCRect CFGameLayer::getMatchRect(CCSprite* match){
    float posx = match->getPosition().x;
    float posy = match->getPosition().y;
    float width = match->getContentSize().width;
    float height = match->getContentSize().height;
    float offsetx = posx - width/2;
    float offsety = posy - height/2;
    CCRect matchRect = CCRectMake(offsetx, offsety, width, height);
    return matchRect;
}