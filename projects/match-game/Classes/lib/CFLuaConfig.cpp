#include "CFLuaConfig.h"

CFLuaConfig::CFLuaConfig(const char* file){
    L=lua_open();
    luaL_openlibs(L);
    if(luaL_dofile(L, file)){
        return;
    }
}

CFLuaConfig::~CFLuaConfig(void)
{
    lua_close(L);
}

bool CFLuaConfig::loadBool(const char* key){
    lua_getglobal(L, key);
    if(lua_isboolean(L, -1)){
        return lua_toboolean(L, -1);
    }
    return false;
}

int CFLuaConfig::loadInteger(const char* key){
    lua_getglobal(L, key);
    if(lua_isnumber(L, -1)){
        return (int) lua_tointeger(L, -1);
    }
    return 0;
}

const char* CFLuaConfig::loadString(const char* key){
    lua_getglobal(L, key);
    if(lua_isstring(L, -1)){
        return (const char*) lua_tostring(L, -1);
    }
    return NULL;
}

double CFLuaConfig::loadDouble(const char* key){
    lua_getglobal(L, key);
    if(lua_isnumber(L,-1)){
        return (double) lua_tonumber(L, -1);
    }
    return 0.0;
}
