CC=gcc
TARGET_ARCH=libmips
OPENSRC=/home/yue/work0615/demolib/speekVoices/src/speexdsp-1.2rc3
ifeq ($(TARGET_ARCH), mips)
TAR = demo
INC=-I $(OPENSRC)/output/mips/include/ -DTEST_MAIN
LIB=-lspeexdsp 
BUILD_PATH=/opt/buildroot-gcc463/usr/bin/
CROSS_COMPILE=$(BUILD_PATH)mipsel-linux-

else  ifeq ($(TARGET_ARCH), libmips)
TAR = librateSpeek.so
INC=-fpic -shared -I $(OPENSRC)/output/mips/include/
LIB=-lspeexdsp -lpthread -lrt
BUILD_PATH=/opt/buildroot-gcc463/usr/bin/
CROSS_COMPILE=$(BUILD_PATH)mipsel-linux-
else 
TAR=demo
LIB=-L /usr/mylib/ -lspeexdsp
INC=-I $(OPENSRC)/output/mips/include/ -DTEST_MAIN
endif
CFLAGS = -Wall $(INC)
LDFLAGS= -lm $(LIB)

CC=$(CROSS_COMPILE)gcc
all +=resexampleRate.o

export CC
$(TAR):$(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) $(LDFLAGS)
	$(RM) -f *.gch *.bak $(all) 
        
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f $(TAR) $(all) 
