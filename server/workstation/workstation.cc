
#include <QFile>
#include <QPainter>

#include "fillfactory.h"
#include "fontmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* File scoped variables
\*****************************************************************************/

// The next handle to give out for a workstation of any type
static int16_t _nextHandle = 100;

// The types of drawing style available
static Qt::PenStyle _styles[] =
	{
	Qt::SolidLine,			// 0 is undefined
	Qt::SolidLine,			// SOLID
	Qt::DashLine,			// LDASHED
	Qt::DotLine,			// DOTTED
	Qt::DashDotLine,		// DASHDOT
	Qt::DashLine,			// DASH
	Qt::DashDotDotLine,		// DASHDOTDOT
	Qt::CustomDashLine		// USERLINE
	};

static const short _devtovdi8[256] =
	{
	0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11,14,12,15,13,255,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,1
	};

#define PALETTE_OFFSET			"/System/Palettes/"
#define SYSTEM_PALETTE_NAME		"system.pal"

/*****************************************************************************\
|* Constructors
\*****************************************************************************/
Workstation::Workstation(QObject *parent )
			:QObject(parent)
			,_deviceId(-1)
			,_handle(-1)
			,_isVirtual(false)
			,_lineType(SOLID)
			,_lineWidth(3)
			,_markerType(MRKR_ASTERISK)
			,_markerColourIndex(G_BLACK)
			,_markerSize(10)
			,_fontId(0)
			,_textColourIndex(G_BLACK)
			,_interiorFillStyle(FIS_SOLID)
			,_fillTypeIndex(PT_DOTS1)
			,_fillColourIndex(G_BLACK)
			,_drawPerimeter(true)
			,_backgroundColourIndex(G_GREEN)
			,_writingMode(WR_REPLACE)
			,_textHeight(12)
			,_textRotation(0)
			,_textEffect(0)
			,_textHAlign(TA_LEFT)
			,_textVAlign(TA_BASE)
			,_enableClip(false)
			,_clip(QRect(0,0,0,0))
			,_startCap(Qt::FlatCap)
			,_endCap(Qt::FlatCap)
			,_io(nullptr)
			,_fm(nullptr)
	{
	_initialise();
	}

Workstation::Workstation(Transport *io, QObject *parent )
			:QObject(parent)
			,_deviceId(-1)
			,_handle(-1)
			,_isVirtual(false)
			,_lineType(SOLID)
			,_lineWidth(3)
			,_markerType(MRKR_ASTERISK)
			,_markerColourIndex(G_BLACK)
			,_markerSize(10)
			,_fontId(0)
			,_textColourIndex(G_BLACK)
			,_interiorFillStyle(FIS_SOLID)
			,_fillTypeIndex(PT_DOTS1)
			,_fillColourIndex(G_BLACK)
			,_drawPerimeter(true)
			,_backgroundColourIndex(G_GREEN)
			,_writingMode(WR_REPLACE)
			,_textHeight(12)
			,_textRotation(0)
			,_textEffect(0)
			,_textHAlign(TA_LEFT)
			,_textVAlign(TA_BASE)
			,_enableClip(false)
			,_clip(QRect(0,0,0,0))
			,_startCap(Qt::FlatCap)
			,_endCap(Qt::FlatCap)
			,_io(io)
			,_fm(nullptr)
	{
	_initialise();
	}

/*****************************************************************************\
|* Base version of the 'open' call which always fails
\*****************************************************************************/
bool Workstation::open(int16_t *workIn, int16_t *handle, int16_t *workOut)
	{
	// Prevent the warning...
	(void)workIn;

	// Only fill in the handle if it's non-nil
	if (handle)
		*handle = -1;

	// If we have a workOut pointer, we assume it's large enough to be filled
	// with the standard response
	if (workOut)
		memset(workOut, 0, sizeof(ScreenCapabilities));

	// And always fail. You need to call a higher level class to get something
	// useful
	return false;
	}

/*****************************************************************************\
|* Base version of the 'backingImage' call which always returns nil
\*****************************************************************************/
QImage * Workstation::backingImage(void)
	{
	return nullptr;
	}

/*****************************************************************************\
|* Base version of update is a NOP
\*****************************************************************************/
void Workstation::update(void)
	{}

void Workstation::update(QRect &)
	{}

void Workstation::update(QRectF &)
	{}

void Workstation::update(int, int, int, int)
	{}

/*****************************************************************************\
|* Set the device ID (screen, resolution, metafile, etc.) Subclasses might
|* override this
\*****************************************************************************/
void Workstation::setDeviceId(int deviceId)
	{
	_deviceId = deviceId;
	}
/*****************************************************************************\
|* Method: Claim the next available handle
\*****************************************************************************/
void Workstation::claimNextHandle(void)
	{
	_handle = _nextHandle ++;
	}

#pragma mark - Private methods

