#include "CFCharpterLayer.h"
#include "lib/CFConfiguration.h"
/**
* �½�ѡ�񳡾�����
*/

CFCharpterLayer::CFCharpterLayer(void):charpters(), charptercount(0), beginex(0),currentx(0),winwidth(0), winheight(0)
{
    CCSize size = CFGameEngine::getWinSize();
    this->winwidth = size.width;
    this->winheight = size.height;
    originpos[0] = ccp(this->winwidth/2-800, this->winheight/2);
    originpos[1] = ccp(this->winwidth/2, this->winheight/2);
    originpos[2] = ccp(this->winwidth/2+800, this->winheight/2);
    //����charpter.plist
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("img/charpter/charpter.plist");
    CCSpriteBatchNode* pCharpterList = CCSpriteBatchNode::batchNodeWithFile("img/charpter/charpter.png");
    this->addChild(pCharpterList);
    CCSprite* pBack = CCSprite::spriteWithSpriteFrameName("back_menu.png");
    CCSprite* pBackSel = CCSprite::spriteWithSpriteFrameName("back_menu_selected.png");
    CCMenuItemSprite* pItem = CCMenuItemSprite::itemFromNormalSprite(pBack,pBackSel,NULL,this, SEL_MenuHandler(&CFCharpterLayer::onBackToMenu));
    CCMenu* pMenu = CCMenu::menuWithItem(pItem);
    pMenu->setPosition(ccp(170,140));
    this->addChild(pMenu, 40);
}


CFCharpterLayer::~CFCharpterLayer(void)
{
    if(charpters){
        for(int i = 0; i < 3;i++){
            CCSprite* charpter = charpters[i];
            if(charpter){
                charpter->removeFromParentAndCleanup(true);
                charpters[i] = NULL;
            }
        }

    }
}

bool CFCharpterLayer::init(){
    if(!CFAbsLayer::init()){
        return false;
    }
    CFConfiguration* pConfig = new CFConfiguration();
    int current = pConfig->getInt("current_charpter", -1);
    if(current == -1){
        pConfig->setInt("current_charpter", 1);
        pConfig->writeToFile();
        current = 1;
    }
    delete pConfig;
    this->charptercount = 6;
    this->initCharpters(current);
    return true;
}

void CFCharpterLayer::onEnter()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    CCLayer::onEnter();
}

void CFCharpterLayer::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCLayer::onExit();
    CCSprite* charpter = charpters[1];
    if(!charpter){
        return;
    }
    int current = charpter->getTag();
    CFConfiguration* config = new CFConfiguration();
    int num = config->getInt("current_charpter", -1);
    if(num != current){
        config->setInt("current_charpter", current);
        config->writeToFile();
    }
    delete config;


}

void CFCharpterLayer::onBackToMenu(CCObject* sender){
    CFGameEngine::getDirector()->popScene();
}

CCScene* CFCharpterLayer::initScene(){
    CCScene* pScene = CCScene::node();
    CFCharpterLayer* pCharpter = CFCharpterLayer::node();
    pScene->addChild(pCharpter, 0);
    return pScene;
}

const char* getCharpterFilePath(int charpter, const char* filename){
    char* str = new char[255];
    sprintf(str, "%d", charpter);
    string imgpath = string("lua/charpter") +str + string("/") + filename;
    return imgpath.c_str();
}

void CFCharpterLayer::initCharpters(int current){
    if(current <= 0){
        throw exception("Charpter index can not less than zero!");
    }
    int left = current - 1;
    if(left != 0){
        charpters[0] = this->createCharpterSprite(left);
        charpters[0]->setPosition(originpos[0]);
        this->addChild(charpters[0],15);
    }
    charpters[1] = this->createCharpterSprite(current);
    charpters[1]->setPosition(originpos[1]);
    this->addChild(charpters[1], 15);
    int right = current + 1;
    if(current + 1 < charptercount){
        charpters[2] = this->createCharpterSprite(right);
        charpters[2]->setPosition(originpos[2]);
        this->addChild(charpters[2],15);
    }
    //���ùؿ�λ��
    this->resetCharpterPosition();
}

CCSprite* CFCharpterLayer::createCharpterSprite(int index){
    CCSprite* pContainer = CCSprite::spriteWithSpriteFrameName("boat.png");
    pContainer->setTag(index);
    //�ӹؿ��½�Ŀ¼������ؿ���ͼ��
    char str1[255];
    sprintf(str1, "%d", index);
    string logo_normal = string("lua/charpter") +str1 + "/charpter_normal.png";
    CCSprite* pNormalSprite = CCSprite::spriteWithFile(logo_normal.c_str());
    pNormalSprite->setScale(0.3);
    CCSize size = pContainer->getContentSize();
    pNormalSprite->setPosition(ccp(size.width/2,size.height/2));
    pContainer->addChild(pNormalSprite);
    return pContainer;
}

