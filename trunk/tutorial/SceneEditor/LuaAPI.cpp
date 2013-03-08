/*
** Lua binding: LuaAPI
*/

#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
 extern "C" int tolua_bnd_takeownership (lua_State* L); // from tolua_map.c
#else
 int tolua_bnd_takeownership (lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

/* Exported function */
TOLUA_API int tolua_LuaAPI_open (lua_State* tolua_S);
LUALIB_API int luaopen_LuaAPI (lua_State* tolua_S);

#include "Global.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_tstring (lua_State* tolua_S)
{
 tstring* self = (tstring*) tolua_tousertype(tolua_S,1,0);
 tolua_release(tolua_S,self);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Global");
 tolua_usertype(tolua_S,"tstring");
}

/* method: getDataRootDirectory of class  Global */
static int tolua_LuaAPI_Global_getDataRootDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Global",0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Global* self = (Global*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDataRootDirectory'",NULL);
#endif
 {
  tstring tolua_ret =  self->getDataRootDirectory();
 {
#ifdef __cplusplus
 void* tolua_obj = new tstring(tolua_ret);
 tolua_pushusertype(tolua_S,tolua_clone(tolua_S,tolua_obj,tolua_collect_tstring),"tstring");
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(tstring));
 tolua_pushusertype(tolua_S,tolua_clone(tolua_S,tolua_obj,NULL),"tstring");
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDataRootDirectory'.",&tolua_err);
 return 0;
#endif
}

/* method: setDataRootDirectory of class  Global */
static int tolua_LuaAPI_Global_setDataRootDirectory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Global",0,&tolua_err) || 
 !tolua_isstring(tolua_S,2,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Global* self = (Global*)  tolua_tousertype(tolua_S,1,0);
  const char* s = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDataRootDirectory'",NULL);
#endif
 {
  self->setDataRootDirectory(s);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDataRootDirectory'.",&tolua_err);
 return 0;
#endif
}

/* method: setHero of class  Global */
static int tolua_LuaAPI_Global_setHero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Global",0,&tolua_err) || 
 !tolua_isstring(tolua_S,2,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Global* self = (Global*)  tolua_tousertype(tolua_S,1,0);
  const char* resID = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setHero'",NULL);
#endif
 {
  self->setHero(resID);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setHero'.",&tolua_err);
 return 0;
#endif
}

/* function: createGlobal */
static int tolua_LuaAPI_createGlobal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  createGlobal();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createGlobal'.",&tolua_err);
 return 0;
#endif
}

/* function: getGlobal */
static int tolua_LuaAPI_getGlobal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Global* tolua_ret = (Global*)  getGlobal();
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Global");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGlobal'.",&tolua_err);
 return 0;
#endif
}

/* Open lib function */
LUALIB_API int luaopen_LuaAPI (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_cclass(tolua_S,"Global","Global","",NULL);
 tolua_beginmodule(tolua_S,"Global");
 tolua_function(tolua_S,"getDataRootDirectory",tolua_LuaAPI_Global_getDataRootDirectory00);
 tolua_function(tolua_S,"setDataRootDirectory",tolua_LuaAPI_Global_setDataRootDirectory00);
 tolua_function(tolua_S,"setHero",tolua_LuaAPI_Global_setHero00);
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"createGlobal",tolua_LuaAPI_createGlobal00);
 tolua_function(tolua_S,"getGlobal",tolua_LuaAPI_getGlobal00);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_LuaAPI_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_LuaAPI);
 lua_pushstring(tolua_S, "LuaAPI");
 lua_call(tolua_S, 1, 0);
 return 1;
}
