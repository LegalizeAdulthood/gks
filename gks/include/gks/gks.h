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

typedef Gchar Gconn;
typedef Gint Gwstype;

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

typedef enum Gclrflag_t
{
    GCONDITIONALLY,
    GALWAYS
} Gclrflag;

typedef enum Gregen_t
{
    GPOSTPONE,
    GPERFORM
} Gregen;

typedef struct Gstrlist_t
{
    Gint n_points;
    const Gchar **strings;
} Gstrlist;

typedef struct Gwsmax_t
{
    Gint open;
    Gint active;
    Gint assoc;
} Gwsmax;

void gopengks(Gfile *errfile, Glong memory);
void gclosegks();

void ginqopst(Gopst *value);
void ginqlevelgks(Glevel *value);
void ginqmaxntrannum(Gint *value, Gint *errorStatus);
void ginqwsmaxnum(Gwsmax *value, Gint *errorStatus);

void ginqavailwstypes(Gint bufSize, Gint start, Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus);

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
void gclosews(Gint wsId);

void gactivatews(Gint wsId);
void gdeactivatews(Gint wsId);
void gclearws(Gint wsId, Gclrflag flag);
void gupdatews(Gint wsId, Gregen flag);

#if defined(__cplusplus)
}
#endif

#endif
