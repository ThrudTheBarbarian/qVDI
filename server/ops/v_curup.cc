
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
void VDI::v_curup(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen->cmgr();
	Workstation *ws			= cmgr->findWorkstationForHandle(socket);
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
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curup(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_curup(fd);
	}
