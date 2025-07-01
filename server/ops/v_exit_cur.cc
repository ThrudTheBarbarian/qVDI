#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.2: Exit alpha mode. This clears the screen and disables the
|*             alpha cursor
|*
|* Original signature is:
|*
|*  vq_exit_cur(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_exit_cur(Transport *io, Workstation *ws)
	{
	setAlphaMode(false);
	v_clrwk(io, ws);
	}
