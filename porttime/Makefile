#makefile for porttime
POBJS = porttime.o ptmacosx_cf.o

CFLAGS = -I ../include

CC=gcc

.PHONY:	clean

all:	libpt.a

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

clean:
	rm -f $(POBJS) 
	rm -f libpt.a;
	cd ../lib; rm -f libpt.a;

libpt.a:	$(POBJS)
	ar -rc libpt.a $(POBJS)
	ranlib  libpt.a

install:	libpt.a
	cp libpt.a ../lib
