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
    void *mesh_new ( const char *mesh_file, void *scene_node ) {
        Ogre::SceneNode *node = static_cast<Ogre::SceneNode *> ( scene_node );

        Ogre::Entity *ent = Kernel::inst().getOGRESceneMgr()->createEntity ( mesh_file );
        node->attachObject ( ent );

        return ent;
    }

    void mesh_del ( void *mesh ) {
        Ogre::Entity *e = static_cast<Ogre::Entity *> ( mesh );

        Kernel::inst().getOGRESceneMgr()->destroyEntity ( e );

        //delete e;
    }

    void *mesh_get_anim ( void *mesh, const char *name ) {
        Ogre::Entity *e = static_cast<Ogre::Entity *> ( mesh );
        Ogre::AnimationState *state = e->getAnimationState ( name );

        //state->setEnabled(true);
        //state->setLoop ( true );

        return state;
    }

    void mesh_anim_update ( void *mesh, int dt ) {
        Ogre::Entity *e = static_cast<Ogre::Entity *> ( mesh );
        float dt_s = dt * ( 1.0 / 1000.0 );

        auto iter = e->getAllAnimationStates()->getEnabledAnimationStateIterator();

        while( iter.hasMoreElements() ) {
            iter.getNext()->addTime( dt_s );
        }
    }

    void mesh_anim_play ( void *state, bool loop ) {
        Ogre::AnimationState *s = static_cast<Ogre::AnimationState *> ( state );
        s->setLoop( loop );
        s->setEnabled ( true );
    }

    void mesh_anim_stop ( void *state ) {
        Ogre::AnimationState *s = static_cast<Ogre::AnimationState *> ( state );
        s->setEnabled ( false );
    }

    int mesh_anim_count( void* mesh ) {
        Ogre::Entity *e = static_cast<Ogre::Entity *> ( mesh );
        auto iter = e->getAllAnimationStates()->getAnimationStateIterator();
        int c = 0;
        while( iter.hasMoreElements() ) {
            iter.getNext();
            c++;
        }

        return c;
    }

    const char* mesh_anim_name( void* mesh, int index ) {
        Ogre::Entity *e = static_cast<Ogre::Entity *> ( mesh );
        auto iter = e->getAllAnimationStates()->getAnimationStateIterator();

        int c = 0;
        while( iter.hasMoreElements() ) {
            auto s = iter.getNext();

            if( c == index ) {
                return s->getAnimationName().c_str();
            }

            c++;
        }

        return "";
    }


}
