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
void VDI::v_rmcur(int socket)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
								   : nullptr;
	if (ws != nullptr)
		{
		screen->setCursor(Qt::BlankCursor);
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rmcur(Transport *io)
	{
	int fd = io->socket()->socketDescriptor();
	v_rmcur(fd);
	}
