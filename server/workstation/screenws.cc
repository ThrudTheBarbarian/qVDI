#include "screenws.h"


/*****************************************************************************\
|* Constructors
\*****************************************************************************/
ScreenWS::ScreenWS(QObject *parent)
	:PhysicalWS{parent}
	{
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

void ScreenWS::update(int x, int y, int w, int h)
	{
	if (_screen != nullptr)
		_screen->update(x, y, w, h);
	}
