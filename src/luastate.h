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

                virtual ~LuaState() {
                    close();
                }

                inline bool isOpen() {
                    return ( pState != nullptr );
                }
                inline bool isClose() {
                    return !isOpen();
                }

                inline lua_State* getLuaState() const {
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

                void removeRef( int ref );

            protected:
                virtual void initState();

            private:
                lua_State *pState;
                std::string LastError;
        };

        class LuaLib {
        public:
            virtual void registerTo( const LuaState& state ) = 0;
            virtual const char *getName() = 0;
        };
    }
}


#endif // LUASTATE_H
