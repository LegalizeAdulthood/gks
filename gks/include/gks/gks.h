#if !defined(GKS_H)
#define GKS_H

#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef char Gchar;
typedef int Gint;
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

typedef enum Glevel_t
{
    GLMA,
    GLMB,
    GLMC,
    GL0A,
    GL0B,
    GL0C,
    GL1A,
    GL1B,
    GL1C,
    GL2A,
    GL2B,
    GL2C
} Glevel;

void gopengks(Gfile *errfile, Glong memory);
void gclosegks();

void ginqopst(Gopst *value);
void ginqlevelgks(Glevel *value);

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);

void gopenws(Gint wsId, const Gchar *connId, Gint wsType);
void gclosews(Gint wsId);

#if defined(__cplusplus)
}
#endif

#endif
