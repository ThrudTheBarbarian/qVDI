#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.15: Get the current cursor position.
|*
|* Original signature is: vq_curaddress(int16_t handle,
|*									    int16_t *row,
|*										int16_t *col);
|*
\*****************************************************************************/
void VDI::vq_curaddress(int handle, int16_t& row, int16_t& col)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		col = _alphaX;
		row = _alphaY;
		}
	else
		{
		WARN("vq_curaddress() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_curaddress(Transport *io, ClientMsg &cm)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		int16_t row = 0, col = 0;
		vq_curaddress(fd, row, col);

		/*********************************************************************\
		|* Construct the message
		\*********************************************************************/
		cm.clear();
		cm.append(row);
		cm.append(col);
		cm.setType(MSG_REPLY(ClientMsg::VQ_CURADDRESS));

		/*********************************************************************\
		|* Send the message down the wire
		\*********************************************************************/
		io->write(cm);
		}
	else
		WARN("vq_curaddress() cannot find IO transport");
	}
