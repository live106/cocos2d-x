#include "CFScoreBoard.h"
#include <math.h>
inline int cun(int n){
   n=abs(n);
   if(n<10){
       return 1;
   }
   return cun(n/10)+1;
}

CFScoreBoard::CFScoreBoard(void):scorePerSec(-10),width(0),height(0)
{
}


CFScoreBoard::~CFScoreBoard(void)
{
}

bool CFScoreBoard::init(int score){
    if(!CCSprite::init()){
        return false;
    }
    this->score = score;
    CCSize size = this->getContentSize();
    width = size.width;
    height = size.height;
    this->schedule(SEL_SCHEDULE(&CFScoreBoard::doAutoScore), 1);
    return true;
}

//��ʱִ�з����Զ�����
void CFScoreBoard::doAutoScore(ccTime dt){
    if(scorePerSec == 0 && ((int)dt%60)!=0){
        return;
    }
    this->score+=scorePerSec;
    int num = cun(score);
    for(int i = 0; i < num;i++){
        int slot = pow(10.0,i);
        int digital = (score/slot)%10;
        char* buffer = new char[5];
        sprintf(buffer, "%d", digital);
        CCLabelBMFont* pLabel =(CCLabelBMFont*) this->getChildByTag(i);
        if(!pLabel){
            pLabel =  CCLabelBMFont::labelWithString(buffer,"fonts/fonts.fnt");
            CCSize labelsize = pLabel->getContentSize();
            pLabel->setPosition(ccp(width-i*20,height/2));
            pLabel->setTag(i);
            this->addChild(pLabel);
        }else{
            pLabel->setString(buffer);
        }
        this->doChangeAnimation(scorePerSec);
        delete[] buffer;
    }
}

//ִ��ȥ�������ֵĶ���
void CFScoreBoard::doChangeAnimation(int change){
    char* buffer = new char[5];
    sprintf(buffer, "%d", change);
    CCLabelBMFont* pLabel =  CCLabelBMFont::labelWithString(buffer,"fonts/fonts.fnt");
    CCSize size = this->getContentSize();
    CCPoint lpos = ccp(size.width/2, size.height/2);
    pLabel->setPosition(lpos);
    this->addChild(pLabel);
    CCMoveTo* pMove = CCMoveTo::actionWithDuration(0.5, ccp(lpos.x, lpos.y-100));
    CCFadeTo* pFade = CCFadeTo::actionWithDuration(0.5, 0);
    CCFiniteTimeAction* pMoveFade = CCSpawn::actions(pMove, pFade,NULL);
    CCDelayTime* pDelay = CCDelayTime::actionWithDuration(0.5);
    CCCallFunc* pFunc = CCCallFuncN::actionWithTarget(this,callfuncN_selector(CFScoreBoard::freeLabel));
    CCAction* pSeq = CCSequence::actions(pMoveFade, pDelay, pFunc,NULL);
    pLabel->runAction(pSeq);
}

//ɾ���ϱ�ǩ�������ڴ�
void CFScoreBoard::freeLabel(CCNode* pOldLabel){
    pOldLabel->removeFromParentAndCleanup(true);
}

//���÷���ÿ���Զ�����ֵ��ͨ��������0�������ӣ����٣�����
void CFScoreBoard::setAutoScore(int scorePerSec){
    this->scorePerSec = scorePerSec;
}

//һ���Ե����ӷ���
void CFScoreBoard::addScore(int score){
    
}

//һ���Եؼ��ٷ���
void CFScoreBoard::removeScore(int score){

}