#if !defined(GKS_ERROR_H)
#define GKS_ERROR_H

// Non-standard
typedef enum GError
{
    // States
    GERROR_NOT_STATE_GKCL = 1,
    GERROR_NOT_STATE_GKOP = 2,
    GERROR_NOT_STATE_WSAC = 3,
    GERROR_NOT_STATE_SGOP = 4,
    GERROR_NOT_STATE_WSAC_SGOP = 5,
    GERROR_NOT_STATE_WSOP_WSAC = 6,
    GERROR_NOT_STATE_WSOP_WSAC_SGOP = 7,
    GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP = 8,
    // Workstations
    GERROR_INVALID_WSID = 20,
    GERROR_INVALID_CONNID = 21,
    GERROR_INVALID_WSTYPE = 22,
    GERROR_WSTYPE_DOES_NOT_EXIST = 23,
    GERROR_WS_IS_OPEN = 24,
    GERROR_WS_NOT_OPEN = 25,
    GERROR_WS_CANT_OPEN = 26,
    GERROR_WISS_NOT_OPEN = 27,
    GERROR_WISS_ALREADY_OPEN = 28,
    GERROR_WS_IS_ACTIVE = 29,
    GERROR_WS_NOT_ACTIVE = 30,
    GERROR_WS_IS_MO = 31,
    GERROR_WS_NOT_MO = 32,
    GERROR_WS_IS_MI = 33,
    GERROR_WS_NOT_MI = 34,
    GERROR_WS_IS_INPUT = 35,
    GERROR_WS_IS_WISS = 36,
    GERROR_WS_NOT_OUTIN = 37,
    GERROR_WS_NOT_INPUT_OUTIN = 38,
    GERROR_WS_NOT_OUTPUT_OUTIN = 39,
    GERROR_WS_NO_PIXEL_READBACK = 40,
    GERROR_WSTYPE_INVALID_GDP = 41,
    GERROR_MAX_OPEN_WS_EXCEEDED = 42,
    GERROR_MAX_ACTIVE_WS_EXCEEDED = 43,
    // Transformations
    GERROR_INVALID_TRAN_NUM = 50,
    GERROR_INVALID_RECT = 51,
    GERROR_VIEWPORT_NOT_IN_NDC = 52,
    GERROR_WS_WINDOW_NOT_IN_NDC = 53,
    GERROR_WS_VIEWPORT_NOT_IN_DISPLAY = 54,
    // Output attributes
    GERROR_INVALID_LINE_INDEX = 60,
    GERROR_LINE_REP_NOT_DEFINED = 61,
    GERROR_LINE_REP_NOT_PREDEFINED = 62,
    GERROR_LINE_TYPE_ZERO = 63,
    GERROR_LINE_TYPE_NOT_SUPPORTED = 64,
    GERROR_NEGATIVE_LINE_WIDTH = 65,
    GERROR_INVALID_MARKER_INDEX = 66,
    GERROR_MARKER_REP_NOT_DEFINED = 67,
    GERROR_MARKER_REP_NOT_PREDEFINED = 68,
    GERROR_MARKER_TYPE_ZERO = 69,
    GERROR_MARKER_TYPE_NOT_SUPPORTED = 70,
    GERROR_NEGATIVE_MARKER_SIZE = 71,
    GERROR_INVALID_TEXT_INDEX = 72,
    GERROR_TEXT_REP_NOT_DEFINED = 73,
    GERROR_TEXT_REP_NOT_PREDEFINED = 74,
    GERROR_TEXT_FONT_ZERO = 75,
    GERROR_FONT_PREC_NOT_SUPPORTED = 76,
    GERROR_CHAR_EXPANSION_NOT_POSITIVE = 77,
    GERROR_CHAR_HEIGHT_NOT_POSITIVE = 78,
    GERROR_CHAR_UP_LENGTH_ZERO = 79,
    GERROR_INVALID_FILL_INDEX = 80,
    GERROR_FILL_REP_NOT_DEFINED = 81,
    GERROR_FILL_REP_NOT_PREDEFINED = 82,
    GERROR_FILL_STYLE_NOT_SUPPORTED = 83,
    GERROR_FILL_STYLE_ZERO = 84,
    GERROR_INVALID_PATTERN_INDEX = 85,
    GERROR_HATCH_STYLE_NOT_SUPPORTED = 86,
    GERROR_PATTERN_SIZE_NOT_POSITIVE = 87,
    GERROR_PATTERN_REP_NOT_DEFINED = 88,
    GERROR_PATTERN_REP_NOT_PREDEFINED = 89,
    GERROR_PATTERN_NOT_SUPPORTED = 90,
    GERROR_INVALID_COLOR_DIMENSIONS = 91,
    GERROR_NEGATIVE_COLOR_INDEX = 92,
    GERROR_INVALID_COLOR_INDEX = 93,
    GERROR_COLOR_REP_NOT_DEFINED = 94,
    GERROR_COLOR_REP_NOT_PREDEFINED = 95,
    GERROR_INVALID_COLOR = 96,
    GERROR_INVALID_PICK_ID = 97,
    // Output primitives
    GERROR_INVALID_NUM_POINTS = 100,
    GERROR_INVALID_STRING_CODE = 101,
    GERROR_INVALID_GDP_ID = 102,
    GERROR_INVALID_GDP_DATA = 103,
    GERROR_ACTIVE_WS_CANT_GENERATE_GDP = 104,
    GERROR_ACTIVE_WS_CANT_GENERATE_GDP_TRANS_CLIP = 105,
    // Segments
    GERROR_INVALID_SEGMENT_NAME = 120,
    GERROR_SEGMENT_IN_USE = 121,
    GERROR_SEGMENT_NOT_EXIST = 122,
    GERROR_SEGMENT_NOT_EXIST_ON_WS = 123,
    GERROR_SEGMENT_NOT_EXIST_ON_WISS = 124,
    GERROR_SEGMENT_IS_OPEN = 125,
    GERROR_INVALID_SEGMENT_PRIORITY_RANGE = 126,
    // Input
    GERROR_INPUT_DEVICE_ID_NOT_PRESENT_ON_WS = 140,
    GERROR_INPUT_DEVICE_NOT_REQUEST_MODE = 141,
    GERROR_INPUT_DEVICE_NOT_SAMPLE_MODE = 142,
    GERROR_EVENT_SAMPLE_MODE_NOT_AVAILABLE = 143,
    GERROR_PROMPT_ECHO_TYPE_NOT_SUPPORTED_ON_WS = 144,
    GERROR_ECHO_AREA_OUTSIDE_DISPLAY = 145,
    GERROR_INVALID_INPUT_DATA = 146,
    GERROR_INPUT_QUEUE_OVERFLOW = 147,
    GERROR_INPUT_QUEUE_NO_OVERFLOW = 148,
    GERROR_INPUT_QUEUE_OVERFLOW_CLOSED_WS = 149,
    GERROR_NO_INPUT_CLASS_IN_REPORT = 150,
    GERROR_INVALID_TIMEOUT = 151,
    GERROR_INVALID_INITIAL_VALUE = 152,
    GERROR_INITIAL_STROKE_POINTS_EXCEED_BUFFER = 153,
    GERROR_STRING_EXCEEDS_BUFFER = 154,
    // Metafiles
    GERROR_INVALID_ITEM_TYPE_FOR_USER_ITEM = 160,
    GERROR_INVALID_ITEM_LENGTH = 161,
    GERROR_NO_MORE_ITEMS = 162,
    GERROR_INVALID_ITEM = 163,
    GERROR_INVALID_ITEM_TYPE = 164,
    GERROR_INVALID_ITEM_CONTENT_DATA = 165,
    GERROR_INVALID_MAX_ITEM_DATA_LENGTH = 166,
    GERROR_CANT_INTERPRET_ITEM = 167,
    GERROR_FUNCTION_NOT_SUPPORTED = 168,
    // Escape
    GERROR_ESCAPE_NOT_SUPPORTED = 180,
    GERROR_INVALID_ESCAPE_ID = 181,
    GERROR_INVALID_ESCAPE_DATA = 182,
    // Miscellaneous
    GERROR_INVALID_ERROR_FILE = 200,
    // System
    GERROR_GKS_STORAGE_OVERFLOW = 300,
    GERROR_SEGMENT_STORAGE_OVERFLOW = 301,
    GERROR_IO_ERROR_WHILE_READING = 302,
    GERROR_IO_ERROR_WHILE_WRITING = 303,
    GERROR_IO_ERROR_WHILE_WRITING_WS = 304,
    GERROR_IO_ERROR_WHILE_READING_WS = 305,
    GERROR_IO_ERROR_LIBRARY = 306,
    GERROR_IO_ERROR_READING_WS_DESC = 307,
    GERROR_MATH_ERROR = 308,
    // C language binding
    GERROR_C_NULLPTR = 2000,
} GError;

typedef enum GFunction
{
    GFN_OPENGKS = 1,
    GFN_CLOSEGKS,
} GFunction;

// Non-standard
const char *gerrortext(Gint error);

#endif
