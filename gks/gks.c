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
