#ifndef VDI_H
#define VDI_H

#include <string>

#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QObject>

#include "gemTypes.h"
#include "properties.h"
#include "transport.h"

#define GEM_ROOT_DIR "/usr/local/atari"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class ConnectionMgr;
class Screen;
class Workstation;

/*****************************************************************************\
|* Class declaration. This is the class that actually draws / configures any
|*                    drawing. It's a singleton shared between all the
|*                    workstations because state is held in the workstation
|*
|* When any drawing is performed, the drawing code must call update(x,y,w,h)
|* so that the widget knows it needs to update within that area
\*****************************************************************************/
class VDI : public QObject
	{
	Q_OBJECT
	NON_COPYABLE_NOR_MOVEABLE(VDI)

	/*************************************************************************\
	|* Properties
	\*************************************************************************/

	// "System" disk root-dir
	GETSET(std::string, rootDir, RootDir);

	// Current workstation
	GETSETP(Workstation*, top, Top);

	// Cursors or graphics
	GET(bool, alphaMode);

	// Height of a character in the system font
	GETSET(int, alphaHeight, AlphaHeight);

	// Width of a character in the system font
	GETSET(int, alphaWidth, AlphaWidth);

	// Current alpha cursor X
	GETSET(int, alphaX, AlphaX);

	// Current alpha cursor Y
	GETSET(int, alphaY, AlphaY);

	// Draw in reverse video ?
	GETSET(bool, reverseVideo, ReverseVideo);

	public:
		/*********************************************************************\
		|* Typedefs and enums
		\*********************************************************************/
		typedef enum
			{
			POLY			= 0,
			RECT			= 1,
			PIE				= 3,
			CIRCLE			= 4,
			ELLIPSE			= 5,
			ELLIPTICAL_PIE	= 7,
			ROUNDED_RECT	= 9
			} FillType;

	private:

		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		bool _alphaCursorShown;			// Cursor is actually drawn
		QImage _cursorBacking;			// What was underneath the cursor
		QTimer * _cron;					// Used for periodic update

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit VDI(QObject *parent = nullptr);

		/*********************************************************************\
		|* Private methods: manipulate the alpha cursor if it's on display
		\*********************************************************************/
		bool _eraseAlphaCursor(void);
		void _drawAlphaCursor(void);


	public slots:
		/*********************************************************************\
		|* A frame should be drawn
		\*********************************************************************/
		void newFrame(void);


	public:
		/*********************************************************************\
		|* Get a reference to the VDI
		\*********************************************************************/
		static VDI& sharedInstance(void)
			{
			static VDI _instance;               // The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* tell the widget it needs to be updated within an area
		\*********************************************************************/
		void update(QRect& area);
		void update(QRectF& area);
		void update(int x, int y, int w, int h);
		void update(void);

		/*********************************************************************\
		|* Set the alpha mode and manipulate the background colour
		\*********************************************************************/
		void setAlphaMode(bool yn);


		/*********************************************************************\
		|* Operation:   1   : Open a physical workstation
		\*********************************************************************/
		void v_opnwk(ConnectionMgr *cm, Transport *io, ClientMsg &msg);

		/*********************************************************************\
		|* Operation:   3   : Clear a workstation
		\*********************************************************************/
		void v_clrwk(Transport *io, Workstation *ws);

		/*********************************************************************\
		|* Opcode 5.1 : Query the number of character cells on the alpha display
		\*********************************************************************/
		void vq_chcells(int socket, int16_t& rows, int16_t& columns);
		void vq_chcells(Transport *io, Workstation *ws, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 5.2 : Exit alpha mode. This clears the screen and disables the
		|*              alpha cursor
		\*********************************************************************/
		void v_exit_cur(Transport *io, Workstation *ws);

		/*********************************************************************\
		|* Opcode 5.3 : Enter alpha mode. This clears the screen and enables the
		|*              alpha cursor
		\*********************************************************************/
		void v_enter_cur(Transport *io, Workstation *ws);

		/*********************************************************************\
		|* Opcode 5.4 : Move the cursor up if possible.
		\*********************************************************************/
		void v_curup(int socket);
		void v_curup(Transport *io);

		/*********************************************************************\
		|* Opcode 5.5 : Move the cursor down if possible.
		\*********************************************************************/
		void v_curdown(int socket);
		void v_curdown(Transport *io);

		/*********************************************************************\
		|* Opcode 5.6 : Move the cursor right if possible.
		\*********************************************************************/
		void v_curright(int socket);
		void v_curright(Transport *io);

		/*********************************************************************\
		|* Opcode 5.7 : Move the cursor left if possible.
		\*********************************************************************/
		void v_curleft(int socket);
		void v_curleft(Transport *io);

		/*********************************************************************\
		|* Opcode 5.8 : Move the cursor home.
		\*********************************************************************/
		void v_curhome(int socket);
		void v_curhome(Transport *io);

		/*********************************************************************\
		|* Opcode 5.9 : Erase from the cursor to the end of the screen.
		\*********************************************************************/
		void v_eeos(int socket);
		void v_eeos(Transport *io);

		/*********************************************************************\
		|* Opcode 5.10: Erase from the cursor to the end of the line
		\*********************************************************************/
		void v_eeol(int socket);
		void v_eeol(Transport *io);

		/*********************************************************************\
		|* Opcode 5.11: Move the cursor to a position (x,y) {1-based}
		\*********************************************************************/
		void vs_curaddress(int socket, int row, int col);
		void vs_curaddress(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 5.12: Draw text at cursor.
		\*********************************************************************/
		void v_curtext(int socket, const char *str);
		void v_curtext(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 5.13: Enable reverse-video text
		\*********************************************************************/
		void v_rvon(int socket);
		void v_rvon(Transport *io);

		/*********************************************************************\
		|* Opcode 5.14: Disable reverse-video text
		\*********************************************************************/
		void v_rvoff(int socket);
		void v_rvoff(Transport *io);

		/*********************************************************************\
		|* Opcode 5.15: Draw text at cursor.
		\*********************************************************************/
		void vq_curaddress(int socket, int16_t& row, int16_t& col);
		void vq_curaddress(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 5.18: Position the graphic cursor.
		\*********************************************************************/
		void v_dspcur(int socket, int16_t x, int16_t y);
		void v_dspcur(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 5.19: Hide the graphic cursor.
		\*********************************************************************/
		void v_rmcur(int socket);
		void v_rmcur(Transport *io);

		/*********************************************************************\
		|* Opcode 6: Draw a poly-line.
		\*********************************************************************/
		void v_pline(int socket, int16_t num, int16_t *pxy);
		void v_pline(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 7: Draw a poly-marker.
		\*********************************************************************/
		void v_pmarker(int socket, int16_t num, int16_t *pxy);
		void v_pmarker(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 8: Draw graphic text
		\*********************************************************************/
		void v_gtext(int socket, int16_t x, int16_t y, int16_t w, char *txt);
		void v_gtext(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|*   9	: Fill a polygon				[type=0] [pxy=x0,y0,x1,y1,...]
		\*********************************************************************/
		void v_fillarea(int socket, FillType type, int16_t num, int16_t *pxy);
		void v_fillarea(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.1	Fill a rectangle		[type=1] [pxy=x0,y0,x1,y1]
		\*********************************************************************/
		void v_bar(int socket, int16_t num, int16_t *pxy);
		void v_bar(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.2 Draw an arc
		\*********************************************************************/
		void v_arc(int socket, int16_t x, int16_t y, int16_t radius,
				   int16_t start, int16_t end);
		void v_arc(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* 11.3: Fill an arc					[type=2] [pxy=x,y,r,begin,end]
		\*********************************************************************/
		void v_pieslice(int socket, int16_t x, int16_t y, int16_t radius,
						int16_t start, int16_t end);
		void v_pieslice(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|*  11.4: Fill a circle					[type=4] [pxy=x,y,r]
		\*********************************************************************/
		void v_circle(int socket, int16_t x, int16_t y, int16_t radius);
		void v_circle(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|*  11.5: Fill an ellipse				[type=5] [pxy=x,y,rx,ry]
		\*********************************************************************/
		void v_ellipse(int socket, int16_t x, int16_t y, int16_t xr, int16_t yr);
		void v_ellipse(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.6	Draw an elliptical arc
		\*********************************************************************/
		void v_ellarc(int socket, int16_t x, int16_t y, int16_t xr, int16_t yr,
					  int16_t start, int16_t end);
		void v_ellarc(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.7	Draw an elliptical pie (filled arc)
		\*********************************************************************/
		void v_ellpie(int socket, int16_t x, int16_t y, int16_t xr, int16_t yr,
					  int16_t start, int16_t end);
		void v_ellpie(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.8	Draw a rounded rect
		\*********************************************************************/
		void v_rbox(int socket, int16_t* pxy);
		void v_rbox(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.9	Fill a rounded rect		[type=9] [pxy=x0,y0,x1,y1]
		\*********************************************************************/
		void v_rfbox(int socket, int16_t* pxy);
		void v_rfbox(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 11.10	Justified text]
		\*********************************************************************/
		void v_justified(int socket, int16_t x, int16_t y,
						 char *str, int16_t len,
						 int16_t wordSpacing, int16_t charSpacing);
		void v_justified(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 12: Set the height of text in pixels. Return metrics
		\*********************************************************************/
		void vst_height(int socket, int16_t height, int16_t& charWidth,
						int16_t& charHeight, int16_t& cellWidth,
						int16_t &cellHeight);
		void vst_height(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 13: Set the text-rotation angle
		\*********************************************************************/
		void vst_rotation(int handle, int16_t angle);
		void vst_rotation(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 14: Set the RGB for a colour
		\*********************************************************************/
		void vs_color(int handle, int16_t idx, int16_t *rgb);
		void vs_color(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 15: Set the style for drawing lines
		\*********************************************************************/
		void vsl_type(int handle, int16_t type);
		void vsl_type(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 16: Set the width for drawing lines
		\*********************************************************************/
		void vsl_width(int handle, int16_t width);
		void vsl_width(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 17: Set the colour for drawing lines
		\*********************************************************************/
		void vsl_color(int handle, int16_t idx);
		void vsl_color(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 18: Set the type of marker drawn
		\*********************************************************************/
		void vsm_type(int handle, int16_t which);
		void vsm_type(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 19: Set the height of marker in pixels
		\*********************************************************************/
		void vsm_height(int handle, int16_t height);
		void vsm_height(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 20: Set the colour for drawing markers
		\*********************************************************************/
		void vsm_color(int handle, int16_t height);
		void vsm_color(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 21: Set the font to use
		\*********************************************************************/
		int16_t vst_font(int handle, int16_t idx);
		void vst_font(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 22: Set the colour for drawing text
		\*********************************************************************/
		void vst_color(int handle, int16_t idx);
		void vst_color(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 23: Set fill-type (none, solid, pattern, hash, user)
		\*********************************************************************/
		void vsf_interior(int handle, int16_t idx);
		void vsf_interior(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 24: Set the fill-pattern index if using pattern/hash fill
		\*********************************************************************/
		void vsf_style(int handle, int16_t idx);
		void vsf_style(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 25: Set the fill-pattern colour
		\*********************************************************************/
		void vsf_color(int handle, int16_t idx);
		void vsf_color(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 26: Query the RGB values of a colour index.
		\*********************************************************************/
		void vq_color(int handle, int16_t idx, int16_t* rgb);
		void vq_color(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 32: Set the writing mode.
		\*********************************************************************/
		void vswr_mode(int handle, int16_t mode);
		void vswr_mode(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 33: Set the input mode.
		\*********************************************************************/
		void vsin_mode(int handle, int16_t device, int16_t mode);
		void vsin_mode(Transport *io, ClientMsg &cm);

		/*********************************************************************\
		|* Opcode 35: Get the current line-drawing style.
		\*********************************************************************/
		void vql_attributes(int handle, int16_t* settings);
		void vql_attributes(Transport *io, ClientMsg &cm);
		};

#endif // VDI_H
