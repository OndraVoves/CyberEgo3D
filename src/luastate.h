/*
    Copyright (c) 2013, <copyright holder> <email>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef LUASTATE_H
#define LUASTATE_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#ifdef __cplusplus
}
#endif

namespace CE3D {
    namespace Lua {
        class LuaState {
            public:
                LuaState() : pState ( nullptr ) {
                }

                LuaState ( lua_State *state ) : pState ( state ) {
                }

                virtual ~LuaState() {
                    close();
                }

                inline bool isOpen() {
                    return ( pState != nullptr );
                }
                inline bool isClose() {
                    return !isOpen();
                }

                inline lua_State *getLuaState() const {
                    return this->pState;
                }

                bool open();
                void close();
                bool doFile ( const char *filename );
                void addPackagePath ( const char *path );

                void callGlobal ( const char *name );
                void callGlobal ( const char *name, float value );

                void createGlobalTable ( const char *name );

                /* Referenced */
            public:
                int getGlobalRef ( const char *name );
                int getTableItemRef ( int table_ref, const char *item );

                void callRef ( int ref );
                void callRef ( int ref, int value );
                void callRef ( int ref, float value );

                void removeRef ( int ref );

            public:
                inline void rregister ( const char *libname, const luaL_Reg *l ) {
                    luaL_register ( this->pState, libname, l );
                }

                inline void getGlobal ( const char *name ) {
                    lua_getglobal ( this->pState, name );
                }

                inline void setGlobal ( const char *name ) {
                    lua_setglobal ( this->pState, name );
                }

                inline void createTable ( const int narr, const int nrec ) {
                    lua_createtable ( this->pState, narr, nrec );
                }

                inline void setTable ( const int index ) {
                    lua_settable ( this->pState, index );
                }

                inline void setField ( const int index, const char *key ) {
                    lua_setfield ( this->pState, index, key );
                }

                inline void getTable ( const int index ) {
                    lua_gettable ( this->pState, index );
                }

                inline void getField ( const int index, const char *key ) {
                    lua_getfield ( this->pState, index, key );
                }

                inline void rawGetI ( int index, int n ) {
                    lua_rawgeti ( this->pState, index, n );
                }

                /* PUSH */
            public:
                inline void pushNil() {
                    lua_pushnil ( this->pState );
                }

                inline void pushBool ( const bool b ) {
                    lua_pushboolean ( this->pState, b );
                }

                inline void pushNumber ( const double n ) {
                    lua_pushnumber ( this->pState, n );
                }

                inline void pushInt ( const int i ) {
                    lua_pushinteger ( this->pState, i );
                }

                inline void pushStr ( const char *str ) {
                    lua_pushstring ( this->pState, str );
                }

                inline void pushStr ( const char *str, const size_t len ) {
                    lua_pushlstring ( this->pState, str, len );
                }

                /* IS */
            public:
                inline bool isNil ( const int index ) {
                    return lua_isnil ( this->pState, index );
                }

                inline bool isBool ( const int index ) {
                    return lua_isboolean ( this->pState, index );
                }

                inline bool isNumber ( const int index ) {
                    return lua_isnumber ( this->pState, index );
                }

                inline bool isStr ( const int index ) {
                    return lua_isstring ( this->pState, index );
                }

                inline bool isTable ( const int index ) {
                    return lua_istable ( this->pState, index );
                }


                /* CHECK */
            public:
                inline int checkInt ( const int narg ) {
                    return luaL_checkint ( this->pState, narg );
                }

                inline double checkNumber ( const int narg ) {
                    return luaL_checknumber ( this->pState, narg );
                }

                inline const char *checkStr ( const int narg ) {
                    return luaL_checkstring ( this->pState, narg );
                }

                inline const char *checkStr ( const int narg, size_t *len ) {
                    return luaL_checklstring ( this->pState, narg, len );
                }

                /* STACK OP */
            public:
                inline int getTop() {
                    return lua_gettop ( this->pState );
                }

                inline void setTop ( const int index ) {
                    lua_settop ( this->pState, index );
                }

                inline void pushValue ( const int index ) {
                    lua_pushvalue ( this->pState, index );
                }

                inline void pop ( const int index ) {
                    setTop ( - ( index ) - 1 );
                }

                inline void remove ( const int index ) {
                    lua_remove ( this->pState, index );
                }

                inline void insert ( const int index ) {
                    lua_insert ( this->pState, index );
                }

                inline void replace ( const int index ) {
                    lua_replace ( this->pState, index );
                }

                /* REGISTR */
            public:
                inline void rawGeti ( const int ref, const int index = LUA_REGISTRYINDEX ) {
                    lua_rawgeti ( this->pState, index, ref );
                }

                inline int ref ( const int index = LUA_REGISTRYINDEX ) {
                    return luaL_ref ( this->pState, index );
                }

                inline void unref ( int ref, const int index = LUA_REGISTRYINDEX  ) {
                    luaL_unref ( this->pState, index, ref );
                }


                /* CALL */
            public:
                typedef enum {
                    NOERR  = 0,
                    ERRRUN = LUA_ERRRUN,
                    ERRMEM = LUA_ERRMEM,
                    ERRERR = LUA_ERRERR
                } call_err;

                inline call_err pcall ( const int nargs, const int nresults, const int errfunc = 0 ) {
                    return call_err ( lua_pcall ( this->pState, nargs, nresults, errfunc ) );
                }

            protected:
                virtual void initState();

            private:
                lua_State *pState;
                std::string LastError;
        };

        class LuaLib {
            public:
                virtual void init ( LuaState &state, const int ce3d_ref ) {};
        };
    }
}


#endif // LUASTATE_H
