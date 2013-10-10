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


#include "../kernel.h"

using namespace CE3D;

extern "C" {
    void *camera_new ( const char *name, void *scene_node ) {
        Ogre::SceneNode *node = static_cast<Ogre::SceneNode *> ( scene_node );

        Kernel::inst().getOGREWIndow()->removeAllViewports();
        Ogre::Camera *camera = Kernel::inst().getOGRESceneMgr()->createCamera ( name );

        Ogre::Viewport *vp = Kernel::inst().getOGREWIndow()->addViewport ( camera );
        camera->setAspectRatio ( Ogre::Real ( vp->getActualWidth() ) / Ogre::Real ( vp->getActualHeight() ) );

        node->attachObject ( camera );

        return camera;
    }

    void camera_del ( void *camera ) {
        Ogre::Camera *c = static_cast<Ogre::Camera *> ( camera );
        Kernel::inst().getOGRESceneMgr()->destroyCamera ( c );
    }


    void camera_setFar ( void *camera, float far ) {
        Ogre::Camera *c = static_cast<Ogre::Camera *> ( camera );
        c->setFarClipDistance ( far );
    }

    void camera_setNear ( void *camera, float near ) {
        Ogre::Camera *c = static_cast<Ogre::Camera *> ( camera );
        c->setNearClipDistance ( near );
    }

    void camera_setLookAt ( void *camera, float x, float y, float z ) {
        Ogre::Camera *c = static_cast<Ogre::Camera *> ( camera );
        c->lookAt ( x, y, z );
    }

}
