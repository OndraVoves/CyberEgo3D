###################
# Cmake settings. #
###################
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
SET( CMAKE_VERBOSE_MAKEFILE OFF )
SET( CMAKE_COLOR_MAKEFILE ON )
SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )
SET( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/dist" )

####################
# Project setting. #
####################
PROJECT( ce3d )

OPTION( APP_DEBUG "Build in debug mode" ON )

#########################
# Definitions.          #
#########################
add_definitions(-ansi)
add_definitions(-Wall)
add_definitions(-W)
add_definitions(-pedantic)
add_definitions(-std=c++11)

IF( APP_DEBUG )
    add_definitions(-O0)
    add_definitions(-g)
    add_definitions(-pg)
    add_definitions(-DDEBUG)
ELSE( APP_DEBUG )
    add_definitions(-O3)
ENDIF( APP_DEBUG )


#########################
# Find and set library. #
#########################
find_package(Ogre REQUIRED)
find_package(OIS  REQUIRED)
#find_package(luajit  REQUIRED)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rd/luajit-2.0/src)

SET( LIBS
#    ${LUA_LIBRARIES}
    ${OIS_LIBRARIES}
    ${OGRE_LIBRARIES}
    OgreOverlay
    luajit
)

include_directories(
#    ${LUA_INCLUDE_DIR}
    ${OIS_INCLUDE_DIR}
    ${OGRE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/3rd/luajit-2.0/src
)


####################
# Set sourcefiles. #
####################
SET( CE3D_SRC
    src/luastate.cpp
    src/kernel.cpp

    src/ois/keyboardapi.cpp
    src/ois/mouseapi.cpp

    src/ogre/scenenodeapi.cpp
    src/ogre/meshapi.cpp
    src/ogre/cameraapi.cpp
)

SET( CE3D_MAIN_SRC
    src/main.cpp
)

SET_PROPERTY( SOURCE ${CE3D_SRC} ${CE3D_MAIN_SRC} APPEND PROPERTY COMPILE_FLAGS ${CFLAGS} )
ADD_EXECUTABLE(ce3d ${CE3D_SRC} ${COMPONENTS} ${CE3D_MAIN_SRC}  )
TARGET_LINK_LIBRARIES(ce3d ${LIBS} )
