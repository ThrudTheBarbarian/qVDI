#include <QApplication>
#include <QPainter>
#include <QScreen>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 107: Set the height of text in points.
|*
|* Original signature is: void vst_point(int16_t handle, int16_t height,
|*						 				 int16_t* charW, int16_t* charH,
|*										 int16_t* cellW, int16_t* cellH);
\*****************************************************************************/
void VDI::vst_point(int handle, int16_t height, int16_t& charWidth,
					 int16_t& charHeight, int16_t& cellWidth,
					 int16_t &cellHeight)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;
	// Get the screen DPI
	QScreen *srn	= QApplication::screens().at(0);
	qreal dpi		= (qreal)srn->logicalDotsPerInch();

	// Get the height, using 1pt = 1/72"
	height	= (height * 72) / dpi;
	height	= (height < 1) ? 1
			: (height > 512) ? 512
			: height;

	if (ws != nullptr)
		{
		ws->setTextHeight(height);

		charWidth	= cellWidth		= (ws->fm()->maxWidth() * dpi) / 72;
		charHeight	= cellHeight	= (ws->fm()->height() * dpi) / 72;
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_point(Transport *io, ClientMsg &cm)
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
	else
		WARN("vst_point() expects 1 argument, got %d", num);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm.clear();
	cm.append(charW);
	cm.append(charH);
	cm.append(cellW);
	cm.append(charH);
	cm.setType(MSG_REPLY(ClientMsg::VST_POINT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	io->write(cm);
	}
