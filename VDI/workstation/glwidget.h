#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QElapsedTimer>
#include <QObject>
#include <QOpenGLWidget>
#include <QPixmap>
#include <QWidget>

#include "properties.h"

class GLWidget : public QOpenGLWidget
	{
	Q_OBJECT	

	/**************************************************************************\
	|* Properties
	\**************************************************************************/
	GET(QElapsedTimer, cron);		// Used for FPS counting

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit GLWidget(QWidget *parent = nullptr);


	protected:
		/*********************************************************************\
		|* Paint the screen
		\*********************************************************************/
		void paintEvent(QPaintEvent *event) override;

		/*********************************************************************\
		|* Resize the screen
		\*********************************************************************/
		void resizeGL(int w, int h) override;

	signals:
		/*********************************************************************\
		|* We have rendered another frame. Tell whoever cares
		\*********************************************************************/
		void frameRendered(void);
	};

#endif // GLWIDGET_H
