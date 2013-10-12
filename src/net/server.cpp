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


#include "server.h"

#include "net.h"
#include "../common/bytebuffer.h"
#include "../kernel.h"
#include <stdio.h>
#include <stdarg.h>

using namespace CE3D::net;

extern "C" {
    void client_call ( int client, int ent, const char *cmd, const char *args_format, ... ) {
        va_list args;
        va_start ( args, args_format );

        CE3D::Kernel::inst().getServer().vaClientCall ( client, ent, cmd, args_format, args );

        va_end ( args );
    }
}

Server::Server() :
    Host ( nullptr ) {
    initClients();
}

Server::~Server() {

}

void Server::initClients() {
    for ( int i = 0; i < MAX_CLIENTS; i++ ) {
        Clients[i].peer = nullptr;
        Clients[i].type = Client::FREE;
        Clients[i].id = i;
    }
}

void Server::step() {
    if ( this->Host ) {
        pumpENetEvents();
    }
}


bool Server::listen ( int port ) {
    Addr.host = ENET_HOST_ANY;
    Addr.port = port;

    this->Host = enet_host_create ( &Addr,
                                    32,
                                    MAX_CHANNELS,
                                    0,
                                    0 );

    if ( this->Host == nullptr ) {
        return false;
    }

    return true;
}

void Server::clientCall ( int client, int ent, const char *cmd, const char *args_format, ... ) {
    va_list args;
    va_start ( args, args_format );

    vaClientCall ( client, ent, cmd, args_format, args );

    va_end ( args );
}

void Server::vaClientCall ( int client, int ent, const char *cmd, const char *args_format, va_list args ) {
    char data[512];
    ByteBuffer buff ( data, 512 );

    buff.write<int> ( net::CMD_CLIENT_CALL );
    buff.write<int> ( ent );
    buff.write<c_str> ( cmd );
    buff.write<c_str> ( args_format );

    int i = 0;
    char c = 0;

    //vprintf( "%f, %f, %f\n", args );
    while ( ( c = args_format[i++] ) != '\0' ) {
        if ( c == 'i' ) {
            int tmp_i = va_arg ( args, int );
            buff.write<int> ( tmp_i );

        } else if ( c == 'f' ) {
            float tmp_f = va_arg ( args, double );
            buff.write<float> ( tmp_f );
        }
    }

    send ( client, 1, &buff, true );
}


void Server::pumpENetEvents() {
    while ( enet_host_service ( this->Host, &Event, 0 ) > 0 ) {
        switch ( this->Event.type ) {
            case ENET_EVENT_TYPE_CONNECT:
                parseConnect ( this->Event );
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                parseDisonnect ( this->Event );
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                parseRecive ( this->Event );
                break;
        }
    }
}

Server::Client *Server::getNewClient() {
    for ( int i = 0; i < MAX_CLIENTS; i++ ) {
        if ( this->Clients[i].type == Client::FREE ) {
            return this->Clients + i;
        }
    }

    return nullptr;
}


void Server::parseConnect ( ENetEvent event ) {
    char hostname[64];
    enet_address_get_host_ip ( &Event.peer->address, hostname, 64 );

    Client *cl = getNewClient();
    if ( cl ) {
        // TODO: console
        printf ( "Client connecting from: %s\n", hostname );

        Event.peer->data = cl;
        cl->peer = Event.peer;
        cl->type = Client::REMOTE;

        sendConnectResult ( cl->id, true );
        //ConsoleSystem::printmsg ( ConsoleSystem::MSG_INFO, "Enet connecting from \"%s:%u\"\n", hostname, Event.peer->address.port );
    } else {
        //ConsoleSystem::printmsg ( ConsoleSystem::MSG_ERROR, "Enet connecting from \"%s:%u\" fail. Server is full\n", hostname, Event.peer->address.port );
    }
}
void Server::parseDisonnect ( ENetEvent event ) {
    Client *cl = static_cast<Client *> ( Event.peer->data );
    if ( cl ) {
        CE3D::Kernel::inst().getMainState().onClientDisonnect( cl->id );
        cl->type = Client::FREE;
    }
}

void Server::parseRecive ( ENetEvent event ) {
    Client *cl = static_cast<Client *> ( Event.peer->data );

    if ( cl ) {
        ByteBuffer buf ( ( char * ) Event.packet->data, Event.packet->dataLength );
        parsePacket ( cl->id, Event.channelID, &buf );
    }

    enet_packet_destroy ( Event.packet );
}

void Server::parsePacket ( const int client, const int chanel, CE3D::ByteBuffer *packet ) {
    int command = packet->read<int>();
    switch ( command ) {
        case CMD_SERVER_CALL:
            parseCall ( packet );
            break;
    }
}

bool Server::send ( int to, int channel, CE3D::ByteBuffer *packet, bool reliable ) {
    enet_uint32 flags = 0;
    if ( reliable ) {
        flags |= ENET_PACKET_FLAG_RELIABLE;
    }

    if ( to >= 0 ) {
        Client *cl = &Clients[to];
        if ( cl->type != Client::REMOTE ) {
            return false;
        }

        ENetPacket *enet_packet = enet_packet_create ( packet->data(), packet->curByte(), flags );
        enet_peer_send ( cl->peer, cl->id, enet_packet );
        return true;

    } else {
        Client *cl = nullptr;
        for ( int i = 0; i < MAX_CLIENTS; i++ ) {
            cl = &Clients[ i ];

            if ( cl->type != Client::REMOTE ) {
                break;
            }

            ENetPacket *enet_packet = enet_packet_create ( packet->data(), packet->curByte(), flags );
            enet_peer_send ( cl->peer, channel, enet_packet );

            return true;
        }
    }

    return true;
}

void Server::parseCall ( CE3D::ByteBuffer *packet ) {
    int ent_id = packet->read<int>();
    c_str cmd = packet->read<c_str>();
    c_str args_format = packet->read<c_str>();

    CE3D::Kernel::inst().getMainState().onNetCall( 2, ent_id, cmd, args_format, packet );
}

bool Server::sendConnectResult ( int client, bool ok ) {
    char data[512];
    ByteBuffer buff ( data, 512 );

    buff.write<int> ( net::CMD_CONNECT_RESULT );
    buff.write<int> ( ok );
    if ( ok ) {
        buff.write<int> ( client );
    }

    send ( client, 1, &buff, true );

    CE3D::Kernel::inst().getMainState().onClientConnect ( client );
}
