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


#ifndef CE3D_SERVER_H
#define CE3D_SERVER_H

extern "C" {
#include "enet/enet.h"
#include <stdio.h>
};

namespace CE3D {

    class ByteBuffer;
    namespace net {
        class Server {
            private:
                struct Client {
                    enum {
                        LOCAL = 0,
                        REMOTE,
                        FREE,
                    } type;

                    const char *name;
                    int id;

                    ENetPeer *peer;
                };

                enum {
                    MAX_CLIENTS = 32
                };

            public:
                Server();
                ~Server();


                bool listen ( int port );

                void step();

                void clientCall ( int client, int ent, const char *cmd, const char *args_format, ... );
                void vaClientCall ( int client, int ent, const char *cmd, const char *args_format, va_list args );

            private:
                void initClients();

            private:
                void pumpENetEvents();
                void parseConnect ( ENetEvent event );
                void parseDisonnect ( ENetEvent event );
                void parseRecive ( ENetEvent event );
                void parsePacket ( const int client, const int chanel, ByteBuffer *packet );
                Client *getNewClient();

            private:
                bool send ( int to, int channel, ByteBuffer *packet, bool reliable );

                void parseCall ( CE3D::ByteBuffer *packet );

            private:
                Client Clients[MAX_CLIENTS];

                ENetAddress Addr;
                ENetHost *Host;
                ENetEvent Event;
        };
    }
}

#endif // CE3D_SERVER_H
