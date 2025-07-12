#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 23: Set the fill-pattern style.
|*
|* Original signature is: vsf_interior(int16_t handle, int16_t type));
\*****************************************************************************/
void VDI::vsf_interior(int handle, int16_t idx)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (idx < FIS_HOLLOW)
		idx = FIS_HOLLOW;
	if (idx > FIS_USER)
		idx = FIS_USER;

	if (ws != nullptr)
		ws->setFillIndex(idx);
	else
		WARN("vsf_interior() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_interior(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t idx	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsf_interior(fd, idx);
		}
	else
		WARN("vsf_interior() expect 1 argument, got %d", num);
	}
