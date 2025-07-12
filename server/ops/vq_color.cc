#include <QFontMetrics>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 26: Query the RGB values of a colour index. Note that the flag is
|*            irrelevant for us, we use truecolour values
|*
|* Original signature is:
|*
|*  vq_color(int16_t handle, int16_t idx, int16_t flag, int16_t* rgb);
\*****************************************************************************/
void VDI::vq_color(int handle, int16_t idx, int16_t* rgb)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	QColor colour = {0,0,0,0};
	if (ws != nullptr)
		{
		if ((idx >= 0) && (idx < 256))
			colour = ws->colour(idx);
		else
			WARN("vq_color index out of range 0..255");
		}
	else
		WARN("vq_color() cannot find workstation for handle %d", handle);

	if (rgb != nullptr)
		{
		rgb[0] = colour.red();
		rgb[1] = colour.green();
		rgb[2] = colour.blue();
		}
	else
		WARN("vq_color passed nil array to store colours into");
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_color(Transport *io, ClientMsg &cm)
	{
	int16_t rgb[3]		= {0,0,0};
	const Payload &p	= cm.payload();
	int num				= p.size();

	if (num ==1)
		{
		int16_t idx			= ntohs(p[0]);
		int fd				= io->socket()->socketDescriptor();
		vq_color(fd, idx, rgb);

		/*********************************************************************\
		|* Construct the message
		\*********************************************************************/
		cm.clear();
		cm.append(rgb, 3);
		cm.setType(MSG_REPLY(ClientMsg::VQ_COLOR));

		/*********************************************************************\
		|* Send the message down the wire
		\*********************************************************************/
		io->write(cm);
		}
	else
		WARN("vq_color() expected 1 arg, got %d", num);
	}
