#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 59	Set a pixel's RGB
|*
|* Original signature is: vs_pixrgb(int16_t handle, int16_t x, int16_t y,
|*									  int16_t* rgb);
\*****************************************************************************/
void VDI::vs_pixrgb(int handle, int16_t x, int16_t y, int16_t *rgb)
	{
	Screen *screen			= Screen::sharedInstance();
	QImage *img				= screen ? screen->bg() : nullptr;

	if (img != nullptr)
		{
		int r = (rgb[0] * 255) / 1000;
		int g = (rgb[1] * 255) / 1000;
		int b = (rgb[2] * 255) / 1000;

		QColor c = QColor(r,g,b);
		img->setPixelColor(x, y, c);
		update(x-1,y-1,3,3);
		}
	else
		{
		WARN("vs_pixrgb() cannot find screen backing for handle %d", handle);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vs_pixrgb(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 5)
		{
		int16_t x		= ntohs(p[0]);
		int16_t y		= ntohs(p[1]);
		int16_t rgb[3];
		for (int i=0; i<3; i++)
			rgb[i] = ntohs(p[i+2]);

		int fd = io->socket()->socketDescriptor();
		vs_pixrgb(fd, x, y, rgb);
		}
	else
		WARN("vs_pixrgb() expect 5 int16_t arguments, got %d", num);
	}
