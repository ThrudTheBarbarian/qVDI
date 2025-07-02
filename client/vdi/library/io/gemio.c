//
//  io.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "gemio.h"

extern void _gemProcessTimerEvent(void);
extern void _gemProcessButtonEvent(int mouseState);
extern void _gemProcessMoveEvent(int x, int y);
extern void _gemProcessCursorEvent(int x, int y);

#define SOCKET_NAME	 					"/tmp/qVDI"


/*****************************************************************************\
|* The socket file descriptor
\*****************************************************************************/
static int 		_gemfd			= -1;

/*****************************************************************************\
|* Implement the queue of pending events
\*****************************************************************************/
static int		_msgNext		= 0;
static MsgList	_msgs;

/*****************************************************************************\
|* Current filter on which events ought to be sent to this app
\*****************************************************************************/
static int16_t	_gemEventFilter	= 0;

/*****************************************************************************\
|* Mouse state, as seen by the constant flow of events into the top-most app
\*****************************************************************************/
static int16_t 	_mx, _my, _mb;

/*****************************************************************************\
|* Similarly, keyboard modifiers state
\*****************************************************************************/
static int16_t _mods;

/*****************************************************************************\
|* Check to see if the connection has been made to the server
\*****************************************************************************/
int _gemIoIsConnected(void)
	{
	return (_gemfd > 0) ? 1 : 0;
	}

/*****************************************************************************\
|* Connect to the server
\*****************************************************************************/
int _gemIoConnect(void)
	{
	int ok = 0;

	/*************************************************************************\
	|* Contact the server
	\*************************************************************************/
	if (_gemfd < 0)
		{
		_gemfd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (_gemfd > 0)
			{
			/*****************************************************************\
			|* Zero everything for portability
			\*****************************************************************/
			struct sockaddr_un addr;
			memset(&addr, 0, sizeof(struct sockaddr_un));

			addr.sun_family = AF_UNIX;
			strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

			/*****************************************************************\
			|* Connect to the VDI process
			\*****************************************************************/
			int ret = connect(_gemfd,
							  (const struct sockaddr *) &addr,
							  sizeof(struct sockaddr_un));
			if (ret == 0)
				ok = 1;
			else
				{
				close(_gemfd);
				_gemfd = -1;
				perror("Connect");
				}
			}
		else
			perror("Socket create");
		}
	
	return ok;
	}


/*****************************************************************************\
|* Disconnect from the server
\*****************************************************************************/
void _gemIoDisconnect(void)
	{
	if (_gemfd > 0)
		{
		close(_gemfd);
		_gemfd = 0;
		}
	}
	

/*****************************************************************************\
|* Read from the socket
\*****************************************************************************/
int _gemIoRead(GemMsg *msg, int msecs)
	{
	int ok = 0;
	
	/*************************************************************************\
	|* If there is a message in the queue, then retrieve that first
	\*************************************************************************/
	if ((_msgs.length > 0) && (_msgs.length < _msgNext))
		{
		*msg = _msgs.data[_msgNext++];
		if (_msgNext == _msgs.length)
			{
			vec_deinit(&_msgs);
			_msgNext = 0;
			ok 		 = 1;
			}
		}

	/*************************************************************************\
	|* Otherwise read the next one from the socket
	\*************************************************************************/
	else
		ok = _gemMsgRead(msg, _gemfd, msecs);

	return ok;
	}

/*****************************************************************************\
|* Function to request a blocking read of a message, filtering on a list of
|* message types. This always tries to read from the socket
\*****************************************************************************/
int _gemIoWaitForMessages(GemMsg *msg, vec_word_t *types)
	{
	return _gemIoWaitForMessagesWithTimeout(msg, types, -1);
	}

/*****************************************************************************\
|* Function to handle the different types of 'automatic' messages
\*****************************************************************************/
static void _handleIncomingAsync(GemMsg incoming)
	{
	if (incoming.type == EVT_TIMER)
		{
		_gemProcessTimerEvent();
		}
	else if (incoming.type == EVT_MOUSE_MOVE)
		{
		_mx 	= ntohs(incoming.vec.data[0]);
		_my 	= ntohs(incoming.vec.data[1]);
		_mb 	= ntohs(incoming.vec.data[2]);
		_mods 	= ntohs(incoming.vec.data[3]);
		_gemProcessMoveEvent(_mx, _my);
		_gemProcessCursorEvent(_mx, _my);
		}
	else if (incoming.type == EVT_MOUSE_DOWN)
		{
		_mx 	= ntohs(incoming.vec.data[0]);
		_my 	= ntohs(incoming.vec.data[1]);
		_mb 	= ntohs(incoming.vec.data[2]);
		_mods 	= ntohs(incoming.vec.data[3]);
		_gemProcessButtonEvent(_mb);
		}
	else if (incoming.type == EVT_MOUSE_UP)
		{
		_mx 	= ntohs(incoming.vec.data[0]);
		_my 	= ntohs(incoming.vec.data[1]);
		_mb 	= ntohs(incoming.vec.data[2]);
		_mods 	= ntohs(incoming.vec.data[3]);
		}
	else if (incoming.type == EVT_KEY_PRESS)
		{
		fprintf(stderr, "Key press %04x\n", ntohs(incoming.vec.data[0]));
		}
	else
		{
		vec_push(&_msgs, incoming);		// Transfers ownership of memory
		}
	}
	
