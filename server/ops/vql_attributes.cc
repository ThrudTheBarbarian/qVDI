#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 35: Get the current line-drawing style
|*
|* Original signature is: vql_attributes(int16_t handle, int16_t *settings);
|*
\*****************************************************************************/
void VDI::vql_attributes(int handle, int16_t* settings)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (settings == nullptr)
		WARN("vql_attributes() passed nil buffer to populate!");
	else if (ws != nullptr)
		{
		settings[0] = ws->lineType();
		settings[1] = ws->lineColourIndex();
		settings[2] = ws->writingMode();
		settings[3] = ws->lineWidth();
		}
	else
		WARN("vql_attributes() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vql_attributes(Transport *io, ClientMsg &cm)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		int16_t settings[4];
		vql_attributes(fd, settings);

		/*********************************************************************\
		|* Construct the message
		\*********************************************************************/
		cm.clear();
		cm.append(settings, 4);
		cm.setType(MSG_REPLY(ClientMsg::VQL_ATTRIBUTES));

		/*********************************************************************\
		|* Send the message down the wire
		\*********************************************************************/
		io->write(cm);
		}
	else
		WARN("vql_attributes() cannot find IO transport");
	}
