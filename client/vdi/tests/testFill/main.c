//
//
// main.c
// testFill
//
// Created by ThrudTheBarbarian on 7/10/25
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

	printf("Test basic fill");
	int16_t pxy[] = {50,50, 100,50, 100,100, 50,100, 50,50};
	v_fillarea(handle, 5, pxy);

	printf("Test rectangle fill");
	int16_t pxy_v_bar[] = {150, 50, 250, 100};
	v_bar(handle, pxy_v_bar);

	printf("Test pieslice fill");
	v_pieslice(handle, 300, 100, 50, 0, 900);

	printf("Test circle fill");
	v_circle(handle, 400, 75, 25);
	}