/*****************************************************************************\
|* Function to request a blocking read of a message, filtering on a list of
|* message types. This always tries to read from the socket
\*****************************************************************************/
int _gemIoWaitForMessagesWithTimeout(GemMsg *msg, vec_word_t *types, int msecs)
	{
	GemMsg incoming;
	
	int ok = _gemMsgRead(&incoming, _gemfd, msecs);
	while (ok == 1)
		{
		int idx = -1;
		/*************************************************************************\
		|* If it's one of the ones we're looking for, return it.
		\*************************************************************************/
		vec_find(types, incoming.type, idx);
		if (idx >= 0)
			{
			*msg = incoming;				// Transfers ownership of memory
			break;
			}

		/*************************************************************************\
		|* Else handle any async messages
		\*************************************************************************/
		else
			_handleIncomingAsync(incoming);
			
		ok = _gemMsgRead(&incoming, _gemfd, msecs);
		}
		
	return ok;
	}
	
/*****************************************************************************\
|* Function to request a blocking read of a message, filtering on a range of
|* message types. This always tries to read from the socket
\*****************************************************************************/
int _gemIoWaitForMessageRangeWithTimeout(GemMsg *msg, int lo, int hi, int msecs)
	{
	GemMsg incoming;
	
	int ok = _gemMsgRead(&incoming, _gemfd, msecs);
	while (ok == 1)
		{
		/*************************************************************************\
		|* If it's one of the ones we're looking for, return it.
		\*************************************************************************/
		if ((incoming.type >= lo) && (incoming.type <= hi))
			{
			*msg = incoming;				// Transfers ownership of memory
			break;
			}

		/*************************************************************************\
		|* Else handle any async messages
		\*************************************************************************/
		else
			_handleIncomingAsync(incoming);
			
		ok = _gemMsgRead(&incoming, _gemfd, msecs);
		}
		
	return ok;
	}


/*****************************************************************************\
|* Function to request a blocking read of a message, filtering on a range of
|* message types. This always tries to read from the socket
\*****************************************************************************/
int _gemIoWaitForMessageRange(GemMsg *msg, int lo, int hi)
	{
	return _gemIoWaitForMessageRangeWithTimeout(msg, lo, hi, -1);
	}

/*****************************************************************************\
|* Function to request a blocking read of a message. This always tries to read
|* from the socket
\*****************************************************************************/
int _gemIoWaitForMessageOfType(GemMsg *msg, int16_t type)
	{
	return _gemIoWaitForMessageOfTypeWithTimeout(msg, type, -1);
	}

/*****************************************************************************\
|* Function to request a blocking read of a message with a timeout
\*****************************************************************************/
int _gemIoWaitForMessageOfTypeWithTimeout(GemMsg *msg, int16_t type, int msecs)
	{
	vec_word_t types;
	vec_init(&types);
	vec_push(&types, type);
	return _gemIoWaitForMessagesWithTimeout(msg, &types, msecs);
	}
	

/*****************************************************************************\
|* Serialise to bytes, and write to the socket
\*****************************************************************************/
int _gemIoWrite(GemMsg *msg)
	{
	int size = msg->vec.length;

	/*************************************************************************\
	|* Create a new buffer with the correct embedded type and length
	\*************************************************************************/
	int bufferLen	= 2+2+size*2;
	uint8_t buf[bufferLen];
	
	/*************************************************************************\
	|* Embedded Length is (1 (for type) + num) 16-bit words
	\*************************************************************************/
	uint16_t *ptr 	= (uint16_t *)(&(buf[0]));
	uint16_t len 	= htons(1 + size);
	*ptr ++ 	 	= len;

	/*************************************************************************\
	|* Do the type
	\*************************************************************************/
	uint16_t type 	= htons((uint16_t)msg->type);
	*ptr ++ 	 	= type;

	/*************************************************************************\
	|* Serialise the data
	\*************************************************************************/
	for (int i=0; i<size; i++)
		*ptr ++ = (uint16_t)(*(msg->vec.data + i));

	for (int i=0; i<bufferLen; i+=2)
		LOGWARN("word %3d : %02x %02x", i/2, buf[i], buf[i+1]);

	/*************************************************************************\
	|* Write to the socket
	\*************************************************************************/
	int numSent = 0;
	while (numSent < bufferLen)
		numSent += write(_gemfd, buf + numSent, bufferLen - numSent);

	LOGWARN("wrote %d bytes", numSent);
	
	/*************************************************************************\
	|* Destroy the message, now that it has been sent
	\*************************************************************************/
	_gemMsgDestroy(msg);
	
	return (numSent == bufferLen) ? 1 : 0;
	}


/*****************************************************************************\
|* Return the current GEM message event-filter
\*****************************************************************************/
int _gemIoEventFilter(void)
	{
	return _gemEventFilter;
	}

/*****************************************************************************\
|* Set the current GEM message event-filter, and tell the server
\*****************************************************************************/
void _gemIoSetEventFilter(int value)
	{
	_gemEventFilter = value;
	
	GemMsg msg;
	_gemMsgInit(&msg, EVT_FILTER_SET);
	_gemMsgAppend(&msg, &_gemEventFilter, 1);
	_gemIoWrite(&msg);
	}

/*****************************************************************************\
|* Sample the current mouse/modifier state, any of these can be NULL
\*****************************************************************************/
void _gemIoMouseState(int16_t *x, int16_t *y, int16_t *btns, int16_t *mods)
	{
	if (x)		*x 		= _mx;
	if (y)		*y		= _my;
	if (btns)	*btns	= _mb;
	if (mods)	*mods	= _mods;
	}
	
