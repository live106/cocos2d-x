#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__
#include "cocos2d.h"
#include <string.h>
#include "CFMatch.h"
#include "CFSlot.h"
#include "CFLevelLoader.h"
#include "lib/CFSprite.h"
#include "lib/CFScoreBoard.h"
#include "CFAbsLayer.h"
#include <math.h>
#define PI 3.1415926

using namespace cocos2d;

class CFLevelLoader;
class CFLevel;
class CFMatchGame;
class CFGameLayer;

class CFTipBoardSprite:public CCSprite{
private:
    CCLabelBMFont* pLabel;
    CCSprite* pLabelBoard;
public:
    CFTipBoardSprite();
    virtual ~CFTipBoardSprite(void);
    virtual bool init();
    LAYER_NODE_FUNC(CFTipBoardSprite);
    void showTips(CFGameLayer* pLayer, const char* tips, float time);
    void hideTips(CFGameLayer* pLayer);
};

class PauseBoardSprite:public CFUnTouchableSprite{
private:
    CCSprite* pBoardWood;
public:
    PauseBoardSprite();
    virtual ~PauseBoardSprite(void);
    virtual bool init();
    virtual bool initBoardWood(CFGameLayer* pLayer);
    virtual bool doPause(CFGameLayer* pLayer);
    virtual bool doResume(CFGameLayer* pLayer);
    LAYER_NODE_FUNC(PauseBoardSprite);
};

class SuccessBoardSprite:public CFUnTouchableSprite{
private:
    CCSprite* pBoardStone;
public:
    SuccessBoardSprite(void);
    virtual ~SuccessBoardSprite(void);
    virtual bool init();
    LAYER_NODE_FUNC(SuccessBoardSprite);
    virtual bool initBoardStone(CFGameLayer* pLayer);
    virtual bool doShow(CFGameLayer* pLayer, int score);//��ʾ��Ϸ�ɹ��Ķ���
    virtual bool doHide(CFGameLayer* pLayer);//����ͨ�ؿ��Ķ���
};

class TimerSprite:public CFUnTouchableSprite{
    TimerSprite();
    virtual ~TimerSprite();
    virtual bool init();
    LAYER_NODE_FUNC(TimerSprite);
};

/************************************************************************/
/* ��Ϸ������ͼ��
/************************************************************************/
class CFGameLayer:public CFAbsLayer{
private:
    int charpter;
    int level;
    int score;
    string levelfile;
    //��Ϸ�����м�����̬sprite
    CCMenuItemSprite* pPause;
    PauseBoardSprite* pPauseBoard;
    SuccessBoardSprite* pSuccessBoard;
    CCProgressTimer* pTimer;
    CFTipBoardSprite* pTips;
    CFScoreBoard* pScoreBoard;
    //�ؿ�������
    CFLevelLoader* pLevelLoader;
    CCRect getMatchRect(CCSprite* match);
    //����
    void showTips(const char* tips, float during);
    void hideTips();
public:
    CFGameLayer();
    virtual ~CFGameLayer();
    virtual bool init();
    LAYER_NODE_FUNC(CFGameLayer);
	static CCScene* initGameScene(int charpter, int level);
	//������س��ؿ�
	void loadLevel(int charpter, int level);
	void unloadLevel();
    //ˢ����Ļ
	virtual void update(ccTime dt);
    //�¼�����
    virtual void onReturnMenu(CCObject* pSender);
    virtual void onNextMenu(CCObject* pSender);
    virtual void onSubmitMenu(CCObject* pSender);
    virtual void onDelMenu(CCObject* pSender);
    virtual void onRuleMenu(CCObject* pSender);
    virtual void onResetMenu(CCObject* pSender);
    virtual void onForwardMenu(CCObject* pSender);
    virtual void onBackwardMenu(CCObject* pSender);
    virtual void onPauseMenu(CCObject* pSender);
    virtual void onResumeMenu(CCObject* pSender);
    virtual void onBackToLevelsMenu(CCObject* pSender);
    virtual void onMuteSound(CCObject* pSender);
    virtual void onGameSuccess();
    virtual void onGameFailed();
};
#endif