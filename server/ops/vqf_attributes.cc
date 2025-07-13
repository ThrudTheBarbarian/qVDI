#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 37: Get the current fill attributes
|*
|* Original signature is: vqf_attributes(int16_t handle, int16_t *settings);
\*****************************************************************************/
void VDI::vqf_attributes(int handle, int16_t* settings)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (settings == nullptr)
		WARN("vqf_attributes() passed nil buffer to populate!");
	else if (ws != nullptr)
		{
		settings[0] = ws->fillTypeIndex();
		settings[1] = ws->fillColourIndex();
		settings[2] = ws->interiorFillStyle();
		settings[3] = ws->drawPerimeter() ? 1 : 0;
		}
	else
		WARN("vqf_attributes() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqf_attributes(Transport *io, ClientMsg &cm)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		int16_t settings[4];
		vqf_attributes(fd, settings);

		/*********************************************************************\
		|* Construct the message
		\*********************************************************************/
		cm.clear();
		cm.append(settings, 4);
		cm.setType(MSG_REPLY(ClientMsg::VQF_ATTRIBUTES));

		/*********************************************************************\
		|* Send the message down the wire
		\*********************************************************************/
		io->write(cm);
		}
	else
		WARN("vqf_attributes() cannot find IO transport");
	}
