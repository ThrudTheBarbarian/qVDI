#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 7: Draw a poly-marker.
|*
|* Original signature is: v_pmarker(int16_t handle, int16_t numPts, int16_t*pxy);
|*
\*****************************************************************************/
void VDI::v_pmarker(int handle, int16_t num, int16_t*pxy)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	QRectF dirty			= QRect(0,0,0,0);

	if (ws != nullptr)
		{
		QPen pen;
		ws->setupPenForMarker(pen);
		int lw = ws->markerSize();

		// Determine the area to mark as dirty
		QList<QPoint> pts;
		int idx = 0;
		for (int i=0; i<num; i++)
			{
			QPoint p(pxy[idx], pxy[idx+1]);

			QRect newRect	= QRect(p.x()-lw, p.y()-lw, lw*2, lw*2);
			dirty			= (idx == 0) ? newRect : dirty.united(newRect);

			pts << p;
			idx += 2;
			}
		QPainter painter(screen->bg());

		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		ws->setWritingMode(painter);

		idx			= 0;
		int size	= ws->markerSize();
		int delta	= (size - 1)/2;

		for (int i=0; i<num; i++)
			{
			int16_t x = pxy[idx++];
			int16_t y = pxy[idx++];
			pts.clear();

			switch (ws->markerType())
				{
				case MRKR_DOT:
					painter.drawPoint(x,y);
					break;

				case MRKR_PLUS:
					pts << QPoint(x, y-delta) << QPoint(x, y+delta)
						<< QPoint(x-delta, y) << QPoint(x+delta, y);
					painter.drawLines(pts);
					break;

				case MRKR_ASTERISK:
					pts << QPoint(x, y-delta) << QPoint(x, y+delta)
						<< QPoint(x-delta, y-delta) << QPoint(x+delta,y+delta)
						<< QPoint(x+delta, y-delta) << QPoint(x-delta,y+delta);
					painter.drawLines(pts);
					break;

				case MRKR_BOX:
					painter.drawRect(x-delta, y-delta, delta*2, delta*2);
					break;

				case MRKR_CROSS:
					pts << QPoint(x-delta, y-delta) << QPoint(x+delta,y+delta)
						<< QPoint(x+delta, y-delta) << QPoint(x-delta,y+delta);
					painter.drawLines(pts);
					break;

				case MRKR_DIAMOND:
					pts << QPoint(x, y-delta) << QPoint(x+delta, y)
						<< QPoint(x, y+delta) << QPoint(x-delta, y)
						<< QPoint(x, y-delta);
					painter.drawPolyline(pts.data(), pts.size());
					break;

				default:
					painter.drawEllipse(x-delta, y-delta, size, size);
					break;
				}
			}

		painter.setClipping(false);

		QRect updateArea = dirty.toRect();
		update(updateArea);
		}
	else
		{
		WARN("v_pmarker() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_pmarker(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int16_t num			= ntohs(p[0]);
	if (num == (int16_t)p.size()-1)
		{
		int16_t *pxy		= (int16_t *)(&(p[1]));

		/**********************************************************************\
		|* Get the data out of the message
		\**********************************************************************/
		for (int i=0; i<num; i++)
			pxy[i] = ntohs(pxy[i]);

		int fd = io->socket()->socketDescriptor();
		v_pmarker(fd, num/2, pxy);
		}
	else
		WARN("v_pmarker got %d args, expected %d", (int)p.size()-1, num);
	}

