
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.7: Move the cursor left if possible.
|*
|* Original signature is: v_curleft(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curleft(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		if (_alphaX > 0)
			{
			bool erased = _eraseAlphaCursor();
			_alphaX --;
			if (erased)
				_drawAlphaCursor();
			}
		}
	else
		{
		WARN("v_curleft() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curleft(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_curleft(fd);
		}
	else
		WARN("v_curleft() cannot find IO transport");
	}
