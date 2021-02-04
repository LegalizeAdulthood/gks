#include <gks/gks.h>

static Gopst g_operState = GGKCL;

void gopengks(Gfile *errfile, Glong memory)
{
    g_operState = GGKOP;
}

void gclosegks()
{
    g_operState = GGKCL;
}

void ginqopst(Gopst *value)
{
    *value = g_operState;
}

void ginqlevelgks(Glevel *value)
{
    *value = GL0A;
}

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    fprintf(errFile, "GKS error %d in function %d\n", errNum, funcName);
}
