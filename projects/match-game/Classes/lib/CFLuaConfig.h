#ifndef __CFLUACONFIG_H__
#define __CFLUACONFIG_H__

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class CFLuaConfig
{
private:
    lua_State* L;
public:
    CFLuaConfig(const char* file);
    const char* loadString(const char* key);
    int loadInteger(const char* key);
    double loadDouble(const char* key);
    bool loadBool(const char* key);
    virtual ~CFLuaConfig(void);
};
#endif
