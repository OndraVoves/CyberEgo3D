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


#include "client.h"
#include "net.h"
#include "../common/bytebuffer.h"
#include "../kernel.h"
#include <stdio.h>

using namespace CE3D::net;

extern "C" {
    void server_call ( int ent, const char *cmd, const char *args_format, ... ) {
        va_list args;
        va_start ( args, args_format );

        CE3D::Kernel::inst().getClient().vaServerCall ( ent, cmd, args_format, args );

        va_end ( args );
    }
}

Client::Client()
    : Host ( nullptr ),
      ServerPeer ( nullptr ) {

}

Client::~Client() {
}

void Client::step() {
    if ( Host ) {
        pumpENetEvents();
    }
}


bool Client::connect ( const char *addr, unsigned int port ) {
    Host = enet_host_create ( NULL,
                              1,
                              MAX_CHANNELS,
                              57600 / 8,
                              14400 / 8 );

    if ( Host == nullptr ) {
        return false;
    }

    enet_address_set_host ( &ServerAddress, addr );
    ServerAddress.port = port;
    ServerPeer = enet_host_connect ( Host, &ServerAddress, 2, 0 );
}

void Client::pumpENetEvents() {
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

void Client::parseConnect ( ENetEvent event ) {
    char hostname[64];
    enet_address_get_host_ip ( &Event.peer->address, hostname, 64 );

    // TODO: console
    printf ( "Connected to: %s\n", hostname );

    /*    Client* cl = getNewClient();
    if ( cl ) {
        Event.peer->data = cl;
        cl->peer = Event.peer;
        //ConsoleSystem::printmsg ( ConsoleSystem::MSG_INFO, "Enet connecting from \"%s:%u\"\n", hostname, Event.peer->address.port );
    } else {
        //ConsoleSystem::printmsg ( ConsoleSystem::MSG_ERROR, "Enet connecting from \"%s:%u\" fail. Client is full\n", hostname, Event.peer->address.port );
    }*/
}
void Client::parseDisonnect ( ENetEvent event ) {
    // TODO: console
    ServerPeer = nullptr;
    printf ( "Disconnected from server\n" );
}

void Client::parseRecive ( ENetEvent event ) {
    ByteBuffer buf ( ( char * ) Event.packet->data, Event.packet->dataLength );
    parsePacket ( Event.channelID, &buf );

    enet_packet_destroy ( Event.packet );
}

void Client::parsePacket ( const int chanel, CE3D::ByteBuffer *packet ) {
    int command = packet->read<int>();
    switch ( command ) {
        case CMD_CLIENT_CALL:
            parseCall ( packet );
            break;

        case CMD_CONNECT_RESULT:
            parseConnectResult( packet );
            break;
    }
}

void Client::parseCall ( CE3D::ByteBuffer *packet ) {
    int ent_id = packet->read<int>();
    c_str cmd = packet->read<c_str>();
    c_str args_format = packet->read<c_str>();

    CE3D::Kernel::inst().doCall ( 1, ent_id, cmd, args_format, packet );
}


void Client::serverCall ( int ent, const char *cmd, const char *args_format, ... ) {
    va_list args;
    va_start ( args, args_format );
    vaServerCall ( ent, cmd, args_format, args );
    va_end ( args );
}

void Client::vaServerCall ( int ent, const char *cmd, const char *args_format, va_list args ) {
    char data[512];
    ByteBuffer buff ( data, 512 );

    buff.write<int> ( net::CMD_SERVER_CALL );
    buff.write<int> ( ent );
    buff.write<c_str> ( cmd );
    buff.write<c_str> ( args_format );

    int i = 0;
    char c = 0;
    while ( ( c = args_format[i++] ) != '\0' ) {
        if( c == 'i') {
            int tmp_i = va_arg ( args, int );
            buff.write<int> ( tmp_i );

        } else if( c == 'f' ) {
            float tmp_f = va_arg ( args, float );
            buff.write<float> ( tmp_f );
        }
    }

    send ( this->ID, &buff, true );
}

bool Client::send ( int channel, CE3D::ByteBuffer *packet, bool reliable ) {
    enet_uint32 flags = 0;

    if ( reliable ) {
        flags |= ENET_PACKET_FLAG_RELIABLE;
    }

    ENetPacket *enet_packet = enet_packet_create ( packet->data(), packet->curByte(), flags );
    enet_peer_send ( ServerPeer, channel, enet_packet );
}

void Client::parseConnectResult ( CE3D::ByteBuffer* packet ) {
    int status = packet->read<int>();
    if( status = 1 ) {
        int id = packet->read<int>();
        this->ID = id;
    } else {
        this->ID = -1;
    }
}

void Client::disconnect() {
    if( ServerPeer ) {
        enet_peer_disconnect( ServerPeer, 0 );
    }
}
