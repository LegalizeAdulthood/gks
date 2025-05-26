#include <gks/gks.h>
#include <gks/gkserror.h>

#include <algorithm>
#include <functional>
#include <numeric>

template <typename T, size_t N>
Gint numOf(T (&ary)[N])
{
    return static_cast<Gint>(N);
}
static Gfile *g_errFile = NULL;
static Gopst g_opState = GGKCL;

enum
{
    MAX_OPEN_WS = 1,
    MAX_ACTIVE_WS = 1,
    MAX_SEGMENT_ASSOC = 0,
    MAX_NUM_TRANSFORMS = 2,
    MAX_NUM_COLORS = 64,
    MAX_WS_TYPES = 1
};

struct GGKSDescription
{
    Glevel level;
    Gint numWsTypes;
    const Gwstype *wsTypes;
    Gwsmax wsmax;
    Gint numTrans;
};

static const Gwstype g_wsTypes[MAX_WS_TYPES] =
{
    GWSTYPE_TEK4105
};

static GGKSDescription g_gksDescription =
{
    GLMA,
    numOf(g_wsTypes),
    g_wsTypes,
    { MAX_OPEN_WS, MAX_ACTIVE_WS, MAX_SEGMENT_ASSOC },
    MAX_NUM_TRANSFORMS
};

struct GGKSState
{
    Gint numOpenWs;
    Gint openWs[MAX_OPEN_WS];
    Gint numActiveWs;
    Gint activeWs[MAX_ACTIVE_WS];
    Gint currentTransform;
    Gint numTransforms;
    Gtran transforms[MAX_NUM_TRANSFORMS];
    Gclip clipping;
    // polyline
    Gint currentLineType;
    Gint currentLineColorIndex;
    Gint currentLineIndex;
    // polymarker
    Gint currentMarkerType;
    Gfloat currentMarkerSize;
    Gint currentMarkerColorIndex;
    Gint currentMarkerIndex;
    // text
    Gfloat currentCharHeight;
    Gpoint currentCharBase;
    Gpoint currentCharUp;
    Gint currentTextColorIndex;
    Gint currentTextIndex;
    Gtxalign currentTextAlign;
    Gtxfp currentTextFontPrec;
    Gfloat currentCharExpandFactor;
    Gfloat currentCharSpacing;
    Gfloat currentCharWidth;
    // fill area
    Gflinter currentFillStyle;
    Gint currentFillStyleIndex;
    Gint currentFillColorIndex;
    Gint currentFillIndex;
    Gpoint currentPatWidthVec;
    Gpoint currentPatHeightVec;
    Gpoint currentPatternRef;
    // attribute source flags
    Gasfs asfs;
    // segments
    // input queue
    // current event report
};

static GGKSState g_initialGksState =
{
    0,
    {-1},
    0,
    {-1},
    0,
    0,
    {
        {
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f }
        }
    },
    GCLIP,
    // polyline
    GLN_SOLID,
    1,
    1,
    // polymarker
    GMK_STAR,
    1.0f,
    1,
    1,
    // text
    0.01f,
    { 1.0f, 0.0f },
    { 0.0f, 1.0f },
    1,
    1,
    { GAH_NORMAL, GAV_NORMAL },
    { 1, GP_STRING },
    1.0f,
    0.0f,
    0.01f,
    // fill area
    GHOLLOW,
    1,
    1,
    1,
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {0.0f, 0.0f},
    // aspect source flags
    {
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL,
        GINDIVIDUAL
    }
};

static GGKSState g_gksState;

struct GWSDesc
{
    // type
    // category

    // device coordinate units
    Gdspsize displaySpaceSize;

    // raster or vector display
    // dynamic modifications accepted for
    //-line rep
    //-marker rep
    //-text rep
    //-fill area rep
    //-pattern rep
    //-color rep
    //-workstation transformation
    // default deferral mode
    // default implicit regeneration mode

    // num line bundles
    // num marker bundles
    // num text bundles
    // num fill area bundles
    // num pattern bundles
    // num color bundles
    Gwstables tableSizes;

    // num available line types
    // list of available line types
    // num available line widths
    // nominal line width
    // minimum line width
    // maximum line width
    // num predefined line bundles
    Glnfac lineFacilities;

    // line bundles
    //  - line type
    //  - line width scale factor
    //  - line color index

    // num available marker types
    // list of available marker types
    // num available marker sizes
    // nominal marker size
    // minimum marker size
    // maximum marker size
    // num predefined marker bundles
    Gmkfac markerFacilities;

    // marker bundles
    //  - marker type
    //  - marker size scale factor
    //  - marker color index

    // num text font/prec pairs
    // list of text font/prec pairs
    // num available character expansion factors
    // minimum character expansion factor
    // maximum character expansion factor
    // num available character heights
    // minimum character height
    // maximum character height
    // num predefined text bundles
    Gtxfac textFacilities;

    // text bundles
    //  - text font/prec
    //  - character expansion factor
    //  - character spacing
    //  - text color index

