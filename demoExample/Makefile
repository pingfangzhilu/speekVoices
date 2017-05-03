CC=gcc

OPENSRC=/home/project/openSrc/speexdsp-1.2rc3
INC=$(OPENSRC)/output/x86/include/
LIB=$(OPENSRC)/output/x86/lib/ -lspeexdsp
CFLAGS = -Wall -I $(INC)
LDFLAGS= -lm -L $(LIB)

TAR=app
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

