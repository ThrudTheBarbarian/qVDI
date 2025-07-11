#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 13: Set the text-rotation angle.
|*
|* Original signature is: vst_rotation(int16_t handle, int16_t angle));
|*
\*****************************************************************************/
void VDI::vst_rotation(int handle, int16_t angle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	/*************************************************************************\
	|* Sanitise the angle input
	\*************************************************************************/
	if (angle < 0)
		angle = 0;

	if (ws != nullptr)
		ws->setTextRotation((angle % 3600)/10);
	else
		WARN("vst_rotation() annot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_rotation(Transport *io, ClientMsg &cm)
	{
	const Payload &p = cm.payload();
	if (p.size() == 1)
		{
		/*********************************************************************\
		|* Get the data out of the message
		\*********************************************************************/
		int16_t angle	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vst_rotation(fd, angle);
		}
	else
		WARN("vst_rotation() needs 1 argument, got %d", (int)p.size());
	}