    // num available fill area interior styles
    // list of available fill area interior styles
    // num available hatch styles
    // list of available hatch styles
    // num predefined fill area bundles
    Gint numAvailFillAreaIntStyles;
    const Gflinter *availFillAreaIntStyles;
    Gintlist availFillAreaHatchStyles;
    Gint numFillAreaBundles;

    // fill area bundle
    //  - fill area interior style
    //  - fill area style index
    //  - fill area color index
    // num predefined pattern representations
    // pattern representation
    //  - pattern array dimensions
    //  - pattern array
    // num available colors

    // color available
    // num predefined color represetnations
    // color representations
    Gcofac colorFacilities;

    //  - red
    //  - green
    //  - blue
    // num available gdp
    // gdp description:
    //  - gdp id
    //  - number of sets of attributes used
    //  - list of sets of attributes used
    // max num line bundle table entries
    // max num marker bundle table entries
    // max num text bundle table entries
    // max num fill area table entries
    // max num pattern indices
    // max num color indices
    // num of segment priorities supported
    // dynamic modification accepted for:
    //  - segment transformation
    //  - visibility (visible -> invisible)
    //  - visibility (invisible -> visible)
    //  - highlighting
    //  - segment priority
    //  - adding primitives to open segment overlapping segment w/higher priority
    //  - delete segment
    // LOCATOR
    // STROKE
    // VALUATOR
    // CHOICE
    // PICK
    // STRING
};

static const Gflinter g_availFillAreaIntStyles[] =
{
    GHOLLOW
};

static Gint g_availLineTypes[] =
{
    GLN_SOLID
};

static Gint g_availMarkerTypes[] =
{
    GMK_DOT,
    GMK_PLUS,
    GMK_STAR,
    GMK_CIRCLE,
    GMK_DIAGONAL_CROSS
};

static Gtxfp g_availTextFontPrecs[] =
{
    {1, GP_STRING}
};

static const GWSDesc g_wsDesc[MAX_WS_TYPES] =
{
    // Tektronix 4105
    {
        // Gdspsize display space size
        {
            GDC_OTHER,
            {1.0f, 1.0f},
            {640, 480}
        },
        // Gwstables bundle table sizes
        {
            0,      // line
            0,      // marker
            0,      // text
            0,      // fill area
            0,      // pattern
            16      // color
        },
        // Glnfac line facilities
        {
            {numOf(g_availLineTypes), g_availLineTypes},
            1,
            1.0f,
            1.0f,
            1.0f,
            1
        },
        // Gmkfac marker facilities
        {
            {numOf(g_availMarkerTypes), g_availMarkerTypes},
            1,
            1.0f,
            1.0f,
            1.0f,
            1
        },
        // Gtxfac text facilities
        {
            numOf(g_availTextFontPrecs),
            g_availTextFontPrecs,
            1,
            1.0f,
            1.0f,
            1,
            1.0f,
            1.0f,
            1
        },

        // num available fill area interior styles, fill area style indices
        numOf(g_availFillAreaIntStyles), g_availFillAreaIntStyles,
        // available fill area hatch styles
        { 0, nullptr },
        // num fill area bundles
        1,

        // Gcofac color facilities
        {16, GCOLOR, 16}
    }
};

struct GWSState
{
    Gint id;
    const Gconn *connId;
    Gwstype type;
    //- category OUTPUT, OUTIN, MO, WISS
    // workstation state
    // set of stored segments
    //- category OUTPUT, OUTIN, MO
    //-- polyline
    //-- polymarker
    //-- text
    //-- fill area
    //-- color table
    Gcobundl colorTable[MAX_NUM_COLORS];
    //-- deferral mode
    Gwsti transform;
    //- category INPUT, OUTIN
    //-- locator
    //-- stroke
    //-- valuator
    //-- choice
    //-- pick
    //-- string
};

static const GWSState g_initialWsState =
{
    0,
    NULL,
    0,
    {
        0
    },
    {
        GNOTPENDING,
        {
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
        },
        {
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
        }
    }
};

static GWSState g_wsState[1];

namespace
{

const std::function<GError()> noError = [] { return GERROR_NONE; };

template <typename T>
GError inquireGKSValue(T *dest, const T &source, GFunction fn, const std::function<GError()> &check = noError)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn, g_errFile);
        return GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    if (const GError error = check())
    {
        gerrorhand(error, fn, g_errFile);
        return error;
    }

    *dest = source;
    return GERROR_NONE;
}

template <typename T>
GError inquireGKSValue(const T &body, GFunction fn, const std::function<GError()> &check = noError)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn, g_errFile);
        return GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    if (const GError error = check())
    {
        gerrorhand(error, fn, g_errFile);
        return error;
    }

    body();
    return GERROR_NONE;
}

template <typename T>
Gint setGksValue(T &dest, T source, GFunction fn, const std::function<GError()> &check = noError)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn, g_errFile);
        return GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    if (const GError error = check())
    {
        gerrorhand(error, fn, g_errFile);
        return error;
    }

    dest = source;
    return GERROR_NONE;
}

template <typename T>
Gint setGksValue(T &dest, T *source, GFunction fn, const std::function<GError()> &check = noError)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn, g_errFile);
        return GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    if (const GError error = check())
    {
        gerrorhand(error, fn, g_errFile);
        return error;
    }

    dest = *source;
    return GERROR_NONE;
}

