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


#include "kernel.h"
#include <Ogre.h>
#include "ois/keyboardapi.h"
#include "ois/mouseapi.h"
#include "common/bytebuffer.h"
#include "OgreFontManager.h"

using namespace CE3D;

Kernel::Kernel(  )
    : LastMS ( 0 ) {
}

Kernel::~Kernel() {
    shutdown();
}


bool Kernel::init ( int argc, const char *argv[] ) {
    MainState.init();

    //TODO: volitelna cesta... pluginy mozna napevno v programu
    OGRERoot = new Ogre::Root ( "./core/ogre/plugins.cfg" );
    new Ogre::FontManager();

    Ogre::ResourceGroupManager &rm = Ogre::ResourceGroupManager::getSingleton();
    rm.addResourceLocation ( "game", "FileSystem" );
    rm.addResourceLocation ( "game/gui", "FileSystem" );
    rm.addResourceLocation ( "game/scripts/cmds", "FileSystem", "LuaCmds" );

    Ogre::ConfigFile cf;
    cf.load ( "./game/resources.cfg" );

    auto seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while ( seci.hasMoreElements() ) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for ( i = settings->begin(); i != settings->end(); ++i ) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation ( archName, typeName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
        }
    }

    const Ogre::RenderSystemList &lRenderSystemList = OGRERoot->getAvailableRenderers();
    if ( lRenderSystemList.size() == 0 ) {
        Ogre::LogManager::getSingleton().logMessage ( "Sorry, no rendersystem was found." );
        return false;
    }

    Ogre::RenderSystem *lRenderSystem = lRenderSystemList[0];
    OGRERoot->setRenderSystem ( lRenderSystem );
    OGRERoot->initialise ( false, "CyberEg{o.o}rg 3D" );

    if ( enet_initialize() < 0 ) {
        //ConsoleSystem::printmsg ( ConsoleSystem::MSG_ERROR, "Enet init fail.\n" );
        return false;
    }

    if ( argc > 1 ) {
        Client.connect ( "localhost", 2222 );
    } else {
        MainState.pushBool ( true );
        MainState.setGlobal ( "isServer" );
        Server.listen ( 2222 );
    }
}

bool Kernel::createWindow ( uint width, uint height ) {
    Ogre::NameValuePairList lParams;
    lParams["FSAA"] = "false";
    lParams["vsync"] = "false";
    OGREWIndow = OGRERoot->createRenderWindow ( "CyberEg{o.o}rg 3D", width, height, false, &lParams );

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    OGRESceneMgr = OGRERoot->createSceneManager ( "OctreeSceneManager" );

    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    OGREWIndow->getCustomAttribute ( "WINDOW", &windowHnd );
    windowHndStr << windowHnd;
    pl.insert ( std::make_pair ( std::string ( "WINDOW" ), windowHndStr.str() ) );
    pl.insert ( std::make_pair ( std::string ( "x11_keyboard_grab" ), "false" ) );
    pl.insert ( std::make_pair ( std::string ( "x11_mouse_grab" ), "false" ) );
    //pl.insert ( std::make_pair ( std::string ( "x11_mouse_hide" ), "false" ) );


    OISInputManager     = OIS::InputManager::createInputSystem ( pl );
    OISKeyboard         = static_cast<OIS::Keyboard *> ( OISInputManager->createInputObject ( OIS::OISKeyboard, true ) );
    OISMouse            = static_cast<OIS::Mouse *> ( OISInputManager->createInputObject ( OIS::OISMouse, true ) );

    updateWindow();

    OGRESceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.5f, 0.5f, 0.5f ) );

    OISKeyboard->setEventCallback ( this );
    OISMouse->setEventCallback ( this );
}

void Kernel::shutdown() {
    OISInputManager->destroyInputObject ( OISKeyboard );
    OISKeyboard = nullptr;

    OISInputManager->destroyInputObject ( OISMouse );
    OISMouse = nullptr;

    OIS::InputManager::destroyInputSystem ( OISInputManager );
    OISInputManager = nullptr;

    //CEGUISystem->destroy();

    //delete DebugDrawer::getSingletonPtr();

    delete OGRERoot;
}


void Kernel::updateWindow() {
    unsigned int width, height, depth;
    int top, left;
    OGREWIndow->getMetrics ( width, height, depth, left, top );
    const OIS::MouseState &ms = OISMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}


void Kernel::run() {
    MainState.callMain();
    //MainLuaStat.doFile ( "./core/scripts/main.lua" );
    //initMainLuaRef();

    //int r = MainLuaStat.getGlobalRef ( "main" );
    //MainLuaStat.callRef ( r );

    int last_render = 0;
    while ( 1 ) {
        //printf( "GAME\n" );

        /* calc d_time */
        ulong ms = OGRERoot->getTimer()->getMilliseconds();
        ulong d_ms = ms - this->LastMS;
        this->LastMS = ms;

        OISKeyboard->capture();
        OISMouse->capture();

        Server.step();
        Client.step();

        /* Tick */
        MainState.onUpdate( d_ms );

        /* Rendering */
        last_render += d_ms;
        if ( last_render >= 33 ) {
            renderFrame( );
            //printf( "RENDER\n" );
            last_render = 0;
        }
    }
}

/*bool Kernel::frameEnded ( const Ogre::FrameEvent &evt ) {
    return true;
}

bool Kernel::frameStarted ( const Ogre::FrameEvent &evt ) {

    return true;
}
*/

bool Kernel::keyPressed ( const OIS::KeyEvent &e ) {
    if ( e.key == OIS::KC_D ) {
        Client.disconnect();
    }

    //MainLuaStat.callRef ( this->LuaKeyPressed, ( int ) e.key );
    return true;
}

bool Kernel::keyReleased ( const OIS::KeyEvent &e ) {
    //MainLuaStat.callRef ( this->LuaKeyReleased, ( int ) e.key );
    return true;
}

bool Kernel::mouseMoved ( const OIS::MouseEvent &e ) {
    //MainLuaStat.callRef ( this->LuaMouseMoved );
    return true;
}

bool Kernel::mousePressed ( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    //MainLuaStat.callRef ( this->LuaMousePressed );
    return true;
}

bool Kernel::mouseReleased ( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    //MainLuaStat.callRef ( this->LuaMouseReleased );
    return true;
}

void Kernel::renderFrame() {
    Ogre::WindowEventUtilities::messagePump();
    OGRERoot->renderOneFrame();
}
