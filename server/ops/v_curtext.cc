#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.6: Draw text at the cursor position.
|*
|* Original signature is: v_curtext(int16_t handle, int8_t *string);
|*
\*****************************************************************************/
void VDI::v_curtext(int socket, const char *str)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(socket)
								   : nullptr;
	if (ws != nullptr)
		{
		bool erased = _eraseAlphaCursor();

		QColor fg	= ws->colour((_reverseVideo) ? G_WHITE : G_BLACK);
		QColor bg	= ws->colour((_reverseVideo) ? G_BLACK : G_WHITE);

		int descent = ws->fm()->descent();
		QRect r		= {	_alphaX * _alphaWidth,
						_alphaY * _alphaHeight + descent,
						0,
						_alphaHeight};

		int len = (int) strlen(str);
		r.setWidth(_alphaWidth * len);


		QPainter P(screen->bg());
		P.fillRect(r,bg);
		P.setPen(fg);
		P.drawText(r.x(), _alphaY * _alphaHeight + _alphaHeight, str);
		update(r);

		int maxX = screen->width() / _alphaWidth;
		_alphaX += len;
		if (_alphaX > maxX)
			_alphaX = maxX;

		if (erased)
			_drawAlphaCursor();
		}
	else
		{
		WARN("Cannot find workstation for socket connection %d", socket);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curtext(Transport *io, ClientMsg &cm)
	{
	QByteArray ba;
	cm.fetchData(0, ba);
	const char *str = ba.constData();

	int fd = io->socket()->socketDescriptor();
	v_curtext(fd, str);
	}
