//
//  CCSpriteBatchNodeLoader.cpp
//  Cocos2dxJS
//
//  Created by Nick Verigakis on 28/01/2013.
//
//

#include "CCSpriteBatchNodeLoader.h"
#define PROPERTY_TEXTURE "textureAtlas"

NS_CC_EXT_BEGIN

void CCSpriteBatchNodeLoader::onHandlePropTypeTextureAtlas(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCTextureAtlas * pCCTextureAtlas, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_TEXTURE) == 0) {
        ((CCSpriteBatchNode *)pNode)->setTextureAtlas(pCCTextureAtlas);
    } else {
        CCNodeLoader::onHandlePropTypeTextureAtlas(pNode, pParent, pPropertyName, pCCTextureAtlas, pCCBReader);
    }
}

NS_CC_EXT_END
