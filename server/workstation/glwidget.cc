#include <QImage>
#include <QPainter>
#include <QPaintEvent>

#include "glwidget.h"
#include "screen.h"
#include "vdi.h"

/*****************************************************************************\
|* Class constructor
\*****************************************************************************/
GLWidget::GLWidget(QWidget *parent)
		 :QOpenGLWidget(parent)
	{
	/**************************************************************************\
	|* Set up so we can notify the VDI when a frame is rendered
	\**************************************************************************/
	connect(this, &GLWidget::frameRendered,
			&(VDI::sharedInstance()),
			QOverload<>::of(&VDI::frameRendered));

	_cron.restart();
	}

/*****************************************************************************\
|* Draw entry routine
\*****************************************************************************/
void GLWidget::paintEvent(QPaintEvent *e)
	{
	static int frames = 0;

	/**************************************************************************\
	|* Paint a single frame to the widget
	\**************************************************************************/
	QPainter painter;
	painter.begin(this);

	QImage *bg = Screen::sharedInstance()->bg();
	if (bg != nullptr)
		{
		QPointF at = e->rect().topLeft();
		painter.drawImage(at, *bg, e->rect());
		}

	painter.end();

	/**************************************************************************\
	|* Say we've painted the frame
	\**************************************************************************/
	emit frameRendered();

	/**************************************************************************\
	|* Calculate our FPS etc every 100 frames
	\**************************************************************************/
	frames ++;
	if (frames == 100)
		{
		qint64 nsecs = _cron.nsecsElapsed();
		int usecs = nsecs / 1000 / 100;

		fprintf(stderr, "usecs/frame = %d fps=%5.3f\n",
				(int)(usecs),
				1000000.0f / usecs);
		frames = 0;
		_cron.restart();
		}
	}

/*****************************************************************************\
|* We were resized, inform the screen it needs to be resized too
\*****************************************************************************/
void GLWidget::resizeGL(int w, int h)
	{
	Screen::sharedInstance()->resizeBackingPixmap(w, h);
	}
