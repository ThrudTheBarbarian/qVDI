
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
void VDI::v_curright(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen->cmgr();
	Workstation *ws			= cmgr->findWorkstationForHandle(socket);
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
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curright(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_curright(fd);
	}
