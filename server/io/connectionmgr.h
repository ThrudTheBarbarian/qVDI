#ifndef CONNECTIONMGR_H
#define CONNECTIONMGR_H

#include <QMap>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>

#include "macros.h"
#include "properties.h"
#include "transport.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class Screen;
class Workstation;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class ConnectionMgr : public QObject
	{
	Q_OBJECT

	/*************************************************************************\
	|* Typedefs, consts etc
	\*************************************************************************/
	const char *SOCKET_IDENTIFIER = "socket_identifier";

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GET(HandleList, timerList);					// List of handles that have
												// timer-events registered

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:
		QMap<qintptr, Transport*> _conns;		// List of connected clients
		QMap<Transport*, Workstation*> _wsList;	// Transport->Workstation
		QLocalServer _server;					// Server socket to listen on
		Screen * _screen;						// Screen to draw on

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit ConnectionMgr(QObject *parent = nullptr);

		/*********************************************************************\
		|* start listening
		\*********************************************************************/
		void start(Screen *screen);

		/*********************************************************************\
		|* stop listening
		\*********************************************************************/
		void stop(void);

		/*********************************************************************\
		|* Return the workstation for a given handle
		\*********************************************************************/
		Workstation * findWorkstationForHandle(qintptr handle);

		/*********************************************************************\
		|* Map a transport object to a workstation
		\*********************************************************************/
		void mapTransportToWorkstation(Transport *io, Workstation *ws);

		/*********************************************************************\
		|* Add or remove a connection from the list that wants timer events
		\*********************************************************************/
		void updateTimerInterest(qintptr handle, int16_t enable);

		/*********************************************************************\
		|* Set the physical workstation as handle 0
		\*********************************************************************/
		//void setPhysicalWorkstation(Workstation *ws);

	signals:
		/*********************************************************************\
		|* Signal: we have a new connection, though we don't know what type of
		|*         device this connection is for, yet
		\*********************************************************************/
		void transportConnected(Transport *io);

	private slots:
		/*********************************************************************\
		|* Slot: We got a new connection
		\*********************************************************************/
		void _connection(void);

		/*********************************************************************\
		|* Slot: We lost a connection
		\*********************************************************************/
		void _disconnection(void);

		/*********************************************************************\
		|* Slot: A client has data to read
		\*********************************************************************/
		void _incomingData(void);

	};

#endif // CONNECTIONMGR_H
