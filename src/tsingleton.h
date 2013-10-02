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


#ifndef CE3D_TSINGLETON_H
#define CE3D_TSINGLETON_H

namespace CE3D {
    template<typename T>
    class TSingleton {
        public:
            TSingleton();
            virtual ~TSingleton();

            static T &inst();
            static T *instPtr();

        private:
            TSingleton ( const TSingleton<T> &other );
            TSingleton<T> operator= ( const TSingleton<T> &other );

        protected:
            static T *Instance;
    };

    template<typename T>
    T *TSingleton<T>::Instance = nullptr;

    template<typename T>
    TSingleton<T>::TSingleton() {
        Instance = static_cast<T *> ( this );
    }

    template<typename T>
    TSingleton<T>::~TSingleton() {
        Instance = nullptr;
    }

    template<typename T>
    T &TSingleton<T>::inst() {
        return ( *Instance );
    }

    template<typename T>
    T *TSingleton<T>::instPtr() {
        return ( Instance );
    }

}

#endif // CE3D_TSINGLETON_H
