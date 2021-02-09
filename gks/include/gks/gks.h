#if !defined(GKS_H)
#define GKS_H

#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef char Gchar;
typedef int Gint;
typedef unsigned int Guint;
typedef float Gfloat;
typedef long Glong;
typedef FILE Gfile;

typedef Gchar Gconn;
typedef Gint Gwstype;

// Predefined line types
#define GLN_SOLID 1
#define GLN_DASHED 2
#define GLN_DOTTED 3
#define GLN_DASHDOT 4

// Predefined marker types
#define GMK_DOT 1
#define GMK_PLUS 2
#define GMK_STAR 3
#define GMK_CIRCLE 4
#define GMK_DIAGONAL_CROSS 5

enum Gasf
{
    GBUNDLED = 1,
    GINDIVIDUAL
};

enum Gclip
{
    GNOCLIP = 1,
    GCLIP
};

enum Gclrflag
{
    GCONDITIONALLY = 1,
    GALWAYS
};

enum Gcoavail
{
    GCOLOUR = 1,
    GMONOCHROME
};

enum Gflinter
{
    GHOLLOW = 1,
    GSOLID,
    GPATTERN,
    GHATCH
};

enum Glevel
{
    GLMA = 1,
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
    GGKCL = 1,
    GGKOP,
    GWSOP,
    GWSAC,
    GSGOP
};

enum Gregen
{
    GPOSTPONE = 1,
    GPERFORM
};

enum Gtxhor
{
    GAH_NORMAL = 1,
    GAH_LEFT,
    GAH_CENTER,
    GAH_RIGHT
};

enum Gtxprec
{
    GP_STRING = 1,
    GP_CHAR,
    GP_STROKE
};

enum Gtxver
{
    GAV_NORMAL = 1,
    GAV_TOP,
    GAV_CAP,
    GAV_HALF,
    GAV_BASE,
    GAV_BOTTOM
};

enum Gwstus
{
    GNOTPENDING = 1,
    GPENDING
};


struct Gasfs
{
    enum Gasf ln_type;
    enum Gasf ln_width;
    enum Gasf ln_color;
    enum Gasf mk_type;
    enum Gasf mk_size;
    enum Gasf mk_color;
    enum Gasf tx_fp;
    enum Gasf tx_exp;
    enum Gasf tx_space;
    enum Gasf tx_color;
    enum Gasf fl_inter;
    enum Gasf fl_style;
    enum Gasf fl_color;
};

struct Gidim
{
    Guint x_dim;
    Guint y_dim;
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

struct Gcobundl
{
    Gfloat red;
    Gfloat green;
    Gfloat blue;
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

struct Ggdp_datarec
{
    Gint number_integer;
    Gint number_float;
    Gint number_strings;
    Gint *list_integers;
    Gfloat *list_floats;
    Gint *list_string_lengths;
    Gchar **list_strings;
};

struct Ggdprec
{
    union
    {
        struct Ggdp_datarec gugdp_datarec;
    };
};

struct Gintlist
{
    Gint number;
    Gint *integers;
};

struct Gpoint
{
    Gfloat x;
    Gfloat y;
};

struct Grect
{
    struct Gpoint ul;
    struct Gpoint lr;
};

struct Gstrlist
{
    Gint n_points;
    Gchar **strings;
};

struct Gtran
{
    struct Glimit w;
    struct Glimit v;
};

struct Gtxalign
{
    enum Gtxhor hor;
    enum Gtxver ver;
};

struct Gtxfp
{
    Gint font;
    enum Gtxprec prec;
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

void ginqasf(struct Gasfs *value, Gint *errorStatus);
void ginqavailwstypes(Gint bufSize, Gint start, struct Gstrlist *wsTypes, Gint *numTypes, Gint *errorStatus);
void ginqcharexpan(Gfloat *value, Gint *errorStatus);
void ginqcharheight(Gfloat *value, Gint *errorStatus);
void ginqcharspace(Gfloat *value, Gint *errorStatus);
void ginqcharup(struct Gpoint *value, Gint *errorStatus);
void ginqclip(struct Gcliprect *value, Gint *errorStatus);
void ginqcolourfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, struct Gcofac *facil, Gint *errorStatus);
void ginqfillcolorind(Gint *value, Gint *errorStatus);
void ginqfillind(Gint *value, Gint *errorStatus);
void ginqfillstyle(enum Gflinter *value, Gint *errorStatus);
void ginqfillstyleind(Gint *value, Gint *errorStatus);
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
void ginqopenws(Gint maxIds, Gint start, struct Gintlist *wsids, Gint *actualIds, Gint *errorStatus);
void ginqopst(enum Gopst *value);
void ginqpatrefpt(struct Gpoint *value, Gint *errorStatus);
void ginqpatsize(struct Gpoint *value, Gint *errorStatus);
void ginqtextalign(struct Gtxalign *value, Gint *errorStatus);
void ginqtextcolorind(Gint *value, Gint *errorStatus);
void ginqtextfontprec(struct Gtxfp *value, Gint *errorStatus);
void ginqtextind(Gint *value, Gint *errorStatus);
void ginqwsmaxnum(struct Gwsmax *value, Gint *errorStatus);

void gsetasf(struct Gasfs *value);
void gsetcharexpan(Gfloat value);
void gsetcharheight(Gfloat value);
void gsetcharspace(Gfloat value);
void gsetcharup(struct Gpoint *value);
void gsetclip(enum Gclip value);
void gsetfillcolorind(Gint value);
void gsetfillind(Gint value);
void gsetfillstyle(enum Gflinter value);
void gsetfillstyleind(Gint value);
void gsetlinecolorind(Gint value);
void gsetlineind(Gint value);
void gsetlinetype(Gint value);
void gsetmarkercolorind(Gint value);
void gsetmarkerind(Gint value);
void gsetmarkersize(Gfloat value);
void gsetmarkertype(Gint value);
void gsettextalign(struct Gtxalign *value);
void gsettextcolorind(Gint value);
void gsettextfontprec(struct Gtxfp *fontPrec);
void gsettextind(Gint value);
void gsetviewport(Gint transform, struct Glimit *value);
void gsetwindow(Gint transform, struct Glimit *value);

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
void gclosews(Gint wsId);

void gactivatews(Gint wsId);
void gclearws(Gint wsId, enum Gclrflag flag);
void gdeactivatews(Gint wsId);
void gupdatews(Gint wsId, enum Gregen flag);

void ginqcolorrep(Gint wsId, Gint index, struct Gcobundl *value);
void ginqwstran(Gint wsId, struct Gwsti *value, Gint *errorStatus);

void gsetcolorrep(Gint wsId, Gint index, struct Gcobundl *value);
void gsetwsviewport(Gint wsId, struct Glimit *value);
void gsetwswindow(Gint wsId, struct Glimit *value);

void gcellarray(struct Grect *rect, struct Gidim *dims, Gint *colors);
void gfillarea(Gint numPoints, struct Gpoint *points);
void ggdp(Gint numPoints, struct Gpoint *points, Gint gdpId, struct Ggdprec *data);
void gpolyline(Gint numPoints, struct Gpoint *points);
void gpolymarker(Gint numPoints, struct Gpoint *points);
void gtext(struct Gpoint *start, const Gchar *text);

#if defined(__cplusplus)
}
#endif

#endif
