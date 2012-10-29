#makefile for pmmac
POBJS = finddefault.o pmmac.o pmmacosxcm.o pmutil.o portmidi.o readbinaryplist.o

CFLAGS = -I ../include

CC=gcc

.PHONY:	clean veryclean

all:	libpm.a

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

clean:
	rm -f $(POBJS) 
	rm -f libpm.a;

veryclean:
	rm -f $(POBJS) 
	rm -f libpm.a;
	cd ../lib; rm -f libpm.a;

libpm.a:	$(POBJS)
	ar -rc libpm.a $(POBJS)
	ranlib  libpm.a

install:	libpm.a
	cp libpm.a ../lib
