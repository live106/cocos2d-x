#ifndef __CFSLOT_H__
#define __CFSLOT_H__
#include "cocos2d.h"
#include "CFLevelLoader.h"
#include "lib/CFSprite.h"

using namespace cocos2d;

class CFLevel;
class CFMatch;

/************************************************************************/
/* ê�����
/************************************************************************/
typedef enum tagSlotState 
{
    kSlotStateUnMounted = 0,
    kSlotStateMounted = 1

} SlotState; 
class CFSlot:public CCSprite{
private:
    int mountMatchIndex;
    CFLevel* pLevel;
    int index;
public:
    CFSlot(CFLevel* pLevel);
    virtual ~CFSlot();
    int getRoate();
    CFLevel* getLevel();
    void setIndex(int index);
    int getIndex();
    //��Match�İ󶨹�ϵ
    void unmountMatch();
    void mountMatch(CFMatch* pMatch);
    bool hasMounted();
    CFMatch* getMountedMatch();
    //����CFSlotʵ��
    static CFSlot* slotWithFrame(CFLevel* pLevel, CCSpriteFrame* pFrame);
};

#endif