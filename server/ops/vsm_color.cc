#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 20: Set the colour for drawing markers.
|*
|* Original signature is: vsm_color(int16_t handle, int16_t idx);
\*****************************************************************************/
void VDI::vsm_color(int handle, int16_t idx)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		if ((idx >= 0) && (idx < 256))
			ws->setMarkerColourIndex(idx);
		else
			WARN("vsm_color() index out of 0..255 range (%d)", idx);
		}
	else
		{
		WARN("vsm_color() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsm_color(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t idx	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsm_color(fd, idx);
		}
	else
		WARN("vsm_color() expect 1 argument, got %d", num);
	}
