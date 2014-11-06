#include "CFSprite.h"
#include "lib/CFGameEngine.h"
#include <math.h>
/************************************************************************/
/* 对触摸事件进行屏蔽的Sprite对象                                         */
/************************************************************************/

CFUnTouchableSprite::CFUnTouchableSprite(void){
}

CFUnTouchableSprite::~CFUnTouchableSprite(void){

}

CCRect CFUnTouchableSprite::rect()
{
    CCSize s = this->getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

void CFUnTouchableSprite::onEnter()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, true);
    CCSprite::onEnter();
}

void CFUnTouchableSprite::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}	

bool CFUnTouchableSprite::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCRect rect = this->rect();
    CCPoint point = convertTouchToNodeSpace(touch);
    bool contains = CCRect::CCRectContainsPoint(rect, point);
    if(!contains) 
      return false;
    return true;
}

void CFUnTouchableSprite::ccTouchMoved(CCTouch* touch, CCEvent* event){
}

void CFUnTouchableSprite::ccTouchCancelled(CCTouch* touch, CCEvent* event){

}

void CFUnTouchableSprite::ccTouchEnded(CCTouch* touch, CCEvent* event){

}

/************************************************************************/
/* 允许触摸的Sprite对象，将触摸事件包装成cf开头的纯虚函数                  */
/************************************************************************/


CFTouchableSprite::CFTouchableSprite(void):t_state(stateUngrabbed){
    
}

CFTouchableSprite::~CFTouchableSprite(void){
    
}

CCRect CFTouchableSprite::rect()
{
    CCSize s = this->getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

void CFTouchableSprite::onEnter()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, false);
    CCSprite::onEnter();
}

void CFTouchableSprite::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}	

bool CFTouchableSprite::ccTouchBegan(CCTouch* touch, CCEvent* event){
    if (t_state != stateUngrabbed) 
        return false;
    CCPoint pos = convertTouchToNodeSpaceAR(touch);
    bool contains = CCRect::CCRectContainsPoint(rect(), pos);
    if ( !contains ) 
        return false;
    t_state = stateGrabbed;
    return this->cfTouchBegan(touch, event);
}

void CFTouchableSprite::ccTouchMoved(CCTouch* touch, CCEvent* event){
    CCAssert(t_state == stateGrabbed, L"Paddle - Unexpected state!");	
    this->cfTouchMoved(touch, event);
}

void CFTouchableSprite::ccTouchEnded(CCTouch* touch, CCEvent* event){
    CCAssert(t_state == stateGrabbed, L"Paddle - Unexpected state!");	
    t_state = stateUngrabbed;
    this->cfTouchEnded(touch, event);
}

void CFTouchableSprite::ccTouchCancelled(CCTouch *touch, CCEvent *event){
    CCAssert(t_state == stateGrabbed, L"Paddle - Unexpected state!");	
    t_state = stateUngrabbed;
    this->cfTouchCancelled(touch, event);
}

/************************************************************************/
/* 辐射条纹CFRadiationSprite的实现                                       */
/************************************************************************/
CFRadiationSprite::CFRadiationSprite():start(false),warning(false){

}

CFRadiationSprite::~CFRadiationSprite(){

}

bool CFRadiationSprite::init(CCSize size){
    if(!CCSprite::init()){
        return false;
    }
    this->size = size;
    return true;
}

void CFRadiationSprite::draw(){
    if(!this->start){
        return;
    }
    CCSize winsize = CFGameEngine::getWinSize();
    static int drawcount = 0;
    float radius = sqrt(pow(winsize.width/2,2)+pow(winsize.height,2))+5;
    float PI = 3.141592653;
    static int roate = 0;
    roate++;
    if(roate%160 == 0){//每40帧转动20度，完成一次转动循环
        roate = 0;
    }
    //初始化图形数据
    ccVertex2F vertices[23];
    vertices[0] = vertex2(0,0);
    vertices[1] = vertex2(0,0);
    for(int i = 0; i < 21;i++){
        vertices[i+2] = vertex2(radius*cos(i*PI/18-roate*PI/1440),radius*sin(i*PI/18-roate*PI/1440));
    }
    ccColor4F colors[23];
    if(this->warning){
        this->setWarningColors(colors);
    }else{
        this->setNormalColors(colors);
    }
    GLubyte faces[60];
    for(int i = 0; i < 20; i++){
        faces[i*3] = i%2?0:1;
        faces[i*3+1] = i+2;
        faces[i*3+2] = i+3;
    }
    //绘制
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    CCPoint pos = this->getPosition();
    //缩小视口，但绘制时，按照屏幕尺寸绘制，不调整横纵比例
    glViewport(pos.x-size.width/2,pos.y,size.width,size.height);
    glTranslatef(winsize.width/2,0,0.0f);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawElements(GL_TRIANGLES,60,GL_UNSIGNED_BYTE,faces);
    glViewport(0,0,winsize.width,winsize.height);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
}

void CFRadiationSprite::setWarningColors(ccColor4F* colors){
    colors[0] = ccc4FFromccc4B(ccc4(112,211,227,255));
    colors[1] = ccc4FFromccc4B(ccc4(112,211,227,200));
    for(int i = 0; i < 21;i++){
        colors[i+2] = ccc4FFromccc4B(ccc4(255,0,0,255));
    }
}

