#ifndef __CFScoreBoard_H__
#define __CFScoreBoard_H__
#include "cocos2d.h"

using namespace cocos2d;

class CFScoreBoard:public CCSprite
{
private:
    int score;
    int subtract;
    int scorePerSec;
    int width;
    int height;
    void CFScoreBoard::doChangeAnimation(int change);
    void freeLabel(CCNode* pOldLabel);
public:
    CFScoreBoard(void);
    virtual ~CFScoreBoard(void);
    virtual bool init(int score);
    LAYER_NODE_FUNC_PARAM(CFScoreBoard,int,score);
    //为0表示不变，为正数每秒递增，为负数每秒递减
    void setAutoScore(int scorePerSec);
    void addScore(int score);
    void removeScore(int score);
    void doAutoScore(ccTime dt);
};
#endif
