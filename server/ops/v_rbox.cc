#include <QPainter>
#include <QPainterPath>

#include <vector>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 11.2	Draw an arc
|*
|* Original signature is: v_arc(int16_t handle, int16_t x, int16_t y,
|*								int16_t radius,
|*						        int16_t begang, int16_t endang);
|*
\*****************************************************************************/
void VDI::v_rbox(int handle, int16_t* pxy)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		QPen pen;
		QPainter painter(screen->bg());

		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		ws->setWritingMode(painter);

		/*********************************************************************\
		|* Figure out the parameters
		\*********************************************************************/
		int16_t x = pxy[0];
		int16_t y = pxy[1];
		int16_t w = pxy[2] - x;
		int16_t h = pxy[3] - y;

		/*********************************************************************\
		|* Draw the path perimeter, set-up the pen
		\*********************************************************************/
		ws->setupPenForLine(pen);
		painter.setPen(pen);
		painter.drawRoundedRect(x, y, w, h, 25, 25, Qt::RelativeSize);

		/*********************************************************************\
		|* Update the screen
		\*********************************************************************/
		int lw = ws->lineWidth();
		update(x-lw, y-lw, w+2*lw, h+2*lw);
		}
	else
		{
		WARN("v_rbox cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rbox(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	if (p.size() == 4)
		{
		int16_t pxy[4];
		pxy[0]	= ntohs(p[0]);
		pxy[1]	= ntohs(p[1]);
		pxy[2]	= ntohs(p[2]);
		pxy[3]	= ntohs(p[3]);

		int fd = io->socket()->socketDescriptor();
		v_rbox(fd, pxy);
		}
	else
		WARN("v_rbox sent %d arguments (need 4)", (int)p.size());
	}