template <typename T>
Gint getWorkstationValue(T *dest, const T &source, GFunction fn, const std::function<GError()> &check = noError)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, fn, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }
    if (const GError error = check())
    {
        gerrorhand(error, fn, g_errFile);
        return error;
    }

    *dest = source;
    return GERROR_NONE;
}

template <typename T>
Gint setWorkstationValue(T &dest, const T *source, GFunction fn, const std::function<GError()> &check = noError)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, fn, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }
    if (const Gint error = check())
    {
        gerrorhand(error, fn, g_errFile);
        return error;
    }

    dest = *source;
    return GERROR_NONE;
}

inline bool wsTypeIsValid(Gwstype wsType)
{
    const Gwstype *const end = std::cend(g_wsTypes);
    return std::find(std::cbegin(g_wsTypes), end, wsType) != end;
}

inline GError checkWsType(Gwstype wsType)
{
    return wsTypeIsValid(wsType) ? GERROR_NONE : GERROR_INVALID_WSTYPE;
}

inline bool wsIsOpen(Gint wsId)
{
    const Gint *const end = std::cend(g_gksState.openWs);
    return std::find(std::cbegin(g_gksState.openWs), end, wsId) != end;
}

inline bool wsIsActive(Gint wsId)
{
    const Gint *const last = std::cend(g_gksState.activeWs);
    return std::find(std::cbegin(g_gksState.activeWs), last, wsId) != last;
}

inline bool rectIsValid(const Glimit *rect)
{
    return rect->xmin < rect->xmax
        && rect->ymin < rect->ymax;
}

inline bool rectIsWithinUnitSquare(const Glimit *rect)
{
    return rect->xmin >= 0.0f && rect->xmax <= 1.0f
        && rect->ymin >= 0.0f && rect->ymax <= 1.0f;
}

}

////////////////////////////////////////////////////////////////

Gint gerrorlog(Gint errNum, Gint funcName, Gfile *errFile)
{
    fprintf(errFile, "GKS error %d in function %d\n", errNum, funcName);
    return GERROR_NONE;
}

Gint gescape(Gint function, Gescin *indata, Gint bufsize, Gescout *outdata, Gint *escoutSize)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_ESCAPE, g_errFile);
        return GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    return GERROR_NONE;
}

////////////////////////////////////////////////////////////////

Gint gopengks(Gfile *errfile, Glong memory)
{
    // Validate errfile first before we use it to report state errors
    if (errfile == nullptr)
    {
        gerrorhand(GERROR_INVALID_ERROR_FILE, GFN_OPEN_GKS, stderr);
        return GERROR_INVALID_ERROR_FILE;
    }
    if (g_opState != GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS, errfile);
        return GERROR_NOT_STATE_GKCL;
    }

    g_errFile = errfile;
    g_opState = GGKOP;
    g_gksState = g_initialGksState;
    return GERROR_NONE;
}

Gint gclosegks()
{
    if (g_opState != GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS, g_errFile);
        return GERROR_NOT_STATE_GKOP;
    }

    g_opState = GGKCL;
    return GERROR_NONE;
}

////////////////////////////////////////////////////////////////

Gint ginqasf(Gasfs *value)
{
    return inquireGKSValue(value, g_gksState.asfs, GFN_INQUIRE_ASPECT_SOURCE_FLAGS);
}

Gint ginqavailwstypes(Gint bufSize, Gint start, Gintlist *wsTypes, Gint *numTypes)
{
    return inquireGKSValue(
        [=]
        {
            *numTypes = g_gksDescription.numWsTypes;
            wsTypes->number = *numTypes;
            std::copy_n(g_gksDescription.wsTypes, g_gksDescription.numWsTypes, wsTypes->integers);
        },
        GFN_INQUIRE_AVAILABLE_WORKSTATION_TYPES);
}

Gint ginqcharexpan(Gfloat *value)
{
    return inquireGKSValue(value, g_gksState.currentCharExpandFactor, GFN_INQUIRE_CHARACTER_EXPANSION_FACTOR);
}

Gint ginqcharbase(Gpoint *value)
{
    return inquireGKSValue(value, g_gksState.currentCharBase, GFN_INQUIRE_CHARACTER_BASE);
}

Gint ginqcharheight(Gfloat *value)
{
    return inquireGKSValue(value, g_gksState.currentCharHeight, GFN_INQUIRE_CHARACTER_HEIGHT);
}

Gint ginqcharspace(Gfloat *value)
{
    return inquireGKSValue(value, g_gksState.currentCharSpacing, GFN_INQUIRE_CHARACTER_SPACING);
}

Gint ginqcharwidth(Gfloat *value)
{
    return inquireGKSValue(value, g_gksState.currentCharWidth, GFN_INQUIRE_CHARACTER_WIDTH);
}

Gint ginqcharup(Gpoint *value)
{
    return inquireGKSValue(value, g_gksState.currentCharUp, GFN_INQUIRE_CHARACTER_UP_VECTOR);
}

