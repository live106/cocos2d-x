#include "CFLevelLoader.h"


/************************************************************************/
/* 几个私有函数
/************************************************************************/

#define MATCH_WIDTH 26
#define MATCH_LENGTH 119
#define NUMBER_GAP 10//数字之间的间隔

int getIntFromLuaTable(lua_State* L, const char* key, int fallback){
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    int value = lua_tointeger(L, -1);
    lua_pop(L, 1);
    if(!value){
        return fallback;
    }
    return value;
}

const char* getStrFromLuaTable(lua_State* L, const char* key, const char* fallback){
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    const char* typestr = lua_tostring(L, -1);
    lua_pop(L, 1);
    if(!typestr){
        return fallback;
    }
    return typestr;
}

/***************************************************************************************
* 关卡载入器的实现代码
****************************************************************************************/

CFLevelLoader::CFLevelLoader(CFGameLayer* pGameLayer):pGameLayer(pGameLayer),pLevel(NULL),pStoneTree(NULL){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("img/game/gameelements.plist");
    pStoneTree = CCSpriteBatchNode::batchNodeWithFile("img/game/gameelements.png");
    pGameLayer->addChild(pStoneTree);
}

CFLevelLoader::~CFLevelLoader(){
    if(pLevel){
        delete pLevel;
        pLevel = NULL;
    }
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->removeSpriteFramesFromFile("img/game/gameelements.plist");
    if(pStoneTree){
        pGameLayer->removeChild(pStoneTree,true);
        pStoneTree=NULL;
    }
}

bool CFLevelLoader::loadLevel(const char* luafile){
    CCCallFunc* pUnload = NULL;
    if(pLevel){//清除上次的初始化
        pUnload = CCCallFunc::actionWithTarget(this->pLevel, SEL_CallFunc(&CFLevel::unload));
    }
    L = luaL_newstate();
    int s = luaL_dofile(L,luafile);
    if(s!=0){
        CCLog("Load File Failed!");
        lua_close(L);
        return false;
    }
    lua_getglobal(L, "level");
    int type = getIntFromLuaTable(L, "type", 0);
    this->pLevel = CFLevel::newInstance(this->pGameLayer, type);
    this->pLevel->load(L);
    lua_close(L);
    CCCallFunc* pDraw = CCCallFunc::actionWithTarget(this->pLevel, SEL_CallFunc(&CFLevel::draw));
    if(pUnload){
        CCDelayTime* pDelay = CCDelayTime::actionWithDuration(2);
        CCAction* pAction = CCSequence::actions(pUnload, pDelay, pDraw, NULL);
        this->pGameLayer->runAction(pAction);
    }else{
        this->pGameLayer->runAction(pDraw);
    }
    return true;

}

CFLevel* CFLevelLoader::getCurrentLevel(){
    if(pLevel){
        return pLevel;
    }
    return NULL;
}

/*******************************************************************************
* 抽象类CFLevel的实现，实现不同类型关卡的公用代码，以及不同类型关卡的静态实例创建函数*
********************************************************************************/

CFLevel::CFLevel(CFGameLayer* pGameLayer):pGameLayer(pGameLayer),pSlots(NULL),pMatches(NULL),pSteps(NULL),pCurrentStep(NULL),stepcount(0),allowtomovenum(0), name(NULL),enable(false){
    pSlots = new CCMutableArray<CFSlot*>();
    pMatches = new CCMutableArray<CFMatch*>();
}

CFLevel::~CFLevel(){
    if(pSlots){
        delete pSlots;
        pSlots=NULL;
    }
    if(pMatches){
        delete pMatches;
        pMatches=NULL;
    }
    if(pSteps){
        freeSteps(pSteps);
        pSteps=NULL;
    }
    if(name){
        delete name;
        name = NULL;
    }
}

const char* CFLevel::getLevelName(){
    return this->name->c_str();
}

CCMutableArray<CFSlot*>* CFLevel::getSlots(){
    return this->pSlots;
}

CCMutableArray<CFMatch*>* CFLevel::getMatches(){
    return this->pMatches;
}

CFSlot* CFLevel::pointInFirstSlot(CCPoint point){
    if(!pSlots){
        return NULL;
    }
    int count = pSlots->count();
    for(int i = 0;i < count ;i++){
        CFSlot* pSlot = pSlots->getObjectAtIndex(i);
        CCPoint spos = pSlot->getPosition();
        CCSize ssize = pSlot->getContentSize();
        int rot = (int)pSlot->getRotation();
        if((rot/90)%2==0){
            CCRect rect = CCRectMake(spos.x-ssize.width/2, spos.y-ssize.height/2, ssize.width,ssize.height);
            if(CCRect::CCRectContainsPoint(rect, point)){
                return pSlot;
            }
        }else{
            CCRect rect = CCRectMake(spos.x-ssize.height/2, spos.y-ssize.width/2, ssize.height,ssize.width);
            if(CCRect::CCRectContainsPoint(rect, point)){
                return pSlot;
            }
        }

    }
    return NULL;
};

//公用的lua参数读取，子类扩展时应该先调用它
bool CFLevel::load(lua_State* L){
    const char* lname = getStrFromLuaTable(L, "name", 0);
    this->name = new string(lname);
    this->timelimit = getIntFromLuaTable(L, "timelimit", 0);
    this->allowtomovenum = getIntFromLuaTable(L, "Allowtomovenum", 1);
    CCCallFunc* pFunc = CCCallFunc::actionWithTarget(this, SEL_CallFunc(&CFLevel::setEnable));
    CCDelayTime* pDelay = CCDelayTime::actionWithDuration(3);
    this->pGameLayer->runAction(CCSequence::actions(pDelay, pFunc, NULL));
    return true;
}

