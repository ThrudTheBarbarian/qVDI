#include <QPainter>

#include "fontmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

VDI::VDI(
	QObject *parent)
	:QObject{parent}
	,_rootDir(GEM_ROOT_DIR)
	,_top(nullptr)
	,_alphaMode(false)
	,_alphaX(0)
	,_alphaY(0)
	,_reverseVideo(false)
	,_alphaCursorShown(false)
	{
	/**************************************************************************\
	|* Create the font manager
	\**************************************************************************/
	FontMgr&fm = FontMgr::sharedInstance();
	fm.setRootDir(_rootDir);
	fm.initialise();

	/**************************************************************************\
	|* Update every 1/60th of a second
	\**************************************************************************/
	_cron = new QTimer(this);
	connect(_cron, &QTimer::timeout,
			this, QOverload<>::of(&VDI::newFrame));
	_cron->start(1000/60);
	}

/*****************************************************************************\
|* Handle updating the widget
\*****************************************************************************/
void VDI::update(void)
	{
	_top->update();
	}

void VDI::update(QRect& r)
	{
	int lw = _top->lineWidth();
	r.adjust(-lw,-lw,lw,lw);
	_top->update(r);
	}

void VDI::update(QRectF& r)
	{
	int lw = _top->lineWidth();
	r.adjust(-lw,-lw,lw,lw);
	_top->update(r);
	}

void VDI::update(int x, int y, int w, int h)
	{
	int lw = _top->lineWidth();
	_top->update(x-lw, y-lw, w+lw+lw, h+lw+lw);
	}

#pragma mark - Slots

/*****************************************************************************\
|* A frame has been rendered on screen, clear the dirty flag
\*****************************************************************************/
void VDI::newFrame(void)
	{
	static int frames = 0;

	/**************************************************************************\
	|* Update the cursor if we need to
	\**************************************************************************/
	frames ++;
	if (_alphaMode && ((frames %30) == 0))
		{
		if (_alphaCursorShown)
			_eraseAlphaCursor();
		else
			_drawAlphaCursor();
		}
	}


/*****************************************************************************\
|* Set the alpha mode and manipulate the background colour
\*****************************************************************************/
void VDI::setAlphaMode(bool yn)
	{
	static int16_t nonAlphaBg;

	if (_top != nullptr)
		{
		if (yn)
			{
			nonAlphaBg = _top->backgroundColourIndex();
			_alphaMode = true;
			_top->setBackgroundColourIndex(G_WHITE);
			}
		else
			{
			_alphaMode = false;
			_top->setBackgroundColourIndex(nonAlphaBg);
			}
		}
	else
		WARN("setAlphaMode called when _top is nil");
	}

/*****************************************************************************\
|* Erase the cursor
\*****************************************************************************/
bool VDI::_eraseAlphaCursor(void)
	{
	bool hid = false;

	if ((_alphaCursorShown == true) && (_top != nullptr))
		{
		Screen *screen		= Screen::sharedInstance();
		_alphaCursorShown	= false;
		int x				= _alphaX * _alphaWidth;
		int y				= _alphaY * _alphaHeight;
		int descent			= _top->fm()->descent();

		QPoint pt = {x,y+descent};
		QPainter painter(screen->bg());
		painter.drawImage(pt,_cursorBacking);
		update(pt.x(), pt.y(), _alphaWidth, _alphaHeight);
		hid = true;
		}

	return hid;
	}

/*****************************************************************************\
|* Draw the cursor
\*****************************************************************************/
void VDI::_drawAlphaCursor(void)
	{
	if ((_alphaCursorShown == false) && (_top != nullptr))
		{
		Screen *screen		= Screen::sharedInstance();
		QImage *img			= screen->bg();

		_alphaCursorShown	= true;
		int x				= _alphaX * _alphaWidth;
		int y				= _alphaY * _alphaHeight;
		int w				= _alphaWidth/2;
		int h				= _alphaHeight;
		int descent			= _top->fm()->descent();

		QRect r				= {x,y+descent,w,h};
		_cursorBacking		= img->copy(r);

		QPainter painter(img);
		painter.fillRect(r, _top->colour(G_BLACK));
		update(r);
		}
	}



#pragma mark - Private methods

