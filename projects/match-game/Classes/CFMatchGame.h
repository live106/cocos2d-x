#ifndef __CFMATCTGAME_H__
#define __CFMATCHGAME_H__
#include <string.h>
#include "cocos2d.h"

#include "CFGameLayer.h"
#include "CFLevelsLayer.h"
#include "CFLoadingLayer.h"
#include "lib/CFGameEngine.h"

using namespace cocos2d;

class CFMatchGame{
public:
    //friend class CFGameLayer;
    static void printVersion(void);
    static void start(void);
    static void muteSound();
    static void unMuteSound();
    static bool isSoundMuted();
};
#endif
