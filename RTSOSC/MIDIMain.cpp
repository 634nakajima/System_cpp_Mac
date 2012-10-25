/*
 *  MIDIMain.cpp
 *  
 *
 *  Created by kannolab on 12/06/22.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include "portmidi.h"
#include "porttime.h"

#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0

int main() {
	
	int cnt,i,dev, outcnt=0, end=60000, chan=0;
	PmError retval;  
	const PmDeviceInfo *info;
	PmEvent msg[32];
	PortMidiStream *mstream;
	
	Pm_Initialize();
	cnt = Pm_CountDevices();
	
	if(cnt) {
		for(i=0; i < cnt; i++){
			info = Pm_GetDeviceInfo(i);
			if(info->input)
				printf("%d: %s \n", i, info->name);
		}
		printf("choose device: ");
		scanf("%d", &dev); 

		Pt_Start(1, NULL, NULL);
		retval = Pm_OpenInput(&mstream, dev, NULL, 512L, NULL,NULL);
		
		if(retval != pmNoError)
			printf("error in input device (%s) \n", Pm_GetErrorText(retval) );
		else {
			while(1){
				/* Poll for inputs */
				if(Pm_Poll(mstream)) {
					/* if there are MIDI events waiting, read them */
					cnt = Pm_Read(mstream, msg, 32);
					for(i=0; i<cnt; i++) {
						printf("status:%d, byte1=%d, "
							   "byte2=%d, time=%.3f\n",
							   Pm_MessageStatus(msg[i].message),
							   Pm_MessageData1(msg[i].message),
							   Pm_MessageData2(msg[i].message),
							   msg[i].timestamp/1000.);
					}
				}
			}	
		}
		Pm_Close(mstream);
	}
	else printf("No MIDI devices found\n");
	Pm_Terminate();
	return 0;
}
