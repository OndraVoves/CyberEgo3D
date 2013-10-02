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


#include "luastate.h"

using namespace CE3D::Lua;

bool LuaState::open() {
    if( isOpen() ) {
        close();
    }

    pState = lua_open();
    if( !pState ) {
        return false;
    }



    initState( );
    return true;
}

void LuaState::close() {
    lua_close( this->pState );
}

bool LuaState::doFile( const char* filename ) {
    int error = luaL_dofile( this->pState, filename );
    if( error ) {
        LastError = lua_tostring( this->pState, -1);
        lua_pop(this->pState, 1);

        fprintf( stderr, "%s\n", this->LastError.c_str() );
    }

    return error;
}

void LuaState::addPackagePath( const char* path ) {
    // TODO: prasarna
    std::string cmd = "package.path = package.path .. \";";
    cmd.append( path );
    cmd.append( "\"" );

    luaL_dostring( this->pState, cmd.c_str() );
}

void LuaState::initState( ) {
    luaL_openlibs( this->pState );
}

void LuaState::callGlobal ( const char* name ) {
    lua_getglobal( this->pState, name );

    if (lua_pcall( this->pState, 0, 0, 0) != 0) {
        fprintf( stderr, "error running function : %s\n",
                 lua_tostring(this->pState, -1) );
    }
}

void LuaState::callGlobal ( const char* name, float value ) {
    lua_getglobal( this->pState, name );

    lua_pushnumber( this->pState, value );

    if (lua_pcall( this->pState, 1, 0, 0) != 0) {
        fprintf( stderr, "error running function : %s\n",
                 lua_tostring(this->pState, -1) );
    }
}
