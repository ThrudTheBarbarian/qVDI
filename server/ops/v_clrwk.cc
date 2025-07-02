#include <QPainter>

#include "screen.h"
#include "transport.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 3: Clear a physical workstation.
|*
|* Original signature is: v_clrwk(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_clrwk(Transport *, Workstation *ws)
	{
	if (ws != nullptr)
		{
		QImage *img		= ws->backingImage();
		QPainter p(img);

		int colourId	= ws->backgroundColourIndex();
		QColor colour	= ws->colour(colourId);

		// Clear the backing pixmap, and update the entire thing
		p.fillRect(0, 0, img->width(), img->height(), colour);
		ws->update();
		}
	else
		WARN("Cannot find workstation to clear");

	}