bool CFLevel::draw(){
    //DO NOTHING
    return true;
}

//卸载一个关卡的内容
bool CFLevel::unload(){
    //先运行卸载动画，再回收资源
    this->hideSlots();
    for(int i = 0; i < pMatches->count();i++){
        CFMatch* pMatch = pMatches->getObjectAtIndex(i);
        CCRotateBy* pRot = CCRotateBy::actionWithDuration(0.5, 360);
        CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, ccp(pMatch->getPosition().x, 1000));
        pMatch->runAction(CCSpawn::actions(pRot, pMove, NULL));
    }
    CCCallFunc* pFunc = CCCallFunc::actionWithTarget(this, SEL_CallFunc(&CFLevel::removeSprites));
    CCDelayTime* pDelay = CCDelayTime::actionWithDuration(1);
    this->pGameLayer->runAction(CCSequence::actions(pDelay, pFunc, NULL));
    return true;
}

void CFLevel::removeSprites(){
    if(pSlots){
        int scount = pSlots->count();
        for(int i = scount-1; i > -1; i--){
            CFSlot* pSlot = pSlots->getObjectAtIndex(i);
            //pGameLayer->removeChild(pSlot, true);
            pSlot->removeFromParentAndCleanup(false);
            pSlots->removeObjectAtIndex(i);
        }
    }
    if(pMatches){
        int mcount = pMatches->count();
        for(int i = mcount-1;i > -1; i--){
            CFMatch* pMatch = pMatches->getObjectAtIndex(i);
            CCMoveTo* pMoveTo = CCMoveTo::actionWithDuration(10, ccp(0,0));
            pMatch->runAction(pMoveTo);
            pMatch->removeFromParentAndCleanup(false);
            pMatches->removeObjectAtIndex(i);
        }
    }
}
//按照type创建不同的CFLevel扩展类实例，并通过lua文件初始化之
CFLevel* CFLevel::newInstance(CFGameLayer* pGameLayer, int type){
    CFLevel* pLevel = NULL;
    switch(type){
    case 0:
        pLevel = new CFExpLevel(pGameLayer);
        break;
    case 1:
        pLevel = new CFSquareLevel(pGameLayer);
        break;
    default:
        throw exception("unsupport level type!");
    }
    return pLevel;
}

void CFLevel::doStep(int fromslotid, int toslotid){
    //执行操作动作
    this->doLevelAction(fromslotid, toslotid);
    //记录步骤
    if(!pSteps){//第一步
        pSteps = (SStep*) malloc(sizeof(SStep));
        pSteps->preview = NULL;
        pSteps->next = NULL;
        pCurrentStep = pSteps;
    }else{
        if(!pCurrentStep){//通过回退功能回退到原点
            pCurrentStep = pSteps;
        }else{
            if(!pCurrentStep->next){//pCurrentStep指向最后一步
                SStep* pStep = (SStep*) malloc(sizeof(SStep));
                pCurrentStep->next = pStep;
                pStep->preview = pCurrentStep;
                pCurrentStep = pStep;
                pCurrentStep->next = NULL;
            }else{//pCurrentStep指向某中间步骤
                pCurrentStep = pCurrentStep->next;
            }
        }
    }
    pCurrentStep->fromslot = fromslotid;
    pCurrentStep->toslot = toslotid;
    //执行doStep代表用手工移动火柴，因此清除pCurrentStep后续内容，以免历史记录混乱
    this->freeSteps(pCurrentStep->next);
    pCurrentStep->next = NULL;
    stepcount++;
    //关卡检查：
    int count = this->getStepsCount();
    if( count<allowtomovenum){
        return;
    }
    bool success = this->checkSuccess();
    if(success){
        this->pGameLayer->onGameSuccess();
    }else{
        this->pGameLayer->onGameFailed();
    }
}

void CFLevel::redoStep(){
    if(!pSteps){
        return;
    }
    SStep* pNext = pCurrentStep?pCurrentStep->next:pSteps;
    if(!pNext){
        return;
    }
    this->doLevelAction(pNext->fromslot, pNext->toslot);
    pCurrentStep = pNext;
    stepcount++;
    //关卡检查：
    int count = this->getStepsCount();
    if( count<allowtomovenum){
        return;
    }
    bool success = this->checkSuccess();
    if(success){
        this->pGameLayer->onGameSuccess();
    }else{
        this->pGameLayer->onGameFailed();
    }
}

void CFLevel::undoStep(){
    if(!pCurrentStep||!pSteps){
        return;
    }
    this->undoLevelAction(pCurrentStep->fromslot,pCurrentStep->toslot);
    pCurrentStep = pCurrentStep->preview;
    stepcount--;
}

void CFLevel::resetSteps(){
    if(!pSlots){
        return;
    }
    while(pCurrentStep){
        this->undoStep();
    }
    stepcount=0;
}

void CFLevel::freeSteps(SStep* pStep){
    if(!pStep){
        return;
    }
    SStep* pNext = pStep->next;
    free(pStep);
    pStep = NULL;
    while(pNext){
        pStep = pNext;
        pNext = pNext->next;
        free(pStep);
        pStep = NULL;
    }
    stepcount=0;
}

int CFLevel::getStepsCount(){
    return stepcount;
}

void CFLevel::showSlots(){
    CCMutableArray<CFSlot*>* pSlots = this->getSlots();
    for(int i = 0; i < pSlots->count();i++){
        CFSlot* pSlot = pSlots->getObjectAtIndex(i);
        pSlot->runAction(CCFadeIn::actionWithDuration(1));
        pSlot->setIsVisible(true);
    }
}

void CFLevel::hideSlots(){
    CCMutableArray<CFSlot*>* pSlots = this->getSlots();
    for(int i = 0; i < pSlots->count();i++){
        CFSlot* pSlot = pSlots->getObjectAtIndex(i);
        pSlot->runAction(CCFadeOut::actionWithDuration(1));
    }
}

