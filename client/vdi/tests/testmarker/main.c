//
//  main.c
//  testmarker
//
//  Created by ThrudTheBarbarian on 7/3/25.
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

	printf("Test basic polymarker\n");
	int16_t pxy[] = {50,50, 100,50, 100,100, 50,100, 200,50};
	v_pmarker(handle, 5, pxy);

	for (int i=1; i<10; i+=2)
		pxy[i] += 20;

	vsm_type(handle, MRKR_CIRCLE);
	v_pmarker(handle, 5, pxy);

	for (int i=0; i<10; i++)
		{
		int16_t px[2] = {50 + i*50, 150};
		vsm_height(handle, 5+4*i);
		v_pmarker(handle, 1, px);
		}
	}
