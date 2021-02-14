#include <gks/gks.h>
#include <gks/gkserror.h>

#include <string.h>

#define NUM_OF(ary_) (sizeof(ary_)/sizeof((ary_)[0]))

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

typedef struct GGKSDescription_t
{
    Glevel level;
    Gint numWsTypes;
    const Gwstype *wsTypes;
    Gwsmax wsmax;
    Gint numTrans;
} GGKSDescription;

static const Gwstype g_wsTypes[MAX_WS_TYPES] =
{
    GWSTYPE_TEK4105
};

static GGKSDescription g_gksDescription =
{
    GLMA,
    NUM_OF(g_wsTypes),
    g_wsTypes,
    { MAX_OPEN_WS, MAX_ACTIVE_WS, MAX_SEGMENT_ASSOC },
    MAX_NUM_TRANSFORMS
};

typedef struct GGKSState_t
{
    Gint openWs[MAX_OPEN_WS];
    Gint activeWs[MAX_ACTIVE_WS];
    Gint currentTransform;
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
} GGKSState;

static GGKSState g_initialGksState =
{
    {0},
    {0},
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

typedef struct GWSDesc_t
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
} GWSDesc;

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
            { NUM_OF(g_availLineTypes), g_availLineTypes },
            1,
            1.0f,
            1.0f,
            1.0f,
            1
        },
        NUM_OF(g_availFillAreaIntStyles), g_availFillAreaIntStyles,
        { 0, NULL },
        1,
        { 16, GCOLOR, 16 }
    }
};

typedef struct GWSState_t
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
} GWSState;

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
    if (g_opState != GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS, g_errFile);
        return;
    }

    g_errFile = errfile;
    g_opState = GGKOP;
    g_gksState = g_initialGksState;
}

void gclosegks(void)
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
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.asfs;
    *errorStatus = GERROR_NONE;
}

void ginqavailwstypes(Gint bufSize, Gint start, Gintlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *numTypes = g_gksDescription.numWsTypes;
    wsTypes->number = *numTypes;
    for (int i = 0; i < g_gksDescription.numWsTypes; ++i)
    {
        wsTypes->integers[i] = g_gksDescription.wsTypes[i];
    }
    *errorStatus = GERROR_NONE;
}

void ginqcharexpan(Gfloat *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentCharExpandFactor;
    *errorStatus = GERROR_NONE;
}

void ginqcharbase(Gpoint *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentCharBase;
    *errorStatus = GERROR_NONE;
}

void ginqcharheight(Gfloat *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentCharHeight;
    *errorStatus = GERROR_NONE;
}

void ginqcharspace(Gfloat *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentCharSpacing;
    *errorStatus = GERROR_NONE;
}

void ginqcharwidth(Gfloat *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentCharWidth;
    *errorStatus = GERROR_NONE;
}

void ginqcharup(Gpoint *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentCharUp;
    *errorStatus = GERROR_NONE;
}

void ginqclip(Gcliprect *value, Gint *errorStatus)
{
    value->ind = g_gksState.clipping;
    const Glimit identity =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };
    value->rec = identity;
    *errorStatus = GERROR_NONE;
}

void ginqcurrntrannum(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTransform;
    *errorStatus = GERROR_NONE;
}

void ginqcolorfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gcofac *value, Gint *errorStatus)
{
    *value = g_wsDesc[wsType-1].colorFacilities;
    *errorStatus = GERROR_NONE;
}

void ginqdisplaysize(Gwstype wsType, Gdspsize *value, Gint *errorStatus)
{
    *value = g_wsDesc[wsType-1].displaySpaceSize;
    *errorStatus = GERROR_NONE;
}

void ginqfillfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gflfac *value, Gint *errorStatus)
{
    const GWSDesc *desc = &g_wsDesc[wsType-1];
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
    *errorStatus = GERROR_NONE;
}

void ginqlinefacil(Gwstype wsType, Gint buffSize, Gint *numLineTypes, Glnfac *value, Gint *errorStatus)
{
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
    *errorStatus = GERROR_NONE;
}

