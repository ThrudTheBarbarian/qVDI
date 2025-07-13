//
//
// main.c
// vdi
//
// Created by ThrudTheBarbarian on 7/12/25
// Copyright © 2025 ThrudTheBarbarian All rights reserved.
//
        
#include <stdio.h>
#include <unistd.h>

#include "vdi.h"



/*****************************************************************************\
|* Program entry point
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	// insert code here...
	int16_t workIn[16];
	int16_t workOut[128];
	int16_t handle;
	
	for (int i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;
	
	printf("Test connection to the service\n");
	v_opnwk(workIn, &handle, workOut);
	printf("  : handle is %d\n\n", handle);

	v_clrwk(handle);

	int16_t rgb[3];
	vq_color(handle, 3, 1, rgb);
	printf("RGB = {%02x, %02x, %02x}\n", rgb[0], rgb[1], rgb[2]);

	vsin_mode(handle, INP_LOCATOR, INPUT_SAMPLE);
	printf("set mouse input mode to 'sample'\n");

	int16_t settings[4];
	vql_attributes(handle, settings);
	printf("line settings are: %04x : %04x : %04x : %04x\n",
			settings[0],
			settings[1],
			settings[2],
			settings[3]
			);
	vqm_attributes(handle, settings);
	printf("Marker settings are: %04x : %04x : %04x : %04x\n",
			settings[0],
			settings[1],
			settings[2],
			settings[3]
			);
	}
