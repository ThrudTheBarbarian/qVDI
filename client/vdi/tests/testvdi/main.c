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
	printf("Move cursor down\n");
	v_curdown(handle);

	sleep(5);
	printf("Move cursor up\n");
	v_curup(handle);
	sleep(5);

	printf("Test exit alpha mode (should clear screen to normal)\n");
	v_exit_cur(handle);

	return 0;
	}
