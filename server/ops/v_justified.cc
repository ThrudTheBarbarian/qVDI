#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|*  11.10: Draw justfied text
\*****************************************************************************/
void VDI::v_justified(int handle, int16_t x, int16_t y,
					  char *str, int16_t xPixels,
					  int16_t wordSpacing, int16_t charSpacing)
	{
	(void)wordSpacing;
	(void)charSpacing;

	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		// v_gtext will justify
		int halign = ws->textHAlign();
		ws->setTextHAlign(TA_FULL);
		v_gtext(handle, x, y, xPixels, str);
		ws->setTextHAlign(halign);
		}
	else
		{
		WARN("v_justified cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_justified(Transport *io, ClientMsg &cm)
	{
	const Payload &p		= cm.payload();
	int16_t num				= p.size();

	if (num > 5)
		{
		int16_t x			= ntohs(p[0]);
		int16_t y			= ntohs(p[1]);
		int16_t xPixels		= ntohs(p[2]);
		int16_t ws			= ntohs(p[3]);
		int16_t cs			= ntohs(p[4]);

		QByteArray ba;
		cm.fetchData(5, ba);

		int fd = io->socket()->socketDescriptor();
		v_justified(fd, x, y, ba.data(), xPixels, ws, cs);
		}
	else
		WARN("v_justified() sent %d arguments (expect > 5)",num);
	}

