#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.13: Enable reverse-video.
|*
|* Original signature is: v_rvon(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_rvon(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen->cmgr();
	Workstation *ws			= cmgr->findWorkstationForHandle(socket);
	if (ws != nullptr)
		{
		_reverseVideo = true;
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rvon(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_rvon(fd);
	}
