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


#include "mainstate.h"

using namespace CE3D::Lua;

MainState::MainState() {

}

MainState::~MainState() {
}

bool MainState::init() {
    if ( !open() ) {
        return false;
    }

    addPackagePath ( ";./core/?.lua" );
    addPackagePath ( ";./core/lua/?.lua" );
    addPackagePath ( ";./game/?.lua" );

    createGlobalTable ( "ce3d" );

    /*create ce3d table*/

    createTable ( 0, 0 );
    setGlobal ( "CE3D" );
//    getGlobal ( "CE3D" );
//    LuaCETable = MainLuaStat.ref();

    return true;
}

bool MainState::callMain() {
    doFile ( "./core/scripts/main.lua" );

    onClientConnectRef =  getGlobalRef( "onClientConnect" );
    onClientDisonnectRef =  getGlobalRef( "onClientDisconnect" );

    onUpdateRef = getGlobalRef( "onUpdate" );
    onNetCallRef = getGlobalRef( "onNetCall" );

    callGlobal("main");
}


void MainState::onClientConnect ( int id ) {
    callRef ( onClientConnectRef, id );
}

void MainState::onClientDisonnect ( int id ) {
    callRef ( onClientDisonnectRef, id );
}

void MainState::onNetCall ( int type, int ent, const char* cmd, const char* args_format, CE3D::ByteBuffer* args ) {
    lua_State *s = getLuaState();
    lua_rawgeti ( s, LUA_REGISTRYINDEX, this->onNetCallRef );

    lua_pushinteger ( s, type );
    lua_pushinteger ( s, ent );
    lua_pushstring ( s, cmd );

    int arg_c = 3;
    int i = 0;
    char c = 0;
    while ( ( c = args_format[i++] ) != '\0' ) {
        switch ( c ) {
            case 'i': {
                arg_c++;
                int i = args->read<int>();
                lua_pushinteger ( s, i );
                break;
            }

            case 'f': {
                arg_c++;
                float f = args->read<float>();
                lua_pushnumber ( s, f );
                break;
            }
        }
    }

    if ( lua_pcall ( s, arg_c, 0, 0 ) != 0 ) {
        fprintf ( stderr, "error running function : %s\n",
                  lua_tostring ( s, -1 ) );
    }
}

void MainState::onUpdate ( int dt ) {
    callRef ( onUpdateRef, dt );
}

