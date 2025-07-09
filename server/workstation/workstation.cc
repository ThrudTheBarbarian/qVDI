
#include <QFile>
#include <QPainter>

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
			,_markerSize(5)
			,_fontId(0)
			,_textColourIndex(G_BLACK)
			,_interiorFillStyle(FIS_SOLID)
			,_fillIndex(PT_DOTS1)
			,_fillColourIndex(G_BLACK)
			,_backgroundColourIndex(G_GREEN)
			,_textHeight(12)
			,_textRotation(0)
			,_textEffect(0)
			,_textHAlign(TA_LEFT)
			,_textVAlign(TA_BASE)
			,_enableClip(false)
			,_clip(QRect(0,0,0,0))
			,_startCap(Qt::FlatCap)
			,_endCap(Qt::FlatCap)
			,_writingMode(WR_REPLACE)
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
			,_markerSize(5)
			,_fontId(0)
			,_textColourIndex(G_BLACK)
			,_interiorFillStyle(FIS_SOLID)
			,_fillIndex(PT_DOTS1)
			,_fillColourIndex(G_BLACK)
			,_backgroundColourIndex(G_GREEN)
			,_textHeight(12)
			,_textRotation(0)
			,_textEffect(0)
			,_textHAlign(TA_LEFT)
			,_textVAlign(TA_BASE)
			,_enableClip(false)
			,_clip(QRect(0,0,0,0))
			,_startCap(Qt::FlatCap)
			,_endCap(Qt::FlatCap)
			,_writingMode(WR_REPLACE)
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
