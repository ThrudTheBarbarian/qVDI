//
//  main.c
//  testvdi
//
//  Created by ThrudTheBarbarian on 6/28/25.
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


	printf("Get the columns and rows on the display...\n");
	int16_t rows, cols;
	vq_chcells(handle, &rows, &cols);
	printf("  : rows:%d cols:%d\n\n", rows, cols);

	printf("Test enter alpha mode (should clear screen white)\n");
	v_enter_cur(handle);
	sleep(1);

	printf("Move cursor down/right\n");
	v_curdown(handle);
	v_curright(handle);
	sleep(2);

	printf("Move cursor up/left\n");
	v_curup(handle);
	v_curleft(handle);
	sleep(2);

	printf("Move cursor down/right and print\n");
	v_curdown(handle);
	v_curright(handle);
	v_curtext(handle, "This is the text");
	sleep(1);
	
	printf("Move cursor home\n");
	v_curhome(handle);
	sleep(2);

	printf("Erasing to end of screen\n");
	v_eeos(handle);
	sleep(2);

	printf("text at 20,20\n");
	v_rvon(handle);
	vs_curaddress(handle, 20, 20);
	v_curtext(handle, "[This is the inverse text]");

	int16_t r,c;
	vq_curaddress(handle, &r, &c);
	printf("Row,Col now %d,%d\n", r, c);

	v_rvoff(handle);
	v_curtext(handle, "[This is the plain text]");
	sleep(2);

	printf("mouse to 100,150\n");
	v_dspcur(handle, 100, 150);
	sleep(2);

	printf("mouse gone\n");
	v_rmcur(handle);
	sleep(2);

	printf("Test exit alpha mode (should clear screen to normal)\n");
	v_exit_cur(handle);
	sleep(2);


	return 0;
	}
