#include <gks/gks.h>
#include <gks/gkserror.h>

#include <algorithm>
#include <cstring>

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
    Gpoint currentPatternSize;
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
    {1.0f, 1.0f},
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

    // num available line types
    // list of available line types
    // num available line widths
    // nominal line width
    // minimum line width
    // maximum line width
    // num line bundles
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

static const GWSDesc g_wsDesc[MAX_WS_TYPES] =
{
    // Tektronix 4105
    {
        {
            GDC_OTHER,
            { 0.0f, 1.0f },
            { 640, 480 }
        },
        {
            { numOf(g_availLineTypes), g_availLineTypes },
            1,
            1.0f,
            1.0f,
            1.0f,
            1
        },
        numOf(g_availFillAreaIntStyles), g_availFillAreaIntStyles,
        { 0, NULL },
        1,
        { 16, GCOLOR, 16 }
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

template <typename T>
void inquireGKSValue(T *dest, const T &source, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *dest = source;
    *errorStatus = GERROR_NONE;
}

template <typename T>
void inquireGKSValue(Gint *errorStatus, const T &body)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    body();
    *errorStatus = GERROR_NONE;
}

template <typename T>
void setGksValue(T &dest, T source, GFunction fn)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn, g_errFile);
        return;
    }

    dest = source;
}

template <typename T>
void setGksValue(T &dest, T *source, GFunction fn)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn, g_errFile);
        return;
    }

    dest = *source;
}

template <typename T>
void getWorkstationValue(T *dest, const T &source, Gint *errorStatus)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        *errorStatus = GERROR_NOT_STATE_WSOP_WSAC_SGOP;
        return;
    }

    *dest = source;
    *errorStatus = GERROR_NONE;
}

template <typename T>
void setWorkstationValue(T &dest, const T *source, GFunction fn)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, fn, g_errFile);
        return;
    }

    dest = *source;
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

}

void gerrorlog(Gint errNum, Gint funcName, Gfile *errFile)
{
    fprintf(errFile, "GKS error %d in function %d\n", errNum, funcName);
}

void gescape(Gint function, Gescin *indata, Gint bufsize, Gescout *outdata, Gint *escoutSize)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_ESCAPE, g_errFile);
        return;
    }
}

void gopengks(Gfile *errfile, Glong memory)
{
    if (errfile == nullptr)
    {
        gerrorhand(GERROR_INVALID_ERROR_FILE, GFN_OPEN_GKS, stderr);
        return;
    }
    if (g_opState != GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS, errfile);
        return;
    }
    g_errFile = errfile;
    g_opState = GGKOP;
    g_gksState = g_initialGksState;
}

void gclosegks()
{
    if (g_opState != GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS, g_errFile);
        return;
    }

    g_opState = GGKCL;
}

void ginqasf(Gasfs *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.asfs, errorStatus);
}

void ginqavailwstypes(Gint bufSize, Gint start, Gintlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    inquireGKSValue(errorStatus, [=] {
            *numTypes = g_gksDescription.numWsTypes;
            wsTypes->number = *numTypes;
            for (int i = 0; i < g_gksDescription.numWsTypes; ++i)
            {
                wsTypes->integers[i] = g_gksDescription.wsTypes[i];
            }
        });
}

void ginqcharexpan(Gfloat *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentCharExpandFactor, errorStatus);
}

void ginqcharbase(Gpoint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentCharBase, errorStatus);
}

void ginqcharheight(Gfloat *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentCharHeight, errorStatus);
}

void ginqcharspace(Gfloat *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentCharSpacing, errorStatus);
}

void ginqcharwidth(Gfloat *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentCharWidth, errorStatus);
}

void ginqcharup(Gpoint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentCharUp, errorStatus);
}

void ginqclip(Gcliprect *value, Gint *errorStatus)
{
    inquireGKSValue(errorStatus, [=] {
            value->ind = g_gksState.clipping;
            const Glimit identity =
            {
                0.0f, 1.0f, 0.0f, 1.0f
            };
            value->rec = identity;
        });
}

void ginqcurrntrannum(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentTransform, errorStatus);
}

void ginqfillcolorind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentFillColorIndex, errorStatus);
}

void ginqfillind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentFillIndex, errorStatus);
}

void ginqfillstyle(Gflinter *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentFillStyle, errorStatus);
}

void ginqfillstyleind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentFillStyleIndex, errorStatus);
}

void ginqlevelgks(Glevel *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksDescription.level, errorStatus);
}

void ginqlinecolorind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentLineColorIndex, errorStatus);
}

void ginqlineind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentLineIndex, errorStatus);
}

void ginqlinetype(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentLineType, errorStatus);
}

