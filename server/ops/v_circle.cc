#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|*  11.4: Fill a circle				[type=4] [pxy=x,y,r]
\*****************************************************************************/
void VDI::v_circle(int socket, int16_t x, int16_t y, int16_t radius)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
								   : nullptr;

	if (ws != nullptr)
		{
		int16_t pxy[] = {x, y, radius};
		v_fillarea(socket, CIRCLE, 3, pxy);
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_circle(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= ntohs(p[0]);
	if (num == 3)
		{
		int16_t x		= ntohs(p[1]);
		int16_t y		= ntohs(p[2]);
		int16_t radius	= ntohs(p[3]);

		int fd = io->socket()->socketDescriptor();
		v_circle(fd, x, y, radius);
		}
	}

