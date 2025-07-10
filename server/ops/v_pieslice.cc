#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|*  11.3: Fill an arc				[type=2] [pxy=x,y,r,begin,end]
\*****************************************************************************/
void VDI::v_pieslice(int socket, int16_t x, int16_t y, int16_t radius,
					 int16_t start, int16_t end)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
								   : nullptr;

	if (ws != nullptr)
		{
		int16_t pxy[] = {x, y, radius, start, end};
		v_fillarea(socket, PIE, 5, pxy);
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_pieslice(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= ntohs(p[0]);
	if (num == 5)
		{
		int16_t x			= ntohs(p[1]);
		int16_t y			= ntohs(p[2]);
		int16_t radius		= ntohs(p[3]);
		int16_t start		= ntohs(p[4]);
		int16_t end			= ntohs(p[5]);

		int fd = io->socket()->socketDescriptor();
		v_pieslice(fd, x, y, radius, start, end);
		}
	}