void CFLevel::setEnable(){
    this->enable = true;
}

void CFLevel::setUnable(){
    this->enable = false;
}

bool CFLevel::isEnable(){
    return this->enable;
}

/************************************************************************/
/* 表达式运算类型关卡实现                                                */
/************************************************************************/

CFExpLevel::CFExpLevel(CFGameLayer* pGameLayer):CFLevel(pGameLayer),pOperators(new CCMutableArray<CCSprite*>()),pLeftNum(NULL),pRightNum(NULL),pOperator(NULL),pEqualSign(NULL){

}

//回收关卡资源
CFExpLevel::~CFExpLevel(){
    this->removeSprites();
}

//创建一个CFSlot实例
CFSlot* CFExpLevel::newSlot(){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* pSlotFrame = pFrameCache->spriteFrameByName("slot.png");
    CFSlot* pSlot = CFSlot::slotWithFrame(this, pSlotFrame);
    int count = pSlots->count();
    pSlot->setIndex(count);
    pSlots->addObject(pSlot);
    this->pGameLayer->addChild(pSlot, 35);
    return pSlot;
}

//创建一个可以拖拽的CFMatch实例
CFMatch* CFExpLevel::newMatch(){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* pMatchFrame = pFrameCache->spriteFrameByName("match.png");
    CFMatch* pMatch = CFMatch::matchDragableWithFrame(this,pMatchFrame);
    int count = pMatches->count();
    pMatch->setIndex(count);
    pMatches->addObject(pMatch);
    this->pGameLayer->addChild(pMatch, 36);
    return pMatch;
}

//传教加减乘除的符号Sprite
CCSprite* CFExpLevel::newOperator(int kind){
    CCSprite* pSprite = NULL;
    switch(kind){
    case 0:
        pSprite = CCSprite::spriteWithSpriteFrameName("add_sign.png");
        break;
    case 1:
        pSprite = CCSprite::spriteWithSpriteFrameName("plus_sign.png");
        break;
    case 2:
        pSprite = CCSprite::spriteWithSpriteFrameName("multi_sign.png");
        break;
    case 3:
        pSprite = CCSprite::spriteWithSpriteFrameName("divide_sign.png");
        break;
    case 4:
        pSprite = CCSprite::spriteWithSpriteFrameName("equal_sign.png");
        break;
    default:
        return NULL;
    }
    pOperators->addObject(pSprite);
    this->pGameLayer->addChild(pSprite);
    return pSprite;
}

//按照Lua设置初始化关卡，并将关卡的sprite绘制到pGameLayer中
bool CFExpLevel::load(lua_State* L){
    if(!CFLevel::load(L)){
        return false;
    }
    this->allowtomovenum = getIntFromLuaTable(L, "Allowtomovenum", 0);
    lua_pushnil(L);
    while(lua_next(L, -2)!=0){
        if(lua_istable(L, -1)){
            const char* key = lua_tostring(L, -2);
            if(!strcmp(key, "slots")){
                if(!initSlots(L)){
                    CCLog("can not load slot!");
                    return false;
                }
            }else{
                CCLog(lua_tostring(L, -2));
            }
        }
        lua_pop(L, 1);
    }
    return true;
}

bool CFExpLevel::draw(){
    if (!CFLevel::draw()){
        return false;
    }
    CCPoint point = CCPoint(50,400);
    bool succez = false;
    succez = drawNumber(pLeftNum, point);
    if(!succez){
        return false;
    }
    succez = drawOperator(pOperator, point);
    if(!succez){
        return false;
    }
    succez = drawNumber(pRightNum, point);
    if(!succez){
        return false;
    }
    succez = drawOperator(pEqualSign, point);
    if(!succez){
        return false;
    }
    succez = drawNumber(pValueNum, point);
    return succez;
}

bool CFExpLevel::unload(){
    if(!CFLevel::unload()){
        return false;
    }
    if(this->pOperators){
        int count = this->pOperators->count();
        for(int i = count-1; i >-1;i--){
            CCSprite* pSprite = this->pOperators->getObjectAtIndex(i);
            if(!pSprite){
                continue;
            }
            pGameLayer->removeChild(pSprite, true);
            this->pOperators->removeObjectAtIndex(i);
        }
    }
    if(pLeftNum){
        delete pLeftNum;
    }
    if(pOperator){
        delete pOperator;
    }
    if(pRightNum){
        delete pRightNum;
    }
    if(pEqualSign){
        delete pEqualSign;
    }
    if(pValueNum){
        delete pValueNum;
    }
    return true;
}

//按照lua设置中slots的table初始化锚点信息
bool CFExpLevel::initSlots(lua_State* L){
    lua_pushnil(L);
    while(lua_next(L, -2)!=0){
        int index = lua_tointeger(L, -2);
        if(lua_istable(L, -1)){
            if(!initSlot(L, index)){
                return false;
            }
        }
        lua_pop(L, 1);
    }
    return true;
}

//根据slots中的类型值不同，分别生成数字，操作符结构体实例
bool CFExpLevel::initSlot(lua_State* L, int index){
    const char* typestr = getStrFromLuaTable(L, "type", NULL);
    if(!typestr){
        CCLog("slot define miss type!");
        return false;
    }
    if(!strcmp(typestr,"number")){
        if(index == 1){//left number
            pLeftNum = new SNumber();
            return initNumber(L, *pLeftNum);
        }else if(index == 3){//right number
            pRightNum = new SNumber();
            return initNumber(L, *pRightNum);
        }else if(index == 5){//value number
            pValueNum = new SNumber();
            return initNumber(L, *pValueNum);
        }else{
            CCLog("Wrong Number Index!");
            return false;
        }
    }else if(!strcmp(typestr, "operator")){
        if(index == 2){
            pOperator = new SOperator();
            return initOperator(L, pOperator);
        }else if(index == 4){
            pEqualSign = new SOperator();
            return initOperator(L, pEqualSign);
        }

    }else{
        CCLog("not support slot type!");
        return false;
    }
    return true;

}

