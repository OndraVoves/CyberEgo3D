#include <stdio.h>
#include <string.h>

#include "./luastate.h"
#include "kernel.h"


int main ( int argc, const char *argv[] ) {
    CE3D::Kernel kernel;

    if ( kernel.init ( argc, argv ) ) {
        kernel.createWindow ( 800, 600 );
        kernel.run();
    }

    return 0;
}