void ginqmarkercolorind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentMarkerColorIndex, errorStatus);
}

void ginqmarkerind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentMarkerIndex, errorStatus);
}

void ginqmarkersize(Gfloat *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentMarkerSize, errorStatus);
}

void ginqmarkertype(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentMarkerType, errorStatus);
}

void ginqmaxntrannum(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksDescription.numTrans, errorStatus);
}

void ginqntran(Gint num, Gtran *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.transforms[num], errorStatus);
}

void ginqopenws(Gint maxIds, Gint start, Gintlist *wsids, Gint *actualIds, Gint *errorStatus)
{
    inquireGKSValue(errorStatus,
        [=] {
            *actualIds = 0;
        });
}

void ginqopst(Gopst *value)
{
    *value = g_opState;
}

void ginqpatrefpt(Gpoint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentPatternRef, errorStatus);
}

void ginqpatsize(Gpoint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentPatternSize, errorStatus);
}

void ginqtextalign(Gtxalign *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentTextAlign, errorStatus);
}

void ginqtextcolorind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentTextColorIndex, errorStatus);
}

void ginqtextfontprec(Gtxfp *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentTextFontPrec, errorStatus);
}

void ginqtextind(Gint *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksState.currentTextIndex, errorStatus);
}

void ginqwsmaxnum(Gwsmax *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_gksDescription.wsmax, errorStatus);
}

void ginqcolorfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gcofac *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_wsDesc[wsType-1].colorFacilities, errorStatus);
}

void ginqdisplaysize(Gwstype wsType, Gdspsize *value, Gint *errorStatus)
{
    inquireGKSValue(value, g_wsDesc[wsType-1].displaySpaceSize, errorStatus);
}

void ginqfillfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gflfac *value, Gint *errorStatus)
{
    inquireGKSValue(errorStatus,
        [=] {
            const GWSDesc *desc = &g_wsDesc[wsType - 1];
            value->n_interiors = desc->numAvailFillAreaIntStyles;
            for (int i = 0; i < desc->numAvailFillAreaIntStyles; ++i)
            {
                value->interiors[i] = desc->availFillAreaIntStyles[i];
            }
            *facilSize = desc->availFillAreaHatchStyles.number;
            for (int i = 0; i < desc->availFillAreaHatchStyles.number; ++i)
            {
                value->hatches[i] = desc->availFillAreaHatchStyles.integers[i];
            }
            value->predefined = desc->numFillAreaBundles;
        });
}

void ginqlinefacil(Gwstype wsType, Gint buffSize, Gint *numLineTypes, Glnfac *value, Gint *errorStatus)
{
    inquireGKSValue(errorStatus,
        [=] {
            const GWSDesc *desc = &g_wsDesc[wsType-1];
            *numLineTypes = desc->lineFacilities.types.number;
            for (int i = 0; i < desc->lineFacilities.types.number; ++i)
            {
                value->types.integers[i] = desc->lineFacilities.types.integers[i];
            }
            value->widths = desc->lineFacilities.widths;
            value->nom_width = desc->lineFacilities.nom_width;
            value->min_width = desc->lineFacilities.min_width;
            value->max_width = desc->lineFacilities.max_width;
            value->predefined = desc->lineFacilities.predefined;
        });
}

void gselntran(Gint value)
{
    if (value < 0 || value > g_gksDescription.numTrans)
    {
        gerrorhand(GERROR_INVALID_TRAN_NUM, GFN_SELECT_NORMALIZATION_TRANSFORMATION, g_errFile);
        return;
    }

    setGksValue(g_gksState.currentTransform, value, GFN_SELECT_NORMALIZATION_TRANSFORMATION);
}

void gsetasf(Gasfs *value)
{
    setGksValue(g_gksState.asfs, value, GFN_SET_ASPECT_SOURCE_FLAGS);
}

void gsetcharexpan(Gfloat value)
{
    if (value <= 0.0f)
    {
        gerrorhand(GERROR_CHAR_EXPANSION_NOT_POSITIVE, GFN_SET_CHARACTER_EXPANSION_FACTOR, g_errFile);
        return;
    }

    setGksValue(g_gksState.currentCharExpandFactor, value, GFN_SET_CHARACTER_EXPANSION_FACTOR);
}

void gsetcharheight(Gfloat value)
{
    setGksValue(g_gksState.currentCharHeight, value, GFN_SET_CHARACTER_HEIGHT);
}

void gsetcharspace(Gfloat value)
{
    setGksValue(g_gksState.currentCharSpacing, value, GFN_SET_CHARACTER_SPACING);
}