//载入一个数字，数字由多个eight组成
bool CFExpLevel::initNumber(lua_State* L, SNumber& num){
    lua_pushnil(L);
    while(lua_next(L, -2)!=0){
        if(lua_istable(L, -1)){
            const char* key = lua_tostring(L, -2);
            if(!strcmp(key, "eights")){
                if(!initEights(L, num)){
                    CCLog("load Eights Failed");
                    return false;
                }
            }
        }
        lua_pop(L, 1);
    }
    return true;
}

//载入一个eight，实际上就是锚点拼接成的一个数字8
bool CFExpLevel::initEights(lua_State* L, SNumber& num){
    lua_pushnil(L);
    num.carry = 0;
    while(lua_next(L, -2)!=0){
        if(lua_istable(L, -1)){
            int index = lua_tointeger(L, -2);
            num.carry = index > num.carry?index:num.carry;
            if(!initEight(L, num.eights[index - 1])){
                return false;
            }
        }
        lua_pop(L, 1);
    }
    return true;
}

//载入一个数字8结构体实例
bool CFExpLevel::initEight(lua_State* L, SEight& eight){
    lua_pushnil(L);
    while(lua_next(L, -2)!=0){
        if(lua_isnumber(L, -1)){
            int index = lua_tointeger(L, -2);
            bool value = lua_tointeger(L, -1);
            eight.slots[index -1] = value;
        }
        lua_pop(L, 1);
    }
    return true;
}

//载入一个操作符结构体实例
bool CFExpLevel::initOperator(lua_State* L, SOperator* oper){
    lua_pushnil(L);
    while(lua_next(L, -2)!=0){
        if(lua_isstring(L, -1)){
            const char* key = lua_tostring(L, -2);
            if(!strcmp(key, "kind")){
                const char* value = lua_tostring(L, -1);
                switch(value[0]){
                case '+':
                    oper->kind = 0;
                    break;
                case '-':
                    oper->kind = 1;
                    break;
                case '*':
                    oper->kind = 2;
                    break;
                case '/':
                    oper->kind = 3;
                    break;
                case '=':
                    oper->kind = 4;
                    break;
                default:
                    return false;
                }
            }
        }
        lua_pop(L, 1);
    }
    return true;
}

//执行动作
void CFExpLevel::doLevelAction(int fromslotid, int toslotid){
    CFSlot* pFromSlot = this->getSlots()->getObjectAtIndex(fromslotid);
    CFSlot* pToSlot = this->getSlots()->getObjectAtIndex(toslotid);
    CFMatch* pMatch = pFromSlot->getMountedMatch();
    if(pMatch){
        pToSlot->mountMatch(pMatch);
        pFromSlot->unmountMatch();
    }
}

//撤销动作
void CFExpLevel::undoLevelAction(int fromslotid, int toslotid){
    this->doLevelAction(toslotid, fromslotid);
}


//检测当前游戏是否能够通关
bool CFExpLevel::checkSuccess(){
    CCMutableArray<CFSlot*>* slots = this->getSlots();
    int index = 0;
    int leftcarry = this->pLeftNum->carry;
    int leftnum = 0;
    for(int i = 0; i < leftcarry;i++){
        bool matches[7];
        for(int j = 0; j < 7;j++){
            CFSlot* slot = slots->getObjectAtIndex(index++);
            matches[j] = slot->hasMounted();
        }
        int num = getEightNumber(matches);
        if(num == -1){
            return false;
        }
        leftnum += num*pow((double)10, leftcarry-i-1);
    }
    int oprtype = this->pOperator->kind;//0==+, 1==-, 2==*, 3==/
    int rightcarry = this->pRightNum->carry;
    int rightnum = 0;
    for(int i = 0; i < rightcarry;i++){
        bool matches[7];
        for(int j = 0; j < 7;j++){
            CFSlot* slot = slots->getObjectAtIndex(index++);
            matches[j] = slot->hasMounted();
        }
        int num = getEightNumber(matches);
        if(num == -1){
            return false;
        }
        rightnum += num*pow((double)10, rightcarry-i-1);
    }
    int valuecarry = this->pValueNum->carry;
    int valuenum = 0;
    for(int i = 0; i < valuecarry;i++){
        bool matches[7];
        for(int j = 0; j < 7;j++){
            CFSlot* slot = slots->getObjectAtIndex(index++);
            matches[j] = slot->hasMounted();
        }
        int num = getEightNumber(matches);
        if(num == -1){
            return false;
        }
        valuenum += num*pow((double)10, valuecarry-i-1);
    }

    if(oprtype == 0){
        return leftnum+rightnum==valuenum;
    }else if(oprtype == 1){
        return leftnum-rightnum==valuenum;
    }else if(oprtype ==2 ){
        return leftnum*rightnum==valuenum;
    }else{
        return leftnum/rightnum==valuenum;
    }
}

