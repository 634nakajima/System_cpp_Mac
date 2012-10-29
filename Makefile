INCLUDES 	= -Iinclude -I./include/lo -I./include/rtsosc
CC          	= g++

.PHONY: liblo pmmac porttime portsf rtsosc clean veryclean

all:	liblo pmmac porttime portsf rtsosc

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

clean:
	cd liblo; make clean;
	cd pm_mac; make clean;
	cd porttime; make clean;
	cd portsf; make clean;
	cd RTSOSC; make clean;

veryclean:
	cd liblo; make veryclean;
	cd pm_mac; make veryclean;
	cd porttime; make veryclean;
	cd portsf; make veryclean;
	cd RTSOSC; make veryclean;
	