Gint ginqclip(Gcliprect *value)
{
    return inquireGKSValue(
        [=]
        {
            value->ind = g_gksState.clipping;
            const Glimit identity = {0.0f, 1.0f, 0.0f, 1.0f};
            value->rec = identity;
        },
        GFN_INQUIRE_CLIPPING);
}

Gint ginqcurrntrannum(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentTransform, GFN_INQUIRE_CURRENT_NORMALIZATION_TRANSFORMATION_NUMBER);
}

Gint ginqfillcolorind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentFillColorIndex, GFN_INQUIRE_FILL_COLOR_INDEX);
}

Gint ginqfillind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentFillIndex, GFN_INQUIRE_FILL_INDEX);
}

Gint ginqfillstyle(Gflinter *value)
{
    return inquireGKSValue(value, g_gksState.currentFillStyle, GFN_INQUIRE_FILL_STYLE);
}

Gint ginqfillstyleind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentFillStyleIndex, GFN_INQUIRE_FILL_STYLE_INDEX);
}

Gint ginqlevelgks(Glevel *value)
{
    return inquireGKSValue(value, g_gksDescription.level, GFN_INQUIRE_LEVEL_OF_GKS);
}

Gint ginqlinecolorind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentLineColorIndex, GFN_INQUIRE_LINE_COLOR_INDEX);
}

Gint ginqlineind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentLineIndex, GFN_INQUIRE_LINE_INDEX);
}

Gint ginqlinetype(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentLineType, GFN_INQUIRE_LINE_TYPE);
}

Gint ginqmarkercolorind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentMarkerColorIndex, GFN_INQUIRE_MARKER_COLOR_INDEX);
}

Gint ginqmarkerind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentMarkerIndex, GFN_INQUIRE_MARKER_INDEX);
}

Gint ginqmarkersize(Gfloat *value)
{
    return inquireGKSValue(value, g_gksState.currentMarkerSize, GFN_INQUIRE_MARKER_SIZE);
}

Gint ginqmarkertype(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentMarkerType, GFN_INQUIRE_MARKER_TYPE);
}

Gint ginqmaxntrannum(Gint *value)
{
    return inquireGKSValue(value, g_gksDescription.numTrans, GFN_INQUIRE_MAXIMUM_NORMALIZATION_TRANSFORMATION_NUMBER);
}

Gint ginqntran(Gint num, Gtran *value)
{
    return inquireGKSValue(value, g_gksState.transforms[num - 1], GFN_INQUIRE_NORMALIZATION_TRANSFORMATION);
}

Gint ginqopenws(Gint maxIds, Gint start, Gintlist *wsids, Gint *actualIds)
{
    return inquireGKSValue([=] { *actualIds = 0; }, GFN_INQUIRE_SET_OF_OPEN_WORKSTATION);
}

Gint ginqopst(Gopst *value)
{
    *value = g_opState;
    return GERROR_NONE;
}

Gint ginqpatheight(Gpoint *value)
{
    return inquireGKSValue(value, g_gksState.currentPatHeightVec, GFN_INQUIRE_PATTERN_HEGIHT);
}

Gint ginqpatrefpt(Gpoint *value)
{
    return inquireGKSValue(value, g_gksState.currentPatternRef, GFN_INQUIRE_PATTERN_REFERENCE_POINT);
}

Gint ginqpatwidth(Gpoint *value)
{
    return inquireGKSValue(value, g_gksState.currentPatWidthVec, GFN_INQUIRE_PATTERN_WIDTH);
}

Gint ginqtextalign(Gtxalign *value)
{
    return inquireGKSValue(value, g_gksState.currentTextAlign, GFN_INQUIRE_TEXT_ALIGNMENT);
}

Gint ginqtextcolorind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentTextColorIndex, GFN_INQUIRE_TEXT_COLOR_INDEX);
}

Gint ginqtextfontprec(Gtxfp *value)
{
    return inquireGKSValue(value, g_gksState.currentTextFontPrec, GFN_INQUIRE_TEXT_FONT_PRECISION);
}

Gint ginqtextind(Gint *value)
{
    return inquireGKSValue(value, g_gksState.currentTextIndex, GFN_INQUIRE_TEXT_INDEX);
}

Gint ginqwsmaxnum(Gwsmax *value)
{
    return inquireGKSValue(value, g_gksDescription.wsmax, GFN_INQUIRE_WORKSTATION_MAXIMUM_NUMBERS);
}

////////////////////////////////////////////////////////////////

Gint ginqcolorfacil(Gwstype wsType, Gint buffSize, Gint *numColorIndices, Gcofac *value)
{
    return inquireGKSValue(
        [wsType, numColorIndices, value]
        {
            *numColorIndices = g_wsDesc[wsType - 1].colorFacilities.colors;
            *value = g_wsDesc[wsType - 1].colorFacilities;
        },
        GFN_INQUIRE_COLOR_FACILITIES, [wsType] { return checkWsType(wsType); });
}

Gint ginqdisplaysize(Gwstype wsType, Gdspsize *value)
{
    return inquireGKSValue(value, g_wsDesc[wsType - 1].displaySpaceSize, GFN_INQUIRE_DISPLAY_SPACE_SIZE,
        [wsType] { return checkWsType(wsType); });
}

