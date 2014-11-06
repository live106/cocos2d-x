#include "CFConfiguration.h"

#include "cocos2d.h"
using namespace cocos2d;

CFConfiguration::CFConfiguration(void):dict(NULL)
{
    string path = this->getFilePath("game.plist");
    dict = CCFileUtils::dictionaryWithContentsOfFile(path.c_str());
}

CFConfiguration::~CFConfiguration(void)
{
    if(dict){
        delete dict;
    }
}

void CFConfiguration::setStr(string key, string value){
    if(!this->dict){
        return;
    }
    CCString* obj = (CCString*)this->dict->objectForKey(key);
    if(obj){
        obj->m_sString=value.c_str();
    }else{
        CCString* str = new CCString(value.c_str());
        this->dict->setObject(str, key);
    }
}

void CFConfiguration::setInt(string key, int value){
    if(!this->dict){
        return;
    }
    char* num = new char[10];
    sprintf(num, "%d",value);
    CCString* obj = (CCString*)this->dict->objectForKey(key);
    if(obj){
        obj->m_sString=num;
    }else{
        CCString* value = new CCString(num);
        this->dict->setObject(value, key);
    }
    delete[] num;
}

CCString* CFConfiguration::getStr(string key){
    if(!this->dict){
        return NULL;
    }
    return (CCString*)dict->objectForKey(key);
}

int CFConfiguration::getInt(string key, int default){
    if(!this->dict){
        return default;
    }
    CCString* str = (CCString*)dict->objectForKey(key);
    if(!str){
        return default;
    }
    return atoi(str->toStdString().c_str());
}

void CFConfiguration::writeKeyValue(TiXmlElement* root, string key, string value){
    TiXmlElement* ele_key = new TiXmlElement("key");
    ele_key->LinkEndChild(new TiXmlText(key.c_str()));
    root->LinkEndChild(ele_key);
    TiXmlElement* ele_value = new TiXmlElement("string");
    ele_value->LinkEndChild(new TiXmlText(value.c_str()));
    root->LinkEndChild(ele_value);
}

void CFConfiguration::writeKeyValue(TiXmlElement* root, string key, TiXmlElement* value){
    TiXmlElement* ele_key = new TiXmlElement("key");
    ele_key->LinkEndChild(new TiXmlText(key.c_str()));
    root->LinkEndChild(ele_key);
    root->LinkEndChild(value);
}

void CFConfiguration::writeToFile(){
   if(!this->dict){
       return;
   }
   TiXmlDocument* doc = new TiXmlDocument();
   TiXmlDeclaration* declare = new TiXmlDeclaration("1.0","UTF-8","");
   doc->LinkEndChild(declare);
   TiXmlElement* ele_plist = new TiXmlElement("plist");
   ele_plist->SetAttribute("version", "1.0");
   doc->LinkEndChild(ele_plist);
   TiXmlElement* ele_dict = new TiXmlElement("dict");
   ele_plist->LinkEndChild(ele_dict);
   vector<string> keys = this->dict->allKeys();
   for(vector<string>::iterator it = keys.begin();it!=keys.end();it++){
      string key = it->c_str();
      CCString* value = (CCString*)this->dict->objectForKey(key);
      this->writeKeyValue(ele_dict, key, value->toStdString());
   }
   doc->SaveFile(this->getFilePath("game.plist").c_str());
   delete doc;
}

string CFConfiguration::getFilePath(string name)
{
    string path("");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // In android, every programe has a director under /data/data.
    // The path is /data/data/ + start activity package name.
    // You can save application specific data here.
    path.append("/data/data/org.cocos2dx.application/");
    path.append(name);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // You can save file in anywhere if you have the permision.
    path = CCFileUtils::fullPathFromRelativePath(name.c_str());
    //path.append("D:/");
    //path.append(name);
#endif
    return path;
}

