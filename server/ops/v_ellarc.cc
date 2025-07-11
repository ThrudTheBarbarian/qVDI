#include <QPainter>
#include <QPainterPath>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 11.6	Draw an elliptical arc
|*
|* Original signature is: v_ellarc(int16_t handle, int16_t x, int16_t y,
|*								   int16_t xradius, int16_t yradius
|*						           int16_t begang, int16_t endang);
\*****************************************************************************/
void VDI::v_ellarc(int handle, int16_t x, int16_t y, int16_t xr, int16_t yr,
				   int16_t start, int16_t end)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		QPainter painter(screen->bg());
		QPen pen;

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
		int16_t X = x - xr;
		int16_t Y = y - yr;
		int16_t dx = xr * 2;
		int16_t dy = yr * 2;
		qreal   s = start / 10.0;
		qreal   e = end / 10.0 - s;

		path.arcMoveTo(X, Y, dx, dy, s);
		path.arcTo(X, Y, dx, dy, s, e);

		/*********************************************************************\
		|* Draw the path perimeter, set-up the pen
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
		WARN("v_ellarc() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_ellarc(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= p.size();
	if (num == 6)
		{
		int16_t x			= ntohs(p[0]);
		int16_t y			= ntohs(p[1]);
		int16_t xr			= ntohs(p[2]);
		int16_t yr			= ntohs(p[3]);
		int16_t start		= ntohs(p[4]);
		int16_t end			= ntohs(p[5]);

		int fd = io->socket()->socketDescriptor();
		v_ellarc(fd, x, y, xr, yr, start, end);
		}
	else
		WARN("v_ellarc() needs 6 arguments, got %d", num);
	}

