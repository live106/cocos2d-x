#include "CFLevelsLayer.h"

/************************************************************************/
/* LevelsLayer实现
/************************************************************************/
CFLevelsLayer::CFLevelsLayer():charpter(0),currentlevel(-1),pMenu(NULL){
}

CFLevelsLayer::~CFLevelsLayer(){

}

bool CFLevelsLayer::init(int num[]){
    if (!CFAbsLayer::init())
    {
        return false;
    }
    this->currentlevel = num[0];
    this->charpter = num[1];
    this->pMenu = CCMenu::menuWithItems(NULL);
    pMenu->setPosition(CCPointZero);
    CFConfiguration* config = new CFConfiguration();
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            int level = (3-j)*5 - (5-i) + 1;
            CCSprite* pNormal = level>currentlevel?CCSprite::spriteWithFile("img/level/level-lock.png"):CCSprite::spriteWithFile("img/level/level.png");
            CCSprite* pSelected = CCSprite::spriteWithFile("img/level/level-lock.png");
            CCMenuItemSprite *item = CCMenuItemSprite::itemFromNormalSprite(pNormal, pSelected,NULL,this,
                TO_SEL_MENUHANDLER(CFLevelsLayer::onStartGame));
            item->setPosition(ccp(80+i*170 + 100, 90+j*190 + 100));
            item->setTag(level);
            char* levelstr = new char[4];
            sprintf(levelstr, "%d", level);
            CCLabelBMFont* pLabel = CCLabelBMFont::labelWithString(levelstr, "fonts/fonts.fnt");
            item->addChild(pLabel);
            pMenu->addChild(item, 2);
            delete[] levelstr;
            if(level>currentlevel){
                continue;
            }
            this->loadLevelScore(config, item, level);
        }
    }
    delete config;
    CCMenuItemImage *item = CCMenuItemImage::itemFromNormalImage("img/level/back.png", "img/level/back.png",this,
        TO_SEL_MENUHANDLER(CFLevelsLayer::onBackToMenu));
    item->setPosition(ccp(600,60));
    pMenu->addChild(item, 0);
    this->addChild(pMenu, 40);
    this->setIsTouchEnabled(true);
    CCParticleFire* m_emitter = CCParticleFire::node();
    m_emitter->retain();
    m_emitter->setPosition(ccp(500, 0));
    m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("texture/fire.png") );
    this->addChild(m_emitter, 40);
    return true;
}

void CFLevelsLayer::loadLevelScore(CFConfiguration* config, CCMenuItemSprite* item, int level){
    char* scorekey = new char[30];
    sprintf(scorekey, "charpter_%d_level_%d_score", this->charpter,level);
    int score = config->getInt(scorekey, 0);
    delete[] scorekey;
    int num = score/20;
    CCSize itemsize = item->getContentSize();
    for(int i = 0;i < 5; i++){
        CCSprite* pSlot = CCSprite::spriteWithFile("img/level/small_star_slot.png");
        CCSize slotsize = pSlot->getContentSize();
        pSlot->setPosition(ccp(itemsize.width/2-2.5*slotsize.width+i*slotsize.width, itemsize.height/2));
        pSlot->setTag(i);
        item->removeChildByTag(i, true);//先清除老的slot
        item->addChild(pSlot);
        if(i<num){
            CCSprite* pStar = CCSprite::spriteWithFile("img/level/small_star.png");
            pStar->setTag(5+i);
            pStar->setPosition(ccp(itemsize.width/2-2.5*slotsize.width+i*slotsize.width, itemsize.height/2));
            item->removeChildByTag(5+i,true);//先清除老的star
            item->addChild(pStar);
        }
    }
}

void CFLevelsLayer::onEnter(){
   CCLayer::onEnter();
   int newlevel = CFLevelsLayer::getCurrentLevel(charpter);
   //每次重新载入关卡选择器时，需要将所有关卡的分数检查一次，绘制成slot和star到界面中
   CFConfiguration* config = new CFConfiguration();
   for(int i = 1; i<= newlevel;i++){
       CCMenuItemSprite* item = (CCMenuItemSprite*)this->pMenu->getChildByTag(i);
       if(!item){
           continue;
       }
       CCNode* img = item->getNormalImage();
       img->removeAllChildrenWithCleanup(true);
       CCSprite* pNormal = CCSprite::spriteWithFile("img/level/level.png");
       item->setNormalImage(pNormal);
       this->loadLevelScore(config, item, i);
   }
   delete config;
   this->currentlevel = newlevel;
}

void CFLevelsLayer::onStartGame(CCObject* pSender){
    CCMenuItemFont* item = (CCMenuItemFont*) pSender;
    int level = item->getTag();
    int currentlvl = CFLevelsLayer::getCurrentLevel(charpter);
    if(level <= currentlvl){
        CCScene* scene = CFGameLayer::initGameScene(charpter, level);
        CFGameEngine::getDirector()->pushScene(scene);
    }
};

int CFLevelsLayer::getCurrentLevel(int charpter){
    CFConfiguration* config = new CFConfiguration();
    char* buffer = new char[20];
    sprintf(buffer, "charpter_%d_progress", charpter);
    int currentlvl = config->getInt(buffer, -1);
    if(currentlvl == -1){
        config->setInt(buffer, 1);
        config->writeToFile();
        currentlvl = 1;
    }
    delete config;
    delete[] buffer;
    return currentlvl;
}


void CFLevelsLayer::onBackToMenu(CCObject* pSender){
    CFGameEngine::getDirector()->popScene();
}

CCScene* CFLevelsLayer::initLevelsScene(int charpter){
    CCScene* mScene = CCScene::node();
    int currentlvl = CFLevelsLayer::getCurrentLevel(charpter);
    int* num = new int[2];
    num[0] = currentlvl;
    num[1] = charpter;
    CFLevelsLayer* pLevelsLayer = CFLevelsLayer::node(num);
    mScene->addChild(pLevelsLayer);
    delete[] num;
    return mScene;
};