#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.10: Erase to end-of-line.
|*
|* Original signature is: v_eeol(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_eeol(int handle)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
								   : nullptr;
	if (ws != nullptr)
		{
		bool erased = _eraseAlphaCursor();

		QRect r = {_alphaX * _alphaWidth,
				   _alphaY * _alphaHeight,
				   screen->width() - _alphaX * _alphaWidth,
				   _alphaHeight};

		QPainter painter(screen->bg());
		painter.fillRect(r, ws->colour(ws->backgroundColourIndex()));
		update(r);

		if (erased)
			_drawAlphaCursor();
		}
	else
		{
		WARN("v_eeol() cannot find workstation for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_eeol(Transport *io)
	{
	if (io && io->socket())
		{
		int fd = io->socket()->socketDescriptor();
		v_eeol(fd);
		}
	else
		WARN("v_eeol() cannot find IO transport");

	}
