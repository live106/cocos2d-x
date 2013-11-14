#ifndef _CCB_CCSPRITEBATCHNODELOADER_H_
#define _CCB_CCSPRITEBATCHNODELOADER_H_

#include "CCNodeLoader.h"

NS_CC_EXT_BEGIN

/* Forward declaration. */
class CCBReader;

class CCSpriteBatchNodeLoader : public CCNodeLoader {
public:
    virtual ~CCSpriteBatchNodeLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCSpriteBatchNodeLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCSpriteBatchNode);
    
    virtual void onHandlePropTypeTextureAtlas(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCTextureAtlas * pCCTextureAtlas, CCBReader * pCCBReader);
};

NS_CC_EXT_END

#endif
