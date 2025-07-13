#include <QFontMetrics>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 60	Get a pixel's RGB
|*
|* Original signature is: vq_pixrgb(int16_t handle, int16_t x, int16_t y,
|*									int16_t* rgb;
\*****************************************************************************/
void VDI::vq_pixrgb(int handle, int16_t x, int16_t y, int16_t *rgb)
	{
	Screen *screen			= Screen::sharedInstance();
	QImage *img				= screen ? screen->bg() : nullptr;
	QRgb pix				= 0xff000000;

	if (img != nullptr)
		pix = img->pixel(x, y);
	else
		WARN("vq_pixrgb() cannot find screen backing for handle %d", handle);

	if (rgb != nullptr)
		{
		rgb[0] = (qRed(pix)   * 1000) / 255;
		rgb[1] = (qGreen(pix) * 1000) / 255;
		rgb[2] = (qBlue(pix)  * 1000) / 255;
		}
	else
		WARN("vq_pixrgb passed nil array to store colours into");
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_pixrgb(Transport *io, ClientMsg &cm)
	{
	int16_t rgb[3]		= {0,0,0};
	const Payload &p	= cm.payload();
	int num				= p.size();

	if (num == 2)
		{
		int16_t x			= ntohs(p[0]);
		int16_t y			= ntohs(p[1]);
		int fd				= io->socket()->socketDescriptor();
		vq_pixrgb(fd, x, y, rgb);
		}
	else
		WARN("vq_pixrgb() expected 2 args, got %d", num);

	/*************************************************************************\
	|* Construct the message
	\*************************************************************************/
	cm.clear();
	cm.append(rgb, 3);
	cm.setType(MSG_REPLY(ClientMsg::VQ_PIXRGB));

	/*************************************************************************\
	|* Send the message down the wire
	\*************************************************************************/
	io->write(cm);
	}
