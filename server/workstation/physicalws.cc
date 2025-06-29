#include "physicalws.h"

/*****************************************************************************\
|* Constructors
\*****************************************************************************/
PhysicalWS::PhysicalWS(QObject *parent)
		   :Workstation{parent}
	{
	}

PhysicalWS::PhysicalWS(Transport *io, QObject *parent)
	:Workstation{io, parent}
	{
	}
