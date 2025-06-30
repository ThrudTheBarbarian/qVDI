#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QLocalSocket>
#include <QObject>

#include "clientmsg.h"
#include "properties.h"

#define VDI_SERVICE_NAME "/tmp/qVDI"

/*****************************************************************************\
|* Class declaration. This is a facade over the different transport-types. In
|*                    development, it's a local unix socket, in the real
|*                    thing, it's an interface to a USB device
\*****************************************************************************/
class Transport : public QObject
	{
	Q_OBJECT

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSETP(QLocalSocket*, socket, Socket);			// Socket transport
	GET(QByteArray, buffer);						// Message buffer

	public:
		/*********************************************************************\
		|* Constructor
		\*********************************************************************/
		explicit Transport(QLocalSocket *socket, QObject *parent = nullptr);

		/*********************************************************************\
		|* Read a message into the message buffer
		\*********************************************************************/
		bool read(ClientMsg &msg);

		/*********************************************************************\
		|* Write a message to the socket
		\*********************************************************************/
		bool write(ClientMsg &msg, bool log=false);

	};

#endif // TRANSPORT_H
