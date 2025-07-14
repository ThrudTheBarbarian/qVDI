#include "clientmsg.h"
#include "gemTypes.h"
#include "screenws.h"
#include "transport.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|*   100 : Open a virtual workstation. For now just make it call into v_openwk
\*****************************************************************************/
void VDI::v_opnvwk(ConnectionMgr *cm, Transport *io, ClientMsg &msg)
	{
	v_opnwk(cm, io, msg);
	}

