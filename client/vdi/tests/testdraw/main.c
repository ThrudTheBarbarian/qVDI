//
//  main.c
//  testline
//
//  Created by ThrudTheBarbarian on 7/2/25.
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

	printf("Test basic polyline");
	int16_t pxy[] = {50,50, 100,50, 100,100, 50,100, 100,50};
	v_pline(handle, 5, pxy);

	printf("Test arc line");
	v_arc(handle, 100, 100, 50, 900, 0);

	printf("Test elliptical arc");
	v_ellarc(handle, 200, 100, 100, 50, 900, 0);

	printf("Test rounded box");
	int16_t pxy_rb[] = {350, 50, 400, 100};
	v_rbox(handle, pxy_rb);
	}