Gint ginqfillfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gflfac *value)
{
    return inquireGKSValue(
        [wsType, facilSize, value]
        {
            const GWSDesc *desc = &g_wsDesc[wsType - 1];
            value->n_interiors = desc->numAvailFillAreaIntStyles;
            std::copy_n(desc->availFillAreaIntStyles, desc->numAvailFillAreaIntStyles, value->interiors);
            *facilSize = desc->availFillAreaHatchStyles.number;
            std::copy_n(desc->availFillAreaHatchStyles.integers, desc->availFillAreaHatchStyles.number, value->hatches);
            value->predefined = desc->numFillAreaBundles;
        },
        GFN_INQUIRE_FILL_FACILITIES, [wsType] { return checkWsType(wsType); });
}

Gint ginqlinefacil(Gwstype wsType, Gint buffSize, Gint *numLineTypes, Glnfac *value)
{
    return inquireGKSValue(
        [wsType, buffSize, numLineTypes, value]
        {
            const Glnfac &facil = g_wsDesc[wsType - 1].lineFacilities;
            *numLineTypes = facil.types.number;
            const int numToCopy = std::min(buffSize, facil.types.number);
            std::copy_n(facil.types.integers, numToCopy, value->types.integers);
            value->widths = facil.widths;
            value->nom_width = facil.nom_width;
            value->min_width = facil.min_width;
            value->max_width = facil.max_width;
            value->predefined = facil.predefined;
        },
        GFN_INQUIRE_LINE_FACILITIES, [wsType] { return checkWsType(wsType); });
}

Gint ginqmarkerfacil(Gwstype wsType, Gint buffSize, Gint *numMarkerTypes, Gmkfac *value)
{
    return inquireGKSValue(
        [wsType, buffSize, numMarkerTypes, value]
        {
            const GWSDesc &desc = g_wsDesc[wsType - 1];
            const Gmkfac &facil = desc.markerFacilities;
            *numMarkerTypes = facil.types.number;
            const int numToCopy = std::min(buffSize, facil.types.number);
            value->types.number = numToCopy;
            std::copy_n(facil.types.integers, numToCopy, value->types.integers);
            value->sizes = facil.sizes;
            value->nom_size = facil.nom_size;
            value->min_size = facil.min_size;
            value->max_size = facil.max_size;
            value->predefined = facil.predefined;
        },
        GFN_INQUIRE_MARKER_FACILITIES, [wsType] { return checkWsType(wsType); });
}

Gint ginqtextfacil(Gwstype wsType, Gint buffSize, Gint *numFontPrecs, Gtxfac *value)
{
    return inquireGKSValue(
        [wsType, buffSize, numFontPrecs, value]
        {
            const GWSDesc &desc = g_wsDesc[wsType - 1];
            const Gtxfac &facil = desc.textFacilities;
            *numFontPrecs = facil.fps;
            const int numToCopy = std::min(buffSize, facil.fps);
            value->fps = numToCopy;
            std::copy_n(facil.fp_list, numToCopy, value->fp_list);
            value->heights = facil.heights;
            value->min_ht = facil.min_ht;
            value->max_ht = facil.max_ht;
            value->expansions = facil.expansions;
            value->min_exp = facil.min_exp;
            value->max_exp = facil.max_exp;
            value->predefined = facil.predefined;
        },
        GFN_INQUIRE_TEXT_FACILITIES, [wsType] { return checkWsType(wsType); });
}

Gint ginqmaxwssttables(Gwstype wsType, Gwstables *value)
{
    return inquireGKSValue(value, g_wsDesc[wsType - 1].tableSizes,
        GFN_INQUIRE_MAXIMUM_LENGTH_OF_WORKSTATION_STATE_TABLES, [wsType] { return checkWsType(wsType); });
}

////////////////////////////////////////////////////////////////

Gint gselntran(Gint value)
{
    return setGksValue(g_gksState.currentTransform, value, GFN_SELECT_NORMALIZATION_TRANSFORMATION,
        [value] { return value < 0 || value > g_gksDescription.numTrans ? GERROR_INVALID_TRAN_NUM : GERROR_NONE; });
}

Gint gsetasf(Gasfs *value)
{
    return setGksValue(g_gksState.asfs, value, GFN_SET_ASPECT_SOURCE_FLAGS);
}

Gint gsetcharexpan(Gfloat value)
{
    return setGksValue(g_gksState.currentCharExpandFactor, value, GFN_SET_CHARACTER_EXPANSION_FACTOR,
        [value] { return value <= 0.0f ? GERROR_CHAR_EXPANSION_NOT_POSITIVE : GERROR_NONE; });
}

Gint gsetcharheight(Gfloat value)
{
    return setGksValue(g_gksState.currentCharHeight, value, GFN_SET_CHARACTER_HEIGHT,
        [value] { return value <= 0.0f ? GERROR_CHAR_HEIGHT_NOT_POSITIVE : GERROR_NONE; });
}

Gint gsetcharspace(Gfloat value)
{
    return setGksValue(g_gksState.currentCharSpacing, value, GFN_SET_CHARACTER_SPACING);
}

