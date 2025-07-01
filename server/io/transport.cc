#include "transport.h"

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
Transport::Transport(QLocalSocket *socket, QObject *parent)
	:QObject{parent}
	,_socket(socket)
	{
	_buffer.clear();
	}

/*****************************************************************************\
|* Read a message. The code will first try to satisfy the response from the
|* passed-in buffer. If that fails it will call read() on the socket to try
|* to source more data. If it still can't parse a message, we return false
|*
|* When parsing a message, we
|*  - parse the length from the byte-array
|*  - decode the type from network format
|*  - copy the payload as-is
|*
\*****************************************************************************/
bool Transport::read(ClientMsg &msg)
	{
	bool ok			= false;
	int loops		= 0;

	/*************************************************************************\
	|* If we don't even have space for the length parameter, call read()
	\*************************************************************************/
	while ((_buffer.size() < 2) && (loops < 5))
		{
		QByteArray moreData = _socket->readAll();
		if (moreData.length() > 0)
			_buffer.append(moreData);
		loops ++;
		}

	/*************************************************************************\
	|* Assuming we now have enough data for at least the length...
	\*************************************************************************/
	if (_buffer.size() >= 2)
		{
		int16_t *words	= (int16_t *) _buffer.data();

		/*********************************************************************\
		|* Clear the message
		\*********************************************************************/
		msg.payload().clear();

		/*********************************************************************\
		|* Read the msg len, if we haven't the size for the rest, call read()
		\*********************************************************************/
		loops = 0;
		int16_t length = ntohs(*words++);
		while ((_buffer.size() < 2 + 2*length) && (loops < 5))
			{
			QByteArray moreData = _socket->readAll();
			if (moreData.length() > 0)
				_buffer.append(moreData);
			loops ++;
			}

		/*********************************************************************\
		|* Read the data if we can - note this is not byte-swapped, and flag ok
		\*********************************************************************/
		if (_buffer.size() >= 2*length)
			{
			msg.setType(ntohs(*(words++)));
			length --;

			for (int i=0; i<length; i++)
				msg.payload().push_back(*words ++);

			_buffer.remove(0, 4 + length * 2);
			ok = true;
			}
		else
			{
			WARN("Insufficient data for msg type 0x%04X "
				 "(%d required, %d available)",
				 _type, length*2, (int)dev->bytesAvailable());
			}
		}

	return ok;
	}
/*****************************************************************************\
|* Write a message to the socket
\*****************************************************************************/
bool Transport::write(ClientMsg &msg, bool log)
	{
	QByteArray ba = msg.encode();
	if (log)
		fprintf(stderr, "Writing %d bytes to socket handle %d\n",
				(int)ba.size(),
				(int)(_socket->socketDescriptor()));

	bool ok = (_socket->write(ba) == ba.length());
	if (!ok)
		fprintf(stderr, "Failed to write %d bytes to socket handle %d",
				(int)ba.size(),
				(int)(_socket->socketDescriptor()));
	return ok;
	}

