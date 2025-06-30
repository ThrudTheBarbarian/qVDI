#ifndef CLIENTMSG_H
#define CLIENTMSG_H

#include <QByteArray>
#include <QIODevice>

#include "properties.h"
#include "macros.h"

#define MSG_REPLY(x) ((int16_t)((x) + ClientMsg::MSG_REPLY_OFFSET))

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class ClientMsg
	{
	/*************************************************************************\
	|* Public typedefs, defines, etc.
	\*************************************************************************/
	public:
		enum
		{
		MSG_REPLY_OFFSET			= 32768,
		INVALID						= -1,
		V_OPNWK						= 1,
		V_CLSWK						= 2,
		V_CLRWK						= 3,
		VQ_CHCELLS					= 5001,		// Really 5.1
		V_EXIT_CUR					= 5002,
		V_ENTER_CUR					= 5003,
		V_CURUP						= 5004,
		V_CURDOWN					= 5005,
		V_CURRIGHT					= 5006,
		V_CURLEFT					= 5007,
		V_CURHOME					= 5008,
		V_EEOS						= 5009,
		V_EEOL						= 5010,
		VS_CURADDRESS				= 5011,
		V_CURTEXT					= 5012,
		V_RVON						= 5013,
		V_RVOFF						= 5014,
		VQ_CURADDRESS				= 5015,
		VQ_TABSTATUS				= 5016,		// Not implemented
		V_HARDCOPY					= 5017,		// Not implemented
		V_DSPCUR					= 5018,
		V_RMCUR						= 5019,
		V_PLINE						= 6,
		V_PMARKER					= 7,
		V_GTEXT						= 8,
		V_FILLAREA					= 9,
		V_BAR						= 11001,	// Really 11.1
		V_ARC						= 11002,
		V_PIE						= 11003,
		V_CIRCLE					= 11004,
		V_ELLIPSE					= 11005,
		V_ELLARC					= 11006,
		V_ELLPIE					= 11007,
		V_RBOX						= 11008,
		V_RFBOX						= 11009,
		V_JUSTIFIED					= 11010,
		VST_HEIGHT					= 12,
		VST_ROTATION				= 13,
		VS_COLOR					= 14,
		VSL_TYPE					= 15,
		VSL_WIDTH					= 16,
		VSL_COLOR					= 17,
		VSM_TYPE					= 18,
		VSM_HEIGHT					= 19,
		VSM_COLOR					= 20,
		VST_FONT					= 21,
		VST_COLOR					= 22,
		VSF_INTERIOR				= 23,
		VSF_STYLE					= 24,
		VSF_COLOR					= 25,
		VQ_COLOR					= 26,
		VSWR_MODE					= 32,
		VSIN_MODE					= 33,
		VQL_ATTRIBUTES				= 35,
		VQM_ATTRIBUTES				= 36,
		VQF_ATTRIBUTES				= 37,
		VQT_ATTRIBUTES				= 38,
		VST_ALIGNMENT				= 39,
		VS_PIXRGB					= 59,
		VQ_PIXRGB					= 60,
		V_OPNVWK					= 100,
		VQ_EXTND					= 102,
		V_CONTOURFILL				= 103,
		VSF_PERIMETER				= 104,
		V_GET_PIXEL					= 105,
		VST_EFFECTS					= 106,
		VST_POINT					= 107,
		VSL_ENDS					= 108,
		VRO_CPYFM					= 109,
		VSC_FORM					= 111,
		VSF_UDPAT					= 112,
		VSF_UDSTY					= 113,
		VQIN_MODE					= 115,
		VQT_EXTENT					= 116,
		VQT_WIDTH					= 117,
		VEX_TIMV					= 118,
		VST_LOAD_FONTS				= 119,
		VRT_CPYFM					= 121,
		V_SHOW_C					= 122,
		V_HIDE_C					= 123,
		VS_CLIP						= 129,
		VQT_NAME					= 130,

		VQ_COLOURS					= 2026,

		AES_APPL_INIT				= 6007,

		AES_WIND_CREATE				= 6604,
		AES_WIND_GET				= 6608,
		AES_WIND_OPEN				= 6610,

		AES_GRAF_HANDLE				= 7002,

		AES_SHEL_ENVRN				= 7900,
		AES_SHEL_FIND				= 7901,
		AES_SHEL_GET				= 7902,
		AES_SHEL_PUT				= 7903,
		AES_SHEL_READ				= 7906,
		AES_SHEL_WRITE				= 7908,

		EVT_MOUSE_MOVE				= 17000,
		EVT_MOUSE_DOWN				= 17001,
		EVT_MOUSE_UP				= 17002,
		EVT_KEY_PRESS				= 17010,
		EVT_TIMER					= 17020,

		EVT_FILTER_SET				= 17100,

		EVT_WM_REDRAW				= 18020
		};

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:
		Payload				_payload;			// Message payload

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSET(int16_t, type, Type);				// Received type

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		ClientMsg();
		ClientMsg(int16_t type);
		ClientMsg(int16_t type, Payload payload);
		~ClientMsg() {}

		/*********************************************************************\
		|* Append to a payload
		\*********************************************************************/
		bool append(int16_t value);
		void append(uint8_t *data, uint32_t num);
		bool append(int16_t *value, int num);
		bool append(QRect r);
		inline bool append(Payload list)
			{
			return append(list.data(), (int)list.size());
			}

		/*********************************************************************\
		|* Get the payload
		\*********************************************************************/
		Payload& payload(void);

		/*********************************************************************\
		|* Populate the message from a timer event
		\*********************************************************************/
		void fromTimerEvent(void);

		/*********************************************************************\
		|* Clear the properties so we can re-use the message
		\*********************************************************************/
		void clear(void);

		/*********************************************************************\
		|* Serialise to a byte-stream
		\*********************************************************************/
		QByteArray encode(void);

		/*********************************************************************\
		|* Fetch a byte-array encoded as {length, data} in the word-stream.
		|* returns the number of words consumed.
		\*********************************************************************/
		int fetchData(int idx, QByteArray& ba);

		/*********************************************************************\
		|* De-serialise from a byte-stream
		\*********************************************************************/
		bool decode(int16_t words, QByteArray& inputData);

	};

#endif // CLIENTMSG_H
