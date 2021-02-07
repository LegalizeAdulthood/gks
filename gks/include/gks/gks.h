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

struct Gescin
{
    union
    {
        struct Guesc_idatarec
        {
            Gint number_integer;
            Gint number_float;
            Gint number_strings;
            Gint *list_integers;
            Gfloat *list_floats;
            Gint *list_string_lengths;
            Gchar **list_strings;
        } esc_idatarec;
    };
};

struct Gescout
{
    union
    {
        struct Guesc_odatarec
        {
            Gint number_integer;
            Gint number_float;
            Gint number_strings;
            Gint *list_integers;
            Gfloat *list_floats;
            Gint *list_string_lengths;
            Gchar **list_strings;
        } esc_odatarec;
    };
};

struct Glimit
{
    Gfloat xmin;
    Gfloat xmax;
    Gfloat ymin;
    Gfloat ymax;
};

struct Gstrlist
{
    Gint n_points;
    const Gchar **strings;
};

struct Gtran
{
    struct Glimit w;
    struct Glimit v;
};

struct Gwsmax
{
    Gint open;
    Gint active;
    Gint assoc;
};

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);

void gescape(Gint function, struct Gescin *inData, Gint bufSize, struct Gescout *outData, Gint *escOutSize);

void gopengks(Gfile *errfile, Glong memory);
void gclosegks(void);

void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus);
void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus);
void ginqlevelgks(enum Glevel *value);
void ginqmaxntrannum(Gint *value, Gint *errorStatus);
void ginqntran(Gint num, struct Gtran *tran, Gint *errorStatus);
void ginqopst(enum Gopst *value);
void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus);

void gsetwindow(Gint transform, struct Glimit *window);

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
void gclosews(Gint wsId);

void gactivatews(Gint wsId);
void gclearws(Gint wsId, enum Gclrflag flag);
void gdeactivatews(Gint wsId);
void gupdatews(Gint wsId, enum Gregen flag);

#if defined(__cplusplus)
}
#endif

#endif