void CFCharpterLayer::deleteCharpter(int index){
    CCSprite* item = (CCSprite*) this->getChildByTag(index);
}

bool CFCharpterLayer::ccTouchBegan(CCTouch* touch, CCEvent* event){
    CCPoint position = convertTouchToNodeSpaceAR(touch);
    beginex = position.x;
    currentx = position.x;
    return true;
}

void CFCharpterLayer::ccTouchMoved(CCTouch* touch, CCEvent* event){
    CCPoint position = convertTouchToNodeSpaceAR(touch);
    int offset = position.x - currentx;
    currentx = position.x;
    if(this->charpters){
        for(int i = 0; i <3;i++){
            CCSprite* charpter = charpters[i];
            if(!charpter){
                continue;
            }
            CCPoint cpos = charpter->getPosition();
            charpter->setPosition(ccp(cpos.x + offset, cpos.y));
        }
    }
}

void CFCharpterLayer::ccTouchEnded(CCTouch* touch, CCEvent* event){
    CCSprite* charpter = charpters[1];
    if(!charpter){
        return;
    }
    //��鵱ǰ����¼��Ƿ�Ϊû���ƶ�λ��
    CCPoint touchpos = convertTouchToNodeSpaceAR(touch);
    if(touchpos.x == beginex){
        CCSize size = charpter->getContentSize();
        CCRect rect = CCRect(touchpos.x,touchpos.y,size.width,size.height);
        if(CCRect::CCRectContainsPoint(rect, touchpos)){
            this->onSelectCharpter(charpter);
            return;
        }
        return;
    }

    //ͨ��charpter��λ�����ж��Ƿ���Ҫ�仯��ǰ��ʾ�½�
    CCPoint pos = charpter->getPosition();
    int current = charpter->getTag();
    if( pos.x> winwidth*3/4 && current > 1){
        this->toPreviousCharpter();
    }else if(pos.x < winwidth/4 && current < charptercount){
        this->toNextCharpter();
    }else{
        this->toCurrentCharpter();
    }
}

void CFCharpterLayer::ccTouchCancelled(CCTouch *touch, CCEvent *event){
    ccTouchEnded(touch, event);
}

void CFCharpterLayer::onSelectCharpter(CCObject* pSender){
    CCMenuItem* pItem = (CCMenuItem*) pSender;
    int charpter = pItem->getTag();
    CCScene* scene = CFLevelsLayer::initLevelsScene(charpter);
    CFGameEngine::getDirector()->pushScene(scene);
}

void CFCharpterLayer::toCurrentCharpter(){
    //���ùؿ�λ��
    this->resetCharpterPosition();
}

void CFCharpterLayer::toNextCharpter(){
    int current = charpters[1]->getTag();
    CCSprite* toberemove = charpters[0];
    if(toberemove){
        toberemove->removeFromParentAndCleanup(true);
    }
    charpters[0] = charpters[1];
    charpters[1] = charpters[2];
    charpters[2] = NULL;
    if(current + 1 < charptercount){
        charpters[2] = this->createCharpterSprite(current+2);
        charpters[2]->setPosition(ccp(originpos[2].x + charpters[1]->getPosition().x - originpos[1].x, originpos[2].y));
        this->addChild(charpters[2],15);
    }
    //���ùؿ�λ��
    this->resetCharpterPosition();
}

void CFCharpterLayer::toPreviousCharpter(){
    int current = charpters[1]->getTag();
    CCSprite* toberemove = charpters[2];
    if(toberemove){
        toberemove->removeFromParentAndCleanup(true);
    }
    charpters[2] = charpters[1];
    charpters[1] = charpters[0];
    charpters[0] = NULL;
    if(current -1 > 1){
        charpters[0] = this->createCharpterSprite(current-2);
        charpters[0]->setPosition(ccp(originpos[0].x + charpters[1]->getPosition().x - originpos[1].x, originpos[0].y));
        this->addChild(charpters[0],15);
    }
    //���ùؿ�λ��
    this->resetCharpterPosition();
}

void CFCharpterLayer::resetCharpterPosition(){
    if( charpters[0]){
        CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, originpos[0]);
        charpters[0]->runAction(pMove);
    }
    if(charpters[1]){
        CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, originpos[1]);
        charpters[1]->runAction(pMove);
    }
    if(charpters[2]){
        CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, originpos[2]);
        charpters[2]->runAction(pMove);
    }
}