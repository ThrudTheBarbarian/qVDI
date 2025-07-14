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

	printf("Test basic fill\n");
	int16_t pxy[] = {50,50, 100,50, 100,100, 50,100, 50,50};
	vsf_interior(handle, FIS_HOLLOW);
	v_fillarea(handle, 5, pxy);

	printf("Test rectangle fill\n");
	int16_t pxy_v_bar[] = {150, 50, 250, 100};
	vsf_interior(handle, FIS_SOLID);
	v_bar(handle, pxy_v_bar);

	printf("Test pieslice fill\n");
	vsf_interior(handle, FIS_PATTERN);
	vsf_style(handle, PT_SHELLS);
	v_pieslice(handle, 300, 100, 50, 0, 900);

	printf("Test circle fill\n");
	vsf_interior(handle, FIS_HATCH);
	vsf_style(handle, PT_DIAG1);
	v_circle(handle, 400, 75, 25);

	printf("Test elliptical fill\n");
	vsf_interior(handle, FIS_HOLLOW);
	v_ellpie(handle, 450, 75, 50,25, 0, 900);

	printf("Test rounded filled box\n");
	int16_t pxy_rfb[] = {550, 50, 600, 100};
	vsf_interior(handle, FIS_SOLID);
	v_rfbox(handle, pxy_rfb);

	printf("Test rounded filled box without perimeter\n");
	int16_t pxy_rfb2[] = {650, 50, 700, 100};
	vsf_interior(handle, FIS_SOLID);
	vsf_perimeter(handle, 0);
	v_rfbox(handle, pxy_rfb2);

	printf("Show patterns\n");
	vsf_interior(handle, FIS_PATTERN);
	for (int i=1; i<PT_PATTERN_MAX; i++)
		{
		vsf_style(handle, i);
		vsf_color(handle, i);
		int16_t box[] = {5+i*30, 110, 25+i*30, 135};
		v_rfbox(handle, box);
		}

	printf("Show hatches\n");
	vsf_interior(handle, FIS_HATCH);
	for (int i=1; i<PT_HATCH_MAX; i++)
		{
		vsf_style(handle, i);
		vsf_color(handle, i+PT_PATTERN_MAX);
		int16_t box[] = {5+i*30, 150, 25+i*30, 175};
		v_rfbox(handle, box);
		}

	printf("Show writing modes\n");
	for (int i=WR_REPLACE; i<= WR_REV_TRANS; i++)
		{
		vsf_interior(handle, FIS_SOLID);
		vsf_color(handle, G_BLACK);
		int16_t box[] = {5+i*30, 180, 25+i*30, 205};
		v_rfbox(handle, box);

		vswr_mode(handle, i);
		vsf_color(handle, G_RED);
		vsf_interior(handle, FIS_PATTERN);
		vsf_style(handle, PT_BALLS);
		v_rfbox(handle, box);
		}

	printf("contour fill\n");
	vsf_interior(handle, FIS_PATTERN);
	vsf_style(handle, PT_ROOF2);
	vsf_color(handle, G_BLUE);
	v_contourfill(handle, 200, 75, G_RED);
	}
