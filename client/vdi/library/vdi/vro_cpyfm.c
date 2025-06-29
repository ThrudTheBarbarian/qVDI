//
//  vro_cpyfm.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/19/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  109  : Blit an opaque rect
\*****************************************************************************/
void vro_cpyfm(int16_t handle, int16_t mode, int16_t *pxy,
			   MFDB *src, MFDB *dst)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VRO_CPYFM);
	_gemMsgAppend(&msg, &mode, 1);
	_gemMsgAppend(&msg, pxy, 8);
	_gemMsgAppendMfdb(&msg, src);
	_gemMsgAppendMfdb(&msg, dst);
	
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

