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


#ifndef CE3D_BYTEBUFFER_H
#define CE3D_BYTEBUFFER_H
#include <stddef.h>
#include <string.h>

namespace CE3D {
    typedef const char *c_str;

    class ByteBuffer {
        public:
            ByteBuffer ( char *data, size_t size );
            virtual ~ByteBuffer();

            template<typename T>
            T read( );
            template<typename T>
            T read ( size_t pos );

            template<typename T>
            int write ( T val );

            template<typename T>
            int write (  size_t pos, T val );

            int writen ( size_t pos, int size, const char *val );

            size_t curByte() const;
            size_t len() const;
            void seek ( size_t pos );

            char *data();

            bool atEnd() const;
        private:
            char *Data;
            size_t Len;
            size_t CurByte;
    };

    inline size_t ByteBuffer::curByte() const {
        return CurByte;
    }

    inline char *ByteBuffer::data() {
        return Data;
    }
    inline size_t ByteBuffer::len() const {
        return Len;
    }
    inline bool ByteBuffer::atEnd() const {
        return ( CurByte >= Len );
    }

    template<typename T>
    inline int ByteBuffer::write ( size_t pos, T val ) {
        if ( !Data ) {
            return 0;
        }

        if ( ( pos + sizeof ( int ) ) > Len ) {
            return 0;
        }

        T *p_f = ( T * ) & ( Data[pos] );
        ( *p_f ) = val;

        return sizeof ( T );
    }


    template<>
    inline int ByteBuffer::write ( size_t pos, const char *val ) {
        if ( !Data ) {
            return 0;
        }

        int len = strlen ( val );
        if ( ( pos + len + sizeof ( int ) ) > Len ) {
            return 0;
        }

        int size = write<int> ( pos, len );

        strcpy ( &Data[pos + size], val );
        return len + 1/*\0*/ + size;
    }

    template<typename T>
    inline int ByteBuffer::write ( T val ) {
        int size = write<T> ( CurByte, val );
        CurByte += size;

        return size;
    }

    template<typename T>
    inline T ByteBuffer::read() {
        // TODO: ASSERT
        if ( !Data ) {
            //return;
        }

        if ( ( CurByte + sizeof ( T ) ) > Len ) {
            // return 0;
        }

        auto val = read<T> ( CurByte );
        CurByte += sizeof ( T );

        return val;
    }

    template<typename T>
    inline T ByteBuffer::read ( size_t pos ) {
        // TODO: ASSERT
        if ( !Data ) {
            return T();
        }

        if ( ( pos + sizeof ( T ) ) > Len ) {
            return T();
        }

        auto val = * ( ( T * ) & ( Data[pos] ) );
        return val;
    }

    template<>
    inline char ByteBuffer::read ( size_t pos ) {
        // TODO: ASSERT
        if ( !Data ) {
            return '\0';
        }

        if ( ( pos + 1 ) > Len ) {
            return '\0';
        }

        return Data[ pos ];
    }

    template<>
    inline c_str ByteBuffer::read ( size_t pos ) {
        // TODO: ASSERT
        if ( !Data ) {
            return 0;
        }

        int len = read<int> ( pos ) + 1;
        if ( !len ) {
            return NULL;
        }

        if ( ( ( pos + sizeof ( int ) ) + len ) > Len ) {
            return 0;
        }

        return & ( Data[ pos + sizeof ( int ) ] );
    }

    template<>
    inline c_str ByteBuffer::read() {
        // TODO: ASSERT
        if ( !Data ) {
            return 0;
        }

        if ( ( CurByte + sizeof ( char ) + 1 + sizeof ( int ) ) > Len ) {
            return 0;
        }


        auto val = read<c_str> ( CurByte );


        CurByte += strlen ( val ) + 1/*'\0'*/ + sizeof ( int ) /*len*/;

        return val;
    }

}

#endif // CE3D_BYTEBUFFER_H
