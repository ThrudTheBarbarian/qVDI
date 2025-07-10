#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|*  11.5: Fill an ellipse			[type=5] [pxy=x,y,rx,ry]
\*****************************************************************************/
void VDI::v_ellipse(int handle, int16_t x, int16_t y, int16_t xr, int16_t yr)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;

	if (ws != nullptr)
		{
		int16_t pxy[] = {x, y, xr, yr};
		v_fillarea(handle, ELLIPSE, 4, pxy);
		}
	else
		{
		WARN("v_ellipse() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_ellipse(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= ntohs(p[0]);
	if (num == 4)
		{
		int16_t x		= ntohs(p[1]);
		int16_t y		= ntohs(p[2]);
		int16_t xr		= ntohs(p[3]);
		int16_t yr		= ntohs(p[4]);

		int fd = io->socket()->socketDescriptor();
		v_ellipse(fd, x, y, xr, yr);
		}
	else
		WARN("v_ellipse() needs 4 arguments, got %d", num);
	}

