#include <gks/gks.h>

static Gopst g_opState = GGKCL;

static const Gchar *g_wsTypes[] =
{
    "tek4105",
};

static struct
{
    Glevel level;
    Gint numAvailWSTypes;
    const Gchar **wsTypes;
    Gint maxOpenWs;
    Gint maxActiveWs;
    Gint maxSegAssoc;
    Gint numTrans;
}
g_gksDescription =
{
    GL0A,
    sizeof(g_wsTypes)/sizeof(g_wsTypes[0]),
    g_wsTypes,
    1,
    1,
    0,
    2
};

void gopengks(Gfile *errfile, Glong memory)
{
    g_opState = GGKOP;
}

void gclosegks()
{
    g_opState = GGKCL;
}

void ginqopst(Gopst *value)
{
    *value = g_opState;
}

void ginqlevelgks(Glevel *value)
{
    *value = g_gksDescription.level;
}

void ginqavailwstypes(Gint bufSize, Gint start, Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus)
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

void gclearws(Gint wsId, Gclrflag flag)
{
}

void gupdatews(Gint wsId, Gregen flag)
{
}
