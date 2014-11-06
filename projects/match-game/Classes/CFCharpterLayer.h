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
    //CFCharpter::node()创建实例
    virtual bool init();
    static CCScene* initScene();
    virtual void onEnter();
    virtual void onExit();
    LAYER_NODE_FUNC(CFCharpterLayer);
    //点击返回菜单
    void onBackToMenu(CCObject* sender);
    //将Touch事件转换为切换事件
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent *event);
    //章节载入，切换，选择操作
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
