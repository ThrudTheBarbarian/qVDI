#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.14: Disable reverse-video.
|*
|* Original signature is: v_rvoff(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_rvoff(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
								   : nullptr;
	if (ws != nullptr)
		{
		_reverseVideo = false;
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rvoff(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_rvoff(fd);
	}
