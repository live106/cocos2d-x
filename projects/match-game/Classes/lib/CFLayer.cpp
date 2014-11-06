#include "CFLayer.h"

CFLayer::CFLayer(){
}


CFLayer::~CFLayer(){
}

bool CFLayer::init(){
    if(CCLayer::init()){
        return true;
    }
    return false;
}


CFDragableLayer::CFDragableLayer(){

}

CFDragableLayer::~CFDragableLayer(){

}

bool CFDragableLayer::init(){
    if(CFLayer::init()){
        return true;
    }
    return false;
}