Gint gsetcharup(Gpoint *value)
{
    return setGksValue(g_gksState.currentCharUp, value, GFN_SET_CHARACTER_UP_VECTOR,
        [value] { return value->x == 0.0f && value->y == 0.0f ? GERROR_CHAR_UP_LENGTH_ZERO : GERROR_NONE; });
}

Gint gsetclip(Gclip value)
{
    return setGksValue(g_gksState.clipping, value, GFN_SET_CLIPPING_INDICATOR);
}

Gint gsetfillcolorind(Gint value)
{
    return setGksValue(g_gksState.currentFillColorIndex, value, GFN_SET_FILL_AREA_COLOR_INDEX,
        [value] { return value < 0 || value > 1 ? GERROR_INVALID_COLOR_INDEX : GERROR_NONE; });
}

Gint gsetfillind(Gint value)
{
    return setGksValue(g_gksState.currentFillIndex, value, GFN_SET_FILL_AREA_INDEX,
        [value] { return value < 1 || value > 1 ? GERROR_INVALID_FILL_INDEX : GERROR_NONE; });
}

Gint gsetfillstyle(Gflinter value)
{
    return setGksValue(g_gksState.currentFillStyle, value, GFN_SET_FILL_AREA_INTERIOR_STYLE);
}

Gint gsetfillstyleind(Gint value)
{
    return setGksValue(g_gksState.currentFillStyleIndex, value, GFN_SET_FILL_AREA_INTERIOR_STYLE,
        [value] { return value == 0 ? GERROR_FILL_STYLE_ZERO : GERROR_NONE; });
}

Gint gsetlinecolorind(Gint value)
{
    return setGksValue(g_gksState.currentLineColorIndex, value, GFN_SET_POLYLINE_COLOR_INDEX,
        [value] { return value < 0 || value > 1 ? GERROR_INVALID_COLOR_INDEX : GERROR_NONE; });
}

Gint gsetlineind(Gint value)
{
    return setGksValue(g_gksState.currentLineIndex, value, GFN_SET_POLYLINE_INDEX,
        [value] { return value < 1 || value > 1 ? GERROR_INVALID_LINE_INDEX : GERROR_NONE; });
}

Gint gsetlinetype(Gint value)
{
    return setGksValue(g_gksState.currentLineType, value, GFN_SET_LINETYPE,
        [value] { return value == 0 ? GERROR_LINE_TYPE_ZERO : GERROR_NONE; });
}

Gint gsetmarkercolorind(Gint value)
{
    return setGksValue(g_gksState.currentMarkerColorIndex, value, GFN_SET_POLYMARKER_COLOR_INDEX,
        [value] { return value < 0 || value > 1 ? GERROR_INVALID_COLOR_INDEX : GERROR_NONE; });
}

Gint gsetmarkerind(Gint value)
{
    return setGksValue(g_gksState.currentMarkerIndex, value, GFN_SET_POLYMARKER_INDEX,
        [value] { return value < 1 || value > 1 ? GERROR_INVALID_MARKER_INDEX : GERROR_NONE; });
}

Gint gsetmarkersize(Gfloat value)
{
    return setGksValue(g_gksState.currentMarkerSize, value, GFN_SET_MARKER_SIZE_SCALE_FACTOR,
        [value] { return value < 0.0f ? GERROR_NEGATIVE_MARKER_SIZE : GERROR_NONE; });
}

Gint gsetmarkertype(Gint value)
{
    return setGksValue(g_gksState.currentMarkerType, value, GFN_SET_MARKER_TYPE,
        [value] { return value == 0 ? GERROR_MARKER_TYPE_ZERO : GERROR_NONE; });
}

Gint gsettextalign(Gtxalign *value)
{
    return setGksValue(g_gksState.currentTextAlign, value, GFN_SET_TEXT_ALIGNMENT);
}

Gint gsettextcolorind(Gint value)
{
    return setGksValue(g_gksState.currentTextColorIndex, value, GFN_SET_TEXT_COLOR_INDEX,
        [value] { return value < 0 || value > 1 ? GERROR_INVALID_COLOR_INDEX : GERROR_NONE; });
}

Gint gsettextfontprec(Gtxfp *fontPrec)
{
    return setGksValue(g_gksState.currentTextFontPrec, fontPrec, GFN_SET_TEXT_FONT_AND_PRECISION,
        [fontPrec] { return fontPrec->font == 0 ? GERROR_TEXT_FONT_ZERO : GERROR_NONE; });
}

Gint gsettextind(Gint value)
{
    return setGksValue(g_gksState.currentTextIndex, value, GFN_SET_TEXT_INDEX,
        [value] { return value < 1 || value > 1 ? GERROR_INVALID_TEXT_INDEX : GERROR_NONE; });
}

Gint gsetviewport(Gint transform, Glimit *value)
{
    return setGksValue(g_gksState.transforms[transform - 1].v, value, GFN_SET_VIEWPORT,
        [transform, value]
        {
            if (transform < 1)
            {
                return GERROR_INVALID_TRAN_NUM;
            }
            if (!rectIsValid(value))
            {
                return GERROR_INVALID_RECT;
            }
            if (!rectIsWithinUnitSquare(value))
            {
                return GERROR_VIEWPORT_NOT_IN_NDC;
            }
            return GERROR_NONE;
        });
}

