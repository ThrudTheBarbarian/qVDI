#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* 39: Request the text alignment and get the actual set values
|*
|* Original signature is: vst_alignment(int16_t handle,
|*									    int16_t hIn,
|*									    int16_t vIn,
|*									    int16_t *hOut,
|*									    int16_t *vOut);
\*****************************************************************************/
void VDI::vst_alignment(int handle, int16_t  hIn,  int16_t vIn,
						int16_t& hOut, int16_t& vOut)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		switch (hIn)
			{
			case TA_CENTER:
			case TA_RIGHT:
			case TA_FULL:
				ws->setTextHAlign(hIn);
				hOut = hIn;
				break;
			default:
				ws->setTextHAlign(TA_LEFT);
				hOut = TA_LEFT;
				break;
			}

		switch (vIn)
			{
			case TA_HALF:
			case TA_ASCENT:
			case TA_TOP:
				ws->setTextVAlign(TA_TOP);
				vOut = TA_TOP;
				break;

			case TA_BOTTOM:
			case TA_DESCENT:
				ws->setTextVAlign(TA_BOTTOM);
				vOut = TA_BOTTOM;
				break;

			default:
				ws->setTextVAlign(TA_BASE);
				vOut = TA_BASE;
				break;
			}
		}
	else
		{
		WARN("vst_alignment() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_alignment(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	int16_t hOut		= TA_LEFT;
	int16_t vOut		= TA_BASE;

	if (num == 2)
		{
		int16_t hIn		= ntohs(p[0]);
		int16_t vIn		= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vst_alignment(fd, hIn, vIn, hOut, vOut);
		}
	else
		WARN("vst_alignment() expects 2 arguments, got %d", num);

	/**************************************************************************\
	|* Construct the reply
	\**************************************************************************/
	cm.clear();
	cm.append(hOut);
	cm.append(vOut);
	cm.setType(MSG_REPLY(ClientMsg::VST_ALIGNMENT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	io->write(cm);
	}
