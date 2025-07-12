#include <string>

//#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "transport.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
ConnectionMgr::ConnectionMgr(QObject *parent)
			  :QObject{parent}
	{
	_server.setSocketOptions(QLocalServer::WorldAccessOption);

	/*************************************************************************\
	|* Clean up any remnants from a previous run
	\*************************************************************************/
	QLocalServer::removeServer(VDI_SERVICE_NAME);

	/*************************************************************************\
	|* Connect up the QLocalServer so it handles incoming connections
	\*************************************************************************/
	connect(&_server, &QLocalServer::newConnection,
			this, QOverload<>::of(&ConnectionMgr::_connection));

	}

/*****************************************************************************\
|* Start listening
\*****************************************************************************/
void ConnectionMgr::start(Screen *screen)
	{
	_screen = screen;
	_server.listen(VDI_SERVICE_NAME);

	const std::string& stdS = _server.fullServerName().toStdString();
	fprintf(stderr, "Now listening on %s\n", stdS.c_str());
	}

/*****************************************************************************\
|* Stop listening
\*****************************************************************************/
void ConnectionMgr::stop(void)
	{
	_server.close();
	}

/*****************************************************************************\
|* We got a new connection
\*****************************************************************************/
void ConnectionMgr::_connection(void)
	{
	QLocalSocket *client = _server.nextPendingConnection();
	Transport *io = new Transport(client, this);
	fprintf(stderr, "Connection! %d client(s)\n", (int) _conns.size());

	/*************************************************************************\
	|* When the socket closes, we can no longer call socketDescriptor() - it
	|* just returns -1. Get the socketDescriptor and stash it into a property
	|* so we have access to it later
	\*************************************************************************/
	client->setProperty(SOCKET_IDENTIFIER, client->socketDescriptor());

	while (client != nullptr)
		{
		_conns[client->socketDescriptor()] = io;

		connect(client, &QLocalSocket::disconnected,
				this, QOverload<>::of(&ConnectionMgr::_disconnection));

		connect(client, &QLocalSocket::readyRead,
				this, QOverload<>::of(&ConnectionMgr::_incomingData));

		client = _server.nextPendingConnection();
		emit transportConnected(io);
		}
	fprintf(stderr, "now %d client(s)\n", (int) _conns.size());
	}

/*****************************************************************************\
|* We got a disconnection
\*****************************************************************************/
void ConnectionMgr::_disconnection(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();

	/*************************************************************************\
	|* The socket has closed, and the handle is therefore lost, but we stashed
	|* the original handle into a property named SOCKET_IDENTIFIER. Retrieve
	|* the orignal handle via the property instead of socketDesctiptor()
	\*************************************************************************/
	qintptr handle		= socket->property(SOCKET_IDENTIFIER).toLongLong();

	/*************************************************************************\
	|* Prevent timer events being sent
	\*************************************************************************/
	if (_timerList.contains(handle))
		_timerList.removeOne(handle);

	/*************************************************************************\
	|* Close down any resources in the AES for this application
	\*************************************************************************/
	//AES::sharedInstance().closeWorkstation(findWorkstationForHandle(handle));

	// FIXME: Needs to cycle back through list, only be nullptr when no
	// applications left
	VDI::sharedInstance().setTop(nullptr);

	/*************************************************************************\
	|* Remove the workstation from the workstations list, which will
	|* delete it since it has 'this' as a QObject parent
	\*************************************************************************/
	Transport *io = _conns[handle];
	_wsList.remove(io);

	/*************************************************************************\
	|* Remove the transport from the connections list, which will
	|* delete it since it has 'this' as a QObject parent
	\*************************************************************************/
	_conns.remove(handle);

	fprintf(stderr, "Disconnection! %d left\n", (int) _conns.size());
	}

/*****************************************************************************\
|* Return the workstation for a given handle
\*****************************************************************************/
Workstation * ConnectionMgr::findWorkstationForHandle(qintptr handle)
	{
	if (_conns.contains(handle))
		{
		Transport *io = _conns[handle];
		if (_wsList.contains(io))
			return _wsList[io];
		}
	return nullptr;
	}

