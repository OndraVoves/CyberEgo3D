#include <stdio.h>
#include <string.h>

#include "./common/luastate.h"
#include "kernel.h"


int main ( int argc, const char *argv[] ) {
    CE3D::Kernel kernel;

    if ( kernel.init ( argc, argv ) ) {
        /* TODO: Nacitat z nastaveni */
        kernel.createWindow ( 800, 600 );
        kernel.run();
    }

    return 0;
}
