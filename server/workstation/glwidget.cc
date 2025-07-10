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
		 :QWidget(parent)
	{
	}

/*****************************************************************************\
|* Draw entry routine
\*****************************************************************************/
void GLWidget::paintEvent(QPaintEvent *e)
	{
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
	}

/*****************************************************************************\
|* We were resized, inform the screen it needs to be resized too
\*****************************************************************************/
void GLWidget::resizeEvent(QResizeEvent *e)
	{
	int w = e->size().width();
	int h = e->size().height();
	Screen::sharedInstance()->resizeBackingPixmap(w, h);
	}
