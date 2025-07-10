#include "screenws.h"

enum
	{
	NO_CHANGE = 1,
	ST_LOW,
	ST_MEDIUM,
	ST_HIGH,
	TT_LOW = 6,
	TT_HIGH = 8,
	TT_TRUECOLOUR
	};

/*****************************************************************************\
|* Constructors
\*****************************************************************************/
ScreenWS::ScreenWS(QObject *parent)
	:PhysicalWS{parent}
	{
	_screen = Screen::sharedInstance();
	}

ScreenWS::ScreenWS(Transport *io, QObject *parent)
	:PhysicalWS{io, parent}
	{
	}

/*****************************************************************************\
|* Get the backing pixmap from the screen, if we have one set up
\*****************************************************************************/
QImage * ScreenWS::backingImage(void)
	{
	if (_screen != nullptr)
		return _screen->bg();
	return nullptr;
	}

/*****************************************************************************\
|* Update the widget in the required area, or entirely
\*****************************************************************************/
void ScreenWS::update(void)
	{
	if (_screen != nullptr)
		_screen->update();
	}

void ScreenWS::update(QRect &r)
	{
	if (_screen != nullptr)
		_screen->update(r);
	}

void ScreenWS::update(QRectF &r)
	{
	if (_screen != nullptr)
		_screen->update(r.toRect());
	}

void ScreenWS::update(int x, int y, int w, int h)
	{
	if (_screen != nullptr)
		_screen->update(x, y, w, h);
	}

/*****************************************************************************\
|* Set the device ID (screen, resolution, metafile, etc.) Subclasses might
|* override this
\*****************************************************************************/
void ScreenWS::setDeviceId(int deviceId)
	{
	// Honour the set operation
	Workstation::setDeviceId(deviceId);

	/*************************************************************************\
	|* Determine the resolution of the screen based on the device and configure
	\*************************************************************************/
	int w = _screen->width();
	int h = _screen->height();

	switch (deviceId)
		{
		case NO_CHANGE:			// This means keep whatever you have
			break;

		case ST_LOW:				// 320 x 200
			w = 320;
			h = 200;
			break;

		case ST_MEDIUM:				// 640 x 200
			w = 640;
			h = 200;
			break;

		case ST_HIGH:				// 640 x 400
			w = 640;
			h = 400;
			break;

		case TT_LOW:				// 640 x 480
			w = 640;
			h = 400;
			break;

		case TT_HIGH:				// 1280 x 960
			w = 1280;
			h = 960;
			break;

		case TT_TRUECOLOUR:			// 320 x 480
			w = 320;
			h = 400;
			break;
		}

	/*************************************************************************\
	|* Only change if necessary
	\*************************************************************************/
	bool resize = (_screen->width() != w) || (_screen->height() != h);
	if (resize || (_screen->bg() == nullptr))
		{
		_screen->resize(w, h);
		// FIXME: Check if the next line is necessary, probably not since the GLWidget should handle it
		_screen->resizeBackingPixmap(w, h);
		}
	}
