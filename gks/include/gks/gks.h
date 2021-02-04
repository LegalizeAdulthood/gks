#if !defined(GKS_H)
#define GKS_H

#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef char Gchar;
typedef Gchar Gconn;
typedef int Gint;
typedef Gint Gwstype;
typedef float Gfloat;
typedef long Glong;
typedef FILE Gfile;

typedef enum Gopst_t
{
    GGKCL,
    GGKOP,
    GWSOP,
    GWSAC,
    GSGOP
} Gopst;

void gopengks(Gfile *errfile, Glong memory);
void gclosegks();

void ginqopst(Gopst *value);

#if defined(__cplusplus)
}
#endif

#endif
