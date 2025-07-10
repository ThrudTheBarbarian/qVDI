#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.18: Position the graphic cursor.
|*
|* Original signature is: v_dspcur(int1`6_t handle, int16_t x, int16_t y)
|*
\*****************************************************************************/
void VDI::v_dspcur(int handle, int16_t x, int16_t y)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		if ((x >= 0) && (y >= 0))
			{
			QPoint p = screen->mapToGlobal(QPoint(x,y));
			fprintf(stderr, "Move to (%d,%d)\n", p.x(), p.y());
			QCursor::setPos(p);
			}
		screen->setCursor(Qt::ArrowCursor);
		}
	else
		{
		WARN("v_dspcur() annot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_dspcur(Transport *io, ClientMsg &cm)
	{
	const Payload &p = cm.payload();
	if (p.size() == 2)
		{
		/*********************************************************************\
		|* Get the data out of the message
		\*********************************************************************/
		int16_t x = ntohs(p[0]);
		int16_t y = ntohs(p[1]);

		int fd = io->socket()->socketDescriptor();
		v_dspcur(fd, x, y);
		}
	else
		WARN("v_dspcur() needs 2 arguments, got %d", (int)p.size());
	}
