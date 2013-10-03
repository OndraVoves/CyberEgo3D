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


#include "scenenodeapi.h"
#include "../kernel.h"
#include "Ogre.h"

using namespace CE3D;

static int newSceneNode ( lua_State *L ) {
    size_t node_name_len = 0;
    const char *node_name = luaL_checklstring ( L, 1, &node_name_len );

    Ogre::SceneNode *node = Kernel::inst().
                            getOGRESceneMgr()->getRootSceneNode()->createChildSceneNode ( node_name );

    lua_pushlightuserdata ( L, node );
    return 1;
}

static int delSceneNode ( lua_State *L ) {
    Ogre::SceneNode *node = static_cast<Ogre::SceneNode *> ( lua_touserdata ( L, 1 ) );
    delete node;

    return 0;
}

static int setPosition ( lua_State *L ) {
    Ogre::SceneNode *node = static_cast<Ogre::SceneNode *> ( lua_touserdata ( L, 1 ) );
    float x = luaL_checknumber ( L, 2 );
    float y = luaL_checknumber ( L, 3 );
    float z = luaL_checknumber ( L, 4 );

    printf ( "%f, %f, %f\n", x, y, z );

    node->setPosition ( x, y, z );

    return 0;
}

static int setDirection ( lua_State *L ) {
    Ogre::SceneNode *node = static_cast<Ogre::SceneNode *> ( lua_touserdata ( L, 1 ) );
    float x = luaL_checknumber ( L, 2 );
    float y = luaL_checknumber ( L, 3 );
    float z = luaL_checknumber ( L, 4 );

    node->setDirection ( Ogre::Vector3 ( x, y, z ) );

    return 0;
}

static int setVisible ( lua_State *L ) {
    Ogre::SceneNode *node = static_cast<Ogre::SceneNode *> ( lua_touserdata ( L, 1 ) );

    int visible = lua_toboolean ( L, 2 );

    node->setVisible ( visible );

    return 0;
}

static luaL_reg lib[] = {
    {"new", newSceneNode },
    {"del", delSceneNode },
    {"setPosition", setPosition },
    {"setDirection", setDirection },
    {"setVisible", setVisible },
    {NULL, NULL}
};

luaL_reg *SceneNodeAPI::getLuaReg() {
    return lib;
}

const char *SceneNodeAPI::getName() {
    return "OGRESceneNode";
}
