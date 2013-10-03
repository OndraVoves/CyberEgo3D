cppcheck -j `cat /proc/cpuinfo | grep processor | wc -l` --enable=all -v --std=c++11 --template=gcc ../src/ 2> cppcheck.out
