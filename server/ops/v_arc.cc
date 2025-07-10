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
void VDI::v_arc(int handle, int16_t x, int16_t y, int16_t radius,
				int16_t start, int16_t end)
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
		|* Create the path that we'll draw
		\*********************************************************************/
		QPainterPath path;
		int16_t X = x - radius;
		int16_t Y = y - radius;
		int16_t d = radius * 2;
		qreal   s = start / 10.0;
		qreal   e = end / 10.0 - s;

		path.arcMoveTo(X, Y, d, d, s);
		path.arcTo(X, Y, d, d, s, e);

		/*********************************************************************\
		|* Draw the path, set-up the pen
		\*********************************************************************/
		ws->setupPenForLine(pen);
		painter.setPen(pen);
		painter.drawPath(path);

		/*********************************************************************\
		|* Update the screen
		\*********************************************************************/
		QRectF dirty = path.boundingRect();
		update(dirty);
		}
	else
		{
		WARN("v_arc cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_arc(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	if (p.size() == 5)
		{
		int16_t x			= ntohs(p[0]);
		int16_t y			= ntohs(p[1]);
		int16_t radius		= ntohs(p[2]);
		int16_t start		= ntohs(p[3]);
		int16_t end			= ntohs(p[4]);

		int fd = io->socket()->socketDescriptor();
		v_arc(fd, x, y, radius, start, end);
		}
	else
		WARN("v_arc only sent %d arguments (need 5)", (int)p.size());
	}

