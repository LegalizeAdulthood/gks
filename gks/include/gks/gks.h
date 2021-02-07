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

enum Gclip
{
    GNOCLIP,
    GCLIP
};

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

enum Glntype
{
    GLN_SOLID = 1,
    GLN_DASHED = 2,
    GLN_DOTTED = 3,
    GLN_DASHDOT = 4
};

enum Gmktype
{
    GMK_DOT = 1,
    GMK_PLUS = 2,
    GMK_STAR = 3,
    GMK_O = 4,
    GMK_X = 5
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

enum Gwstus
{
    GNOTPENDING,
    GPENDING
};


struct Glimit
{
    Gfloat xmin;
    Gfloat xmax;
    Gfloat ymin;
    Gfloat ymax;
};

struct Gcliprect
{
    enum Gclip ind;
    struct Glimit rec;
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

struct Gpoint
{
    Gfloat x;
    Gfloat y;
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

struct Gwsti
{
    enum Gwstus wstus;
    struct Gtran request;
    struct Gtran current;
};

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);

void gescape(Gint function, struct Gescin *inData, Gint bufSize, struct Gescout *outData, Gint *escOutSize);

void gopengks(Gfile *errfile, Glong memory);
void gclosegks(void);

void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus);
void ginqclip(struct Gcliprect *value, Gint *errorStatus);
void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus);
void ginqlevelgks(enum Glevel *value, Gint *errorStatus);
void ginqlinecolorind(Gint *value, Gint *errorStatus);
void ginqlineind(Gint *value, Gint *errorStatus);
void ginqlinetype(Gint *value, Gint *errorStatus);
void ginqmarkercolorind(Gint *value, Gint *errorStatus);
void ginqmarkerind(Gint *value, Gint *errorStatus);
void ginqmarkersize(Gfloat *value, Gint *errorStatus);
void ginqmarkertype(Gint *value, Gint *errorStatus);
void ginqmaxntrannum(Gint *value, Gint *errorStatus);
void ginqntran(Gint num, struct Gtran *value, Gint *errorStatus);
void ginqopst(enum Gopst *value);
void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus);

void gsetclip(enum Gclip value);
void gsetlinecolorind(Gint value);
void gsetlineind(Gint value);
void gsetlinetype(Gint value);
void gsetmarkercolorind(Gint value);
void gsetmarkerind(Gint value);
void gsetmarkersize(Gfloat value);
void gsetmarkertype(Gint value);
void gsetviewport(Gint transform, struct Glimit *value);
void gsetwindow(Gint transform, struct Glimit *value);

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
void gclosews(Gint wsId);

void gactivatews(Gint wsId);
void gclearws(Gint wsId, enum Gclrflag flag);
void gdeactivatews(Gint wsId);
void gupdatews(Gint wsId, enum Gregen flag);

void ginqwstran(Gint wsId, struct Gwsti *value, Gint *errorStatus);

void gsetwsviewport(Gint wsId, struct Glimit *value);
void gsetwswindow(Gint wsId, struct Glimit *value);

void gpolyline(Gint numPoints, struct Gpoint *points);
void gpolymarker(Gint numPoints, struct Gpoint *points);

#if defined(__cplusplus)
}
#endif

#endif
