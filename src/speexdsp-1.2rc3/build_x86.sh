PWD=`pwd`

PREFIX=$PWD/output/x86/

./configure --prefix=$PREFIX
make clean
make 
make install

