#include "screen.h"
#include "workstation.h"

static int16_t _nextHandle = 100;

/*****************************************************************************\
|* Constructors
\*****************************************************************************/
Workstation::Workstation(QObject *parent )
			:QObject(parent)
			,_deviceId(-1)
			,_handle(-1)
			,_isVirtual(false)
			,_lineType(SOLID)
			,_markerType(MRKR_DOT)
			,_markerColourIndex(G_BLACK)
			,_fontId(0)
			,_textColourIndex(G_BLACK)
			,_interiorFillStyle(FIS_SOLID)
			,_fillIndex(PT_DOTS1)
			,_fillColourIndex(G_BLACK)
			,_alphaMode(false)
			,_alphaX(0)
			,_alphaY(0)
			,_reverseVideo(false)
			,_io(nullptr)
	{
	}

Workstation::Workstation(Transport *io, QObject *parent )
			:QObject(parent)
			,_deviceId(-1)
			,_handle(-1)
			,_isVirtual(false)
			,_lineType(SOLID)
			,_markerType(MRKR_DOT)
			,_markerColourIndex(G_BLACK)
			,_fontId(0)
			,_textColourIndex(G_BLACK)
			,_interiorFillStyle(FIS_SOLID)
			,_fillIndex(PT_DOTS1)
			,_fillColourIndex(G_BLACK)
			,_alphaMode(false)
			,_alphaX(0)
			,_alphaY(0)
			,_reverseVideo(false)
			,_io(io)
	{
	}

/*****************************************************************************\
|* Base version of the 'open' call which always fails
\*****************************************************************************/
bool Workstation::open(int16_t *workIn, int16_t *handle, int16_t *workOut)
	{
	// Prevent the warning...
	(void)workIn;

	// Only fill in the handle if it's non-nil
	if (handle)
		*handle = -1;

	// If we have a workOut pointer, we assume it's large enough to be filled
	// with the standard response
	if (workOut)
		memset(workOut, 0, sizeof(ScreenCapabilities));

	// And always fail. You need to call a higher level class to get something
	// useful
	return false;
	}

/*****************************************************************************\
|* Base version of the 'backingImage' call which always returns nil
\*****************************************************************************/
QImage * Workstation::backingImage(void)
	{
	return nullptr;
	}

/*****************************************************************************\
|* Base version of update is a NOP
\*****************************************************************************/
void Workstation::update(void)
	{}

void Workstation::update(QRect &)
	{}

void Workstation::update(int, int, int, int)
	{}

/*****************************************************************************\
|* Set the device ID (screen, resolution, metafile, etc.) Subclasses might
|* override this
\*****************************************************************************/
void Workstation::setDeviceId(int deviceId)
	{
	_deviceId = deviceId;
	}
/*****************************************************************************\
|* Method: Claim the next available handle
\*****************************************************************************/
void Workstation::claimNextHandle(void)
	{
	_handle = _nextHandle ++;
	}
