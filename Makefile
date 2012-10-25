INCLUDES 	= -Iinclude -I./include/lo -I./include/rtsosc
LIBS 		= -Llib -llo -lportaudio -lpm -lpt -lportsf -lrtsosc
FRAMEWORKS	= -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework CoreMIDI
CC          	= g++
RTSOSCAPP	= MyModule.o main.o
PROGS		= myApp

.PHONY: liblo pmmac porttime portsf rtsosc clean

all:	liblo pmmac porttime portsf rtsosc myApp

liblo:
	cd liblo; make; make install;

pmmac:
	cd pm_mac; make; make install;

porttime:
	cd porttime; make; make install;

portsf:
	cd portsf; make; make install;

rtsosc:
	cd RTSOSC; make; make install;

.cpp.o:
	$(CC) $(INCLUDES) -c $<

myApp: $(RTSOSCAPP)
	$(CC) -o $@ $(RTSOSCAPP) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

clean:
	rm -f *.o   
	rm -f $(PROGS)
	cd liblo; make clean; cd ../
	cd pm_mac; make clean; cd ../
	cd porttime; make clean; cd ../
	cd portsf; make clean; cd ../
	cd RTSOSC; make clean; cd ../


	