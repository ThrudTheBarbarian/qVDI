
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.4: Move the cursor up if possible.
|*
|* Original signature is: v_curup(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curup(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		if (_alphaY > 0)
			{
			bool erased = _eraseAlphaCursor();
			_alphaY --;
			if (erased)
				_drawAlphaCursor();
			}
		}
	else
		{
		WARN("v_curup() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curup(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_curup(fd);
		}
	else
		WARN("v_curup() cannot find IO transport");
	}
