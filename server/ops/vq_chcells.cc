#include <QFontMetrics>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 5.1: Query the number of character cells on the alpha display.
|*
|* Original signature is:
|*
|*  vq_chcells(int16_t handle, int16_t& rows, int16_t& columns);
|*
\*****************************************************************************/
void VDI::vq_chcells(int handle, int16_t& rows, int16_t& columns)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		QFontMetrics *fm	= ws->fm();
		QRect bounds		= fm->boundingRect("W");
		_alphaHeight		= bounds.height();
		_alphaWidth			= bounds.width();
		rows				= screen->height() / _alphaHeight;
		columns				= screen->width() / _alphaWidth;
		}
	else
		WARN("v_chcells() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_chcells(Transport *io, Workstation *, ClientMsg &cm)
	{
	int16_t rows	= 0;
	int16_t cols	= 0;
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();

		vq_chcells(fd, rows, cols);

		/*********************************************************************\
		|* Construct the message
		\*********************************************************************/
		cm.clear();
		cm.append(rows);
		cm.append(cols);
		cm.setType(MSG_REPLY(ClientMsg::VQ_CHCELLS));

		/*********************************************************************\
		|* Send the message down the wire
		\*********************************************************************/
		io->write(cm);
		}
	else
		WARN("vq_chcells() cannot find IO transport");
	}