void CFExpLevel::getEightObj(int num, SEight* eight){
    switch(num){
    case 0:
        eight->slots[0]=true;
        eight->slots[1]=true;
        eight->slots[2]=true;
        eight->slots[3]=false;
        eight->slots[4]=true;
        eight->slots[5]=true;
        eight->slots[6]=true;
        break;
    case 1:
        eight->slots[0]=false;
        eight->slots[1]=false;
        eight->slots[2]=true;
        eight->slots[3]=false;
        eight->slots[4]=false;
        eight->slots[5]=true;
        eight->slots[6]=false;
        eight->slots[7]=true;
        break;
    case 2:
        eight->slots[0]=false;
        eight->slots[1]=true;
        eight->slots[2]=true;
        eight->slots[3]=true;
        eight->slots[4]=true;
        eight->slots[5]=false;
        eight->slots[6]=true;
        break;
    case 3:
        eight->slots[0]=false;
        eight->slots[1]=true;
        eight->slots[2]=true;
        eight->slots[3]=true;
        eight->slots[4]=false;
        eight->slots[5]=true;
        eight->slots[6]=true;
        break;
    case 4:
        eight->slots[0]=true;
        eight->slots[1]=false;
        eight->slots[2]=true;
        eight->slots[3]=true;
        eight->slots[4]=false;
        eight->slots[5]=true;
        eight->slots[6]=false;
        break;
    case 5:
        eight->slots[0]=true;
        eight->slots[1]=true;
        eight->slots[2]=false;
        eight->slots[3]=true;
        eight->slots[4]=false;
        eight->slots[5]=true;
        eight->slots[6]=true;
        break;
    case 6:
        eight->slots[0]=true;
        eight->slots[1]=true;
        eight->slots[2]=false;
        eight->slots[3]=true;
        eight->slots[4]=true;
        eight->slots[5]=true;
        eight->slots[6]=true;
        break;
    case 7:
        eight->slots[0]=false;
        eight->slots[1]=true;
        eight->slots[2]=true;
        eight->slots[3]=false;
        eight->slots[4]=false;
        eight->slots[5]=true;
        eight->slots[6]=false;
        break;
    case 8:
        eight->slots[0]=true;
        eight->slots[1]=true;
        eight->slots[2]=true;
        eight->slots[3]=true;
        eight->slots[4]=true;
        eight->slots[5]=true;
        eight->slots[6]=true;
        break;
    case 9:
        eight->slots[0]=true;
        eight->slots[1]=true;
        eight->slots[2]=true;
        eight->slots[3]=true;
        eight->slots[4]=false;
        eight->slots[5]=true;
        eight->slots[6]=true;
        break;
    }
}

/**           
* 0={true, true, true, false, true, true, true}
* 1={false, false,true,false,false,true,false}
* 2={false, true, true, true, true, false, true}
* 3={false, true, true, true, false, true, true}
* 4={true, false, true, true, false, true, false}
* 5={true, true, false, true, false, true, true}
* 6={true, true, false, true, ture, true, true}
* 7={false, true, true, false, false, true, false}
* 8={true, true, true, true, true, true, true}
* 9={true, true, true, true, false, true, true}
*/
int CFExpLevel::getEightNumber(bool num[]){
    if(num[0] && num[1] && num[2] && !num[3] && num[4] && num[5] && num[6]){
        return 0;
    }
    if(!num[0] && !num[1] && num[2] && !num[3] && !num[4] && num[5] && !num[6]){
        return 1;
    }
    if(!num[0] && num[1] && num[2] && num[3] && num[4] && !num[5] && num[6]){
        return 2;
    }
    if(!num[0] && num[1] && num[2] && num[3] && !num[4] && num[5] && num[6]){
        return 3;
    }
    if(num[0] && !num[1] && num[2] && num[3] &&!num[4] && num[5] && !num[6]){
        return 4;
    }
    if(num[0] && num[1] &&! num[2] && num[3] && !num[4] && num[5] && num[6]){
        return 5;
    }
    if(num[0] && num[1] && !num[2] && num[3] && num[4] && num[5] && num[6]){
        return 6;
    }
    if(!num[0] && num[1] && num[2] && !num[3] && !num[4] && num[5] && !num[6]){
        return 7;
    }
    if(num[0] && num[1] && num[2] && num[3] && num[4] && num[5] && num[6]){
        return 8;
    }
    if(num[0] && num[1] && num[2] && num[3] && !num[4] && num[5] && num[6]){
        return 9;
    }
    return -1;
}



//point在执行前表示将要绘制的左中点，执行后会设置为绘制的右中点
bool CFExpLevel::drawNumber(SNumber* num, CCPoint& point){
    for(int i = 0; i < num->carry;i++){
        SEight eight = num->eights[i];
        int num = this->getEightNumber(eight.slots);
        if(!drawEight(&eight, point)){
            return false;
        }
    }
    return true;
}


//point在执行前表示将要绘制的左中点，执行后会设置为绘制的右中点
bool CFExpLevel::drawEight(SEight* eight, CCPoint& point){
    //初始化相应的CFSlot组合
    CFSlot** slots = (CFSlot**)malloc(sizeof(CFSlot**)*7);
    for(int i = 0; i < 7;i++){
        slots[i]=this->newSlot();
        slots[i]->setIsVisible(false);
    }
    slots[0]->setPosition(ccp(point.x, point.y+MATCH_LENGTH/2));
    slots[1]->setPosition(ccp(point.x+MATCH_LENGTH/2, point.y + MATCH_LENGTH));
    slots[1]->setRotation(90);
    slots[2]->setPosition(ccp(point.x+MATCH_LENGTH, point.y+MATCH_LENGTH/2));
    slots[3]->setPosition(ccp(point.x+MATCH_LENGTH/2, point.y));
    slots[3]->setRotation(90);
    slots[4]->setPosition(ccp(point.x, point.y-MATCH_LENGTH/2));
    slots[5]->setPosition(ccp(point.x+MATCH_LENGTH, point.y-MATCH_LENGTH/2));
    slots[6]->setPosition(ccp(point.x+MATCH_LENGTH/2, point.y-MATCH_LENGTH));
    slots[6]->setRotation(90);
    point.x += MATCH_LENGTH + MATCH_WIDTH + NUMBER_GAP;
    //初始化相应的CFMatch组合
    /**
    CFMatch** matches = (CFMatch**)malloc(sizeof(CFMatch*)*7);
    for(int i = 0; i < 7;i++){
    matches[i] = this->newMatch();
    matches[i]->setPosition(ccp(2000,2000));
    }*/
    //执行数字滚动动画
    for(int i = 0; i < 7;i++){
        if(eight->slots[i]){
            CFMatch* pMatch = this->newMatch();
            pMatch->setPosition(slots[i]->getPosition());
            slots[i]->mountMatch(pMatch);
        }else{
            slots[i]->unmountMatch();
        }
    }
    free(slots);
    return true;
}

