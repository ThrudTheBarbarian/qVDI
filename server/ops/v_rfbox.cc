#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|*  11.3: Fill an arc				[type=2] [pxy=x,y,r,begin,end]
\*****************************************************************************/
void VDI::v_rfbox(int handle, int16_t* pxy)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		v_fillarea(handle, ROUNDED_RECT, 4, pxy);
	else
		WARN("v_rfbox() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rfbox(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= ntohs(p[0]);
	if (num == 4)
		{
		int16_t pxy[4];
		pxy[0]		= ntohs(p[1]);
		pxy[1]		= ntohs(p[2]);
		pxy[2]		= ntohs(p[3]);
		pxy[3]		= ntohs(p[4]);

		int fd = io->socket()->socketDescriptor();
		v_rfbox(fd, pxy);
		}
	else
		WARN("v_rfbox() got %d args, need 4", num);
	}

