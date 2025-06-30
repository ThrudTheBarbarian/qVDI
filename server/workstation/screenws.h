#ifndef SCREENWS_H
#define SCREENWS_H

#include <QObject>

#include "physicalws.h"
#include "screen.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/


/*****************************************************************************\
|* Class declaration. This class represents the state for drawing to a screen.
\*****************************************************************************/
class ScreenWS : public PhysicalWS
	{
	Q_OBJECT

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSETP(Screen*, screen, Screen);		// The screen to use

	/*************************************************************************\
	|* constructor
	\*************************************************************************/
	public:
		explicit ScreenWS(QObject *parent = nullptr);
		explicit ScreenWS(Transport *io, QObject *parent=nullptr);

	/*************************************************************************\
	|* Method: Fetch the backing store for this workstation, if there is one
	\*************************************************************************/
	virtual QImage * backingImage(void) override;

	/*************************************************************************\
	|* Method: tell the workstation it needs to update within a given area, or
	|*         even the entire area
	\*************************************************************************/
	virtual void update(void) override;
	virtual void update(QRect& r) override;
	virtual void update(int x, int y, int w, int h) override;

	/*************************************************************************\
	|* Method: Override the device-id set procedure to manage the different
	|*         resolutions that might be requested. Note we always use a
	|*         truecolour display, but the size is honoured
	\*************************************************************************/
	virtual void setDeviceId(int deviceId) override ;
	};

#endif // SCREENWS_H