/*****************************************************************************\
|* Private Method: Initialise the workstation state
\*****************************************************************************/
void Workstation::_initialise(void)
	{
	/*************************************************************************\
	|* Read the palette file
	\*************************************************************************/
	std::string pPath = VDI::sharedInstance().rootDir() + PALETTE_OFFSET
						+ SYSTEM_PALETTE_NAME;

	QFile file(QString(pPath.c_str()));
	int size	= (int)file.size();

	if (file.open(QIODeviceBase::ReadOnly))
		{
		uint8_t *data = (uint8_t *) calloc(1, size);
		if (file.read((char *)data, size) != size)
			{
			WARN("Cannot read palette at %s", pPath.c_str());
			FREE(data);
			return;
			}

		uint16_t * rgb	= (strncmp((char *)data, "PA01", 4) == 0)
							? (uint16_t *)(data + 4)
							: (uint16_t *)data;


		for (int i=0; i<256; i++)
			{
			uint8_t r = ((int)(ntohs(*rgb)) * 255) / 1000; rgb ++;
			uint8_t g = ((int)(ntohs(*rgb)) * 255) / 1000; rgb ++;
			uint8_t b = ((int)(ntohs(*rgb)) * 255) / 1000; rgb ++;
			setColour(i, r, g, b);
			}
		FREE(data);
		}
	else
		WARN("Cannot open palette file %s", pPath.c_str());

	/*************************************************************************\
	|* Set the font to be the system font, whatever that may be
	\*************************************************************************/
	setFontId(-1);

	/*************************************************************************\
	|* Define a default user line style
	\*************************************************************************/
	_userLineType << 3 << 1;
	}

/*****************************************************************************\
|* Set up the font and query the font metrics for it
\*****************************************************************************/
bool Workstation::setFontId(int fontId)
	{
	bool ok = false;

	QFont *font = FontMgr::sharedInstance().fetch(fontId);
	if (font != nullptr)
		{
		ok = true;
		_currentFont = *font;
		if (_fm)
			DELETE(_fm);
		_fm = new QFontMetrics(_currentFont);
		_fontId = fontId;
		}
	else
		WARN("Cannot find system font. We're probably going to crash...");
	return ok;
	}

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForLine(QPen& pen)
	{
	if (_lineType == DASH)
		{
		QList<qreal> dashes;
		dashes << 3 << 3;
		pen.setDashPattern(dashes);
		}
	else if (_lineType == USERLINE)
		pen.setDashPattern(_userLineType);
	else
		pen.setStyle(_styles[_lineType]);

	pen.setColor(_palette[_lineColourIndex]);
	pen.setWidth(_lineWidth);
	}

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForText(QPen& pen)
	{
	pen.setStyle(_styles[SOLID]);

	QColor c = _palette[_textColourIndex];
	if (_textEffect & TXT_LIGHT)
		c = c.lighter();

	pen.setColor(c);
	pen.setBrush(c);
	pen.setWidth(1);
	}

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForMarker(QPen& pen)
	{
	pen.setStyle(_styles[SOLID]);

	pen.setColor(_palette[_markerColourIndex]);
	pen.setBrush(_palette[_markerColourIndex]);
	pen.setWidth(1);
	}

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForFill(QPen& pen)
	{
	FillFactory& ff = FillFactory::sharedInstance();

	pen.setColor(_palette[_fillColourIndex]);
	switch (_fillTypeIndex)
		{
		case FIS_HOLLOW:
			break;
		case FIS_SOLID:
			pen.setBrush(_palette[_fillColourIndex]);
			break;
		default:
				{
				QImage& img = ff.patternFor(_fillTypeIndex, _interiorFillStyle);
				if (img.colorCount() == 2)
					{
					if (_writingMode == WR_TRANSPARENT)
						img.setColor(0, qRgba(0,0,0,0));
					else
						img.setColor(0, _palette[0].rgba());
					img.setColor(1, _palette[_fillColourIndex].rgba());
					}
				pen.setBrush(img);
				}
			break;
		}
	}

/*****************************************************************************\
|* Set the text height and re-create the metrics
\*****************************************************************************/
void Workstation::setTextHeight(int height)
	{
	_textHeight = height;
	_currentFont.setPixelSize(height);
	if (_fm)
		DELETE(_fm);
	_fm = new QFontMetrics(_currentFont);
	}

/*****************************************************************************\
|* Return the palette in VDI form
\*****************************************************************************/
bool Workstation::colourPalette(int16_t *rgb)
	{
	int idx = 0;
	for (int i=0; i<256; i++)
		{
		rgb[idx++] = (_palette[i].red() * 1000) / 256;
		rgb[idx++] = (_palette[i].green() * 1000) / 256;
		rgb[idx++] = (_palette[i].blue() * 1000) / 256;
		}
	return true;
	}

/*****************************************************************************\
|* Return the palette in QT form
\*****************************************************************************/
bool Workstation::colourTable(QList<QRgb>& palette)
	{
	palette.clear();
	palette.reserve(257);
	for (int i=0; i<256; i++)
		palette.append(_palette[_devtovdi8[i]].rgb());
	return true;
	}


/*****************************************************************************\
|* Set up the writing mode
\*****************************************************************************/
void Workstation::setWritingMode(QPainter& painter)
	{
	switch (_writingMode)
		{
		case WR_REPLACE:
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			break;
		case WR_TRANSPARENT:
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			break;
		case WR_XOR:
			painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
			break;
		case WR_REV_TRANS:	// Life is too short to figure out how this maps...
			painter.setCompositionMode(QPainter::RasterOp_NotSourceAndDestination);
			break;
		}
	}
