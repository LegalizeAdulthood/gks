#include <gks/gks.h>

#include <string.h>

static enum Gopst g_opState = GGKCL;

static const Gchar *g_wsTypes[] =
{
    "tek4105",
};

enum
{
    MAX_OPEN_WS = 1,
    MAX_ACTIVE_WS = 1,
    MAX_SEGMENT_ASSOC = 0,
    MAX_NUM_TRANSFORMS = 2,
    MAX_NUM_COLORS = 64
};

typedef struct GGKSDescription_t
{
    enum Glevel level;
    Gint numAvailWSTypes;
    const Gchar **wsTypes;
    struct Gwsmax wsmax;
    Gint numTrans;
} GGKSDescription;

static GGKSDescription g_gksDescription =
{
    GLMA,
    sizeof(g_wsTypes)/sizeof(g_wsTypes[0]),
    g_wsTypes,
    { MAX_OPEN_WS, MAX_ACTIVE_WS, MAX_SEGMENT_ASSOC },
    MAX_NUM_TRANSFORMS
};


typedef struct GGKSState_t
{
    Gint openWs[MAX_OPEN_WS];
    Gint activeWs[MAX_ACTIVE_WS];
    Gint currentTransform;
    struct Gtran transforms[MAX_NUM_TRANSFORMS];
    enum Gclip clipping;
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
    struct Gpoint currentCharUp;
    Gint currentTextColorIndex;
    Gint currentTextIndex;
    struct Gtxalign currentTextAlign;
    struct Gtxfp currentTextFontPrec;
    Gfloat currentCharExpandFactor;
    Gfloat currentCharSpacing;
    // fill area
    enum Gflinter currentFillStyle;
    Gint currentFillStyleIndex;
    Gint currentFillColorIndex;
    Gint currentFillIndex;
    struct Gpoint currentPatternSize;
    struct Gpoint currentPatternRef;
    // attribute source flags
    struct Gasfs asfs;
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
    { 0.0f, 1.0f },
    1,
    1,
    { GAH_NORMAL, GAV_NORMAL },
    { 1, GP_STRING },
    1.0f,
    0.0f,
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
    struct Gcobundl colorTable[MAX_NUM_COLORS];
    //-- deferral mode
    struct Gwsti transform;
    //- category INPUT, OUTIN
    //-- locator
    //-- stroke
    //-- valuator
    //-- choice
    //-- pick
    //-- string
} GWSState;

static const GWSState g_initialWSState =
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

void gescape(Gint function, struct Gescin *indata, Gint bufsize, struct Gescout *outdata, Gint *escoutSize)
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

void ginqasf(struct Gasfs *value, Gint *errorStatus)
{
    *value = g_gksState.asfs;
    *errorStatus = 0;
}

void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    *numTypes = g_gksDescription.numAvailWSTypes;
    for (int i = 0; i < g_gksDescription.numAvailWSTypes; ++i)
    {
        strcpy(wsTypes->strings[i], g_gksDescription.wsTypes[i]);
    }
    *errorStatus = 0;
}

void ginqcharexpan(Gfloat *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharExpandFactor;
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

void ginqcharup(struct Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentCharUp;
    *errorStatus = 0;
}

void ginqclip(struct Gcliprect *value, Gint *errorStatus)
{
    value->ind = g_gksState.clipping;
    const struct Glimit identity =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };
    value->rec = identity;
    *errorStatus = 0;
}

void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus)
{
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

void ginqfillstyle(enum Gflinter *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillStyle;
    *errorStatus = 0;
}

void ginqfillstyleind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentFillStyleIndex;
    *errorStatus = 0;
}

void ginqlevelgks(enum Glevel *value, Gint *errorStatus)
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

void ginqntran(Gint num, struct Gtran *value, Gint *errorStatus)
{
    *value = g_gksState.transforms[num];
    *errorStatus = 0;
}

void ginqopenws(Gint maxIds, Gint start, struct Gintlist *wsids, Gint *actualIds, Gint *errorStatus)
{
    *actualIds = 0;
    *errorStatus = 0;
}

void ginqopst(enum Gopst *value)
{
    *value = g_opState;
}

void ginqpatrefpt(struct Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentPatternRef;
    *errorStatus = 0;
}

void ginqpatsize(struct Gpoint *value, Gint *errorStatus)
{
    *value = g_gksState.currentPatternSize;
    *errorStatus = 0;
}

void ginqtextalign(struct Gtxalign *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextAlign;
    *errorStatus = 0;
}

void ginqtextcolorind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextColorIndex;
    *errorStatus = 0;
}

void ginqtextfontprec(struct Gtxfp *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextFontPrec;
    *errorStatus = 0;
}

void ginqtextind(Gint *value, Gint *errorStatus)
{
    *value = g_gksState.currentTextIndex;
    *errorStatus = 0;
}

void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus)
{
    *value = g_gksDescription.wsmax;
    *errorStatus = 0;
}

void gsetasf(struct Gasfs *value)
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

void gsetcharup(struct Gpoint *value)
{
    g_gksState.currentCharUp = *value;
}

void gsetclip(enum Gclip value)
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

void gsetfillstyle(enum Gflinter value)
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

void gsettextalign(struct Gtxalign *value)
{
    g_gksState.currentTextAlign = *value;
}

void gsettextcolorind(Gint value)
{
    g_gksState.currentTextColorIndex = value;
}

void gsettextfontprec(struct Gtxfp *fontPrec)
{
    g_gksState.currentTextFontPrec = *fontPrec;
}

void gsettextind(Gint value)
{
    g_gksState.currentTextIndex = value;
}

void gsetviewport(Gint transform, struct Glimit *value)
{
    g_gksState.transforms[transform].v = *value;
}

void gsetwindow(Gint transform, struct Glimit *value)
{
    g_gksState.transforms[transform].w = *value;
}

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType)
{
    g_opState = GWSOP;
    g_gksState.openWs[0] = wsId;

    g_wsState[0] = g_initialWSState;
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

void gdeactivatews(Gint wsId)
{
    g_opState = GWSOP;

    g_gksState.activeWs[0] = -1;
}

void gclearws(Gint wsId, enum Gclrflag flag)
{
}

void gupdatews(Gint wsId, enum Gregen flag)
{
}

void ginqcolorrep(Gint wsId, Gint index, struct Gcobundl *value)
{
    *value = g_wsState[0].colorTable[index];
}

void ginqwstran(Gint wsId, struct Gwsti *value, Gint *errorStatus)
{
    *value = g_wsState[0].transform;
    *errorStatus = 0;
}

void gsetcolorrep(Gint wsId, Gint index, struct Gcobundl *value)
{
    g_wsState[0].colorTable[index] = *value;
}

void gsetwsviewport(Gint wsId, struct Glimit *value)
{
    g_wsState[0].transform.current.v = *value;
}

void gsetwswindow(Gint wsId, struct Glimit *value)
{
    g_wsState[0].transform.current.w = *value;
}

void gcellarray(struct Grect *rect, struct Gidim *dims, Gint *colors)
{
}

void gfillarea(Gint numPoints, struct Gpoint *points)
{
}

void ggdp(Gint numPoints, struct Gpoint *points, Gint gdpId, struct Ggdprec *data)
{
}

void gpolyline(Gint numPoints, struct Gpoint *points)
{
}

void gpolymarker(Gint numPoints, struct Gpoint *points)
{
}

void gtext(struct Gpoint *start, const Gchar *text)
{
}
