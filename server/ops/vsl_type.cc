#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 15: Set the style for drawing lines.
|*
|* Original signature is: vsl_type(int16_t handle, int16_t which);
\*****************************************************************************/
void VDI::vsl_type(int handle, int16_t idx)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;

	if ((idx < SOLID) || (idx > USERLINE))
		idx = SOLID;

	if (ws != nullptr)
		{
		ws->setLineType(idx);
		}
	else
		{
		WARN("v_rvon() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_type(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t style		= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsl_type(fd, style);
		}
	else
		WARN("vsl_type() expect 1 argument, got %d", num);
	}
