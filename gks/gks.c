#include <gks/gks.h>

#include <string.h>

#define NUM_OF(ary_) (sizeof(ary_)/sizeof((ary_)[0]))

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

static const GWSDesc g_wsDesc[MAX_WS_TYPES] =
{
    // Tektronix 4105
    {
        {
            GDC_OTHER,
            { 0.0f, 1.0f },
            { 640, 480 }
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

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    fprintf(errFile, "GKS error %d in function %d\n", errNum, funcName);
}

void gescape(Gint function, Gescin *indata, Gint bufsize, Gescout *outdata, Gint *escoutSize)
{
}

void gopengks(Gfile *errfile, Glong memory)
{
    g_opState = GGKOP;
    g_gksState = g_initialGksState;
}

void gclosegks(void)
{
    g_opState = GGKCL;
}

void ginqasf(Gasfs *value, Gint *errorStatus)
{
    *value = g_gksState.asfs;
    *errorStatus = 0;
}

void ginqavailwstypes(Gint bufSize, Gint start, Gintlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    *numTypes = g_gksDescription.numWsTypes;
    wsTypes->number = *numTypes;
    for (int i = 0; i < g_gksDescription.numWsTypes; ++i)
    {
        wsTypes->integers[i] = g_gksDescription.wsTypes[i];
    }
    *errorStatus = 0;
}

void ginqcharexpan(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharExpandFactor;
    *errorStatus = 0;
}

void ginqcharbase(Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharBase;
    *errorStatus = 0;
}

void ginqcharheight(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharHeight;
    *errorStatus = 0;
}

void ginqcharspace(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharSpacing;
    *errorStatus = 0;
}

void ginqcharwidth(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharWidth;
    *errorStatus = 0;
}

void ginqcharup(Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharUp;
    *errorStatus = 0;
}

void ginqclip(Gcliprect *value, Gint *errorStatus)
{
    value->ind = g_gksState.clipping;
    const Glimit identity =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };
    value->rec = identity;
    *errorStatus = 0;
}

void ginqcolorfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gcofac *value, Gint *errorStatus)
{
    *value = g_wsDesc[wsType-1].colorFacilities;
    *errorStatus = 0;
}

void ginqdisplaysize(Gwstype wsType, Gdspsize *value, Gint *errorStatus)
{
    *value = g_wsDesc[wsType-1].displaySpaceSize;
    *errorStatus = 0;
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
    *errorStatus = 0;
}

void ginqfillcolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillColorIndex;
    *errorStatus = 0;
}

void ginqfillind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillIndex;
    *errorStatus = 0;
}

void ginqfillstyle(Gflinter *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillStyle;
    *errorStatus = 0;
}

void ginqfillstyleind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillStyleIndex;
    *errorStatus = 0;
}

void ginqlevelgks(Glevel *value, Gint *errorStatus)
{
    *value = g_gksDescription.level;
    *errorStatus = 0;
}

void ginqlinecolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentLineColorIndex;
    *errorStatus = 0;
}

void ginqlineind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentLineIndex;
    *errorStatus = 0;
}

void ginqlinetype(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentLineType;
    *errorStatus = 0;
}

void ginqmarkercolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerColorIndex;
    *errorStatus = 0;
}

void ginqmarkerind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerIndex;
    *errorStatus = 0;
}

void ginqmarkersize(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerSize;
    *errorStatus = 0;
}

void ginqmarkertype(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentMarkerType;
    *errorStatus = 0;
}

void ginqmaxntrannum(Gint *value, Gint *errorStatus)
{
    *value = g_gksDescription.numTrans;
    *errorStatus = 0;
}

void ginqntran(Gint num, Gtran *value, Gint *errorStatus)
{
    *value = g_gksState.transforms[num];
    *errorStatus = 0;
}

void ginqopenws(Gint maxIds, Gint start, Gintlist *wsids, Gint *actualIds, Gint *errorStatus)
{
    *actualIds = 0;
    *errorStatus = 0;
}

void ginqopst(Gopst *value)
{
    *value = g_opState;
}