void gselntran(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SELECT_NORMALIZATION_TRANSFORMATION, g_errFile);
        return;
    }

    g_gksState.currentTransform = value;
}

void ginqfillcolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillColorIndex;
    *errorStatus = GERROR_NONE;
}

void ginqfillind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillIndex;
    *errorStatus = GERROR_NONE;
}

void ginqfillstyle(Gflinter *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillStyle;
    *errorStatus = GERROR_NONE;
}

void ginqfillstyleind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillStyleIndex;
    *errorStatus = GERROR_NONE;
}

void ginqlevelgks(Glevel *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
    }
    else
    {
        *value = g_gksDescription.level;
        *errorStatus = GERROR_NONE;
    }
}

void ginqlinecolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentLineColorIndex;
    *errorStatus = GERROR_NONE;
}

void ginqlineind(Gint *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentLineIndex;
    *errorStatus = GERROR_NONE;
}

void ginqlinetype(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentLineType;
    *errorStatus = GERROR_NONE;
}

void ginqmarkercolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerColorIndex;
    *errorStatus = GERROR_NONE;
}

void ginqmarkerind(Gint *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksState.currentMarkerIndex;
    *errorStatus = GERROR_NONE;
}

void ginqmarkersize(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerSize;
    *errorStatus = GERROR_NONE;
}

void ginqmarkertype(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerType;
    *errorStatus = GERROR_NONE;
}

void ginqmaxntrannum(Gint *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksDescription.numTrans;
    *errorStatus = GERROR_NONE;
}

void ginqntran(Gint num, Gtran *value, Gint *errorStatus)
{
    *value = g_gksState.transforms[num];
    *errorStatus = GERROR_NONE;
}

void ginqopenws(Gint maxIds, Gint start, Gintlist *wsids, Gint *actualIds, Gint *errorStatus)
{
    *actualIds = 0;
    *errorStatus = GERROR_NONE;
}

void ginqopst(Gopst *value)
{
    *value = g_opState;
}

void ginqpatrefpt(Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentPatternRef;
    *errorStatus = GERROR_NONE;
}

void ginqpatsize(Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentPatternSize;
    *errorStatus = GERROR_NONE;
}

void ginqtextalign(Gtxalign *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextAlign;
    *errorStatus = GERROR_NONE;
}

void ginqtextcolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextColorIndex;
    *errorStatus = GERROR_NONE;
}

void ginqtextfontprec(Gtxfp *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextFontPrec;
    *errorStatus = GERROR_NONE;
}

void ginqtextind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextIndex;
    *errorStatus = GERROR_NONE;
}

void ginqwsmaxnum(Gwsmax *value, Gint *errorStatus)
{
    if (g_opState == GGKCL)
    {
        *errorStatus = GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP;
        return;
    }

    *value = g_gksDescription.wsmax;
    *errorStatus = GERROR_NONE;
}

void gsetasf(Gasfs *value)
{
    g_gksState.asfs = *value;
}

void gsetcharexpan(Gfloat value)
{
    g_gksState.currentCharExpandFactor = value;
}

void gsetcharheight(Gfloat value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_CHARACTER_HEIGHT, g_errFile);
        return;
    }

    g_gksState.currentCharHeight = value;
}

void gsetcharspace(Gfloat value)
{
    g_gksState.currentCharSpacing = value;
}

void gsetcharup(Gpoint *value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_CHARACTER_UP_VECTOR, g_errFile);
        return;
    }

    g_gksState.currentCharUp = *value;
}

void gsetclip(Gclip value)
{
    g_gksState.clipping = value;
}

void gsetfillcolorind(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_FILL_AREA_COLOR_INDEX, g_errFile);
        return;
    }

    g_gksState.currentFillColorIndex = value;
}

void gsetfillind(Gint value)
{
    g_gksState.currentFillIndex = value;
}

void gsetfillstyle(Gflinter value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_FILL_AREA_INTERIOR_STYLE, g_errFile);
        return;
    }

    g_gksState.currentFillStyle = value;
}

void gsetfillstyleind(Gint value)
{
    g_gksState.currentFillStyleIndex = value;
}

