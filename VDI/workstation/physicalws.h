#ifndef PHYSICALWS_H
#define PHYSICALWS_H

#include <QObject>

#include "workstation.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/

/*****************************************************************************\
|* Class declaration. This class is the base for an actual workstation
\*****************************************************************************/
class PhysicalWS : public Workstation
	{
	Q_OBJECT

	/*************************************************************************\
	|* constructor
	\*************************************************************************/
	public:
	explicit PhysicalWS(QObject *parent = nullptr);
	explicit PhysicalWS(Transport *io, QObject *parent = nullptr);


		};

#endif // PHYSICALWS_H
