#ifndef __CFSPRITE_H__
#define __CFSPRITE_H__

#include "cocos2d.h"

using namespace cocos2d;

typedef enum tagCFTouchableState
{
    stateGrabbed,
    stateUngrabbed
} CFTouchableState; 

/************************************************************************/
/* ���Sprite�������串�ǵ�����Sprite��ʹ������Sprite�������¼�            */
/************************************************************************/
class CFUnTouchableSprite:public CCSprite, public CCTargetedTouchDelegate

{
public:
    CFUnTouchableSprite(void);
    virtual ~CFUnTouchableSprite(void);
    virtual void onEnter();
    virtual void onExit();
    virtual CCRect rect();//����ʵ��
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent *event);
};

/************************************************************************/
/* ��װ�˴����¼���Sprite                                                */
/************************************************************************/
class CFTouchableSprite:public CCSprite, public CCTargetedTouchDelegate{
private:
    CFTouchableState t_state;
public:
    CFTouchableSprite(void);
    virtual ~CFTouchableSprite(void);
    virtual void onEnter();
    virtual void onExit();
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchMoved(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void ccTouchCancelled(CCTouch *touch, CCEvent *event);
    virtual CCRect rect();
    virtual bool cfTouchBegan(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchMoved(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchEnded(CCTouch* touch, CCEvent* event) = 0;
    virtual void cfTouchCancelled(CCTouch *touch, CCEvent *event) = 0;
};

/************************************************************************/
/* ͨ��һ�����һ�����Σ��Ե�λ���ģ�����Ϊ�߽磬��ʾ�������Ƶ�Sprite       */
/************************************************************************/
class CFRadiationSprite:public CCSprite{
private:
    bool start;
    bool warning;
    CCSize size;
    void draw();
    void setWarningColors(ccColor4F* colors);
    void setNormalColors(ccColor4F* colors);
public:
    CFRadiationSprite();
    virtual ~CFRadiationSprite(void);
    virtual bool init(CCSize size);
    LAYER_NODE_FUNC_PARAM(CFRadiationSprite,CCSize, size);    
    void startAnimation();
    void stopAnimation();
    void startWarning(float time);
    void stopWarning();
};

/************************************************************************/
/* һ��ˮƽ����ģ��Sprite                                                */
/************************************************************************/
class CFHorizonWaterLinesSprite:public CCSprite{
private:
    bool start;
    bool warning;
    CCSize size;
    void draw();
    ccColor4F getWarningColors();
    ccColor4F getNormalColors();
public:
    CFHorizonWaterLinesSprite();
    virtual ~CFHorizonWaterLinesSprite(void);
    virtual bool init(CCSize size);
    LAYER_NODE_FUNC_PARAM(CFHorizonWaterLinesSprite, CCSize, size);
    void startAnimation();
    void stopAnimation();
};
#endif
