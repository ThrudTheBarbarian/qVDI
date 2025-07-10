
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.5: Move the cursor down if possible.
|*
|* Original signature is: v_curdown(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curdown(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		int lastRow = screen->height() / _alphaHeight;
		if (_alphaY < lastRow - 1)
			{
			bool erased = _eraseAlphaCursor();
			_alphaY ++;
			if (erased)
				_drawAlphaCursor();
			}
		}
	else
		{
		WARN("v_curdown() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curdown(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_curdown(fd);
		}
	else
		WARN("v_curdown() cannot find IO transport");
	}
