#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 16: Set the width for drawing lines.
|*
|* Original signature is: vsl_width(int16_t handle, int16_t width);
|*
\*****************************************************************************/
void VDI::vsl_width(int handle, int16_t width)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (width < 1)
		width = 1;

	if (ws != nullptr)
		{
		ws->setLineWidth(width);
		}
	else
		{
		WARN("vsl_width() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_width(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t width	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsl_width(fd, width);
		}
	else
		WARN("vsl_width() expect 1 argument, got %d", num);
	}
