#include "CFAbsLayer.h"

CFAbsLayer::CFAbsLayer(void):pFrameCache(NULL), pBackGround(NULL),pMountains(NULL),pWaterLine(NULL),pRightTree(NULL),pLeftTree(NULL),pStone(NULL)
{
    winsize = CFGameEngine::getWinSize();
}


CFAbsLayer::~CFAbsLayer(void)
{
}

bool CFAbsLayer::init(){
    if (!CCLayer::init())
    {
        return false;
    }
    float winwidth = winsize.width;
    float winheight = winsize.height;
    //加载stone truee
    pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("img/game/stonetree.plist");
    CCSpriteBatchNode* pStoneTree = CCSpriteBatchNode::batchNodeWithFile("img/game/stonetree.png");
    this->addChild(pStoneTree);
    //加载mountains stone
    pFrameCache->addSpriteFramesWithFile("img/gamelayer/mountain_stone.plist");
    CCSpriteBatchNode* pMountainStone = CCSpriteBatchNode::batchNodeWithFile("img/gamelayer/mountain_stone.png");
    this->addChild(pMountainStone);

    //绘制背景天空
    pBackGround = CFRadiationSprite::node(CCSizeMake(winwidth,winsize.height*3/4));
    pBackGround->setPosition(ccp(winsize.width/2,winsize.height/4));
    this->addChild(pBackGround, 0);
    pBackGround->startAnimation();

    //绘制山头
    CCSprite* pMountains = CCSprite::spriteWithSpriteFrameName("mountains.png");
    pMountains->setPosition(ccp(winsize.width/2, pMountains->getContentSize().height/2));
    this->addChild(pMountains, 5);
    //绘制水面
    pWaterLine = CFHorizonWaterLinesSprite::node(CCSizeMake(winwidth/2,winsize.height/4));
    if(pWaterLine){
        pWaterLine->setPosition(ccp(winwidth/2, winheight/8));
        pWaterLine->startAnimation();
        this->addChild(pWaterLine, 10);
    }else{
        CCLog("error on loading CFHorizonWaterLineSprite");
    }
    pLeftTree = CCSprite::spriteWithFile("img/abslayer/left_right_tree.png");
    pLeftTree->setPosition(ccp(winwidth/2,winheight/2));
    this->addChild(pLeftTree,20);
    //绘制石头
    pStone = CCSprite::spriteWithSpriteFrameName("stone.png");
    CCSize stonesize = pStone->getContentSize();
    pStone->setPosition(ccp(winwidth/2,stonesize.height/2));
    this->addChild(pStone,30);
    return true;
}
