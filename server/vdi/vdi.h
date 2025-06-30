#ifndef VDI_H
#define VDI_H

#include <string>

#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QObject>

#include "gemTypes.h"
#include "properties.h"
#include "transport.h"

#define GEM_ROOT_DIR "/usr/local/atari"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class Screen;
class Workstation;

/*****************************************************************************\
|* Class declaration. This is the class that actually draws / configures any
|*                    drawing. It's a singleton shared between all the
|*                    workstations because state is held in the workstation
|*
|* When any drawing is performed, the drawing code must call update(x,y,w,h)
|* so that the widget knows it needs to update within that area
\*****************************************************************************/
class VDI : public QObject
	{
	Q_OBJECT
	NON_COPYABLE_NOR_MOVEABLE(VDI)

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSET(std::string, rootDir, RootDir);		// "System" disk root-dir
	GETSETP(Workstation*, top, Top);			// Current workstation

	public:
		/*********************************************************************\
		|* Typedefs and enums
		\*********************************************************************/
		typedef enum
			{
			POLY			= 0,
			RECT			= 1,
			PIE				= 3,
			CIRCLE			= 4,
			ELLIPSE			= 5,
			ELLIPTICAL_PIE	= 7,
			ROUNDED_RECT	= 9
			} FillType;

	private:

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit VDI(QObject *parent = nullptr);


	public slots:
		/*********************************************************************\
		|* A frame has been drawn
		\*********************************************************************/
		void frameRendered(void);

	public:
		/*********************************************************************\
		|* Get a reference to the VDI
		\*********************************************************************/
		static VDI& sharedInstance(void)
			{
			static VDI _instance;               // The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* tell the widget it needs to be updated within an area
		\*********************************************************************/
		void update(QRect& area);
		void update(int x, int y, int w, int h);
		void update(void);


		/*********************************************************************\
		|* Operation:   1   : Open a physical workstation
		\*********************************************************************/
		Workstation * v_opnwk(Transport *io, ClientMsg &msg);

		/*********************************************************************\
		|* Operation:   3   : Clear a workstation
		\*********************************************************************/
		void v_clrwk(Transport *io, Workstation *ws);
	};

#endif // VDI_H
