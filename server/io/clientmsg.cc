#include <QtCore/QVarLengthArray>

#include <QIODevice>
#include <QRect>

#include "clientmsg.h"
#include "gemTypes.h"
//#include "keymapper.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
ClientMsg::ClientMsg()
	{
	}

ClientMsg::ClientMsg(int16_t type)
	{
	_payload.push_back(type & 0xFF);
	_payload.push_back((type >> 8) & 0xFF);
	}

ClientMsg::ClientMsg(int16_t type, Payload payload)
	:_payload(payload)
	{
	_payload.push_back(type);
	_payload.insert(_payload.end(), payload.begin(), payload.end());
	}

/*****************************************************************************\
|* Return the payload
\*****************************************************************************/
Payload& ClientMsg::payload(void)
	{
	return _payload;
	}

/*****************************************************************************\
|* Serialise the message to something we can transmit. This will:
|*
|*  - encode the length in network format
|*  - encode the type in network format
|*  - append the data as-is. This should be encoded if it is 16-bit
|*
\*****************************************************************************/
QByteArray ClientMsg::encode(void)
	{
	int len = (int)(_payload.size() + 2);
	QVarLengthArray<int16_t, 1024> buf(len);

	buf[0] = htons((int16_t)(len-1));
	buf[1] = htons(_type);

	int idx = 2;
	for (int16_t word : _payload)
		buf[idx++] = word;

	return QByteArray((const char *)(&(buf[0])), len*2);
	}

/*****************************************************************************\
|* De-serialise the message to the original form. This will:
|*
|*  - parse the length from the byte-array
|*  - decode the type from network format
|*  - copy the payload as-is
|*
\*****************************************************************************/
bool ClientMsg::decode(int16_t words, QByteArray& ba)
	{
	_payload.clear();
	bool ok			= false;
	_type			= INVALID;
	int len			= (int)ba.size() / 2 - 1;
	int16_t *ptr	= (int16_t *)(ba.data());

	if (len == words)
		{
		_type = ntohs(*ptr++);
		for (int i=0; i<len; i++)
			_payload.push_back((*ptr++));

		ok = true;
		}
	return ok;
	}


/*****************************************************************************\
|* De-serialise the message to the original form
\*****************************************************************************/
void ClientMsg::clear(void)
	{
	_payload.clear();
	_type = 0;
	}


/*****************************************************************************\
|* Construct a message from a timer event
\*****************************************************************************/
void ClientMsg::fromTimerEvent(void)
	{
	_type = EVT_TIMER;
	_payload.clear();
	}

/*****************************************************************************\
|* Append 16-bit int(s) to the payload. Data will be byte-swapped
\*****************************************************************************/
bool ClientMsg::append(int16_t value)
	{
	_payload.push_back(htons(value));
	return true;
	}

bool ClientMsg::append(int16_t *data, int num)
	{
	for (int i=0; i<num; i++)
		_payload.push_back(htons(*data++));
	return true;
	}


/*****************************************************************************\
|* Append data to the payload with a prepended length. The length will be
|* byte-swapped, the data will not
\*****************************************************************************/
void ClientMsg::append(uint8_t *data, uint32_t numBytes)
	{
	/*************************************************************************\
	|* append the length of the data blob
	\*************************************************************************/
	append(numBytes >> 16);
	append(numBytes & 0xFFFF);

	/*************************************************************************\
	|* Append the data blob without byte-swapping. These are bytes...
	\*************************************************************************/
	for (uint32_t i=0; i<numBytes/2; i++)
		{
		int16_t *word = ((int16_t *)data) + i;
		_payload.push_back(*word);
		}

	/*************************************************************************\
	|* Append the remaining word if necessary. This will be byte-swapped
	|* since it's an 8-bit value being read as a 16-bit word
	\*************************************************************************/
	if (numBytes & 1)
		{
		int16_t value = data[numBytes-1];
		append(value);
		}
	}

/*****************************************************************************\
|* Append a rectangle to the message
\*****************************************************************************/
bool ClientMsg::append(QRect r)
	{
	bool ok = true;

	_payload.push_back(htons((int16_t)r.x()));
	_payload.push_back(htons((int16_t)r.y()));
	_payload.push_back(htons((int16_t)r.width()));
	_payload.push_back(htons((int16_t)r.height()));

	return ok;
	}

/*****************************************************************************\
|* Get a QByteArray from the payload. This will read the length (byte-swapped)
|* and then the data (not byte-swapped), appending the last byte if necessary
\*****************************************************************************/
int ClientMsg::fetchData(int idx, QByteArray& ba)
	{
	uint32_t bytes		= ntohs(_payload[idx]);
	bytes				= bytes << 16;
	bytes			   += ntohs(_payload[idx+1]);

	const char *data	= (const char *)(&(_payload[idx+2]));
	int words			= bytes/2;

	ba.clear();
	ba.append(data, words*2);
	if (bytes & 1)
		{
		int16_t val		= ntohs(_payload[idx + words + 2]);
		const char c	= val & 0xFF;
		ba.append(c);
		words ++;
		}

	return words + 2;
	}
