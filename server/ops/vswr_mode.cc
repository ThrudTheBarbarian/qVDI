#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 32: Set the writing mode.
|*
|* Original signature is: vswr_mode(int16_t handle, int16_t mode);
\*****************************************************************************/
void VDI::vswr_mode(int handle, int16_t mode)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if ((mode < WR_REPLACE) || (mode > WR_REV_TRANS))
		WARN("Unrecognised mode (%d) in vswr_mode()", mode);
	else if (ws != nullptr)
		ws->setWritingMode(mode);
	else
		WARN("vswr_mode() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vswr_mode(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t mode	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vswr_mode(fd, mode);
		}
	else
		WARN("vswr_mode() expect 1 argument, got %d", num);
	}
