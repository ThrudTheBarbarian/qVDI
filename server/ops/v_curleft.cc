
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
void VDI::v_curleft(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen->cmgr();
	Workstation *ws			= cmgr->findWorkstationForHandle(socket);
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
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curleft(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_curleft(fd);
	}
