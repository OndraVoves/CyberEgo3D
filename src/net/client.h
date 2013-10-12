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


#ifndef CE3D_NET_CLIENT_H
#define CE3D_NET_CLIENT_H

extern "C" {
#include "enet/enet.h"
};

#include <stdarg.h>
#include <stdio.h>


namespace CE3D {

    class ByteBuffer;

    namespace net {

        class Client {
            public:
                Client();
                ~Client();

                bool connect ( const char *addr, unsigned int port );
                void disconnect();

                void step();

                void serverCall ( int ent, const char *cmd, const char *args_format, ... );
                void vaServerCall ( int ent, const char *cmd, const char *args_format, va_list args );

            private:
                void pumpENetEvents();
                void parseConnect ( ENetEvent event );
                void parseDisonnect ( ENetEvent event );
                void parseRecive ( ENetEvent event );
                void parsePacket ( const int chanel, ByteBuffer *packet );

            private:
                void parseCall ( ByteBuffer *packet );
                void parseConnectResult( ByteBuffer *packet );

            private:
                bool send ( int channel, ByteBuffer *packet, bool reliable );

            private:
                ENetAddress ServerAddress;
                ENetHost *Host;
                ENetPeer *ServerPeer;
                ENetEvent Event;
                int         ID;

                char ConnectAddres[255];
                int  ConnectPort;
        };

    }

}

#endif // CE3D_NET_CLIENT_H
