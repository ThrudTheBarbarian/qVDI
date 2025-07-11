#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 12: Set the height of text in pixels. Return metrics.
\*****************************************************************************/
void VDI::vst_height(int handle, int16_t height, int16_t& charWidth,
					 int16_t& charHeight, int16_t& cellWidth,
					 int16_t &cellHeight)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	height	= (height < 1) ? 1
			: (height > 512) ? 512
			: height;

	if (ws != nullptr)
		{
		ws->setTextHeight(height);

		charWidth	= cellWidth		= ws->fm()->maxWidth();
		charHeight	= cellHeight	= ws->fm()->height();
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_height(Transport *io, ClientMsg &cm)
	{
	int16_t charW		= 0;
	int16_t charH		= 0;
	int16_t cellW		= 0;
	int16_t cellH		= 0;

	const Payload &p	= cm.payload();
	int16_t num			= p.size();
	if (num == 1)
		{
		int16_t height		= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vst_height(fd, height, charW, charH, cellW, cellH);
		}

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm.clear();
	cm.append(charW);
	cm.append(charH);
	cm.append(cellW);
	cm.append(charH);
	cm.setType(MSG_REPLY(ClientMsg::VST_HEIGHT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	io->write(cm);
	}
