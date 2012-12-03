#コンパイルするソースコード
RTSOSCAPP	= MyModule.o main.o

#実行ファイル名
PROG		= moduleList

INCLUDES 	= -I../include -I../include/lo -I../include/rtsosc
LIBS 		= -L../lib -llo -lportaudio -lpm -lpt -lportsf -lrtsosc
FRAMEWORKS	= -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework CoreMIDI
CC          	= g++

.PHONY: liblo pmmac porttime portsf rtsosc clean

all:	liblo pmmac porttime portsf rtsosc myApp

liblo:
	cd ../liblo; make; make install;

pmmac:
	cd ../pm_mac; make; make install;

porttime:
	cd ../porttime; make; make install;

portsf:
	cd ../portsf; make; make install;

rtsosc:
	cd ../RTSOSC; make; make install;

.cpp.o:
	$(CC) $(INCLUDES) -c $<

myApp: $(RTSOSCAPP)
	$(CC) -o $(PROG) $(RTSOSCAPP) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

clean:
	rm -f *.o   
	rm -f $(PROG)