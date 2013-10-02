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


#ifndef CE3D_OGRE_H
#define CE3D_OGRE_H

#include <Ogre.h>
#include <OIS.h>
#include "lua/luastate.h"
#include "tsingleton.h"

namespace CE3D {
    class Kernel : public OIS::MouseListener,
                   public OIS::KeyListener,
                   public Ogre::FrameListener,
                   public TSingleton<Kernel> {
public:
    bool init();
    void shutdown();

    bool createWindow( uint width, uint height );

    void run();

public:
    bool mouseMoved ( const OIS::MouseEvent &e );
    bool mousePressed ( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased ( const OIS::MouseEvent &e, OIS::MouseButtonID id );

public:
    bool keyPressed ( const OIS::KeyEvent &e );
    bool keyReleased ( const OIS::KeyEvent &e );

public:
    bool frameStarted ( const Ogre::FrameEvent &evt );
    bool frameEnded ( const Ogre::FrameEvent &evt );

private:
    void updateWindow();

public:
    Ogre::Root         * getOGRERoot(){return this->OGRERoot;}
    Ogre::RenderWindow * getOGREWIndow(){return this->OGREWIndow;}
    Ogre::SceneManager * getOGRESceneMgr(){return this->OGRESceneMgr;}

private:
    Ogre::Root         *OGRERoot;
    Ogre::RenderWindow *OGREWIndow;
    Ogre::SceneManager *OGRESceneMgr;

    OIS::InputManager  *OISInputManager;
    OIS::Keyboard      *OISKeyboard;
    OIS::Mouse         *OISMouse;

    Lua::LuaState MainLuaStat;
};
}

#endif // CE3D_OGRE_H
