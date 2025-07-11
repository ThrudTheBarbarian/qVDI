#include <QPainter>
#include <QPainterPath>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|*  11.3: Fill an arc				[type=2] [pxy=x,y,r,begin,end]
\*****************************************************************************/
void VDI::v_ellpie(int handle, int16_t x, int16_t y, int16_t xr, int16_t yr,
				   int16_t start, int16_t end)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;
	if (ws != nullptr)
		{
		int16_t pxy[] = {x, y, xr, yr, start, end};
		v_fillarea(handle, ELLIPTICAL_PIE, 6, pxy);
		}
	else
		{
		WARN("v_pieslice() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_ellpie(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= ntohs(p[0]);
	if (num == 6)
		{
		int16_t x			= ntohs(p[1]);
		int16_t y			= ntohs(p[2]);
		int16_t xr			= ntohs(p[3]);
		int16_t yr			= ntohs(p[4]);
		int16_t start		= ntohs(p[5]);
		int16_t end			= ntohs(p[6]);

		int fd = io->socket()->socketDescriptor();
		v_ellpie(fd, x, y, xr, yr, start, end);
		}
	else
		WARN("v_ellpie() got %d args, need 5", num);
	}

