#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 22: Set the colour for drawing text.
|*
|* Original signature is: vst_color(int16_t handle, int16_t idx);
\*****************************************************************************/
void VDI::vst_effects(int handle, int16_t effect)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		ws->setTextEffect(effect & TXT_MASK);
	else
		WARN("vst_effects() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_effects(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t effect	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vst_effects(fd, effect);
		}
	else
		WARN("vst_effects() expect 1 argument, got %d", num);
	}
