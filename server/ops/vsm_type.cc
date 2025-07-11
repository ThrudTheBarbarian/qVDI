#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 18: Set the type of marker drawn.
|*
|* Original signature is: vsm_type(int16_t handle, int16_t which);
\*****************************************************************************/
void VDI::vsm_type(int handle, int16_t type)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if ((type < MRKR_DOT) || (type > MRKR_CIRCLE))
		type = MRKR_CIRCLE;

	if (ws != nullptr)
		{
		ws->setMarkerType(type);
		}
	else
		{
		WARN("vsm_type() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsm_type(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t type	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsm_type(fd, type);
		}
	else
		WARN("vsm_type() expect 1 argument, got %d", num);
	}