Gint gsetwindow(Gint tranId, Glimit *value)
{
    return setGksValue(g_gksState.transforms[tranId-1].w, value, GFN_SET_WINDOW,
        [tranId, value] {
            if (tranId < 1)
            {
                return GERROR_INVALID_TRAN_NUM;
            }
            if (!rectIsValid(value))
            {
                return GERROR_INVALID_RECT;
            }
            return GERROR_NONE;
        });
}

////////////////////////////////////////////////////////////////

Gint gopenws(Gint wsId, const Gconn *connId, Gwstype wsType)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_OPEN_WORKSTATION, g_errFile);
        return GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    if (!wsTypeIsValid(wsType))
    {
        gerrorhand(GERROR_INVALID_WSTYPE, GFN_OPEN_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSTYPE;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_OPEN_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSID;
    }
    if (wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_IS_OPEN, GFN_OPEN_WORKSTATION, g_errFile);
        return GERROR_WS_IS_OPEN;
    }

    g_opState = GWSOP;
    const int i = g_gksState.numOpenWs;
    g_gksState.openWs[i] = wsId;
    g_wsState[i] = g_initialWsState;
    g_wsState[i].id = wsId;
    g_wsState[i].connId = connId;
    g_wsState[i].type = wsType;
    ++g_gksState.numOpenWs;
    return GERROR_NONE;
}

Gint gclosews(Gint wsId)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_CLOSE_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSID;
    }
    if (wsIsActive(wsId))
    {
        gerrorhand(GERROR_WS_IS_ACTIVE, GFN_CLOSE_WORKSTATION, g_errFile);
        return GERROR_WS_IS_ACTIVE;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_CLOSE_WORKSTATION, g_errFile);
        return GERROR_WS_NOT_OPEN;
    }

    std::stable_partition(&g_gksState.openWs[0], &g_gksState.openWs[g_gksState.numOpenWs],
        [wsId](Gint value) { return value != wsId; });
    --g_gksState.numOpenWs;
    g_gksState.openWs[g_gksState.numOpenWs] = -1;
    if (g_gksState.numOpenWs == 0)
    {
        g_opState = GGKOP;
    }
    return GERROR_NONE;
}

////////////////////////////////////////////////////////////////

Gint gactivatews(Gint wsId)
{
    if (g_opState != GWSOP && g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSID;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return GERROR_WS_NOT_OPEN;
    }
    if (wsIsActive(wsId))
    {
        gerrorhand(GERROR_WS_IS_ACTIVE, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return GERROR_WS_IS_ACTIVE;
    }

    g_gksState.activeWs[g_gksState.numActiveWs] = wsId;
    ++g_gksState.numActiveWs;
    g_opState = GWSAC;
    return GERROR_NONE;
}

Gint gclearws(Gint wsId, Gclrflag flag)
{
    if (g_opState != GWSOP && g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_CLEAR_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSID;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_CLEAR_WORKSTATION, g_errFile);
        return GERROR_WS_NOT_OPEN;
    }
    return GERROR_NONE;
}

Gint gdeactivatews(Gint wsId)
{
    if (g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION, g_errFile);
        return GERROR_NOT_STATE_WSAC;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_DEACTIVATE_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSID;
    }
    if (!wsIsActive(wsId))
    {
        gerrorhand(GERROR_WS_NOT_ACTIVE, GFN_DEACTIVATE_WORKSTATION, g_errFile);
        return GERROR_WS_NOT_ACTIVE;
    }

    --g_gksState.numActiveWs;
    g_gksState.activeWs[g_gksState.numActiveWs] = -1;
    if (g_gksState.numActiveWs == 0)
    {
        g_opState = GWSOP;
    }
    return GERROR_NONE;
}

Gint gupdatews(Gint wsId, Gregen flag)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_UPDATE_WORKSTATION, g_errFile);
        return GERROR_INVALID_WSID;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_UPDATE_WORKSTATION, g_errFile);
        return GERROR_WS_NOT_OPEN;
    }
    return GERROR_NONE;
}

////////////////////////////////////////////////////////////////

Gint ginqcolorindices(Gint wsId, Gint sizeIndices, Gint start, Gintlist *indices, Gint *numIndices)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_COLOR_INDICES, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_INQUIRE_COLOR_INDICES, g_errFile);
        return GERROR_WS_NOT_OPEN;
    }

    *numIndices = 2;
    std::iota(indices->integers, indices->integers + *numIndices, 0);
    return GERROR_NONE;
}

Gint ginqcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    return getWorkstationValue(value, g_wsState[0].colorTable[index], GFN_INQUIRE_COLOR_REPRESENTATION,
        [wsId, index]
        {
            if (!wsIsOpen(wsId))
            {
                gerrorhand(GERROR_WS_NOT_OPEN, GFN_INQUIRE_COLOR_REPRESENTATION, g_errFile);
                return GERROR_WS_NOT_OPEN;
            }
            if (index < 0 || index > 1)
            {
                gerrorhand(GERROR_INVALID_COLOR_INDEX, GFN_INQUIRE_COLOR_REPRESENTATION, g_errFile);
                return GERROR_INVALID_COLOR_INDEX;
            }
            return GERROR_NONE;
        });
}

