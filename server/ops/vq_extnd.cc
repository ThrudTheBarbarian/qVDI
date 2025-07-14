#include <cstring>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 102: Query for workstation information.
|*
|* Original signature is: vq_extnd(int16_t handle,
|*								   int16_t extend,
|*								   int16_t *workOut		[n ints returned]
\*****************************************************************************/
void VDI::vq_extnd(int handle, int16_t extend, int16_t* data)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
							: nullptr;
	if (ws != nullptr)
		{
		int idx = 0;
		if (data != nullptr)
			{
			if (extend == 0)
				{
				data[idx++]	= screen->width()-1;		// xMax
				data[idx++]	= screen->height()-1;		// yMax
				data[idx++]	= 0;						// Can scale
				data[idx++]	= 18;						// Pixel width in microns
				data[idx++]	= 18;						// Pixel height in microns
				data[idx++]	= 0;						// smooth scaling
				data[idx++]	= USERLINE;					// # line types
				data[idx++]	= 0;						// smooth line widths
				data[idx++]	= MRKR_DIAMOND;				// # marker types
				data[idx++]	= 0;						// smooth marker scaling
				data[idx++]	= 1;						// number of fonts
				data[idx++]	= PT_CHEVRON;				// # pattern types
				data[idx++]	= PT_SQUARE2;				// # hatching styles
				data[idx++]	= 32767;					// # colours
				data[idx++]	= GDP_NONE;					// # GDP types
				data[idx++]	= GDP_BAR;					// rect
				data[idx++]	= GDP_ARC;					// arc
				data[idx++]	= GDP_PIESLICE;				// filled arc
				data[idx++]	= GDP_FILLED_CIRCLE;		// filled circle
				data[idx++]	= GDP_FILLED_ELLIPSE;		// filled ellipse
				data[idx++]	= GDP_ELLIPTICAL_ARC;		// elliptical arc
				data[idx++]	= GDP_ELLIPTICAL_PIE;		// filled ell. arc
				data[idx++]	= GDP_ROUNDED_RECT;			// rounded rect
				data[idx++]	= GDP_FILLED_RRECT;			// filled r.rect
				data[idx++]	= GDP_JUSTIFIED_TEXT;		// justified text
				data[idx++]	= GDP_FILL;					// attrib
				data[idx++]	= GDP_LINE;					// attrib
				data[idx++]	= GDP_FILL;					// attrib
				data[idx++]	= GDP_FILL;					// attrib
				data[idx++]	= GDP_FILL;					// attrib
				data[idx++]	= GDP_LINE;					// attrib
				data[idx++]	= GDP_FILL;					// attrib
				data[idx++]	= GDP_LINE;					// attrib
				data[idx++]	= GDP_FILL;					// attrib
				data[idx++]	= GDP_TEXT;					// attrib
				data[idx++]	= 1;						// colour-capable
				data[idx++]	= 1;						// can rotate text
				data[idx++]	= 1;						// can perform fills
				data[idx++]	= 0;						// no cell-array
				data[idx++]	= 0;						// > 32767 colours
				data[idx++]	= 2;						// # locators (keyb,mse)
				data[idx++]	= 1;						// #valuators (arrows)
				data[idx++]	= 1;						// # choice (fns)
				data[idx++]	= 1;						// keyboard present
				data[idx++]	= 2;						// both input & output
				data[idx++]	= 5;						// min char width (pix)
				data[idx++]	= 4;						// min char height
				data[idx++]	= 7;						// max char width
				data[idx++]	= 13;						// max char ht
				data[idx++]	= 1;						// min line width
				data[idx++]	= 0;						// unused
				data[idx++]	= 128;						// max line width
				data[idx++]	= 0;						// unused
				data[idx++]	= 15;						// min marker width
				data[idx++]	= 1;						// min marker ht
				data[idx++]	= 120;						// max marker wd
				data[idx++]	= 88;						// max marker ht
				}
			else
				{
				data[idx++]	= 4;						// combined alpha/gfx
				data[idx++]	= 32767;					// # colours
				data[idx++]	= TXT_MASK;					// # text effects
				data[idx++]	= 1;						// can scale rasters
				data[idx++]	= 24;						// # bitplanes
				data[idx++]	= 1;						// pseudo CLUT
				data[idx++]	= 32767;					// # ops/sec
				data[idx++]	= 1;						// can contour fill
				data[idx++]	= 2;						// smooth text rotate
				data[idx++]	= 4;						// # writing modes
				data[idx++]	= 2;						// can req and sample
				data[idx++]	= 1;						// can text align
				data[idx++]	= 0;						// inking ? Nope
				data[idx++]	= 2;						// rubberband lns/rcts
				data[idx++]	= -1;						// unlimited vertices
				data[idx++]	= -1;						// unlimited intin
				data[idx++]	= 2;						// # mouse buttons
				data[idx++]	= 1;						// line-types for >1
				data[idx++]	= 4;						// wrmodes for >1 lines
				data[idx++]	= 1;						// clipping flag
				data[idx++]	= 0;						// No extended precision
				data[28]    = 1;						// has bezier
				data[30]	= 1;						// vrt/vro_cpyfm can scale
				}
			}
		else
			WARN("vq_extnd passed nil array to store into");
		}
	else
		WARN("vq_extnd() cannot find workstation for handle %d", handle);

	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_extnd(Transport *io, ClientMsg &cm)
	{
	int16_t data[57];
	const Payload &p	= cm.payload();
	int num				= p.size();
	int16_t mode		= 0;

	if (num ==1)
		{
		mode		= ntohs(p[0]);
		int fd		= io->socket()->socketDescriptor();
		vq_extnd(fd, mode, data);
		}
	else
		{
		WARN("vq_extnd() expected 1 arg, got %d", num);
		memset(data, 0, sizeof(int16_t)*57);
		}

	/*************************************************************************\
	|* Construct the message
	\*************************************************************************/
	cm.clear();
	cm.setType(MSG_REPLY(ClientMsg::VQ_EXTND));
	cm.append(data, (mode == 0) ? 57 : 31);

	/*************************************************************************\
	|* Send the message down the wire
	\*************************************************************************/
	io->write(cm);
	}
