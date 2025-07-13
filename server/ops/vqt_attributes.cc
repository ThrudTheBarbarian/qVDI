#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 38: Get the current text style
|*
|* Original signature is: vqt_attributes(int16_t handle, int16_t *settings);
\*****************************************************************************/
void VDI::vqt_attributes(int handle, int16_t* settings)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (settings == nullptr)
		WARN("vqt_attributes() passed nil buffer to populate!");
	else if (ws != nullptr)
		{
		settings[0] = ws->fontId();
		settings[1] = ws->textColourIndex();
		settings[2] = ws->textRotation();
		settings[3] = ws->textHAlign();
		settings[4] = ws->textVAlign();
		settings[5] = ws->writingMode();
		settings[6] = ws->fm()->maxWidth();
		settings[7] = ws->fm()->height();
		settings[8] = settings[6];
		settings[9] = settings[7];
		}
	else
		WARN("vqt_attributes() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqt_attributes(Transport *io, ClientMsg &cm)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		int16_t settings[10];
		vqt_attributes(fd, settings);

		/*********************************************************************\
		|* Construct the message
		\*********************************************************************/
		cm.clear();
		cm.append(settings, 10);
		cm.setType(MSG_REPLY(ClientMsg::VQT_ATTRIBUTES));

		/*********************************************************************\
		|* Send the message down the wire
		\*********************************************************************/
		io->write(cm);
		}
	else
		WARN("vqt_attributes() cannot find IO transport");
	}
