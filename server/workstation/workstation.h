#ifndef WORKSTATION_H
#define WORKSTATION_H

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

	// current line colour index
	GETSET(int16_t, lineColourIndex, LineColourIndex);

	// current marker type
	GETSET(int16_t, markerType, MarkerType);

	// Default marker colour
	GETSET(int16_t, markerColourIndex, MarkerColourIndex);

	// Default font id
	GETSET(int16_t, fontId, FontId);

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

	// Cursors or graphics
	GET(bool, alphaMode);

	// Current alpha cursor X
	GET(int, alphaX);

	// Current alpha cursor Y
	GET(int, alphaY);

	// Draw in reverse video ?
	GETSET(bool, reverseVideo, ReverseVideo);

	// Transport for data to and from clients. Can be nil
	GETSETP(Transport *, io, Io);

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
	};

#endif // WORKSTATION_H