void CFRadiationSprite::setNormalColors(ccColor4F* colors){
    static bool addr = true;
    static int r = 0;
    r = addr?++r:--r;
    if(r==255||r==0){//循环变化底色
        addr=!addr;
    }
    static int g = 0;
    g=r==255?0:g;
    static int b = 0;
    b=b==255?0:b;
    colors[0] = ccc4FFromccc4B(ccc4(r,g,b,255));
    colors[1] = ccc4FFromccc4B(ccc4(r,g,b,200));
    for(int i = 0; i < 21;i++){
        colors[i+2] = ccc4FFromccc4B(ccc4(100,0,255,255));
    }
}

void CFRadiationSprite::startAnimation(){
    this->start = true;
}

void CFRadiationSprite::stopAnimation(){
    this->start = false;
}

void CFRadiationSprite::startWarning(float time){
   this->warning = true;
   CCCallFunc* pFunc = CCCallFunc::actionWithTarget(this, SEL_CallFunc(&CFRadiationSprite::stopWarning));
   CCDelayTime* pDelay = CCDelayTime::actionWithDuration(time);
   this->runAction(CCSequence::actions(pDelay, pFunc,NULL));
}

void CFRadiationSprite::stopWarning(){
    this->warning = false;
}

/************************************************************************/
/* 水平波纹sprite实现                                                    */
/************************************************************************/

CFHorizonWaterLinesSprite::CFHorizonWaterLinesSprite(){

}

CFHorizonWaterLinesSprite::~CFHorizonWaterLinesSprite(){

}

bool CFHorizonWaterLinesSprite::init(CCSize size){
    if(!CCSprite::init()){
        return false;
    }
    this->size = size;
}

void CFHorizonWaterLinesSprite::draw(){
    if(!this->start){
        return;
    }
    CCSize winsize = CFGameEngine::getWinSize();
    static int drawcount = 0;
    int linecount = 8;
    int splitlen = size.height/(linecount+1);
    //初始化图形数据
    ccVertex2F* vertices = new ccVertex2F[8+linecount*4];
    int offset = -drawcount;
    vertices[0]=vertex2(0,0);
    vertices[1]=vertex2(size.width, 0);
    for(int i = 0; i < linecount+1; i++){
        vertices[i*4+2] = vertex2(0, splitlen*(i+1)+offset);
        vertices[i*4+3] = vertex2(size.width, splitlen*(i+1)+offset);
        vertices[i*4+4] = vertex2(0, splitlen*(i+1)+offset);
        vertices[i*4+5] = vertex2(size.width, splitlen*(i+1)+offset);
    }
    vertices[6+linecount*4]=vertex2(0, size.height);
    vertices[6+linecount*4+1]=vertex2(size.width, size.height);
    static bool revert = true;
    ccColor4F colorA;
    ccColor4F colorB;
    if(revert){
        colorA = this->getNormalColors();
        colorB = this->getWarningColors();
    }else{
        colorB = this->getNormalColors();
        colorA = this->getWarningColors();
    }
    if(++drawcount > splitlen){
        drawcount=0;
        revert = revert?false:true;
    }
    ccColor4F* colors = new ccColor4F[8+linecount*4];
    colors[0]=colorA;
    colors[1]=colorA;
    for(int i = 0; i < linecount+1;i++){
        colors[i*4+2] = i%2?colorB:colorA;
        colors[i*4+3] = i%2?colorB:colorA;
        colors[i*4+4] = i%2?colorA:colorB;
        colors[i*4+5] = i%2?colorA:colorB;
    }
    colors[6+linecount*4] = linecount%2?colorA:colorB;
    colors[6+linecount*4+1] = linecount%2?colorA:colorB;

    GLubyte* faces = new GLubyte[linecount*6+12];
    for(int i = 0; i < linecount+2; i++){
        faces[i*6] = i*4;
        faces[i*6+1] = i*4+1;
        faces[i*6+2] = i*4+2;
        faces[i*6+3] = i*4+1;
        faces[i*6+4] = i*4+2;
        faces[i*6+5] = i*4+3;
    }
    //绘制
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    CCPoint pos = this->getPosition();
    glTranslatef(pos.x-size.width/2,pos.y-size.height/2,0);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawElements(GL_TRIANGLES,linecount*6+12,GL_UNSIGNED_BYTE,faces);
    glViewport(0,0,winsize.width,winsize.height);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    delete[] vertices;
    delete[] colors;
    delete[] faces;
}

ccColor4F inline CFHorizonWaterLinesSprite::getWarningColors(){
    static int g = 0;
    static int b = 0;
    static bool addr = true;
    static int r = 0;
    r = addr?++r:--r;
    if(r==255||r==0){//循环变化底色
        addr=!addr;
    }
    return ccc4FFromccc4B(ccc4(r,0,0,200));
}

ccColor4F inline CFHorizonWaterLinesSprite::getNormalColors(){
    static int g = 0;
    static int b = 0;
    static bool addr = true;
    static int r = 0;
    r = addr?++r:--r;
    if(r==255||r==0){//循环变化底色
        addr=!addr;
    }
    return ccc4FFromccc4B(ccc4(r,0,0,255));
}

void CFHorizonWaterLinesSprite::startAnimation(){
    this->start = true;
}

void CFHorizonWaterLinesSprite::stopAnimation(){
    this->start = false;
}