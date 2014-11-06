#include "CFMatchGame.h"
#include "lib/CFGameEngine.h"
/************************************************************************/
/* MatchGame µœ÷
/************************************************************************/

void CFMatchGame::start(void){
    CFGameEngine::initDirector(kCCDeviceOrientationLandscapeLeft, false, 1.0/60);
    CCScene* loadingScene = CFLoadingLayer::initLoadingScene();
    CFGameEngine::runWithScene(loadingScene);
}


void CFMatchGame::printVersion()
{

}

void CFMatchGame::muteSound(){
    CCLog("mute sound");
}

void CFMatchGame::unMuteSound(){
    CCLog("unmute sound");
}

bool CFMatchGame::isSoundMuted(){
    return false;
}

