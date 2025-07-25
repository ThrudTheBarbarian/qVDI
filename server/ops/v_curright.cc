
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.6: Move the cursor right if possible.
|*
|* Original signature is: v_curright(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curright(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		int lastCol =screen->width() / _alphaWidth;
		if (_alphaX < lastCol-1)
			{
			bool erased = _eraseAlphaCursor();
			_alphaX ++;
			if (erased)
				_drawAlphaCursor();
			}
		}
	else
		{
		WARN("v_curright() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curright(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_curright(fd);
		}
	else
		WARN("v_curright() cannot find IO transport");
	}
