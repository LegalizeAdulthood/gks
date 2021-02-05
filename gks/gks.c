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
    // normalization transformation
    // polyline
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
    0
};

static GGKSState g_gksState;

typedef struct GWSState_t
{
    const Gchar *type;

} GWSState;

static GWSState g_wsState[1];

void gopengks(Gfile *errfile, Glong memory)
{
    g_opState = GGKOP;
    g_gksState = g_initialGksState;
}

void gclosegks()
{
    g_opState = GGKCL;
}

void ginqopst(enum Gopst *value)
{
    *value = g_opState;
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

void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus)
{
    *value = g_gksDescription.wsmax;
    *errorStatus = 0;
}

void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus)
{
}

void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    *numTypes = g_gksDescription.numAvailWSTypes;
    wsTypes->n_points = *numTypes;
    wsTypes->strings = g_gksDescription.wsTypes;
}

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    fprintf(errFile, "GKS error %d in function %d\n", errNum, funcName);
}

void gopenws(Gint wsId, const Gchar *connId, Gint wsType)
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
