#ifndef SCREEN_H
#define SCREEN_H

#include <QMainWindow>
#include <QTimer>

#include "connectionmgr.h"
#include "gemTypes.h"
#include "properties.h"

/*****************************************************************************\
|* Namespaces
\*****************************************************************************/

QT_BEGIN_NAMESPACE
namespace Ui
	{
	class Screen;
	}
QT_END_NAMESPACE

/*****************************************************************************\
|* Hardware capabilities
\*****************************************************************************/
typedef struct _screenCapabilties
	{
	// Maximum addressable width (0 - Xmax)
	int16_t xMax;

	// 	Maximum addressable height (0 - Ymax)
	int16_t yMax;

	// Device capable of producing a precisely scaled image ?
	int16_t scalable;

	// Pixel width in microns
	int16_t pixelWidth;

	// Pixel height in microns
	int16_t pixelHeight;

	// Number of character heights (0: continous scaling)
	int numCharHeights;

	// Number of line types
	int numLineTypes;

	// Number of line widths (0: continous scaling)
	int16_t numLineWidths;

	// Number of marker types
	int16_t numMarkerTypes;

	// Number of marker heights (0: continous scaling)
	int16_t numMarkerHeights;

	// Number of accessible fonts
	int16_t numFonts;

	// Number of patterns
	int16_t numPatterns;

	// Number of hatch styles
	int16_t numHatchStyles;

	// Number of simultaneously depicted colours (e.g. 256 on an 8 bit screen)
	int16_t numColours;

	// Number of GDPs (General Drawing Primitives) supported and details
	int16_t numGdps;
	int16_t gdpIndices[GDP_MAX];
	int16_t gdpAttrs[GDP_MAX];

	// Colour capability flags
	int16_t colourFlags;

	// Text rotation capability flags
	int16_t textRotationFlags;

	// Fill area capability flags
	int16_t fillAreaFlags;

	// CELLARRAY capability flag
	int16_t cellArrayFlags;

	// Number of colour levels available, or:
	//		0 =	More than 32767
	//		2 =	Monochrome
	int16_t numColourLevels;

	// Number of locator devices available for graphic cursor control
	int16_t numLocatorDevices;

	// Number of valuator devices available for various inputs
	int16_t numValuatorDevices;

	// Number of choice devices available
	int16_t numChoiceDevices;

	// Number of string devices available
	int16_t numStringDevices;

	// Type of device (input, output, both,...)
	int16_t deviceType;

	// Minimum, maximum character width in pixels
	int16_t minCharacterWidth;

	// Minimum character height in pixels
	int16_t minCharacterHeight;

	// Maximum character width in pixels
	int16_t maxCharacterWidth;

	// Maximum character width in pixels
	int16_t maxCharacterHeight;

	// padding
	int16_t padding1;

	// Maximum line width (in pixels)
	int16_t maxLineWidth;

	// padding
	int16_t padding2;

	// Minimum marker width in pixels
	int16_t minMarkerWidth;

	// Minimum marker height in pixels
	int16_t minMarkerHeight;

	// Maximum marker width in pixels
	int16_t maxMarkerWidth;

	// Maximum marker height in pixels
	int16_t maxMarkerHeight;

	} ScreenCapabilities;

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/

/*****************************************************************************\
|* Class declaration. This class is the drawing surface of a GEM workstation.
|*                    It is shared between all the workstations of a given
|*                    graphics device
\*****************************************************************************/
class Screen : public QMainWindow
	{
	Q_OBJECT

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSETP(QImage*, bg, Bg);					// Backing store for this WS
	GET(ScreenCapabilities, props);				// Fundamentals of this screen

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		Ui::Screen *ui;							// Form for the screen
		ConnectionMgr *	_conmgr;				// Connection manager
		QTimer *		_refreshTimer;			// Used to update the screen

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		Screen(QWidget *parent = nullptr);

	/*************************************************************************\
	|* constructor and destructor
	\*************************************************************************/
		public:
		~Screen();

	/*************************************************************************\
	|* Get a reference to the Screen
	\*************************************************************************/
	static Screen* sharedInstance(void)
		{
		static Screen *_instance = new Screen();	// The actual instance
		return _instance;
		}

	/*************************************************************************\
	|* We were resized
	\*************************************************************************/
	void resizeBackingPixmap(int w, int h);



	public slots:
		/*********************************************************************\
		|* Slot: We got a connection
		\*********************************************************************/
		void connectedTo(Transport *);
	};
#endif // SCREEN_H
