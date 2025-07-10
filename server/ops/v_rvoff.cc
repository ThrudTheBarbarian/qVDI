#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.14: Disable reverse-video.
|*
|* Original signature is: v_rvoff(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_rvoff(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		_reverseVideo = false;
		}
	else
		{
		WARN("v_rvoff() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rvoff(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_rvoff(fd);
		}
	else
		WARN("v_rvoff() cannot find IO transport");
	}
