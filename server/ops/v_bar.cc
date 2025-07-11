#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 11.1	Fill a rectangle		[type=1] [pxy=x0,y0,x1,y1]
|*
\*****************************************************************************/
void VDI::v_bar(int socket, int16_t num, int16_t*pxy)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
						   : nullptr;

	if (ws != nullptr)
		{
		v_fillarea(socket, RECT, num, pxy);
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_bar(Transport *io, ClientMsg &cm)
	{
	const Payload &p		= cm.payload();
	int16_t num				= ntohs(p[0]);
	if (num == (int16_t)(p.size()-1))
		{
		int16_t *pxy		= (int16_t *)(&(p[1]));

		/**********************************************************************\
		|* Get the data out of the message
		\**********************************************************************/
		for (int i=0; i<num*2; i++)
			pxy[i] = ntohs(pxy[i]);

		int fd = io->socket()->socketDescriptor();
		v_bar(fd, num, pxy);
		}
	else
		WARN("v_bar sent %d arguments (expect %d)", (int)p.size()-1, num);
	}

