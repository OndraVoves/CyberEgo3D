#include <stdio.h>
#include <string.h>

#include "./lua/luastate.h"
#include "kernel.h"


int main ( int argc, const char *argv[] ) {
    CE3D::Kernel kernel;

    kernel.init();
    kernel.createWindow ( 800, 600 );
    kernel.run();

    return 0;
}
