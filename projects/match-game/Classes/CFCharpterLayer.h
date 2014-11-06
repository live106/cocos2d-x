#ifndef __CFCHARPTERLAYER_H__
#define __CFCHARPTERLAYER_H__


#include "cocos2d.h"
#include "CFLevelsLayer.h"
#include "lib/CFGameEngine.h"
#include "lib/CFSprite.h"
#include "CFAbsLayer.h"

using namespace cocos2d;

class CFCharpterLayer:public CFAbsLayer
{
private:
    CCSprite* charpters[3];
    CCPoint originpos[3];
    int beginex;
    int currentx;
    int winwidth;
    int winheight;
    int charptercount;
public:
    CFCharpterLayer(void);
    virtual ~CFCharpterLayer(void);
    //CFCharpter::node()����ʵ��
    virtual bool init();
    static CCScene* initScene();
    virtual void onEnter();
    virtual void onExit();
    LAYER_NODE_FUNC(CFCharpterLayer);
    //������ز˵�
    void onBackToMenu(CCObject* sender);
    //��Touch�¼�ת��Ϊ�л��¼�
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent *event);
    //�½����룬�л���ѡ�����
    virtual void initCharpters(int current);
    virtual CCSprite* createCharpterSprite(int index);
    virtual void deleteCharpter(int level);
    void toPreviousCharpter();
    void toNextCharpter();
    void toCurrentCharpter();
    void onSelectCharpter(CCObject* pSender);
    void resetCharpterPosition();
};
#endif
