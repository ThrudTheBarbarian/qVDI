#include "clientmsg.h"
#include "gemTypes.h"
#include "screenws.h"
#include "transport.h"
#include "vdi.h"
#include "workstation.h"

#define CHECK_RANGE(x,a,b)		(((x) < (a)) || ((x) > (b))) ? (a) : (x)
#define CHECK_RANGE2(x,a,b,c)	(((x) < (a)) || ((x) > (b))) ? (c) : (x)
#define CHECK_RANGE3(x,a,b,c,d)	(((x) < (a)) || ((x) > (b))) ? (c) : (d)

static int16_t _defaultValues[] =
	{
		0,					// 0 : Xmax
		0,					// 1 : Ymax
		0,					// 2 : device can scale
		18,					// 3 : pixel width in microns
		18,					// 4 : pixel height in microns
		0,					// 5 : continuous character height scaling
		USERLINE,			// 6 : # line types (1->n)
		0,					// 7 : continuous line width scaling
		MRKR_DIAMOND,		// 8 : # marker types (1->n)
		0,					// 9 : continuous marker height scaling
		1,					// 10: number of accessible fonts
		PT_CHEVRON,			// 11: # pattern types (1->n)
		PT_SQUARE2,			// 12: # hatch-styles (1->n)
		32767,				// 13: # of simultaneous colours
		GDP_NONE,			// 14: # of generalised drawing types
		GDP_BAR,			// 15: Rectangle
		GDP_ARC,			// 16: Arc
		GDP_PIESLICE,		// 17: Arc + filled to center
		GDP_FILLED_CIRCLE,	// 18: Filled circle
		GDP_FILLED_ELLIPSE,	// 19: Filled ellipse
		GDP_ELLIPTICAL_ARC,	// 20: Elliptical arc
		GDP_ELLIPTICAL_PIE,	// 21: Elliptical arc + filled to center
		GDP_ROUNDED_RECT,	// 22: Rounded rect
		GDP_FILLED_RRECT,	// 23: Filled rounded rect
		GDP_JUSTIFIED_TEXT,	// 24: Justified text
		GDP_FILL,			// 25: Attribute for rectangle
		GDP_LINE,			// 26: Attribute for arc
		GDP_FILL,			// 27: Attribute for pieslice
		GDP_FILL,			// 28: Attribute for filled circle
		GDP_FILL,			// 29: Attribute for filled ellipse
		GDP_LINE,			// 30: Attribute for elliptical arc
		GDP_FILL,			// 31: Attribute for elliptical pie
		GDP_LINE,			// 32: Attribute for rounded rect
		GDP_FILL,			// 33: Attribute for filled rounded rect
		GDP_TEXT,			// 34: Attribute for text
		1,					// 35: we are colour-capable
		1,					// 36: we can rotate text
		1,					// 37: we can perform fills
		0,					// 38: we do not perform cell-array drawing
		0,					// 39: we have > 32767 colours
		2,					// 40: # locators (keyboard & mouse)
		1,					// 41: # valuators (arrow keys on keyboard)
		1,					// 42: # choice devices (function keys)
		1,					// 43: we have a keyboard
		2,					// 44: device is input & output
		5,					// 45: minimum character width in pixels
		4,					// 46: minimum character height in pixels
		7,					// 47: maximum character width in pixels
		13,					// 48: maximum character height in pixels
		1,					// 49: minimum line width in pixels
		0,					// 50: unused
		128,				// 51: maximum line width in pixels
		0,					// 52: unused
		15,					// 53: minimum marker width in pixels
		1,					// 54: minimum marker height in pixels
		120,				// 55: maximum marker width in pixels
		88,					// 56: maximum marker height in pixels
};

