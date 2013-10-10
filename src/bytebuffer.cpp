/*
    Copyright (c) 2013, Ondra Voves o.voves@gmail.com
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

    THIS SOFTWARE IS PROVIDED BY Ondra Voves o.voves@gmail.com ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Ondra Voves o.voves@gmail.com BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "bytebuffer.h"
#include <string.h>

using namespace CE3D;

ByteBuffer::ByteBuffer ( char *data, size_t size )
    : Data ( data ), Len ( size ), CurByte ( 0 ) {

}

ByteBuffer::~ByteBuffer() {

}

void ByteBuffer::seek ( size_t pos ) {
    if ( pos > Len ) {
        return;
    }

    CurByte = pos;
}

int ByteBuffer::writen ( size_t pos, int size, const char *val ) {
    if ( val ) {
        memcpy ( &Data[ pos ], ( void * ) val, size );
    } else {
        memset ( &Data[ pos ], 0, size );
    }

    CurByte += size;

    return size;
}

