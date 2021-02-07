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
    // normalization transformation
    // polyline
    // polymarker
    // text
    // fill area
    // segments
    // input queue
    // current event report
} GGKSState;

static const struct Gtran identity =
{
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f }
};

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
    }
};

static GGKSState g_gksState;

typedef struct GWSState_t
{
    const Gchar *type;

} GWSState;

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

void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus)
{
}

void ginqlevelgks(enum Glevel *value)
{
    *value = g_gksDescription.level;
}

void ginqmaxntrannum(Gint *value, Gint *errorStatus)
{
    *value = g_gksDescription.numTrans;
    *errorStatus = 0;
}

void ginqntran(Gint num, struct Gtran *tran, Gint *errorStatus)
{
    *tran = g_gksState.transforms[num];
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

void gsetviewport(Gint transform, struct Glimit *viewport)
{
    g_gksState.transforms[transform].v = *viewport;
}

void gsetwindow(Gint transform, struct Glimit *window)
{
    g_gksState.transforms[transform].w = *window;
}

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType)
{
    g_opState = GWSOP;
}

void gclosews(Gint wsId)
{
    g_opState = GGKOP;
}

void gactivatews(Gint wsId)
{
    g_opState = GWSAC;
}

void gdeactivatews(Gint wsId)
{
    g_opState = GWSOP;
}

void gclearws(Gint wsId, enum Gclrflag flag)
{
}

void gupdatews(Gint wsId, enum Gregen flag)
{
}
