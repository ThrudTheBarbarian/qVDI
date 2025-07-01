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
void VDI::vq_chcells(qintptr handle, int16_t& rows, int16_t& columns)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen->cmgr();
	Workstation *ws			= cmgr->findWorkstationForHandle(handle);
	if (ws != nullptr)
		{
		QFontMetrics *fm	= ws->fm();
		QRect bounds		= fm->boundingRect("W");
		ws->setAlphaHeight(bounds.height());
		ws->setAlphaWidth(bounds.width());
		rows				= screen->height() / bounds.height();
		columns				= screen->width() / bounds.width();
		fprintf(stderr, "ch: %d, %d    scrn: %d, %d",
			bounds.width(), bounds.height(),
				screen->width(), screen->height());
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_chcells(Transport *io, Workstation *ws, ClientMsg &cm)
	{
	int16_t rows	= 0;
	int16_t cols	= 0;
	int fd			= io->socket()->socketDescriptor();

	vq_chcells(fd, rows, cols);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm.clear();
	cm.append(rows);
	cm.append(cols);
	cm.setType(MSG_REPLY(ClientMsg::VQ_CHCELLS));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	io->write(cm);
	}
