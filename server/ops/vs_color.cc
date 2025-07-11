#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 14: Set the RGB for a colour
|*
|* Original signature is: vs_color(int16_t handle, int16_t idx, int16_t rgb[3]);
\*****************************************************************************/
void VDI::vs_color(int handle, int16_t idx, int16_t *rgb)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		if ((idx >= 0) && (idx < 256))
			{
			for (int i=0; i<3; i++)
				rgb[i]	= (rgb[i] < 0) ? 0
						: (rgb[1] > 1000) ? 1000
						: rgb[i];

			uint8_t R = (uint8_t)(((255 * rgb[0]) / 1000));
			uint8_t G = (uint8_t)(((255 * rgb[1]) / 1000));
			uint8_t B = (uint8_t)(((255 * rgb[2]) / 1000));
			ws->setColour(idx, R, G, B);
			}
		else
			WARN("index %d out of range for vs_color", idx);
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vs_color(Transport *io, ClientMsg &cm)
	{
	const Payload &p		= cm.payload();
	int16_t num				= p.size();
	if (num == 4)
		{
		/**********************************************************************\
		|* Get the data out of the message
		\**********************************************************************/
		int16_t idx			= ntohs(p[0]);

		int16_t rgb[3];
		for (int i=0; i<3; i++)
			rgb[i] = ntohs(p[i+1]);

		int fd = io->socket()->socketDescriptor();
		vs_color(fd, idx, rgb);
		}
	else
		WARN("vs_color sent %d arguments (expect 4)", num);
	}

