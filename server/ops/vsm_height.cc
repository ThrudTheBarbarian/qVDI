#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 19: Set the height of marker in pixels.
|*
|* Original signature is: vsm_height(int16_t handle, int16_t height);
\*****************************************************************************/
void VDI::vsm_height(int handle, int16_t height)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	height	= (height < 1) ? 1
			: (height > 255) ? 255
			: ((height &1) == 0) ? height + 1
			: height;

	if (ws != nullptr)
		{
		ws->setMarkerSize(height);
		}
	else
		{
		WARN("vsm_height() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsm_height(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t height	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsm_height(fd, height);
		}
	else
		WARN("vsm_height() expect 1 argument, got %d", num);
	}