void gsetcharup(Gpoint *value)
{
    setGksValue(g_gksState.currentCharUp, value, GFN_SET_CHARACTER_UP_VECTOR);
}

void gsetclip(Gclip value)
{
    setGksValue(g_gksState.clipping, value, GFN_SET_CLIPPING_INDICATOR);
}

void gsetfillcolorind(Gint value)
{
    setGksValue(g_gksState.currentFillColorIndex, value, GFN_SET_FILL_AREA_COLOR_INDEX);
}

void gsetfillind(Gint value)
{
    setGksValue(g_gksState.currentFillIndex, value, GFN_SET_FILL_AREA_COLOR_INDEX);
}

void gsetfillstyle(Gflinter value)
{
    setGksValue(g_gksState.currentFillStyle, value, GFN_SET_FILL_AREA_INTERIOR_STYLE);
}

void gsetfillstyleind(Gint value)
{
    setGksValue(g_gksState.currentFillStyleIndex, value, GFN_SET_FILL_AREA_INTERIOR_STYLE);
}

void gsetlinecolorind(Gint value)
{
    setGksValue(g_gksState.currentLineColorIndex, value, GFN_SET_POLYLINE_COLOR_INDEX);
}

void gsetlineind(Gint value)
{
    if (value < 1)
    {
        gerrorhand(GERROR_INVALID_LINE_INDEX, GFN_SET_POLYLINE_INDEX, g_errFile);
    }

    setGksValue(g_gksState.currentLineIndex, value, GFN_SET_POLYLINE_INDEX);
}

void gsetlinetype(Gint value)
{
    if (value == 0)
    {
        gerrorhand(GERROR_LINE_TYPE_ZERO, GFN_SET_LINETYPE, g_errFile);
        return;
    }

    setGksValue(g_gksState.currentLineType, value, GFN_SET_LINETYPE);
}

void gsetmarkercolorind(Gint value)
{
    setGksValue(g_gksState.currentMarkerColorIndex, value, GFN_SET_POLYMARKER_COLOR_INDEX);
}

void gsetmarkerind(Gint value)
{
    setGksValue(g_gksState.currentMarkerIndex, value, GFN_SET_POLYMARKER_INDEX);
}

void gsetmarkersize(Gfloat value)
{
    setGksValue(g_gksState.currentMarkerSize, value, GFN_SET_MARKER_SIZE_SCALE_FACTOR);
}

void gsetmarkertype(Gint value)
{
    setGksValue(g_gksState.currentMarkerType, value, GFN_SET_MARKER_TYPE);
}

void gsettextalign(Gtxalign *value)
{
    setGksValue(g_gksState.currentTextAlign, value, GFN_SET_TEXT_ALIGNMENT);
}

void gsettextcolorind(Gint value)
{
    setGksValue(g_gksState.currentTextColorIndex, value, GFN_SET_TEXT_COLOR_INDEX);
}

void gsettextfontprec(Gtxfp *fontPrec)
{
    setGksValue(g_gksState.currentTextFontPrec, fontPrec, GFN_SET_TEXT_FONT_AND_PRECISION);
}

void gsettextind(Gint value)
{
    setGksValue(g_gksState.currentTextIndex, value, GFN_SET_TEXT_INDEX);
}

void gsetviewport(Gint transform, Glimit *value)
{
    setGksValue(g_gksState.transforms[transform].v, value, GFN_SET_VIEWPORT);
}

void gsetwindow(Gint transform, Glimit *value)
{
    setGksValue(g_gksState.transforms[transform].w, value, GFN_SET_WINDOW);
}

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_OPEN_WORKSTATION, g_errFile);
        return;
    }
    {
        const Gwstype *const end = &g_gksDescription.wsTypes[g_gksDescription.numWsTypes];
        if (std::find(&g_gksDescription.wsTypes[0], end, wsType) == end)
        {
            gerrorhand(GERROR_INVALID_WSTYPE, GFN_OPEN_WORKSTATION, g_errFile);
            return;
        }
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_OPEN_WORKSTATION, g_errFile);
        return;
    }
    if (wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_IS_OPEN, GFN_OPEN_WORKSTATION, g_errFile);
        return;
    }

    g_opState = GWSOP;
    const int i = g_gksState.numOpenWs;
    g_gksState.openWs[i] = wsId;
    g_wsState[i] = g_initialWsState;
    g_wsState[i].id = wsId;
    g_wsState[i].connId = connId;
    g_wsState[i].type = wsType;
    ++g_gksState.numOpenWs;
}

