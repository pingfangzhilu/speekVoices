PWD=`pwd`

CC=/opt/buildroot-gcc463/usr/bin/mipsel-linux-gcc
export CC
PREFIX=$PWD/output/mips/

./configure --prefix=$PREFIX --host=mipsel-linux --disable-float-api --disable-arm4-asm
make clean
make 
make install

