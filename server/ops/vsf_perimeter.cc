#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 104: Set whether to draw a perimeter around a fill.
|*
|* Original signature is: vsf_perimeter(int16_t handle, int16_t enable));
\*****************************************************************************/
void VDI::vsf_perimeter(int handle, int16_t enable)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		ws->setDrawPerimeter(enable != 0);
		}
	else
		{
		WARN("vsf_perimeter() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_perimeter(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t enable		= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsf_perimeter(fd, enable);
		}
	else
		WARN("vsf_perimeter() expect 1 argument, got %d", num);
	}
