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
	workIn[2] = 1;

	printf("Test connection to the service\n");
	v_opnwk(workIn, &handle, workOut);
	printf("  : handle is %d\n\n", handle);

	v_clrwk(handle);

	printf("Test basic polyline\n");
	int16_t pxy[] = {50,90, 100,90, 100,140, 50,140, 100,90};
	v_pline(handle, 5, pxy);

	printf("Test arc line\n");
	v_arc(handle, 100, 140, 50, 900, 0);

	printf("Test elliptical arc\n");
	v_ellarc(handle, 200, 140, 100, 50, 900, 0);

	printf("Test rounded box\n");
	int16_t pxy_rb[] = {350, 90, 400, 140};
	v_rbox(handle, pxy_rb);

	int16_t pxy_1[] = {50,80, 200,80};
	for (int i=SOLID; i<USERLINE; i++)
		{
		vsl_type(handle, i);
		v_pline(handle, 2, pxy_1);
		pxy_1[1] -= 10;
		pxy_1[3] -= 10;
		}

	int16_t pxy_2[] = {450,140, 600,140};
	for (int i=1; i<10; i++)
		{
		vsl_color(handle, i);
		vsl_width(handle, i);
		v_pline(handle, 2, pxy_2);
		pxy_2[1] -= i+5;
		pxy_2[3] -= i+5;
		}

	int16_t rgb[3];
	for (int i=0; i<50; i++)
		{
		rgb[0] = i*10 + 200;
		rgb[1] = 1000 - i*20;
		rgb[2] = i*10 + 250;
		vs_pixrgb(handle, 5, 5+i, rgb);
		}

	vq_pixrgb(handle, 5, 15, rgb);
	printf("rgb @ (5,15) = %d,%d,%d   cf: 300,800,350\n", rgb[0], rgb[1], rgb[2]);
	}
