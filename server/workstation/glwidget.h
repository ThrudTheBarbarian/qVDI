#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QElapsedTimer>
#include <QObject>
#include <QOpenGLWidget>
#include <QPixmap>
#include <QWidget>

#include "properties.h"

class GLWidget : public QWidget
	{
	Q_OBJECT	

	/**************************************************************************\
	|* Properties
	\**************************************************************************/

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
		virtual void resizeEvent(QResizeEvent *e) override;

	signals:
		/*********************************************************************\
		|* We have rendered another frame. Tell whoever cares
		\*********************************************************************/
		void frameRendered(void);
	};

#endif // GLWIDGET_H
