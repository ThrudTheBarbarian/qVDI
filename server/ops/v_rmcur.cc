#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.19: Hide the graphic cursor.
|*
|* Original signature is: v_rmcur(int1`6_t handle)
|*
\*****************************************************************************/
void VDI::v_rmcur(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		screen->setCursor(Qt::BlankCursor);
		}
	else
		{
		WARN("v_rmcur() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rmcur(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_rmcur(fd);
		}
	else
		WARN("v_rmcur() cannot find IO transport");
	}
