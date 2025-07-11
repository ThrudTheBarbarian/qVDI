//
//
// main.c
// testText
//
// Created by ThrudTheBarbarian on 7/9/25
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
	workIn[3] = MRKR_DIAMOND;

	printf("Test connection to the service\n");
	v_opnwk(workIn, &handle, workOut);
	printf("  : handle is %d\n\n", handle);

	v_clrwk(handle);

	printf("Test basic text\n");
	v_gtext(handle, 100, 100, "This is the text");

	printf("Justified text\n");
	int jwid = 150;
	int16_t pxy[] = {100,145,100+jwid,145};
	v_pline(handle, 2, pxy);
	v_justified(handle, 100, 130, "This is justified", jwid, 1, 1);
	}
