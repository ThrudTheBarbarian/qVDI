#include <QPainter>

#include "debug.h"
#include "fontmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 21: Set the font to use.
|*
|* Original signature is: int16_t vst_font (int16_t handle, int16_t font);
\*****************************************************************************/
int16_t VDI::vst_font(int handle, int16_t fontId)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	QFont *font = FontMgr::sharedInstance().fetch(fontId);
	if (font == nullptr)
		fontId = -1;

	if (ws != nullptr)
		ws->setFontId(fontId);
	else
		WARN("vst_font() cannot find workstation for handle %d", handle);

	return fontId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_font(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t fontId	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		fontId = vst_font(fd, fontId);

		/**********************************************************************\
		|* Construct the message
		\**********************************************************************/
		cm.clear();
		cm.append(fontId);
		cm.setType(MSG_REPLY(ClientMsg::VST_FONT));

		/**********************************************************************\
		|* Send the message down the wire
		\**********************************************************************/
		io->write(cm);
		}
	else
		WARN("vst_font() expect 1 argument, got %d", num);
	}
