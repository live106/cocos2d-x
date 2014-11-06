#ifndef __CFLEVELLOADER_H__
#define __CFLEVELLOADER_H__

#include "cocos2d.h"
#include "CFGameLayer.h"
#include "CFMatch.h"
#include "CFSlot.h"
#include "lib/CFGameEngine.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

using namespace cocos2d;

class CFLevelLoader;
class CFLevel;
class CFExpLevel;
class CFSquareLevel;
class CFGameLayer;
class CFSlot;
class CFMatch;

//一个用火柴摆出来的8
typedef struct SEight{
    int posx;
    int posy;
    bool slots[7];
} SEight;

//由多个用火柴摆出来的8组成，最多支持9999
typedef struct SNumber{
    int carry;//进位
    SEight eights[4];
}SNumber;

//一个火柴摆出来的运算符，包括+,-,x,/,= 加减乘除等于，分别对应kind值：0，1，2，3，4
typedef struct SOperator{
    int kind;
}SOperator;

//一个火柴摆出来的3×3矩阵
typedef struct SSquare{
    bool slots[24];
} SSquare;

//游戏的步骤记录
typedef struct SStep{
    int fromslot;
    int toslot;
    SStep* preview;
    SStep* next;
} SStep;


/************************************************************************/
/* 游戏关卡载入管理者                                                    */
/************************************************************************/

class CFLevelLoader{
private:
    //私有变量
    lua_State* L;
    CFGameLayer* pGameLayer;
    CFLevel* pLevel;
    CCSpriteBatchNode* pStoneTree;
public:
    CFLevelLoader(CFGameLayer* pGameLayer);
    virtual ~CFLevelLoader(void);
    bool loadLevel(const char* luafile);
    bool unloadLevel();
    CFLevel* getCurrentLevel();
};
/************************************************************************/
/* 游戏关卡抽象类                                                        */
/************************************************************************/
class CFLevel:public SelectorProtocol{
private:
    SStep* pSteps;
    SStep* pCurrentStep;
protected:
    bool enable;
    string* name;
    int type;
    int timelimit;
    int stepcount;
    int allowtomovenum;
    CCMutableArray<CFSlot*>* pSlots;
    CCMutableArray<CFMatch*>* pMatches;
    CFGameLayer* pGameLayer;
    //由子类实现的，游戏的基本动作，对于表达式游戏来说，是移动或者移回火柴，对于矩阵游戏来说，是显示或者隐藏火柴
    virtual void doLevelAction(int fromslotid, int toslotid) = 0;
    virtual void undoLevelAction(int fromslotid, int toslotid) = 0;
public:
    CFLevel(CFGameLayer* pGameLayer);
    virtual ~CFLevel(void);
    //纯虚函数，不同类型的游戏需要实现的特性被抽象为纯虚函数
    static CFLevel* newInstance(CFGameLayer* pGameLayer, int type);
    //执行一步，前一步，后一步，重置
    void doStep(int fromslotid, int toslotid);
    void undoStep();
    void redoStep();
    void resetSteps();
    void freeSteps(SStep* pStep);
    int getStepsCount();
    //获取游戏元素
    CCMutableArray<CFSlot*>* getSlots();
    CCMutableArray<CFMatch*>* getMatches();
    CFSlot* pointInFirstSlot(CCPoint point);
    const char* getLevelName();
    //游戏函数
    virtual bool load(lua_State* L) = 0;
    virtual bool draw();
    virtual bool unload();
    void removeSprites();
    virtual bool checkSuccess() = 0;
    //设置关卡可用性，CFMatch根据isEnable决定是否允许移动
    bool isEnable();
    void setEnable();
    void setUnable();
    //游戏关卡状态切换
    void showSlots();  //只有在拖拽火柴过程中才显示slot，平时不显示
    void hideSlots();
    void showMatches();//显示火柴，带动画
    void hideMatches();//隐藏火柴，带动画
};

/************************************************************************/
/* 表达式类型关卡                                                        */
/************************************************************************/
class CFExpLevel:public CFLevel{
private:
    int allowtomovenum;
    SNumber* pLeftNum;
    SOperator* pOperator;
    SNumber* pRightNum;
    SOperator* pEqualSign;
    SNumber* pValueNum;

    CCMutableArray<CCSprite*>* pOperators;
    bool initSlots(lua_State* L);
    bool initSlot(lua_State* L, int index);
    bool initEqualSign(lua_State* L, SOperator* equal);
    bool initOperator(lua_State* L, SOperator* oper);
    bool initNumber(lua_State* L, SNumber& num);
    bool initEights(lua_State* L, SNumber& num);
    bool initEight(lua_State* L, SEight& eight);

    int getEightNumber(bool num[]);
    void getEightObj(int num, SEight* eight);
    CFMatch* newMatch();//不同类型游戏中，match，slot行为可能不一样
    CFSlot* newSlot();
    CCSprite* newOperator(int kind);

    void runNumberAnimation(CFSlot** slots, CFMatch** matches, int num);
    void showEight();
    bool drawNumber(SNumber* num, CCPoint& point);
    bool drawEight(SEight* eight, CCPoint& point);
    bool drawOperator(SOperator* oper, CCPoint& point);
public:
    CFExpLevel(CFGameLayer* pGameLayer);
    virtual ~CFExpLevel(void);
    bool load(lua_State* L);
    bool draw();
    bool unload();
    //将火柴移动到指定slotid中
    void doLevelAction(int fromslotid, int toslotid);
    //将火柴移动到指定slotid中
    void undoLevelAction(int fromslotid, int toslotid);
    bool checkSuccess();
};

/************************************************************************/
/* 矩阵类型关卡                                                          */
/************************************************************************/

class CFSquareLevel:public CFLevel{
private:
    int s_total;
    int s_big;
    int s_middle;
    int s_small;
    int s_same;
    SSquare* pSquare;
    bool checkMatch(CCMutableArray<CFSlot*>* slots, int index);
    int getBigSquareCount(bool matches[]);
    int getMiddleSquareCount(bool matches[]);
    int getSmallSquareCount(bool matches[]);
    CFMatch* newMatch();//不同类型游戏中，match，slot行为可能不一样
    CFSlot* newSlot();
    CCSprite* newTipBoard();
    bool initSlots(lua_State* L);
    //游戏内menu点击事件
    void onNextStepMenu(CCObject* pSender);
    void onPreStepMenu(CCObject* pSender);
    void onSubmitMenu(CCObject* pSender);
public:
    CFSquareLevel(CFGameLayer* pGameLayer);
    virtual ~CFSquareLevel(void);
    bool load(lua_State* L);
    bool draw();
    bool unload();
    //将火柴隐藏
    void doLevelAction(int fromslotid, int toslotid);
    //将火柴显示
    void undoLevelAction(int fromslotid, int toslotid);
    bool checkSuccess();
};

#endif
