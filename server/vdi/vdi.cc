#include <QPainter>

#include "fontmgr.h"
#include "vdi.h"
#include "workstation.h"

VDI::VDI(
	QObject *parent)
	:QObject{parent}
	,_rootDir(GEM_ROOT_DIR)
	,_top(nullptr)
	{
	FontMgr&fm = FontMgr::sharedInstance();
	fm.setRootDir(_rootDir);
	fm.initialise();
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
	_top->update(r);
	}

void VDI::update(int x, int y, int w, int h)
	{
	_top->update(x, y, w, h);
	}

#pragma mark - Slots

/*****************************************************************************\
|* A frame has been rendered on screen, clear the dirty flag
\*****************************************************************************/
void VDI::frameRendered(void)
	{
	/**************************************************************************\
	|* Update the cursor if we need to
	\**************************************************************************/
	// if (_alphaMode && ((_frames %30) == 0))
	// 	{
	// 	if (_alphaCursorShown)
	// 		_eraseAlphaCursor();
	// 	else
	// 		_drawAlphaCursor();
	// 	}
	}



#pragma mark - Private methods

