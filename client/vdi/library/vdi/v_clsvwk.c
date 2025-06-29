//
//  v_clsvwk.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/17/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   2  : Close the physical workstation
\*****************************************************************************/
void v_clswk(int16_t handle)
	{
	_gemIoDisconnect();
	}

/*****************************************************************************\
|*   16  : Close the virtual workstation
\*****************************************************************************/
void v_clsvwk(int16_t handle)
	{
	_gemIoDisconnect();
	}

