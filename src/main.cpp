#include <stdio.h>
#include <string.h>

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
            LuaState() : pState(nullptr) {
            }

            virtual ~LuaState() {
                close();
            }

            inline bool isOpen() { return (pState != nullptr); }
            inline bool isClose() { return !isOpen(); }

            bool open() {
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

            bool close() {
                lua_close( this->pState );
            }

            bool doFile( const char* filename ) {
                int error = luaL_dofile( this->pState, filename );
                if( error ) {
                    LastError = lua_tostring( this->pState, -1);
                    lua_pop(this->pState, 1);

                    fprintf( stderr, "%s\n", this->LastError.c_str() );
                }

                return error;
            }

        protected:
            virtual void initState( ) {
                luaL_openlibs( this->pState );
                addPackagePath( "./core/lua/?.lua;./core/components/?.lua" );
            }

            void addPackagePath( const char* path ) {
                // TODO: prasarna
                std::string cmd = "package.path = package.path .. \";";
                cmd.append( path );
                cmd.append( "\"" );

                luaL_dostring( this->pState, cmd.c_str() );
            }

        private:
            lua_State *pState;
            std::string LastError;
    };
}
}


int main( int argc, const char* argv[] ) {
    CE3D::Lua::LuaState L;

    L.open();
    L.doFile( "./core/main.lua" );

    return 0;
}
