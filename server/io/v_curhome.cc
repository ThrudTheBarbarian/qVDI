
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.8: Move the cursor home.
|*
|* Original signature is: v_curhome(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curhome(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen->cmgr();
	Workstation *ws			= cmgr->findWorkstationForHandle(socket);
	if (ws != nullptr)
		{
		bool erased = _eraseAlphaCursor();
		_alphaX = _alphaY = 0;
		if (erased)
			_drawAlphaCursor();
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curhome(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_curhome(fd);
	}