void gclosews(Gint wsId)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION, g_errFile);
        return;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_CLOSE_WORKSTATION, g_errFile);
        return;
    }
    if (wsIsActive(wsId))
    {
        gerrorhand(GERROR_WS_IS_ACTIVE, GFN_CLOSE_WORKSTATION, g_errFile);
        return;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_CLOSE_WORKSTATION, g_errFile);
        return;
    }

    std::stable_partition(&g_gksState.openWs[0], &g_gksState.openWs[g_gksState.numOpenWs],
        [wsId](Gint value) { return value != wsId; });
    --g_gksState.numOpenWs;
    g_gksState.openWs[g_gksState.numOpenWs] = -1;
    if (g_gksState.numOpenWs == 0)
    {
        g_opState = GGKOP;
    }
}

void gactivatews(Gint wsId)
{
    if (g_opState != GWSOP && g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return;
    }
    if (wsIsActive(wsId))
    {
        gerrorhand(GERROR_WS_IS_ACTIVE, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return;
    }

    g_gksState.activeWs[g_gksState.numActiveWs] = wsId;
    ++g_gksState.numActiveWs;
    g_opState = GWSAC;
}

void gclearws(Gint wsId, Gclrflag flag)
{
    if (g_opState != GWSOP && g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION, g_errFile);
        return;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_CLEAR_WORKSTATION, g_errFile);
        return;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_CLEAR_WORKSTATION, g_errFile);
        return;
    }
}

void gdeactivatews(Gint wsId)
{
    if (g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION, g_errFile);
        return;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_DEACTIVATE_WORKSTATION, g_errFile);
        return;
    }
    if (!wsIsActive(wsId))
    {
        gerrorhand(GERROR_WS_NOT_ACTIVE, GFN_DEACTIVATE_WORKSTATION, g_errFile);
        return;
    }

    --g_gksState.numActiveWs;
    g_gksState.activeWs[g_gksState.numActiveWs] = -1;
    if (g_gksState.numActiveWs == 0)
    {
        g_opState = GWSOP;
    }
}

void gupdatews(Gint wsId, Gregen flag)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION, g_errFile);
        return;
    }
    if (wsId < 0 || wsId >= g_gksDescription.wsmax.open)
    {
        gerrorhand(GERROR_INVALID_WSID, GFN_UPDATE_WORKSTATION, g_errFile);
        return;
    }
    if (!wsIsOpen(wsId))
    {
        gerrorhand(GERROR_WS_NOT_OPEN, GFN_UPDATE_WORKSTATION, g_errFile);
        return;
    }
}

void ginqcolorrep(Gint wsId, Gint index, Gcobundl *value, Gint *errorStatus)
{
    getWorkstationValue(value, g_wsState[0].colorTable[index], errorStatus);
}

void ginqwstran(Gint wsId, Gwsti *value, Gint *errorStatus)
{
    getWorkstationValue(value, g_wsState[0].transform, errorStatus);
}

void gsetcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    setWorkstationValue(g_wsState[0].colorTable[index], value, GFN_SET_COLOR_REPRESENTATION);
}

void gsetwsviewport(Gint wsId, Glimit *value)
{
    setWorkstationValue(g_wsState[0].transform.current.v, value, GFN_SET_WORKSTATION_VIEWPORT);
}

void gsetwswindow(Gint wsId, Glimit *value)
{
    setWorkstationValue(g_wsState[0].transform.current.w, value, GFN_SET_WORKSTATION_WINDOW);
}

void gcellarray(Grect *rect, Gidim *dims, Gint *colors)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_CELL_ARRAY, g_errFile);
        return;
    }
    if (dims->x_dim < 1 || dims->y_dim < 1)
    {
        gerrorhand(GERROR_INVALID_COLOR_DIMENSIONS, GFN_CELL_ARRAY, g_errFile);
        return;
    }
}

void gfillarea(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA, g_errFile);
        return;
    }
    if (numPoints < 3)
    {
        gerrorhand(GERROR_INVALID_NUM_POINTS, GFN_FILL_AREA, g_errFile);
        return;
    }
}

void ggdp(Gint numPoints, Gpoint *points, Gint gdpId, Ggdprec *data)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP, g_errFile);
        return;
    }
}

void gpolyline(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE, g_errFile);
        return;
    }
    if (numPoints < 2)
    {
        gerrorhand(GERROR_INVALID_NUM_POINTS, GFN_POLYLINE, g_errFile);
        return;
    }
}

void gpolymarker(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER, g_errFile);
        return;
    }
    if (numPoints < 1)
    {
        gerrorhand(GERROR_INVALID_NUM_POINTS, GFN_POLYMARKER, g_errFile);
        return;
    }
}

void gtext(Gpoint *start, const Gchar *text)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT, g_errFile);
        return;
    }
}
