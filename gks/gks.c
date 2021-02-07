#include <gks/gks.h>

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
    MAX_NUM_TRANSFORMS = 2
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
    // polymarker
    // text
    // fill area
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
    GLN_SOLID,
    1
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
    //-- deferral mode
    //-- workstation transformation
    //--- workstation transformation update state
    //--- requested workstation window
    //--- requested workstation viewport
    //--- current workstation window
    //--- current workstation viewport
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

void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    *numTypes = g_gksDescription.numAvailWSTypes;
    wsTypes->n_points = *numTypes;
    wsTypes->strings = g_gksDescription.wsTypes;
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
}

void ginqlevelgks(enum Glevel *value)
{
    *value = g_gksDescription.level;
}

void ginqlinecolorind(Gint *value)
{
    *value = g_gksState.currentLineColorIndex;
}

void ginqlinetype(Gint *value)
{
    *value = g_gksState.currentLineType;
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

void ginqopst(enum Gopst *value)
{
    *value = g_opState;
}

void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus)
{
    *value = g_gksDescription.wsmax;
    *errorStatus = 0;
}

void gsetclip(enum Gclip value)
{
    g_gksState.clipping = value;
}

void gsetlinecolorind(Gint value)
{
    g_gksState.currentLineColorIndex = value;
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

    g_gksState.openWs[0] = 0;
}

void gactivatews(Gint wsId)
{
    g_opState = GWSAC;

    g_gksState.activeWs[0] = wsId;
}

void gdeactivatews(Gint wsId)
{
    g_opState = GWSOP;

    g_gksState.activeWs[0] = 0;
}

void gclearws(Gint wsId, enum Gclrflag flag)
{
}

void gupdatews(Gint wsId, enum Gregen flag)
{
}

void ginqwstran(Gint wsId, struct Gwsti *value, Gint *errorStatus)
{
    *value = g_wsState[0].transform;
    *errorStatus = 0;
}

void gsetwsviewport(Gint wsId, struct Glimit *value)
{
    g_wsState[0].transform.current.v = *value;
}

void gsetwswindow(Gint wsId, struct Glimit *value)
{
    g_wsState[0].transform.current.w = *value;
}

void gpolyline(Gint numPoints, struct Gpoint *points)
{
}

void gsetlinetype(Gint value)
{
    g_gksState.currentLineType = value;
}
