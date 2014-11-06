#ifndef __CFCONFIGURATION_H__
#define __CFCONFIGURATION_H__
#include "lib/tinyxml/tinyxml.h"

#include "cocos2d.h"
using namespace cocos2d;

class CFConfiguration:public CCObject
{
private:
    CCDictionary<std::string, CCObject*>* dict;
    string CFConfiguration::getFilePath(string name);
    virtual void writeKeyValue(TiXmlElement* root, string key, string value);
    virtual void writeKeyValue(TiXmlElement* root, string key, TiXmlElement* value);
public:
    CFConfiguration(void);
    virtual ~CFConfiguration(void);
    void setStr(string key, string value);
    void setInt(string key, int value);
    CCString* getStr(string key);
    int getInt(string key,int default);
    void writeToFile();
};
#endif