//point在执行前表示将要绘制的左中点，执行后会设置为绘制的右中点
bool CFExpLevel::drawOperator(SOperator* oper, CCPoint& point){
    CCSprite* pSign = this->newOperator(oper->kind);
    if(!pSign){
        return false;
    }
    pSign->setPosition(ccp(point.x + MATCH_LENGTH/2, point.y));
    CCRotateBy* pRot = CCRotateBy::actionWithDuration(0.5,360);
    CCDelayTime* pDelay = CCDelayTime::actionWithDuration(0.5);
    CCAction* pSeq = CCSequence::actions(pRot, pDelay,pRot->reverse(),NULL);
    pSign->runAction(pSeq);
    point.x += MATCH_LENGTH + MATCH_WIDTH + NUMBER_GAP;
    return true;
}

/************************************************************************/
/* 矩阵类型关卡实现                                                      */
/************************************************************************/

CFSquareLevel::CFSquareLevel(CFGameLayer* pGameLayer):CFLevel(pGameLayer), pSquare(NULL){
    pSlots = new CCMutableArray<CFSlot*>();
    pMatches = new CCMutableArray<CFMatch*>();
}

CFSquareLevel::~CFSquareLevel(){
    this->unload();
}

CFSlot* CFSquareLevel::newSlot(){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* pSlotFrame = pFrameCache->spriteFrameByName("slot.png");
    CFSlot* pSlot = CFSlot::slotWithFrame(this, pSlotFrame);
    int count = pSlots->count();
    pSlot->setIndex(count);
    pSlots->addObject(pSlot);
    this->pGameLayer->addChild(pSlot, 35);
    return pSlot;
}

CFMatch* CFSquareLevel::newMatch(){
    CCSpriteFrameCache* pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* pMatchFrame = pFrameCache->spriteFrameByName("match.png");
    CFMatch* pMatch = CFMatch::matchRemoveableWithFrame(this,pMatchFrame);
    int count = pMatches->count();
    pMatch->setIndex(count);
    pMatches->addObject(pMatch);
    this->pGameLayer->addChild(pMatch,36);
    return pMatch;
}

CCSprite* CFSquareLevel::newTipBoard(){
    CCSprite* pTipBoard = CCSprite::spriteWithFile("img/game/square/tipboard.png");
    CCSize csize = pTipBoard->getContentSize();
    char word1[40];
    sprintf(word1,"Remove %d matches leave %d squares", 4, 3);
    //sprintf("You have removed %d matches,click Submit to check result");
    //sprintf("There are %d matches left");
    CCLabelBMFont* pLabel = CCLabelBMFont::labelWithString("Remove 4 Matches Leave 5 Square","fonts/fonts.fnt");
    pLabel->setPosition(ccp(csize.width/2,csize.height/2));
    pLabel->setColor(ccc3(200,20,20));
    pTipBoard->addChild(pLabel);
    pTipBoard->removeChild(pLabel,true);
    CCSize winsize = CFGameEngine::getWinSize();

    //下一步
    CCSprite* pNext = CCSprite::spriteWithSpriteFrameName("nextstep.png");
    CCSprite* pNextSelected = CCSprite::spriteWithSpriteFrameName("nextstep_selected.png");
    CCMenuItem* pNextItem = CCMenuItemSprite::itemFromNormalSprite(pNext, pNextSelected, NULL, this, TO_SEL_MENUHANDLER(CFSquareLevel::onNextStepMenu));
    pNextItem->setPosition(ccp(100,300));
    //上一步
    CCSprite* pPre = CCSprite::spriteWithSpriteFrameName("prestep.png");
    CCSprite* pPreSelected = CCSprite::spriteWithSpriteFrameName("prestep_selected.png");
    CCMenuItem* pPreItem = CCMenuItemSprite::itemFromNormalSprite(pPre, pPreSelected, NULL, this, TO_SEL_MENUHANDLER(CFSquareLevel::onPreStepMenu));
    pPreItem->setPosition(ccp(100,200));
    //提交
    CCSprite* pSubmit = CCSprite::spriteWithSpriteFrameName("submit.png");
    CCSprite* pSubmitSelected = CCSprite::spriteWithSpriteFrameName("submit_selected.png");
    CCMenuItem* pSubmitItem = CCMenuItemSprite::itemFromNormalSprite(pSubmit, pSubmitSelected, NULL, this, TO_SEL_MENUHANDLER(CFSquareLevel::onSubmitMenu));
    pSubmitItem->setPosition(ccp(100,100));
    CCMenu* pMenu = CCMenu::menuWithItems(pNextItem, pPreItem, pSubmitItem, NULL);
    pMenu->setPosition(ccp(100,100));
    pTipBoard->addChild(pMenu);
    //上一步
    pTipBoard->setPosition(ccp(winsize.width - csize.width/2, winsize.height/2));
    this->pGameLayer->addChild(pTipBoard);
    return pTipBoard;
}

void CFSquareLevel::onNextStepMenu(CCObject* pSender){
    this->redoStep();
}

void CFSquareLevel::onPreStepMenu(CCObject* pSender){
    this->undoStep();
}

