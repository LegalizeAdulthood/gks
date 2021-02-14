#include <gks/gks.h>
#include <gks/gkserror.h>

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    gerrorlog(errNum, funcName, errFile);
}

static struct
{
    Gint code;
    const char *text;
}
g_errorText[] = 
{
    // States
    { GERROR_NOT_STATE_GKCL, "GKS not in proper state: GKS shall be in the state GKCL" },
    { GERROR_NOT_STATE_GKOP, "GKS not in proper state: GKS shall be in the state GKOP" },
    { GERROR_NOT_STATE_WSAC, "GKS not in proper state: GKS shall be in the state WSAC" },
    { GERROR_NOT_STATE_SGOP, "GKS not in proper state: GKS shall be in the state SGOP" },
    { GERROR_NOT_STATE_WSAC_SGOP, "GKS not in proper state: GKS shall be either in the state WSAC or in the state SGOP" },
    { GERROR_NOT_STATE_WSOP_WSAC, "GKS not in proper state: GKS shall be either in the state WSOP or in the state WSAC" },
    { GERROR_NOT_STATE_WSOP_WSAC_SGOP, "GKS not in proper state: GKS shall be in one of the states WSOP, WSAC or SGOP" },
    { GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, "GKS not in proper state: GKS shall be in one of the states GKOP, WSOP, WSAC or SGOP" },
    // Workstations
    { GERROR_INVALID_WSID, "Specified workstation identifier is invalid" },
    { GERROR_INVALID_CONNID, "Specified connection identifier is invalid" },
    { GERROR_INVALID_WSTYPE, "Specified workstation type is invalid" },
    { GERROR_WSTYPE_DOES_NOT_EXIST, "Specified workstation type does not exist" },
    { GERROR_WS_IS_OPEN, "Specified workstation is open" },
    { GERROR_WS_NOT_OPEN, "Specified workstation is not open" },
    { GERROR_WS_CANT_OPEN, "Specified workstation cannot be opened" },
    { GERROR_WISS_NOT_OPEN, "Workstation Independent Segment Storage is not open" },
    { GERROR_WISS_ALREADY_OPEN, "Workstation Independent Segment Storage is already open" },
    { GERROR_WS_IS_ACTIVE, "Specified workstation is active" },
    { GERROR_WS_NOT_ACTIVE, "Specified workstation is not active" },
    { GERROR_WS_IS_MO, "Specified workstation is of category MO" },
    { GERROR_WS_NOT_MO, "Specified workstation is not of category MO" },
    { GERROR_WS_IS_MI, "Specified workstation is of category MI" },
    { GERROR_WS_NOT_MI, "Specified workstation is not of category MI" },
    { GERROR_WS_IS_INPUT, "Specified workstation is of category INPUT" },
    { GERROR_WS_IS_WISS, "Specified workstation is Workstation Independent Segment Storage" },
    { GERROR_WS_NOT_OUTIN, "Specified workstation is not of category OUTIN" },
    { GERROR_WS_NOT_INPUT_OUTIN, "Specified workstation is neither of category INPUT nor of category OUTIN" },
    { GERROR_WS_NOT_OUTPUT_OUTIN, "Specified workstation is neither of category OUTPUT nor of category OUTIN" },
    { GERROR_WS_NO_PIXEL_READBACK, "Specified workstation has no pixel store readback capability" },
    { GERROR_WSTYPE_INVALID_GDP, "Specified workstation type is not able to generate the specified generalized drawing primitive" },
    { GERROR_MAX_OPEN_WS_EXCEEDED, "Maximum number of simultaneously open workstations would be exceeded" },
    { GERROR_MAX_ACTIVE_WS_EXCEEDED, "Maximum number of simultaneously active workstations would be exceeded" },
    // Transformations
    { GERROR_INVALID_TRAN_NUM, "Transformation number is invalid" },
    { GERROR_INVALID_RECT, "Rectangle definition is invalid" },
    { GERROR_VIEWPORT_NOT_IN_NDC, "Viewport is not within the Normalized Device Coordinate unit square" },
    { GERROR_WS_WINDOW_NOT_IN_NDC, "Workstation window is not within the Normalized Device Coordinate unit square" },
    { GERROR_WS_VIEWPORT_NOT_IN_DISPLAY, "Workstation viewport is not within the display space" },
    // Output attributes
    { GERROR_INVALID_LINE_INDEX, "Polyline index is invalid" },
    { GERROR_LINE_REP_NOT_DEFINED, "A representation for the specified polyline index has not been defined on this workstation" },
    { GERROR_LINE_REP_NOT_PREDEFINED, "A representation for the specified polyline index has not been predefined on this workstation" },
    { GERROR_LINE_TYPE_ZERO, "Linetype is equal to zero" },
    { GERROR_LINE_TYPE_NOT_SUPPORTED, "Specified linetype is not supported on this workstation" },
    { GERROR_NEGATIVE_LINE_WIDTH, "Linewidth scale factor is less than zero" },
    { GERROR_INVALID_MARKER_INDEX, "Polykmarker index is invalid" },
    { GERROR_MARKER_REP_NOT_DEFINED, "" },
    { GERROR_MARKER_REP_NOT_PREDEFINED, "" },
    { GERROR_MARKER_TYPE_ZERO, "" },
    { GERROR_MARKER_TYPE_NOT_SUPPORTED, "" },
    { GERROR_NEGATIVE_MARKER_SIZE, "" },
    { GERROR_INVALID_TEXT_INDEX, "" },
    { GERROR_TEXT_REP_NOT_DEFINED, "" },
    { GERROR_TEXT_REP_NOT_PREDEFINED, "" },
    { GERROR_TEXT_FONT_ZERO, "" },
    { GERROR_FONT_PREC_NOT_SUPPORTED, "" },
    { GERROR_CHAR_EXPANSION_NOT_POSITIVE, "" },
    { GERROR_CHAR_HEIGHT_NOT_POSITIVE, "" },
    { GERROR_CHAR_UP_LENGTH_ZERO, "" },
    { GERROR_INVALID_FILL_INDEX, "" },
    { GERROR_FILL_REP_NOT_DEFINED, "" },
    { GERROR_FILL_REP_NOT_PREDEFINED, "" },
    { GERROR_FILL_STYLE_NOT_SUPPORTED, "" },
    { GERROR_FILL_STYLE_ZERO, "" },
    { GERROR_INVALID_PATTERN_INDEX, "" },
    { GERROR_HATCH_STYLE_NOT_SUPPORTED, "" },
    { GERROR_PATTERN_SIZE_NOT_POSITIVE, "" },
    { GERROR_PATTERN_REP_NOT_DEFINED, "" },
    { GERROR_PATTERN_REP_NOT_PREDEFINED, "" },
    { GERROR_PATTERN_NOT_SUPPORTED, "" },
    { GERROR_INVALID_COLOR_DIMENSIONS, "" },
    { GERROR_NEGATIVE_COLOR_INDEX, "" },
    { GERROR_INVALID_COLOR_INDEX, "" },
    { GERROR_COLOR_REP_NOT_DEFINED, "" },
    { GERROR_COLOR_REP_NOT_PREDEFINED, "" },
    { GERROR_INVALID_COLOR, "" },
    { GERROR_INVALID_PICK_ID, "" },
    // Output primitives
    { GERROR_INVALID_NUM_POINTS, "" },
    { GERROR_INVALID_STRING_CODE, "" },
    { GERROR_INVALID_GDP_ID, "" },
    { GERROR_INVALID_GDP_DATA, "" },
    { GERROR_ACTIVE_WS_CANT_GENERATE_GDP, "" },
    { GERROR_ACTIVE_WS_CANT_GENERATE_GDP_TRANS_CLIP, "" },
    // Segments
    { GERROR_INVALID_SEGMENT_NAME, "" },
    { GERROR_SEGMENT_IN_USE, "" },
    { GERROR_SEGMENT_NOT_EXIST, "" },
    { GERROR_SEGMENT_NOT_EXIST_ON_WS, "" },
    { GERROR_SEGMENT_NOT_EXIST_ON_WISS, "" },
    { GERROR_SEGMENT_IS_OPEN, "" },
    { GERROR_INVALID_SEGMENT_PRIORITY_RANGE, "" },
    // Input
    { GERROR_INPUT_DEVICE_ID_NOT_PRESENT_ON_WS, "" },
    { GERROR_INPUT_DEVICE_NOT_REQUEST_MODE, "" },
    { GERROR_INPUT_DEVICE_NOT_SAMPLE_MODE, "" },
    { GERROR_EVENT_SAMPLE_MODE_NOT_AVAILABLE, "" },
    { GERROR_PROMPT_ECHO_TYPE_NOT_SUPPORTED_ON_WS, "" },
    { GERROR_ECHO_AREA_OUTSIDE_DISPLAY, "" },
    { GERROR_INVALID_INPUT_DATA, "" },
    { GERROR_INPUT_QUEUE_OVERFLOW, "" },
    { GERROR_INPUT_QUEUE_NO_OVERFLOW, "" },
    { GERROR_INPUT_QUEUE_OVERFLOW_CLOSED_WS, "" },
    { GERROR_NO_INPUT_CLASS_IN_REPORT, "" },
    { GERROR_INVALID_TIMEOUT, "" },
    { GERROR_INVALID_INITIAL_VALUE, "" },
    { GERROR_INITIAL_STROKE_POINTS_EXCEED_BUFFER, "" },
    { GERROR_STRING_EXCEEDS_BUFFER, "" },
    // Metafiles
    { GERROR_INVALID_ITEM_TYPE_FOR_USER_ITEM, "" },
    { GERROR_INVALID_ITEM_LENGTH, "" },
    { GERROR_NO_MORE_ITEMS, "" },
    { GERROR_INVALID_ITEM, "" },
    { GERROR_INVALID_ITEM_TYPE, "" },
    { GERROR_INVALID_ITEM_CONTENT_DATA, "" },
    { GERROR_INVALID_MAX_ITEM_DATA_LENGTH, "" },
    { GERROR_CANT_INTERPRET_ITEM, "" },
    { GERROR_FUNCTION_NOT_SUPPORTED, "" },
    // Escape
    { GERROR_ESCAPE_NOT_SUPPORTED, "" },
    { GERROR_INVALID_ESCAPE_ID, "" },
    { GERROR_INVALID_ESCAPE_DATA, "" },
    // Miscellaneous
    { GERROR_INVALID_ERROR_FILE, "" },
    // System
    { GERROR_GKS_STORAGE_OVERFLOW, "" },
    { GERROR_SEGMENT_STORAGE_OVERFLOW, "" },
    { GERROR_IO_ERROR_WHILE_READING, "" },
    { GERROR_IO_ERROR_WHILE_WRITING, "" },
    { GERROR_IO_ERROR_WHILE_WRITING_WS, "" },
    { GERROR_IO_ERROR_WHILE_READING_WS, "" },
    { GERROR_IO_ERROR_LIBRARY, "" },
    { GERROR_IO_ERROR_READING_WS_DESC, "" },
    { GERROR_MATH_ERROR, "" },
    // C language binding
    { GERROR_C_NULLPTR, "" },
};

const char *gerrortext(Gint error)
{
    for (int i = 0; i < sizeof(g_errorText)/sizeof(g_errorText[0]); ++i)
    {
        if (g_errorText[i].code > error)
        {
            break;
        }
        if (g_errorText[i].code == error)
        {
            return g_errorText[i].text;
        }
    }

    return NULL;
}