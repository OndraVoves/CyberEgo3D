#!/bin/sh

wget -c http://www.inf.puc-rio.br/~roberto/lpeg/lpeg-0.12.tar.gz
tar -xzf ./lpeg-0.12.tar.gz
git clone https://github.com/harningt/luajson.git


git clone https://github.com/lsalzman/enet.git
git clone http://luajit.org/git/luajit-2.0.git luajit
hg clone https://bitbucket.org/sinbad/ogre -u v1-9


cd ./enet
autoreconf -vfi
./configure && make -j 2

cd ../luajit
make -j 2

cd ../lpeg-0.12
make -j 2

cd ../ogre
mkdir build
cd build
cmake -D OGRE_BUILD_SAMPLES=0 ../
make -j 2


cp lpeg.so ../../dist/
#cp ./ogre/build/lib/
