INCLUDES 	= -Iinclude -I./include/lo
LIBS 		= -Llib -llo -lportaudio -lpm -lpt -lportsf
FRAMEWORKS	= -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework CoreMIDI
SYSTEM		= main.o Data.o Module.o RoutingTable.o Audio.o Delay.o DAC.o ADC.o Coordinator.o MToken.o Envelope.o Sine.o AudioSource.o ModuleController.o ModuleList.o Serial.o XBeeController.o Tile.o AudioClock.o
MIDI		= MIDIMain.o
XBEE		= XBeeTest.o Data.o Module.o RoutingTable.o Audio.o Coordinator.o MToken.o XBeeController.o Serial.o Tile.o ModuleList.o AudioClock.o
ML		= MLTest.o Data.o Module.o RoutingTable.o Audio.o Delay.o DAC.o ADC.o Coordinator.o MToken.o Envelope.o Sine.o AudioSource.o ModuleController.o ModuleList.o Serial.o XBeeController.o Tile.o AudioClock.o
ODORE		= DAC.o Data.o Module.o RoutingTable.o Audio.o AudioSource.o Serial.o AudioClock.o odore.o odoremain.o
CC          	= g++
PROGS		= system midi xbee ml odore

.PHONY: liblo pmmac porttime portsf clean

all:	liblo pmmac porttime portsf system midi xbee ml odore

liblo:
	cd liblo; make; make install;

pmmac:
	cd pm_mac; make; make install;

porttime:
	cd porttime; make; make install;

portsf:
	cd portsf; make; make install;

.cpp.o:
	$(CC) $(INCLUDES) -c $<

midi: $(MIDI)
	$(CC) -o $@ $(MIDI) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

system: $(SYSTEM)
	$(CC) -o $@ $(SYSTEM) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

xbee: $(XBEE)
	$(CC) -o $@ $(XBEE) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

ml: $(ML)
	$(CC) -o $@ $(ML) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

odore: $(ODORE)
	$(CC) -o $@ $(ODORE) $(LIBS) $(INCLUDES) $(FRAMEWORKS)

clean:
	rm -f *.o   
	rm -f $(PROGS)
	cd liblo; make clean; cd ../
	cd pm_mac; make clean; cd ../
	cd porttime; make clean; cd ../
	cd portsf; make clean; cd ../

	