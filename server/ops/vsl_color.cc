#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 17: Set the colour for drawing lines.
|*
|* Original signature is: vsl_color(int16_t handle, int16_t idx);
\*****************************************************************************/
void VDI::vsl_color(int handle, int16_t idx)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		if ((idx >= 0) && (idx < 256))
			ws->setLineColourIndex(idx);
		else
			WARN("vsl_color index has to be 0...255 (got %d)", idx);
		}
	else
		{
		WARN("vsl_color() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_color(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t idx		= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsl_color(fd, idx);
		}
	else
		WARN("vsl_color() expects 1 argument, got %d", num);
	}
