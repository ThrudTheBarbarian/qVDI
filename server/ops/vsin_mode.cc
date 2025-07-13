#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 33: Set the input mode.
|*
|* Original signature is: vsin_mode(int16_t handle,
|*									int16_t device,
|*									int16_t mode);
\*****************************************************************************/
void VDI::vsin_mode(int handle, int16_t device, int16_t mode)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if ((device >= INP_LOCATOR) && (device < INP_MAX))
		WARN("Unrecognised device (%d) in vsin_mode()", device);
	else if (!((mode == INPUT_REQUEST) || (mode == INPUT_SAMPLE)))
		WARN("Unrecognised mode (%d) in vsin_mode()", device);
	else if (ws != nullptr)
		ws->setInputMode(device, mode);
	else
		WARN("vsin_mode() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsin_mode(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 2)
		{
		int16_t dev		= ntohs(p[0]);
		int16_t mode	= ntohs(p[1]);

		int fd = io->socket()->socketDescriptor();
		vsin_mode(fd, dev, mode);
		}
	else
		WARN("vsin_mode() expect 1 argument, got %d", num);
	}
