#include <QColor>
#include <QRgb>

#include "clientmsg.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 105	Get a pixel. This is compatible with the old GEM interface
|*              but only works to 16-bit colour resolution. Use the new
|*				vq_pixrgb (opcode 60) to get 24-bit RGB
|*
|* Original signature is: v_get_pixel(int16_t handle, int16_t x, int16_t y,
|*									  int16_t* pixel, int16_t *idx);
\*****************************************************************************/
void VDI::v_get_pixel(int handle, int16_t x, int16_t y,
					  int16_t& pixel, int16_t& idx)
	{
	Screen *screen			= Screen::sharedInstance();
	QImage *img				= screen ? screen->bg() : nullptr;

	if (img != nullptr)
		{
		QRgb rgb = img->pixel(x, y);

		idx		= -1;
		pixel	= (((uint16_t)(qRed(rgb) & 0x1f)) << 11)
				| (((uint16_t)(qGreen(rgb) & 0x3f)) << 5)
				| ((uint16_t)qBlue(rgb) & 0x1f);
		}
	else
		WARN("v_get_pixel() cannot find screen backing for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_get_pixel(Transport *io, ClientMsg &cm)
	{
	int16_t px			= 0;
	int16_t idx			= -1;
	const Payload &p	= cm.payload();
	int num				= p.size();

	if (num == 2)
		{
		int16_t x			= ntohs(p[0]);
		int16_t y			= ntohs(p[1]);
		int fd				= io->socket()->socketDescriptor();
		v_get_pixel(fd, x, y, px, idx);
		}
	else
		WARN("v_get_pixel() expected 2 args, got %d", num);

	/*************************************************************************\
	|* Construct the message
	\*************************************************************************/
	cm.clear();
	cm.append(px);
	cm.append(idx);
	cm.setType(MSG_REPLY(ClientMsg::V_GET_PIXEL));

	/*************************************************************************\
	|* Send the message down the wire
	\*************************************************************************/
	io->write(cm);
	}
