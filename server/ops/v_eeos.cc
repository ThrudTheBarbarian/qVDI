#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.9: Erase to end-of-screen.
|*
|* Original signature is: v_eeos(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_eeos(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr  ? cmgr->findWorkstationForHandle(handle)
									: nullptr;
	if (ws != nullptr)
		{
		v_eeol(handle);

		int H = screen->height();
		int W = screen->width();
		int y = (_alphaY + 1) * _alphaHeight;

		QRect r = {0, y, W, H-y};
		QPainter painter(screen->bg());
		painter.fillRect(r, ws->colour(ws->backgroundColourIndex()));
		update(r);
		}
	else
		{
		WARN("v_eeos() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_eeos(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_eeos(fd);
		}
	else
		WARN("v_eeos() cannot find IO transport");
	}
