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
