
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 5.3: Enter alpha mode. This clears the screen and enables the
|*             alpha cursor
|*
|* Original signature is:
|*
|*  v_enter_cur(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_enter_cur(Transport *io, Workstation *ws)
	{
	_alphaX		= 0;
	_alphaY		= 0;
	setAlphaMode(true);
	v_clrwk(io, ws);
	}