/*****************************************************************************\
|* Map a transport object to a workstation
\*****************************************************************************/
void ConnectionMgr::mapTransportToWorkstation(Transport *io, Workstation *ws)
	{
	_wsList[io] = ws;
	}


/*****************************************************************************\
|* Enable (Add) or disable (remove) a timer-interest to the list
\*****************************************************************************/
void ConnectionMgr::updateTimerInterest(qintptr handle, int16_t enable)
	{
	if (enable)
		{
		if (!_timerList.contains(handle))
			_timerList.append(handle);
		}
	else if (_timerList.contains(handle))
		_timerList.removeOne(handle);
	}

/*****************************************************************************\
|* We got incoming...
\*****************************************************************************/
void ConnectionMgr::_incomingData(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();
	Transport *io			= _conns[socket->socketDescriptor()];
	Workstation *ws			= _wsList[io];

	//fprintf(stderr, " .. data available: %lld bytes\n", socket->bytesAvailable());

	ClientMsg cm;

	bool bytesAvailable = socket->bytesAvailable() > 0;
	bool bufferNotEmpty = io->buffer().length() > 0;
	while (bytesAvailable || bufferNotEmpty)
		{
		io->read(cm);
		fprintf(stderr, " .. dispatch message of type: %d\n", cm.type());
		switch (cm.type())
			{
			// 1
			// ---------------------------------------------------------------
			case ClientMsg::V_OPNWK:
				VDI::sharedInstance().v_opnwk(this, io, cm);
				break;

			// 3
			// ---------------------------------------------------------------
			case ClientMsg::V_CLRWK:
				VDI::sharedInstance().v_clrwk(io, ws);
				break;

			// 5.1
			// ---------------------------------------------------------------
			case ClientMsg::VQ_CHCELLS:
				VDI::sharedInstance().vq_chcells(io, ws, cm);
				break;

			// 5.2
			// ---------------------------------------------------------------
			case ClientMsg::V_EXIT_CUR:
				VDI::sharedInstance().v_exit_cur(io, ws);
				break;

			// 5.3
			// ---------------------------------------------------------------
			case ClientMsg::V_ENTER_CUR:
				VDI::sharedInstance().v_enter_cur(io, ws);
				break;

			// 5.4
			// ---------------------------------------------------------------
			case ClientMsg::V_CURUP:
				VDI::sharedInstance().v_curup(io);
				break;

			// 5.5
			// ---------------------------------------------------------------
			case ClientMsg::V_CURDOWN:
				VDI::sharedInstance().v_curdown(io);
				break;

			// 5.6
			// ---------------------------------------------------------------
			case ClientMsg::V_CURRIGHT:
				VDI::sharedInstance().v_curright(io);
				break;

			// 5.7
			// ---------------------------------------------------------------
			case ClientMsg::V_CURLEFT:
				VDI::sharedInstance().v_curleft(io);
				break;

			// 5.8
			// ---------------------------------------------------------------
			case ClientMsg::V_CURHOME:
				VDI::sharedInstance().v_curhome(io);
				break;

			// 5.9
			// ---------------------------------------------------------------
			case ClientMsg::V_EEOS:
				VDI::sharedInstance().v_eeos(io);
				break;

			// 5.10
			// ---------------------------------------------------------------
			case ClientMsg::V_EEOL:
				VDI::sharedInstance().v_eeol(io);
				break;

			// // 5.11
			// ---------------------------------------------------------------
			case ClientMsg::VS_CURADDRESS:
				VDI::sharedInstance().vs_curaddress(io, cm);
				break;

			// 5.12
			// ---------------------------------------------------------------
			case ClientMsg::V_CURTEXT:
				VDI::sharedInstance().v_curtext(io, cm);
				break;

			// 5.13
			// ---------------------------------------------------------------
			case ClientMsg::V_RVON	:
				VDI::sharedInstance().v_rvon(io);
				break;

			// 5.14
			// ---------------------------------------------------------------
			case ClientMsg::V_RVOFF:
				VDI::sharedInstance().v_rvoff(io);
				break;

			// 5.15
			// ---------------------------------------------------------------
			case ClientMsg::VQ_CURADDRESS:
				VDI::sharedInstance().vq_curaddress(io, cm);
				break;

			// 5.18
			// ---------------------------------------------------------------
			case ClientMsg::V_DSPCUR:
				VDI::sharedInstance().v_dspcur(io, cm);
				break;

			// 5.19
			// ---------------------------------------------------------------
			case ClientMsg::V_RMCUR:
				VDI::sharedInstance().v_rmcur(io);
				break;

			// 6
			// ---------------------------------------------------------------
			case ClientMsg::V_PLINE:
				VDI::sharedInstance().v_pline(io, cm);
				break;

			// 7
			// ---------------------------------------------------------------
			case ClientMsg::V_PMARKER:
				VDI::sharedInstance().v_pmarker(io, cm);
				break;

			// 8
			// ---------------------------------------------------------------
			case ClientMsg::V_GTEXT:
				VDI::sharedInstance().v_gtext(io, cm);
				break;

			// 9
			// ---------------------------------------------------------------
			case ClientMsg::V_FILLAREA:
				VDI::sharedInstance().v_fillarea(io, cm);
				break;

			// 11.1
			// ---------------------------------------------------------------
			case ClientMsg::V_BAR:
				VDI::sharedInstance().v_bar(io, cm);
				break;

			// 11.2
			// ---------------------------------------------------------------
			case ClientMsg::V_ARC:
				VDI::sharedInstance().v_arc(io, cm);
				break;

			// 11.3
			// ---------------------------------------------------------------
			case ClientMsg::V_PIESLICE:
				VDI::sharedInstance().v_pieslice(io, cm);
				break;

			// 11.4
			// ---------------------------------------------------------------
			case ClientMsg::V_CIRCLE:
				VDI::sharedInstance().v_circle(io, cm);
				break;

			// 11.5
			// ---------------------------------------------------------------
			case ClientMsg::V_ELLIPSE:
				VDI::sharedInstance().v_ellipse(io, cm);
				break;

			// 11.6
			// ---------------------------------------------------------------
			case ClientMsg::V_ELLARC:
				VDI::sharedInstance().v_ellarc(io, cm);
				break;

			// 11.7
			// ---------------------------------------------------------------
			case ClientMsg::V_ELLPIE:
				VDI::sharedInstance().v_ellpie(io, cm);
				break;

			// 11.8
			// ---------------------------------------------------------------
			case ClientMsg::V_RBOX:
				VDI::sharedInstance().v_rbox(io, cm);
				break;

			// 11.9
			// ---------------------------------------------------------------
			case ClientMsg::V_RFBOX:
				VDI::sharedInstance().v_rfbox(io, cm);
				break;

			// 11.10
			// ---------------------------------------------------------------
			case ClientMsg::V_JUSTIFIED:
				VDI::sharedInstance().v_justified(io, cm);
				break;

			// 12
			// ---------------------------------------------------------------
			case ClientMsg::VST_HEIGHT:
				VDI::sharedInstance().vst_height(io, cm);
				break;

			// 13
			// ---------------------------------------------------------------
			case ClientMsg::VST_ROTATION:
				VDI::sharedInstance().vst_rotation(io, cm);
				break;

			// 14
			// ---------------------------------------------------------------
			case ClientMsg::VS_COLOR:
				VDI::sharedInstance().vs_color(io, cm);
				break;

			// 15
			// ---------------------------------------------------------------
			case ClientMsg::VSL_TYPE:
				VDI::sharedInstance().vsl_type(io, cm);
				break;

			// 16
			// ---------------------------------------------------------------
			case ClientMsg::VSL_WIDTH:
				VDI::sharedInstance().vsl_width(io, cm);
				break;

			// 17
			// ---------------------------------------------------------------
			case ClientMsg::VSL_COLOR:
				VDI::sharedInstance().vsl_color(io, cm);
				break;

			// 18
			// ---------------------------------------------------------------
			case ClientMsg::VSM_TYPE:
				VDI::sharedInstance().vsm_type(io, cm);
				break;

			// 19
			// ---------------------------------------------------------------
			case ClientMsg::VSM_HEIGHT:
				VDI::sharedInstance().vsm_height(io, cm);
				break;

			// 20
			// ---------------------------------------------------------------
			case ClientMsg::VSM_COLOR:
				VDI::sharedInstance().vsm_color(io, cm);
				break;

			// 21
			// ---------------------------------------------------------------
			case ClientMsg::VST_FONT:
				VDI::sharedInstance().vst_font(io, cm);
				break;

			// 22
			// ---------------------------------------------------------------
			case ClientMsg::VST_COLOR:
				VDI::sharedInstance().vst_color(io, cm);
				break;

			// 23vsf_color
			// ---------------------------------------------------------------
			case ClientMsg::VSF_INTERIOR:
				VDI::sharedInstance().vsf_interior(io, cm);
				break;

			// 24
			// ---------------------------------------------------------------
			case ClientMsg::VSF_STYLE:
				VDI::sharedInstance().vsf_style(io, cm);
				break;

			// 25
			// ---------------------------------------------------------------
			case ClientMsg::VSF_COLOR:
				VDI::sharedInstance().vsf_color(io, cm);
				break;

			// 26
			// ---------------------------------------------------------------
			case ClientMsg::VQ_COLOR:
				VDI::sharedInstance().vq_color(io, cm);
				break;

		// 	case ClientMsg::VSWR_MODE:				// 32
		// 		VDI::sharedInstance().vswr_mode(ws, &cm);
		// 		break;

		// 	case ClientMsg::VSIN_MODE:				// 33
		// 		VDI::sharedInstance().vsin_mode(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQL_ATTRIBUTES:			// 35
		// 		VDI::sharedInstance().vql_attributes(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQM_ATTRIBUTES:			// 36
		// 		VDI::sharedInstance().vqm_attributes(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQF_ATTRIBUTES:			// 37
		// 		VDI::sharedInstance().vqf_attributes(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQT_ATTRIBUTES:			// 38
		// 		VDI::sharedInstance().vqt_attributes(ws, &cm);
		// 		break;

		// 	case ClientMsg::VST_ALIGNMENT:			// 39
		// 		VDI::sharedInstance().vst_alignment(ws, &cm);
		// 		break;

		// 	case ClientMsg::VS_PIXRGB	:			// 59
		// 		VDI::sharedInstance().vs_pixrgb(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQ_PIXRGB	:			// 60
		// 		VDI::sharedInstance().vq_pixrgb(ws, &cm);
		// 		break;

		// 	case ClientMsg::V_OPNVWK:				// 100
		// 		VDI::sharedInstance().v_opnvwk(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQ_EXTND:				// 102
		// 		VDI::sharedInstance().vq_extnd(ws, &cm);
		// 		break;

		// 	case ClientMsg::V_CONTOURFILL:			// 103
		// 		VDI::sharedInstance().v_contourfill(ws, &cm);
		// 		break;

		// 	case ClientMsg::V_GET_PIXEL:			// 105
		// 		VDI::sharedInstance().v_get_pixel(ws, &cm);
		// 		break;

		// 	case ClientMsg::VSF_PERIMETER:			// 105
		// 		VDI::sharedInstance().vsf_perimeter(ws, &cm);
		// 		break;

		// 	case ClientMsg::VST_EFFECTS:			// 106
		// 		VDI::sharedInstance().vst_effects(ws, &cm);
		// 		break;

		// 	case ClientMsg::VST_POINT:				// 107
		// 		VDI::sharedInstance().vst_point(ws, &cm);
		// 		break;

		// 	case ClientMsg::VSL_ENDS:				// 108
		// 		VDI::sharedInstance().vsl_ends(ws, &cm);
		// 		break;

		// 	case ClientMsg::VRO_CPYFM:				// 109
		// 		VDI::sharedInstance().vro_cpyfm(ws, &cm);
		// 		break;

		// 	case ClientMsg::VSC_FORM:				// 111
		// 		VDI::sharedInstance().vsc_form(ws, &cm);
		// 		break;

		// 	case ClientMsg::VSF_UDPAT:				// 112
		// 		VDI::sharedInstance().vsf_udpat(ws, &cm);
		// 		break;

		// 	case ClientMsg::VSF_UDSTY:				// 113
		// 		VDI::sharedInstance().vsf_udsty(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQIN_MODE:				// 115
		// 		VDI::sharedInstance().vqin_mode(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQT_EXTENT:				// 116
		// 		VDI::sharedInstance().vqt_extent(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQT_WIDTH:				// 117
		// 		VDI::sharedInstance().vqt_width(ws, &cm);
		// 		break;

		// 	case ClientMsg::VEX_TIMV:				// 118
		// 		VDI::sharedInstance().vex_timv(ws, &cm);
		// 		break;

		// 	case ClientMsg::VST_LOAD_FONTS:			// 119
		// 		VDI::sharedInstance().vst_load_fonts(ws, &cm);
		// 		break;

		// 	case ClientMsg::VRT_CPYFM:				// 121
		// 		VDI::sharedInstance().vrt_cpyfm(ws, &cm);
		// 		break;

		// 	case ClientMsg::V_SHOW_C:				// 122
		// 		VDI::sharedInstance().v_show_c(ws, &cm);
		// 		break;

		// 	case ClientMsg::V_HIDE_C:				// 123
		// 		VDI::sharedInstance().v_hide_c(ws, &cm);
		// 		break;

		// 	case ClientMsg::VS_CLIP:				// 129
		// 		VDI::sharedInstance().vs_clip(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQT_NAME:				// 130
		// 		VDI::sharedInstance().vqt_name(ws, &cm);
		// 		break;

		// 	case ClientMsg::VQ_COLOURS:				// 2026
		// 		VDI::sharedInstance().vq_colours(ws, &cm);
		// 		break;


		// 	case ClientMsg::AES_APPL_INIT:			// 6007
		// 		AES::sharedInstance().appl_init(ws, &cm);
		// 		break;


		// 	case ClientMsg::AES_WIND_CREATE:		// 6604
		// 		AES::sharedInstance().wind_create(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_WIND_GET:			// 6608
		// 		AES::sharedInstance().wind_get(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_WIND_OPEN:			// 6610
		// 		AES::sharedInstance().wind_open(ws, &cm);
		// 		break;


		// 	case ClientMsg::AES_GRAF_HANDLE:		// 7002
		// 		AES::sharedInstance().graf_handle(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_SHEL_ENVRN:			// 7900
		// 		AES::sharedInstance().shel_envrn(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_SHEL_FIND:			// 7901
		// 		AES::sharedInstance().shel_find(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_SHEL_GET:			// 7902
		// 		AES::sharedInstance().shel_get(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_SHEL_PUT:			// 7903
		// 		AES::sharedInstance().shel_put(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_SHEL_READ:			// 7906
		// 		AES::sharedInstance().shel_read(ws, &cm);
		// 		break;

		// 	case ClientMsg::AES_SHEL_WRITE:			// 7908
		// 		AES::sharedInstance().shel_write(ws, &cm);
		// 		break;

		// 	case ClientMsg::EVT_FILTER_SET:			// 17100
		// 		VDI::sharedInstance().setEventFilter(ws, &cm);
		// 		break;

			default:
				fprintf(stderr, "\n** Unknown message type %d", cm.type());
				break;
			}

		bytesAvailable = socket->bytesAvailable() > 0;
		bufferNotEmpty = io->buffer().length() > 0;
		}
	}
