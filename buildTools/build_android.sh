#!/bin/sh
export TMPDIR="/tmp/"
export NDKROOT="/opt/android-ndk-r9d/"
export PRJ_ROOT="/home/project/linponeOutput/linponeAndroid/"
export PREBUILT=$NDKROOT/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86
CC=$PREBUILT/bin/arm-linux-androideabi-gcc
export CC
CFLAGS="--sysroot=$NDKROOT/platforms/android-9/arch-arm -fPIC -DANDROID -DHAVE_PTHREAD -mfpu=neon -mfloat-abi=softfp -I$NDKROOT/platforms/android-9/arch-arm/usr/include"
export CFLAGS
LDFLAGS="-Wl,-T,$PREBUILT/arm-linux-androideabi/lib/ldscripts/armelf_linux_eabi.x -Wl,-rpath-link=$NDKROOT/platforms/android-9/arch-arm/usr/lib -L$NDKROOT/platforms/android-9/arch-arm/usr/lib -nostdlib -lc -lm -ldl -lgcc"
export LDFLAGS
./configure --prefix=$PRJ_ROOT/ --host=arm-linux  --disable-shared --enable-static --disable-strict --disable-oggtest --with-ogg=/$PRJ_ROOT/
make clean
make 
make install