/*****************************************************************************\
|*   1   : Open a physical workstation
\*****************************************************************************/
void VDI::v_opnwk(ConnectionMgr *cm, Transport *io, ClientMsg &msg)
	{
	/*************************************************************************\
	|* Set any defaults that were sent through (<0 == ignore)
	\*************************************************************************/
	const Payload &payload = msg.payload();
	int16_t workIn[16];

	for (int i=0; i<12; i++)
		workIn[i] = ntohs(payload[i]);

	Workstation *ws = nullptr;
	/*************************************************************************\
	|* See if this is a screen device, if so (and it better had be, that's all
	|* we support) then create an instance of a ScreenWS
	\*************************************************************************/
	if ((workIn[0] >= 1) && (workIn[0] <= 10))
		ws = new ScreenWS(this);
	else
		ws = new Workstation(this);
	cm->mapTransportToWorkstation(io, ws);

	ws->setDeviceId(workIn[0]);
	ws->claimNextHandle();

	/*************************************************************************\
	|* workIn[1]:  Default line type (vsl_type)
	\*************************************************************************/
	ws->setLineType(CHECK_RANGE(workIn[1], SOLID, USERLINE));

	/*************************************************************************\
	|* workIn[2]:  Default line colour index
	\*************************************************************************/
	ws->setLineColourIndex(CHECK_RANGE2(workIn[2],
										G_WHITE,
										G_LMAGENTA,
										G_BLACK));

	/*************************************************************************\
	|* workIn[3]:  Default marker type (vsm_type)
	\*************************************************************************/
	ws->setMarkerType(CHECK_RANGE(workIn[3], MRKR_DOT, MRKR_DIAMOND));

	/*************************************************************************\
	|* workIn[4]:  Default marker colour
	\*************************************************************************/
	ws->setMarkerColourIndex(CHECK_RANGE2(workIn[4],
										  G_WHITE,
										  G_LMAGENTA,
										  G_BLACK));

	/*************************************************************************\
	|* workIn[5]:  Font id
	\*************************************************************************/
	// FIXME: This should actually look things up
	ws->setFontId(CHECK_RANGE(workIn[5], -1, 1024));

	/*************************************************************************\
	|* workIn[6]:  Default text colour index
	\*************************************************************************/
	ws->setTextColourIndex(CHECK_RANGE2(workIn[6],
										G_WHITE,
										G_LMAGENTA,
										G_BLACK));

	/*************************************************************************\
	|* workIn[7]:  Type of fill (vsf_interior)
	\*************************************************************************/
	ws->setInteriorFillStyle(CHECK_RANGE2(workIn[7],
										  FIS_HOLLOW,
										  FIS_USER,
										  FIS_SOLID));

	/*************************************************************************\
	|* workIn[8]:  Style to use for filling (vsf_style)
	\*************************************************************************/
	ws->setFillIndex(CHECK_RANGE(workIn[8], PT_DOTS1, PT_CHEVRON));

	/*************************************************************************\
	|* workIn[9]:  Default fill colour
	\*************************************************************************/
	ws->setFillColourIndex(CHECK_RANGE2(workIn[9],
										G_WHITE,
										G_LMAGENTA,
										G_BLACK));

	/*************************************************************************\
	|*  workIn[10]:  co-ordinate type
	\*************************************************************************/
	if ((workIn[10] == COORD_NDC) || (workIn[10] == COORD_RASTER))
		ws->setCoordType(workIn[10]);
	else
		ws->setCoordType(COORD_RASTER);

	/*************************************************************************\
	|*  workIn[11]:  page size
	\*************************************************************************/
	int pg = workIn[11] & 0xF;
	ws->setPageSize(CHECK_RANGE3(pg,
								 PG_DEFAULT,
								 PG_DIN_B5,
								 PG_DEFAULT,
								 workIn[11]));

	// workIn[12]:  not used
	// workIn[13]:  pointer to filename or 0L. Ignore for now
	// workIn[14]:  not used
	// workIn[15]:  not used

	/*********************************************************************\
	|* Prepare the response
	\*********************************************************************/
	int numEntries	= sizeof(_defaultValues)/sizeof(int16_t);
	int16_t workOut[60];
	Q_ASSERT(numEntries < 60);
	memcpy(workOut, _defaultValues, sizeof(_defaultValues));

	Screen *screen	= Screen::sharedInstance();
	workOut[0] = screen->width()-1;
	workOut[1] = screen->height()-1;

	/*********************************************************************\
	|* Fill out the response
	\*********************************************************************/
	msg.clear();
	msg.setType(MSG_REPLY(ClientMsg::V_OPNWK));
	msg.append(workOut, numEntries);
	msg.append(ws->handle());

	/*********************************************************************\
	|* Fetch the character width, height and screen rows/cols
	\*********************************************************************/
	int16_t rows, cols;
	vq_chcells(io->socket()->socketDescriptor(), rows, cols);

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	io->write(msg, true);

	_top = ws;	// FIXME: Should be in a LIFO queue
	}

