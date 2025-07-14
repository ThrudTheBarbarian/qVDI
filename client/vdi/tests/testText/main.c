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
	v_gtext(handle, 100, 100, "This is basic text");

	printf("Full-Justified text\n");
	int jwid = 180;
	int16_t pxy[] = {5,38,5+jwid,38};
	v_pline(handle, 2, pxy);


	int16_t hOut, vOut;
	int16_t charW, charH, cellW, cellH;
	vst_height(handle, 14, &charW, &charH, &cellW, &cellH);

	v_justified(handle, 5, 44, "Full-out justified text", jwid, 1, 1);

	vst_color(handle, 1);
	vst_alignment(handle, TA_LEFT, TA_BASE, &hOut, &vOut);
	v_justified(handle, 5, 20, "Left justified", jwid, 0, 0);

	vst_alignment(handle, TA_RIGHT, TA_BASE, &hOut, &vOut);
	v_justified(handle, 5, 14, "Right justified", jwid, 0, 0);

	vst_height(handle, 32, &charW, &charH, &cellW, &cellH);
	v_gtext(handle, 180, 200, "32-pix text");

	vst_height(handle, 14, &charW, &charH, &cellW, &cellH);
	int16_t rgb[3];
	for (int i=0; i<3600; i+=225)
		{
		vst_rotation(handle, i);
		rgb[0] = i%1000;
		rgb[1] = (i*37)%1000;
		rgb[2] = (i*23)%1000;
		vs_color(handle, 1, rgb);
		v_gtext(handle, 600, 150, "... this is rotated");
		}

	int16_t black[3] = {0,0,0};
	vst_height(handle, 18, &charW, &charH, &cellW, &cellH);
	vs_color(handle, 1, black);
	vst_rotation(handle, 0);
	for (int i=-1; i<6; i++)
		{
		vst_color(handle, i+3);
		vst_font(handle, i);
		v_gtext(handle, 300, 40 + 20*i, "Test a font face");
		}

	vst_height(handle, 24, &charW, &charH, &cellW, &cellH);
	vst_color(handle, G_BLACK);
	vst_effects(handle, TXT_BOLD);
	v_gtext(handle, 5, 170, "Bold text");

	vst_effects(handle, TXT_ITALIC);
	v_gtext(handle, 5, 195, "Italic text");

	vst_effects(handle, TXT_UNDERLINE);
	v_gtext(handle, 5, 220, "Underline text");

	vst_effects(handle, TXT_OUTLINE);
	v_gtext(handle, 5, 245, "Outline text");

	}
