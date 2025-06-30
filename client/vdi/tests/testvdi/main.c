//
//  main.c
//  testvdi
//
//  Created by ThrudTheBarbarian on 6/28/25.
//
#include "vdi.h"

#include <stdio.h>

/*****************************************************************************\
|* For debugging
\*****************************************************************************/
int debugLevel(void)
	{ return 10; }

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
	printf("  : handle is %d\n", handle);

	printf("Clear the workstation\n");
	v_clrwk(handle);
	
	return 0;
	}
