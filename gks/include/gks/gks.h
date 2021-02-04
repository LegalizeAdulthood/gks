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

void gopengks(Gfile *errfile, Glong memory);
void gclosegks();

#if defined(__cplusplus)
}
#endif

#endif
