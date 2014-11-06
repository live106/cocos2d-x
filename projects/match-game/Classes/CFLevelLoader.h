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

//һ���û��ڳ�����8
typedef struct SEight{
    int posx;
    int posy;
    bool slots[7];
} SEight;

//�ɶ���û��ڳ�����8��ɣ����֧��9999
typedef struct SNumber{
    int carry;//��λ
    SEight eights[4];
}SNumber;

//һ�����ڳ����������������+,-,x,/,= �Ӽ��˳����ڣ��ֱ��Ӧkindֵ��0��1��2��3��4
typedef struct SOperator{
    int kind;
}SOperator;

//һ�����ڳ�����3��3����
typedef struct SSquare{
    bool slots[24];
} SSquare;

//��Ϸ�Ĳ����¼
typedef struct SStep{
    int fromslot;
    int toslot;
    SStep* preview;
    SStep* next;
} SStep;


/************************************************************************/
/* ��Ϸ�ؿ����������                                                    */
/************************************************************************/

class CFLevelLoader{
private:
    //˽�б���
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
/* ��Ϸ�ؿ�������                                                        */
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
    //������ʵ�ֵģ���Ϸ�Ļ������������ڱ��ʽ��Ϸ��˵�����ƶ������ƻػ�񣬶��ھ�����Ϸ��˵������ʾ�������ػ��
    virtual void doLevelAction(int fromslotid, int toslotid) = 0;
    virtual void undoLevelAction(int fromslotid, int toslotid) = 0;
public:
    CFLevel(CFGameLayer* pGameLayer);
    virtual ~CFLevel(void);
    //���麯������ͬ���͵���Ϸ��Ҫʵ�ֵ����Ա�����Ϊ���麯��
    static CFLevel* newInstance(CFGameLayer* pGameLayer, int type);
    //ִ��һ����ǰһ������һ��������
    void doStep(int fromslotid, int toslotid);
    void undoStep();
    void redoStep();
    void resetSteps();
    void freeSteps(SStep* pStep);
    int getStepsCount();
    //��ȡ��ϷԪ��
    CCMutableArray<CFSlot*>* getSlots();
    CCMutableArray<CFMatch*>* getMatches();
    CFSlot* pointInFirstSlot(CCPoint point);
    const char* getLevelName();
    //��Ϸ����
    virtual bool load(lua_State* L) = 0;
    virtual bool draw();
    virtual bool unload();
    void removeSprites();
    virtual bool checkSuccess() = 0;
    //���ùؿ������ԣ�CFMatch����isEnable�����Ƿ������ƶ�
    bool isEnable();
    void setEnable();
    void setUnable();
    //��Ϸ�ؿ�״̬�л�
    void showSlots();  //ֻ������ק�������в���ʾslot��ƽʱ����ʾ
    void hideSlots();
    void showMatches();//��ʾ��񣬴�����
    void hideMatches();//���ػ�񣬴�����
};

/************************************************************************/
/* ���ʽ���͹ؿ�                                                        */
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
    CFMatch* newMatch();//��ͬ������Ϸ�У�match��slot��Ϊ���ܲ�һ��
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
    //������ƶ���ָ��slotid��
    void doLevelAction(int fromslotid, int toslotid);
    //������ƶ���ָ��slotid��
    void undoLevelAction(int fromslotid, int toslotid);
    bool checkSuccess();
};

/************************************************************************/
/* �������͹ؿ�                                                          */
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
    CFMatch* newMatch();//��ͬ������Ϸ�У�match��slot��Ϊ���ܲ�һ��
    CFSlot* newSlot();
    CCSprite* newTipBoard();
    bool initSlots(lua_State* L);
    //��Ϸ��menu����¼�
    void onNextStepMenu(CCObject* pSender);
    void onPreStepMenu(CCObject* pSender);
    void onSubmitMenu(CCObject* pSender);
public:
    CFSquareLevel(CFGameLayer* pGameLayer);
    virtual ~CFSquareLevel(void);
    bool load(lua_State* L);
    bool draw();
    bool unload();
    //���������
    void doLevelAction(int fromslotid, int toslotid);
    //�������ʾ
    void undoLevelAction(int fromslotid, int toslotid);
    bool checkSuccess();
};

#endif