void ginqpatrefpt(Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentPatternRef;
    *errorStatus = 0;
}

void ginqpatsize(Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentPatternSize;
    *errorStatus = 0;
}

void ginqtextalign(Gtxalign *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextAlign;
    *errorStatus = 0;
}

void ginqtextcolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextColorIndex;
    *errorStatus = 0;
}

void ginqtextfontprec(Gtxfp *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextFontPrec;
    *errorStatus = 0;
}

void ginqtextind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextIndex;
    *errorStatus = 0;
}

void ginqwsmaxnum(Gwsmax *value, Gint *errorStatus)
{
    *value = g_gksDescription.wsmax;
    *errorStatus = 0;
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
    g_gksState.currentCharHeight = value;
}

void gsetcharspace(Gfloat value)
{
    g_gksState.currentCharSpacing = value;
}

void gsetcharup(Gpoint *value)
{
    g_gksState.currentCharUp = *value;
}

void gsetclip(Gclip value)
{
    g_gksState.clipping = value;
}

void gsetfillcolorind(Gint value)
{
    g_gksState.currentFillColorIndex = value;
}

void gsetfillind(Gint value)
{
    g_gksState.currentFillIndex = value;
}

void gsetfillstyle(Gflinter value)
{
    g_gksState.currentFillStyle = value;
}

void gsetfillstyleind(Gint value)
{
    g_gksState.currentFillStyleIndex = value;
}

void gsetlinecolorind(Gint value)
{
    g_gksState.currentLineColorIndex = value;
}

void gsetlineind(Gint value)
{
    g_gksState.currentLineIndex = value;
}

void gsetlinetype(Gint value)
{
    g_gksState.currentLineType = value;
}

void gsetmarkercolorind(Gint value)
{
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
    g_gksState.currentMarkerType = value;
}

void gsettextalign(Gtxalign *value)
{
    g_gksState.currentTextAlign = *value;
}

void gsettextcolorind(Gint value)
{
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
    g_gksState.transforms[transform].v = *value;
}

void gsetwindow(Gint transform, Glimit *value)
{
    g_gksState.transforms[transform].w = *value;
}

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType)
{
    g_opState = GWSOP;
    g_gksState.openWs[0] = wsId;

    g_wsState[0] = g_initialWsState;
    g_wsState[0].id = wsId;
    g_wsState[0].connId = connId;
    g_wsState[0].type = wsType;
}

void gclosews(Gint wsId)
{
    g_opState = GGKOP;

    g_gksState.openWs[0] = -1;
}

void gactivatews(Gint wsId)
{
    g_opState = GWSAC;

    g_gksState.activeWs[0] = wsId;
}

void gclearws(Gint wsId, Gclrflag flag)
{
}

void gdeactivatews(Gint wsId)
{
    g_opState = GWSOP;

    g_gksState.activeWs[0] = -1;
}

void gupdatews(Gint wsId, Gregen flag)
{
}

void ginqcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    *value = g_wsState[0].colorTable[index];
}

void ginqwstran(Gint wsId, Gwsti *value, Gint *errorStatus)
{
    *value = g_wsState[0].transform;
    *errorStatus = 0;
}

void gsetcolorrep(Gint wsId, Gint index, Gcobundl *value)
{
    g_wsState[0].colorTable[index] = *value;
}

void gsetwsviewport(Gint wsId, Glimit *value)
{
    g_wsState[0].transform.current.v = *value;
}

void gsetwswindow(Gint wsId, Glimit *value)
{
    g_wsState[0].transform.current.w = *value;
}

void gcellarray(Grect *rect, Gidim *dims, Gint *colors)
{
}

void gfillarea(Gint numPoints, Gpoint *points)
{
}

void ggdp(Gint numPoints, Gpoint *points, Gint gdpId, Ggdprec *data)
{
}

void gpolyline(Gint numPoints, Gpoint *points)
{
}

void gpolymarker(Gint numPoints, Gpoint *points)
{
}

void gtext(Gpoint *start, const Gchar *text)
{
}
