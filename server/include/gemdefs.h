#ifndef GEMDEFS_H
#define GEMDEFS_H

/*****************************************************************************\
|* Define a constant so we know it's our version of GEM
\*****************************************************************************/
#ifndef QGEM
#  define QGEM 1
#endif


/*****************************************************************************\
|* Typedefs
\*****************************************************************************/
#ifdef __cplusplus
	#include <vector>
	#include <QList>
	#include <QString>

	typedef std::vector<int16_t>				Payload;
	typedef std::vector<uint8_t>				ByteStream;
	typedef QList<qreal> 						LineStyle;
	typedef QList<qintptr>						HandleList;
	typedef QMap<QString, QString>				StringMap;
#else
	#error Cannot compile qVDI without a C++ compiler
#endif

/*****************************************************************************\
|* List of available GDPs
\*****************************************************************************/
enum
	{
	GDP_V_MIN = 0,
	GDP_V_BAR,
	GDP_V_ARC,
	GDP_V_PIESLICE,
	GDP_V_CIRCLE,
	GDP_V_ELLIPSE,
	GDP_V_ELLARC,
	GDP_V_ELLPIE,
	GDP_V_RBOX,
	GDP_V_RFBOX,
	GDP_V_JUSTIFIED,
	GDP_V_MAX
	};


/*****************************************************************************\
|* List of available attributes for a GDP
\*****************************************************************************/
enum
	{
	GDP_ATTR_MIN = -1,
	GDP_ATTR_POLYLINE,
	GDP_ATTR_MARKER,
	GDP_ATTR_TEXT,
	GDP_ATTR_FILLAREA,
	GDP_ATTR_NONE,
	GDP_ATTR_MAX
	};

/*****************************************************************************\
|* Number of locator devices available for graphic cursor control
\*****************************************************************************/
enum
	{
	LOCATOR_DEVICE_MIN = -1,
	LOCATOR_DEVICE_NONE = 0,
	LOCATOR_DEVICE_KEYBOARD_ONLY,
	LOCATOR_DEVICE_KEYBOARD_AND_MOUSE,
	LOCATOR_DEVICE_MAX
	};

/*****************************************************************************\
|* Number of valuator devices available for graphic cursor control
\*****************************************************************************/
enum
	{
	VALUATOR_DEVICE_MIN = -1,
	VALUATOR_DEVICE_NONE,
	VALUATOR_DEVICE_KEYBOARD_ONLY,
	VALUATOR_DEVICE_OTHER,
	VALUATOR_DEVICE_MAX
	};

/*****************************************************************************\
|* Number of choice devices available for graphic cursor control
\*****************************************************************************/
enum
	{
	CHOICE_DEVICE_MIN = -1,
	CHOICE_DEVICE_NONE = 0,
	CHOICE_DEVICE_FUNCTION_KEYS,
	CHOICE_DEVICE_FUNCTION_KEYS_AND_KEYPAD,
	CHOICE_DEVICE_MAX
	};

/*****************************************************************************\
|* Number of string devices available for graphic cursor control
\*****************************************************************************/
enum
	{
	STRING_DEVICE_MIN = -1,
	STRING_DEVICE_NONE = 0,
	STRING_DEVICE_KEYBOARD,
	STRING_DEVICE_MAX
	};


/*****************************************************************************\
|* Device type
\*****************************************************************************/
enum
	{
	DEVICE_TYPE_OUTPUT_MIN = -2,
	DEVICE_TYPE_OUTPUT_LANDSCAPE,
	DEVICE_TYPE_OUTPUT_ONLY = 0,
	DEVICE_TYPE_INPUT_ONLY,
	DEVICE_TYPE_INPUT_AND_OUTPUT,
	DEVICE_TYPE_RESERVED,
	DEVICE_TYPE_METAFILE,
	DEVICE_TYPE_MAX
	};


/*****************************************************************************\
|* Available line types
\*****************************************************************************/
enum
	{
	LINE_TYPE_MIN = 0,
	LINE_TYPE_SOLID,
	LINE_TYPE_LONG_DASH,
	LINE_TYPE_DOT,
	LINE_TYPE_DASH_DOT,
	LINE_TYPE_DASH,
	LINE_TYPE_DASH_DOT_DOT,
	LINE_TYPE_USER,
	LINE_TYPE_MAX
	};


/*****************************************************************************\
|* Available marker types
\*****************************************************************************/
enum
	{
	MARKER_TYPE_MIN = 0,
	MARKER_TYPE_DOT,
	MARKER_TYPE_PLUS,
	MARKER_TYPE_ASTERISK,
	MARKER_TYPE_SQUARE,
	MARKER_TYPE_CROSS,
	MARKER_TYPE_DIAMOND,
	MARKER_TYPE_MAX
	};

/*****************************************************************************\
|* Available fill styles
\*****************************************************************************/
enum
	{
	FILL_STYLE_MIN = -1,
	FILL_STYLE_HOLLOW,
	FILL_STYLE_PATTERN,
	FILL_STYLE_HATCH,
	FILL_STYLE_USER,
	FILL_STYLE_MAX
	};

/*****************************************************************************\
|* Available hatch fill styles
\*****************************************************************************/
enum
	{
	FILL_HATCH_MIN=-1,
	FILL_HATCH_12,
	FILL_HATCH_25,
	FILL_HATCH_37,
	FILL_HATCH_50,
	FILL_HATCH_62,
	FILL_HATCH_75,
	FILL_HATCH_87,
	FILL_HATCH_100,
	FILL_HATCH_MAX
	};

/*****************************************************************************\
|* Available pattern fill styles
\*****************************************************************************/
	enum
	{
	FILL_PATTERN_MIN=-1,
	FILL_PATTERN_BRICK,
	FILL_PATTERN_BRICK_DIAGONAL,
	FILL_PATTERN_INVERTED_V,
	FILL_PATTERN_SCALES,
	FILL_PATTERN_CROSS_HATCH,
	FILL_PATTERN_WEIRD,
	FILL_PATTERN_DIAGONAL_DASH,
	FILL_PATTERN_WEAVE,
	FILL_PATTERN_UP_DOWN,
	FILL_PATTERN_DOTTED_BOX,
	FILL_PATTERN_INVERT_DIAMOND,
	FILL_PATTERN_THIN_DIAGONAL,
	FILL_PATTERN_THICK_DIAGONAL,
	FILL_PATTERN_BOTH_DIAGONAL,
	FILL_PATTERN_VERTICAL,
	FILL_PATTERN_HORIZONTAL,
	FILL_PATTERN_VERT_HORIZ,
	FILL_PATTERN_WIDE_THIN_DIAGONAL,
	FILL_PATTERN_WIDE_THICK_DIAGONAL,
	FILL_PATTERN_WIDE_BOTH_DIAGONAL,
	FILL_PATTERN_WIDE_VERTICAL,
	FILL_PATTERN_WIDE_HORIZONTAL,
	FILL_PATTERN_VERT_HORIZ_2,
	FILL_PATTERN_MAX
	};

#endif // GEMDEFS_H