void CFSquareLevel::onSubmitMenu(CCObject* pSender){
    bool success = this->checkSuccess();
    if(success){
        this->pGameLayer->onGameSuccess();
    }else{
        this->pGameLayer->onGameFailed();
    }
}

bool CFSquareLevel::load(lua_State* L){
    if(!CFLevel::load(L)){
        return false;
    }
    lua_pushnil(L);
    while(lua_next(L, -2)!=0){
        if(lua_istable(L, -1)){
            const char* key = lua_tostring(L, -2);
            if(!strcmp(key, "slots")){
                initSlots(L);
            }else if(!strcmp(key, "normalsquares")){//允许正方形数目
                s_total = lua_tonumber(L, -1);
            }else if(!strcmp(key, "bigsquare")){//允许的大正方形数目
                s_big = lua_tonumber(L, -1);      
            }else if(!strcmp(key, "mediumsquare")){//允许的中等大小正方形数目
                s_middle = lua_tonumber(L, -1);
            }else if(!strcmp(key, "smallsquare")){//允许的小正方形数目
                s_small = lua_tonumber(L, -1);
            }else if(!strcmp(key, "samesquare")){
                s_same = lua_tonumber(L, -1);
            }
        }
        lua_pop(L, 1);
    }
    this->newTipBoard();
    return true;
}

bool CFSquareLevel::draw(){
    if(!CFLevel::draw()){
        return false;
    }
    for(int i = 0; i < 24;i++){
        bool value = this->pSquare->slots[i];
        if(value){
            CFMatch* pMatch = this->newMatch();
            CFSlot* pSlot=this->getSlots()->getObjectAtIndex(i);
            pSlot->setIsVisible(false);
            pMatch->setPosition(pSlot->getPosition());
            pSlot->mountMatch(pMatch);
        }
    }
    return true;
}

bool CFSquareLevel::unload(){
    if(this->pSlots){
        for(int i = 0; i < this->pSlots->count();i++){
            CFSlot* pSlot = this->pSlots->getObjectAtIndex(i);
            pSlot->removeFromParentAndCleanup(true);
        }
        delete pSlots;
        pSlots = NULL;
    }
    if(this->pMatches){
        for(int i = 0; i < this->pMatches->count();i++){
            CFMatch* pMatch = this->pMatches->getObjectAtIndex(i);
            pMatch->removeFromParentAndCleanup(true);
        }
        delete pMatches;
        pMatches = NULL;
    }
    return true;
}

bool CFSquareLevel::initSlots(lua_State* L){
    lua_pushnil(L);
    CCSize size = CFGameEngine::getWinSize();
    CCPoint center = ccp(size.width/2-200, size.height/2);
    this->pSquare = new SSquare();
    while(lua_next(L, -2)!=0){
        if(lua_isnumber(L, -2)){
            int index = lua_tonumber(L, -2);
            bool value = lua_tonumber(L, -1);
            this->pSquare->slots[index-1] = value;
            CFSlot* pSlot = this->newSlot();
            switch(index){
            case 1:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH-MATCH_WIDTH, center.y+MATCH_LENGTH*3/2+MATCH_WIDTH*3/2));
                pSlot->setRotation(90);
                break;
                   }
            case 2:{
                pSlot->setPosition(ccp(center.x, center.y+MATCH_LENGTH*3/2+MATCH_WIDTH*3/2));
                pSlot->setRotation(90);
                break;
                   }
            case 3:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH+MATCH_WIDTH, center.y+MATCH_LENGTH*3/2+MATCH_WIDTH*3/2));
                pSlot->setRotation(90);
                break;
                   }
            case 4:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH*3/2-MATCH_WIDTH*3/2, center.y+MATCH_LENGTH+MATCH_WIDTH));
                break;
                   }
            case 5:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH/2-MATCH_WIDTH/2, center.y+MATCH_LENGTH+MATCH_WIDTH));
                break;
                   }
            case 6:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH/2+MATCH_WIDTH/2, center.y+MATCH_LENGTH+MATCH_WIDTH));
                break;
                   }
            case 7:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH*3/2+MATCH_WIDTH*3/2, center.y+MATCH_LENGTH+MATCH_WIDTH));
                break;
                   }
            case 8:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH-MATCH_WIDTH, center.y+MATCH_LENGTH/2+MATCH_WIDTH/2));
                pSlot->setRotation(90);
                break;
                   }
            case 9:{
                pSlot->setPosition(ccp(center.x, center.y+MATCH_LENGTH/2+MATCH_WIDTH/2));
                pSlot->setRotation(90);
                break;
                   }
            case 10:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH+MATCH_WIDTH, center.y+MATCH_LENGTH/2+MATCH_WIDTH/2));
                pSlot->setRotation(90);
                break;
                    }
            case 11:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH*3/2-MATCH_WIDTH*3/2, center.y));
                break;
                    }
            case 12:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH/2-MATCH_WIDTH/2, center.y));
                break;
                    }
            case 13:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH/2+MATCH_WIDTH/2, center.y));
                break;
                    }
            case 14:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH*3/2+MATCH_WIDTH*3/2, center.y));
                break;
                    }
            case 15:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH-MATCH_WIDTH, center.y-MATCH_LENGTH/2-MATCH_WIDTH/2));
                pSlot->setRotation(90);
                break;
                    }
            case 16:{
                pSlot->setPosition(ccp(center.x, center.y-MATCH_LENGTH/2-MATCH_WIDTH/2));
                pSlot->setRotation(90);
                break;
                    }
            case 17:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH+MATCH_WIDTH, center.y-MATCH_LENGTH/2-MATCH_WIDTH/2));
                pSlot->setRotation(90);
                break;
                    }
            case 18:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH*3/2-MATCH_WIDTH*3/2, center.y-MATCH_LENGTH-MATCH_WIDTH));
                break;
                    }
            case 19:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH/2-MATCH_WIDTH/2, center.y-MATCH_LENGTH-MATCH_WIDTH));
                break;
                    }
            case 20:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH/2+MATCH_WIDTH/2, center.y-MATCH_LENGTH-MATCH_WIDTH));
                break;
                    }
            case 21:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH*3/2+MATCH_WIDTH*3/2, center.y-MATCH_LENGTH-MATCH_WIDTH));
                break;
                    }
            case 22:{
                pSlot->setPosition(ccp(center.x-MATCH_LENGTH-MATCH_WIDTH, center.y-MATCH_LENGTH*3/2-MATCH_WIDTH*3/2));
                pSlot->setRotation(90);
                break;
                    }
            case 23:{
                pSlot->setPosition(ccp(center.x, center.y-MATCH_LENGTH*3/2-MATCH_WIDTH*3/2));
                pSlot->setRotation(90);
                break;
                    }
            case 24:{
                pSlot->setPosition(ccp(center.x+MATCH_LENGTH+MATCH_WIDTH, center.y-MATCH_LENGTH*3/2-MATCH_WIDTH*3/2));
                pSlot->setRotation(90);
                break;
                    }
            default:{
                exception("error in lua level file, match index can not large than 24");
                break;
                    }
            }
        }
        lua_pop(L, 1);
    }
    return true;
}

