#include <gks/gks.h>

static Gopst g_opState = GGKCL;

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
    *value = GL0A;
}

static const Gchar *g_wsTypes[] =
{
    "tek4105",
};

void ginqavailwstypes(Gint bufSize, Gint start, Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus)
{
    *numTypes = sizeof(g_wsTypes)/sizeof(g_wsTypes[0]);
    wsTypes->n_points = *numTypes;
    wsTypes->strings = g_wsTypes;
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
