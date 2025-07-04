#ifndef WORKSTATION_H
#define WORKSTATION_H

#include <QFont>
#include <QImage>
#include <QObject>

#include "properties.h"
#include "transport.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/

/*****************************************************************************\
|* Class declaration. This class holds the state of the workstation - it's
|*                    effectively a graphics context. You get one by opening
|*                    the physical or virtual workstation for a device.
\*****************************************************************************/
class Workstation : public QObject
	{
	Q_OBJECT

	/*************************************************************************\
	|* Properties
	\*************************************************************************/

	// The current device-type (1=same-res, 2+n = forced res, >10 = device
	GET(int, deviceId);

	// The handle as passed to the client
	GET(int16_t, handle);

	// Whether this is a workstation opened by v_opnvwk()
	GETSET(bool, isVirtual, IsVirtual);

	// current line type
	GETSET(int16_t, lineType, LineType);

	// current line width
	GETSET(int16_t, lineWidth, LineWidth);

	// current line colour index
	GETSET(int16_t, lineColourIndex, LineColourIndex);

	// User defined line
	GETSET(LineStyle, userLineType, UserLineType);

	// current marker type
	GETSET(int16_t, markerType, MarkerType);

	// Default marker colour
	GETSET(int16_t, markerColourIndex, MarkerColourIndex);

	// Default marker size, top-to-bottom and left-to-right
	GETSET(int16_t, markerSize, MarkerSize);

	// Default font id
	GET(int16_t, fontId);

	// current text colour index
	GETSET(int16_t, textColourIndex, TextColourIndex);

	// Default fill interior style
	GETSET(int16_t, interiorFillStyle, InteriorFillStyle);

	// Default fill index (which hatch or pattern to use, if interiorStyle
	// says we fill in one of those two methods)
	GETSET(int16_t, fillIndex, FillIndex);

	// current fill colour index
	GETSET(int16_t, fillColourIndex, FillColourIndex);

	// current background fill colour index
	GETSET(int16_t, backgroundColourIndex, BackgroundColourIndex);

	// Which co-ordinate system to use
	GETSET(int16_t, coordType, CoordType);

	// Which page size to use
	GETSET(int16_t, pageSize, PageSize);

	// Whether we're enabling clip or not
	GETSET(bool, enableClip, EnableClip);

	// The actual clipping rectangle
	GETSET(QRect, clip, Clip);

	// How we draw the ends of the line. This specifies start and end
	// separately, but QT only offers both or none, so we do both
	GETSET(Qt::PenCapStyle, startCap, StartCap);
	GETSET(Qt::PenCapStyle, endCap, endCap);

	// How to draw things on-screen
	GET(int16_t, writingMode);

	// Transport for data to and from clients. Can be nil
	GETSETP(Transport *, io, Io);

	// Font-metrics for this WS
	GETP(QFontMetrics*, fm);

	// Current font
	GET(QFont, currentFont);

	/*************************************************************************\
	|* Private methods
	\*************************************************************************/
	void _initialise(void);				// Set up the initial state

	/*************************************************************************\
	|* Protected state
	\*************************************************************************/
	protected:
		QColor _palette[256];			// First 256 colours

	/*************************************************************************\
	|* constructor
	\*************************************************************************/
	public:
		explicit Workstation(QObject *parent = nullptr);
		explicit Workstation(Transport *io, QObject *parent = nullptr);


		/*********************************************************************\
		|* Method: Fetch the backing store for this workstation, if avaialble
		\*********************************************************************/
		virtual QImage * backingImage(void);

		/*********************************************************************\
		|* Method: open a workstation, returning success or failure, and
		|*         taking in the standard arrays/handle pointer. The data in
		|*		   work_out will be filled out, and the handle set to >0 on
		|*         success.
		\*********************************************************************/
		virtual bool open(int16_t *workIn, int16_t *handle, int16_t *workOut);

		/*********************************************************************\
		|* Method: Set the device type for the workstation, this can be
		|*         overridden in subclasses
		\*********************************************************************/
		virtual void setDeviceId(int deviceId);

		/*********************************************************************\
		|* Method: Claim the next available handle
		\*********************************************************************/
		void claimNextHandle(void);

		/*********************************************************************\
		|* Method: tell the workstation it needs to update within a given area,
		|*         or even the entire area
		\*********************************************************************/
		virtual void update(void);
		virtual void update(QRect& r);
		virtual void update(int x, int y, int w, int h);

		/*********************************************************************\
		|* Get/Set colours in the palette
		\*********************************************************************/
		inline void setColour(uint32_t idx,
							  uint8_t r,
							  uint8_t g,
							  uint8_t b,
							  uint8_t a = 255)
			{
			if (idx < 256)
				_palette[idx] = QColor(r,g,b,a);
			}

		inline void setColour(uint32_t idx, QColor c)
			{
			if (idx < 256)
				_palette[idx] = c;
			}

		inline QColor colour(int idx)
			{
			if (idx < 256)
				return _palette[idx];
			return QColor(0,0,0,255);
			}

		/*********************************************************************\
		|* Create a font by font-id
		\*********************************************************************/
		bool setFontId(int fontId);

		/*********************************************************************\
		|* Set up the pen for drawing based on the local state
		\*********************************************************************/
		void setupPenForLine(QPen& pen);
		void setupPenForMarker(QPen& pen);

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		void setWritingMode(QPainter& painter);
		};

#endif // WORKSTATION_H