void CFSquareLevel::doLevelAction(int fromslotid, int toslotid){
    CFSlot* pFromSlot = this->getSlots()->getObjectAtIndex(fromslotid);
    CFMatch* pMatch = pFromSlot->getMountedMatch();
    pMatch->setIsVisible(false);
}

void CFSquareLevel::undoLevelAction(int fromslotid, int toslotid){
    CFSlot* pFromSlot = this->getSlots()->getObjectAtIndex(fromslotid);
    CFMatch* pMatch = pFromSlot->getMountedMatch();
    pMatch->setIsVisible(true);
}

bool CFSquareLevel::checkSuccess(){
    CCMutableArray<CFSlot*>* slots = this->getSlots();
    int count = slots->count();
    if(count != 24){
        return false;
    }
    bool matches[24];
    for(int i = 0;i < 24;i++){
        matches[i] = this->checkMatch(slots, i);
    }
    int c_big = this->getBigSquareCount(matches);
    int c_middle = this->getMiddleSquareCount(matches);
    int c_small = this->getSmallSquareCount(matches);
    int c_total = c_big + c_middle + c_small;
    if(s_total != c_total){
        return false;
    }
    if(s_big !=0 && s_big != c_big){
        return false;
    }
    if(s_middle != 0 && s_middle!=c_middle){
        return false;
    }
    if(s_small != 0 && s_small != c_small){
        return false;
    }
    return true;
}

bool CFSquareLevel::checkMatch(CCMutableArray<CFSlot*>* slots, int index){
    CFSlot* pSlot = slots->getObjectAtIndex(index);
    CFMatch* pMatch = pSlot->getMountedMatch();
    if(!pMatch){
        return false;
    }
    return pMatch->getIsVisible();
}

int CFSquareLevel::getBigSquareCount(bool matches[]){
    //0,1,2,3,6,10,13,17,20,21,22,23 
    if(matches[0]&&matches[1]&&matches[2]&&matches[3]&&matches[6]&&matches[10]&&matches[13]&&matches[17]&&matches[20]&&matches[21]&&matches[22]&&matches[23]){
        return 1;		
    }
    return 0;
}

int CFSquareLevel::getMiddleSquareCount(bool matches[]){
    int result = 0;	
    //0,1,3,5,10,12,14,15
    if(matches[0]&&matches[1]&&matches[3]&&matches[5]&&matches[10]&&matches[12]&&matches[14]&&matches[15]){
        result++;		
    }
    //1,2,4,6,11,13,15,16
    if(matches[1]&&matches[2]&&matches[4]&&matches[6]&&matches[11]&&matches[13]&&matches[15]&&matches[16]){
        result++;
    }
    //7,8,10,12,17,19,21,22
    if(matches[7]&&matches[8]&&matches[10]&&matches[12]&&matches[17]&&matches[19]&&matches[21]&&matches[22]){
        result++;
    }
    //8,9,11,13,18,20,22,23
    if(matches[8]&&matches[9]&&matches[11]&&matches[13]&&matches[18]&&matches[20]&&matches[22]&&matches[23]){
        result++;
    }
    return result;
}

int CFSquareLevel::getSmallSquareCount(bool matches[]){
    int result = 0;
    //0,3,4,7
    if(matches[0]&&matches[3]&&matches[4]&&matches[7]){
        result++;
    }
    //1,4,5,8
    if(matches[1]&&matches[4]&&matches[5]&&matches[8]){
        result++;
    }
    //2,5,6,9
    if(matches[2]&&matches[5]&&matches[6]&&matches[9]){
        result++;
    }
    //7,10,11,14
    if(matches[7]&&matches[10]&&matches[11]&&matches[14]){
        result++;
    }
    //8,11,12,15
    if(matches[8]&&matches[11]&&matches[12]&&matches[15]){
        result++;
    }
    //9,12,13,16
    if(matches[9]&&matches[12]&&matches[13]&&matches[16]){
        result++;
    }
    //14,17,18,21
    if(matches[14]&&matches[17]&&matches[18]&&matches[21]){
        result++;
    }
    //15,18,19,22
    if(matches[15]&&matches[18]&&matches[19]&&matches[22]){
        result++;
    }
    //16,19,20,23
    if(matches[16]&&matches[19]&&matches[20]&&matches[23]){
        result++;
    }
    return result;
}
