#include <QPainter>

#include "screen.h"
#include "ui_screen.h"

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
Screen::Screen(QWidget *parent)
	   : QMainWindow(parent)
	   , ui(new Ui::Screen)
	{
	ui->setupUi(this);

	/*************************************************************************\
	|* Start the connection manager to listen for clients
	\*************************************************************************/
	_conmgr = new ConnectionMgr();
	_conmgr->start(this);

	/*************************************************************************\
	|* We want to know when the connection manager got a hit
	\*************************************************************************/
	connect(_conmgr, &ConnectionMgr::transportConnected,
			this, &Screen::connectedTo);
	}

/*****************************************************************************\
|* Destructor
\*****************************************************************************/
Screen::~Screen()
	{
	delete ui;
	}

/*****************************************************************************\
|* We were resized. This is called once at startup and every time the window
|* is resized thereafter
\*****************************************************************************/
void Screen::resizeBackingPixmap(int w, int h)
	{
	// Create an uninitialised image of the correct size in an efficient format
	QImage *img = new QImage(w, h, QImage::Format_RGB32);

	if (img != nullptr)
		{
		// Fill the image with the default background colour (light grey)
		QPainter P(img);
		QRect all(0, 0, w, h);
		P.fillRect(all, QColor::fromRgb(0xc0, 0xc0, 0xc0));

		// Fill as much of the new image as we can from the old
		if (_bg != nullptr)
			{
			QRect old(QPoint(0,0), _bg->size());
			QRect rect = all.intersected(old);
			P.drawImage(rect,*_bg);
			delete _bg;
			}

		// And set the new image to be the result
		_bg = img;
		}
	}


/*****************************************************************************\
|* We got a connection, grab focus
\*****************************************************************************/
void Screen::connectedTo(Transport *)
	{
	raise();			// Seems to be needed on the Mac
	activateWindow();	// Should be all that's necessary
	}
