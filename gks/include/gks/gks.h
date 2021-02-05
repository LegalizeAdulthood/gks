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

enum Gclrflag
{
    GCONDITIONALLY,
    GALWAYS
};

enum Gcoavail
{
    GCOLOUR,
    GMONOCHROME
};

enum Glevel
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
};

enum Gopst
{
    GGKCL,
    GGKOP,
    GWSOP,
    GWSAC,
    GSGOP
};

enum Gregen
{
    GPOSTPONE,
    GPERFORM
};

struct Gcofac
{
    Gint colours;
    enum Gcoavail coavail;
    Gint predefined;
};

struct Gstrlist
{
    Gint n_points;
    const Gchar **strings;
};

struct Gwsmax
{
    Gint open;
    Gint active;
    Gint assoc;
};

void gopengks(Gfile *errfile, Glong memory);
void gclosegks();

void ginqopst(enum Gopst *value);
void ginqlevelgks(enum Glevel *value);
void ginqmaxntrannum(Gint *value, Gint *errorStatus);
void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus);
void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus);

void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus);

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
void gclosews(Gint wsId);

void gactivatews(Gint wsId);
void gdeactivatews(Gint wsId);
void gclearws(Gint wsId, enum Gclrflag flag);
void gupdatews(Gint wsId, enum Gregen flag);

#if defined(__cplusplus)
}
#endif

#endif