Gint ginqtextextent(Gint wsId, Gpoint *pos, const Gchar *text, Gextent *value)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_TEXT_EXTENT, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }

    return GERROR_NONE;
}

Gint ginqwsconntype(Gint wsId, Gint buffSize, Gint *ctSize, Gwsct *ct)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_WORKSTATION_CONNECTION_AND_TYPE, g_errFile);
        return GERROR_NOT_STATE_WSOP_WSAC_SGOP;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_INQUIRE_WORKSTATION_CONNECTION_AND_TYPE, g_errFile);
        return GERROR_WS_NOT_OPEN;
    }

    *ctSize = 1;
    ct->conn = g_wsState[0].connId;
    ct->type = g_wsState[0].type;
    return GERROR_NONE;
}

Gint ginqwstran(Gint wsId, Gwsti *value)
{
    return getWorkstationValue(value, g_wsState[wsId].transform, GFN_INQUIRE_WORKSTATION_TRANSFORMATION,
        [wsId] { return wsIsOpen(wsId) ? GERROR_NONE : GERROR_WS_NOT_OPEN; });
}

////////////////////////////////////////////////////////////////

Gint gsetcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    return setWorkstationValue(g_wsState[wsId].colorTable[index], value, GFN_SET_COLOR_REPRESENTATION,
        [wsId, index]
        {
            if (!wsIsOpen(wsId))
            {
                return GERROR_WS_NOT_OPEN;
            }
            if (index < 0 || index > 1)
            {
                return GERROR_INVALID_COLOR_INDEX;
            }
            return GERROR_NONE;
        });
}

Gint gsetwsviewport(Gint wsId, Glimit *value)
{
    return setWorkstationValue(g_wsState[0].transform.current.v, value, GFN_SET_WORKSTATION_VIEWPORT,
        [wsId, value]
        {
            if (!wsIsOpen(wsId))
            {
                return GERROR_WS_NOT_OPEN;
            }
            if (!rectIsValid(value))
            {
                return GERROR_INVALID_RECT;
            }
            const Gpoint &size = g_wsDesc[0].displaySpaceSize.device;
            if (value->xmin < 0 || value->xmin >= size.x || value->xmax < 0 || value->xmax >= size.x ||
                value->ymin < 0 || value->ymin >= size.y || value->ymax < 0 || value->ymax >= size.y)
            {
                return GERROR_WS_VIEWPORT_NOT_IN_DISPLAY;
            }
            return GERROR_NONE;
        });
}

Gint gsetwswindow(Gint wsId, Glimit *value)
{
    return setWorkstationValue(g_wsState[0].transform.current.w, value, GFN_SET_WORKSTATION_WINDOW,
        [wsId, value]{
            if (!wsIsOpen(wsId))
            {
                return GERROR_WS_NOT_OPEN;
            }
            if (!rectIsValid(value))
            {
                return GERROR_INVALID_RECT;
            }
            return GERROR_NONE;
        });
}

////////////////////////////////////////////////////////////////

Gint gcellarray(Grect *rect, Gidim *dims, Gint *colors)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_CELL_ARRAY, g_errFile);
        return GERROR_NOT_STATE_WSAC_SGOP;
    }
    if (dims->x_dim < 1 || dims->y_dim < 1)
    {
        gerrorhand(GERROR_INVALID_COLOR_DIMENSIONS, GFN_CELL_ARRAY, g_errFile);
        return GERROR_INVALID_COLOR_DIMENSIONS;
    }
    return GERROR_NONE;
}

Gint gfillarea(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA, g_errFile);
        return GERROR_NOT_STATE_WSAC_SGOP;
    }
    if (numPoints < 3)
    {
        gerrorhand(GERROR_INVALID_NUM_POINTS, GFN_FILL_AREA, g_errFile);
        return GERROR_INVALID_NUM_POINTS;
    }
    return GERROR_NONE;
}

Gint ggdp(Gint numPoints, Gpoint *points, Gint gdpId, Ggdprec *data)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP, g_errFile);
        return GERROR_NOT_STATE_WSAC_SGOP;
    }
    return GERROR_NONE;
}

Gint gpolyline(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE, g_errFile);
        return GERROR_NOT_STATE_WSAC_SGOP;
    }
    if (numPoints < 2)
    {
        gerrorhand(GERROR_INVALID_NUM_POINTS, GFN_POLYLINE, g_errFile);
        return GERROR_INVALID_NUM_POINTS;
    }
    return GERROR_NONE;
}

Gint gpolymarker(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER, g_errFile);
        return GERROR_NOT_STATE_WSAC_SGOP;
    }
    if (numPoints < 1)
    {
        gerrorhand(GERROR_INVALID_NUM_POINTS, GFN_POLYMARKER, g_errFile);
        return GERROR_INVALID_NUM_POINTS;
    }
    return GERROR_NONE;
}

Gint gtext(Gpoint *start, const Gchar *text)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT, g_errFile);
        return GERROR_NOT_STATE_WSAC_SGOP;
    }
    return GERROR_NONE;
}
