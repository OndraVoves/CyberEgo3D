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


#include "camera.h"
#include "../kernel.h"

using namespace CE3D;

static int create( lua_State *L ) {
    size_t camera_name_len = 0;
    const char* camera_name = luaL_checklstring ( L, 1, &camera_name_len );

    Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(lua_touserdata(L, 2));

    Kernel::inst().getOGREWIndow()->removeAllViewports();
    Ogre::Camera *camera = Kernel::inst().getOGRESceneMgr()->createCamera ( camera_name );\
    Ogre::Viewport *vp = Kernel::inst().getOGREWIndow()->addViewport ( camera );
    camera->setAspectRatio ( Ogre::Real ( vp->getActualWidth() ) / Ogre::Real ( vp->getActualHeight() ) );

    node->attachObject( camera );


    lua_pushlightuserdata( L, camera );
    return 1;
}

static int del( lua_State *L ) {
    Ogre::Camera* camera = static_cast<Ogre::Camera*>(lua_touserdata(L, 1));
    delete camera;
    return 0;
}

static int setNear( lua_State *L ) {
    Ogre::Camera* camera = static_cast<Ogre::Camera*>(lua_touserdata(L, 1));
    float near = luaL_checknumber( L, 2 );

    camera->setNearClipDistance( near );
    return 0;
}

static int setFar( lua_State *L ) {
    Ogre::Camera* camera = static_cast<Ogre::Camera*>(lua_touserdata(L, 1));
    float far = luaL_checknumber( L, 2 );

    camera->setFarClipDistance( far );
    return 0;
}

static int setLookAt( lua_State *L ) {
    Ogre::Camera* camera = static_cast<Ogre::Camera*>(lua_touserdata(L, 1));

    float x = luaL_checknumber( L, 2 );
    float y = luaL_checknumber( L, 3 );
    float z = luaL_checknumber( L, 4 );

    camera->lookAt( x, y, z );
    return 0;
}


static luaL_reg lib[] = {
    {"new", create },
    {"del", del },
    {"setNear", setNear },
    {"setFar", setFar },
    {"setLookAt", setLookAt },
    {NULL, NULL}
};

luaL_reg* CameraAPI::getLuaReg() {
    return lib;
}

const char* CameraAPI::getName() {
    return "OGRECamera";
}