void gsetlinecolorind(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_POLYLINE_COLOR_INDEX, g_errFile);
        return;
    }

    g_gksState.currentLineColorIndex = value;
}

void gsetlineind(Gint value)
{
    g_gksState.currentLineIndex = value;
}

void gsetlinetype(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_LINETYPE, g_errFile);
        return;
    }

    g_gksState.currentLineType = value;
}

void gsetmarkercolorind(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_POLYMARKER_COLOR_INDEX, g_errFile);
        return;
    }

    g_gksState.currentMarkerColorIndex = value;
}

void gsetmarkerind(Gint value)
{
    g_gksState.currentMarkerIndex = value;
}

void gsetmarkersize(Gfloat value)
{
    g_gksState.currentMarkerSize = value;
}

void gsetmarkertype(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_MARKER_TYPE, g_errFile);
        return;
    }

    g_gksState.currentMarkerType = value;
}

void gsettextalign(Gtxalign *value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_TEXT_ALIGNMENT, g_errFile);
        return;
    }

    g_gksState.currentTextAlign = *value;
}

void gsettextcolorind(Gint value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_TEXT_COLOR_INDEX, g_errFile);
        return;
    }

    g_gksState.currentTextColorIndex = value;
}

void gsettextfontprec(Gtxfp *fontPrec)
{
    g_gksState.currentTextFontPrec = *fontPrec;
}

void gsettextind(Gint value)
{
    g_gksState.currentTextIndex = value;
}

void gsetviewport(Gint transform, Glimit *value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_VIEWPORT, g_errFile);
        return;
    }

    g_gksState.transforms[transform].v = *value;
}

void gsetwindow(Gint transform, Glimit *value)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_WINDOW, g_errFile);
        return;
    }

    g_gksState.transforms[transform].w = *value;
}

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType)
{
    if (g_opState == GGKCL)
    {
        gerrorhand(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_OPEN_WORKSTATION, g_errFile);
        return;
    }

    g_opState = GWSOP;
    g_gksState.openWs[0] = wsId;

    g_wsState[0] = g_initialWsState;
    g_wsState[0].id = wsId;
    g_wsState[0].connId = connId;
    g_wsState[0].type = wsType;
}

void gclosews(Gint wsId)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION, g_errFile);
        return;
    }

    g_opState = GGKOP;
    g_gksState.openWs[0] = -1;
}

void gactivatews(Gint wsId)
{
    if (g_opState != GWSOP && g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION, g_errFile);
        return;
    }

    g_opState = GWSAC;
    g_gksState.activeWs[0] = wsId;
}

void gclearws(Gint wsId, Gclrflag flag)
{
    if (g_opState != GWSOP && g_opState != GWSAC)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION, g_errFile);
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

    g_opState = GWSOP;

    g_gksState.activeWs[0] = -1;
}

void gupdatews(Gint wsId, Gregen flag)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION, g_errFile);
        return;
    }
}

void ginqcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    *value = g_wsState[0].colorTable[index];
}

void ginqwstran(Gint wsId, Gwsti *value, Gint *errorStatus)
{
    *value = g_wsState[0].transform;
    *errorStatus = GERROR_NONE;
}

void gsetcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION, g_errFile);
        return;
    }

    g_wsState[0].colorTable[index] = *value;
}

void gsetwsviewport(Gint wsId, Glimit *value)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT, g_errFile);
        return;
    }

    g_wsState[0].transform.current.v = *value;
}

void gsetwswindow(Gint wsId, Glimit *value)
{
    if (g_opState == GGKCL || g_opState == GGKOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW, g_errFile);
        return;
    }

    g_wsState[0].transform.current.w = *value;
}

void gcellarray(Grect *rect, Gidim *dims, Gint *colors)
{
}

void gfillarea(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA, g_errFile);
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
}

void gpolymarker(Gint numPoints, Gpoint *points)
{
    if (g_opState != GWSAC && g_opState != GSGOP)
    {
        gerrorhand(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER, g_errFile);
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
