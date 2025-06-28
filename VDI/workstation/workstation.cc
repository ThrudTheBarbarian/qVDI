#include "screen.h"
#include "workstation.h"

/*****************************************************************************\
|* Constructors
\*****************************************************************************/
Workstation::Workstation(QObject *parent )
			:QObject(parent)
			,_io(nullptr)
	{
	}

Workstation::Workstation(Transport *io, QObject *parent )
			:QObject(parent)
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
