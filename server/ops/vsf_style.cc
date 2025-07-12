#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 24: Set the fill-pattern index.
|*
|* Original signature is: vsf_style(int16_t handle, int16_t which));
\*****************************************************************************/
void VDI::vsf_style(int handle, int16_t idx)
	{
	Screen *screen			= Screen::sharedInstance();
	ConnectionMgr *cmgr		= screen ? screen->cmgr() : nullptr;
	Workstation *ws			= cmgr ? cmgr->findWorkstationForHandle(handle)
						   : nullptr;

	if (ws != nullptr)
		{
		int style = ws->interiorFillStyle();
		int top = (style == FIS_PATTERN) ? PT_PATTERN_MAX
				: (style == FIS_HATCH) ? PT_HATCH_MAX
				: 1;
		idx = idx % top;
		ws->setFillTypeIndex(idx);
		}
	else
		WARN("vsf_style() cannot find workstation for handle %d", handle);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_style(Transport *io, ClientMsg &cm)
	{
	const Payload &p	= cm.payload();
	int num				= p.size();
	if (num == 1)
		{
		int16_t idx	= ntohs(p[0]);

		int fd = io->socket()->socketDescriptor();
		vsf_style(fd, idx);
		}
	else
		WARN("vsf_style() expect 1 argument, got %d", num);
	}
