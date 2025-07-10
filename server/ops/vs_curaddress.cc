#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.12: Set the alpha cursor to the specified position.
|*
|* Original signature is: v_curaddress(int16_t handle, int16_t row, int16_t col);
|*
\*****************************************************************************/
void VDI::vs_curaddress(int socket, int row, int col)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
								   : nullptr;
	if (ws != nullptr)
		{
		int maxX = (screen->width()  / _alphaWidth ) - 1;
		int maxY = (screen->height() / _alphaHeight) - 1;

		row = (row < 0 )	? 0
			: (row > maxY)	? maxY
							: row;

		col = (col < 0 )	? 0
			: (col > maxX)	? maxX
							: col;

		bool erased = _eraseAlphaCursor();
		_alphaX = col;
		_alphaY = row;
		if (erased)
			_drawAlphaCursor();
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vs_curaddress(Transport *io, ClientMsg &cm)
	{
	const Payload &p = cm.payload();
	int16_t row = ntohs(p[0]);
	int16_t col = ntohs(p[1]);

	int fd = io->socket()->socketDescriptor();
	vs_curaddress(fd, row, col);
	}
