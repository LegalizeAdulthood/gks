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

// Predefined workstation types
#define GWSTYPE_TEK4105 1

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

typedef enum Gasf
{
    GBUNDLED = 1,
    GINDIVIDUAL
} Gasf;

typedef enum Gclip
{
    GNOCLIP = 1,
    GCLIP
} Gclip;

typedef enum Gclrflag
{
    GCONDITIONALLY = 1,
    GALWAYS
} Gclrflag;

typedef enum Gcoavail
{
    GCOLOR = 1,
    GMONOCHROME
} Gcoavail;

typedef enum Gdevunits
{
    GDC_METERS,
    GDC_OTHER
} Gdevunits;

typedef enum Gflinter
{
    GHOLLOW = 1,
    GSOLID,
    GPATTERN,
    GHATCH
} Gflinter;

typedef enum Glevel
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
} Glevel;

typedef enum Gopst
{
    GGKCL = 1,
    GGKOP,
    GWSOP,
    GWSAC,
    GSGOP
} Gopst;

typedef enum Gregen
{
    GPOSTPONE = 1,
    GPERFORM
} Gregen;

typedef enum Gtxhor
{
    GAH_NORMAL = 1,
    GAH_LEFT,
    GAH_CENTER,
    GAH_RIGHT
} Gtxhor;

typedef enum Gtxprec
{
    GP_STRING = 1,
    GP_CHAR,
    GP_STROKE
} Gtxprec;

typedef enum Gtxver
{
    GAV_NORMAL = 1,
    GAV_TOP,
    GAV_CAP,
    GAV_HALF,
    GAV_BASE,
    GAV_BOTTOM
} Gtxver;

typedef enum Gwstus
{
    GNOTPENDING = 1,
    GPENDING
} Gwstus;


typedef struct Gasfs
{
    Gasf ln_type;
    Gasf ln_width;
    Gasf ln_color;
    Gasf mk_type;
    Gasf mk_size;
    Gasf mk_color;
    Gasf tx_fp;
    Gasf tx_exp;
    Gasf tx_space;
    Gasf tx_color;
    Gasf fl_inter;
    Gasf fl_style;
    Gasf fl_color;
} Gasfs;

typedef struct Gidim
{
    Guint x_dim;
    Guint y_dim;
} Gidim;

typedef struct Glimit
{
    Gfloat xmin;
    Gfloat xmax;
    Gfloat ymin;
    Gfloat ymax;
} Glimit;

typedef struct Gcliprect
{
    Gclip ind;
    Glimit rec;
} Gcliprect;

typedef struct Gcobundl
{
    Gfloat red;
    Gfloat green;
    Gfloat blue;
} Gcobundl;

typedef struct Gcofac
{
    Gint colors;
    Gcoavail coavail;
    Gint predefined;
} Gcofac;

typedef struct Guesc_idatarec
{
    Gint number_integer;
    Gint number_float;
    Gint number_strings;
    Gint *list_integers;
    Gfloat *list_floats;
    Gint *list_string_lengths;
    Gchar **list_strings;
} Guesc_idatarec;

typedef union Gescin
{
    Guesc_idatarec esc_idatarec;
} Gescin;

typedef struct Guesc_odatarec
{
    Gint number_integer;
    Gint number_float;
    Gint number_strings;
    Gint *list_integers;
    Gfloat *list_floats;
    Gint *list_string_lengths;
    Gchar **list_strings;
} Guesc_odatarec;

typedef union Gescout
{
    Guesc_odatarec esc_odatarec;
} Gescout;

typedef struct Ggdp_datarec
{
    Gint number_integer;
    Gint number_float;
    Gint number_strings;
    Gint *list_integers;
    Gfloat *list_floats;
    Gint *list_string_lengths;
    Gchar **list_strings;
} Ggdp_datarec;

typedef union Ggdprec
{
    Ggdp_datarec gugdp_datarec;
} Ggdprec;

typedef struct Gpoint
{
    Gfloat x;
    Gfloat y;
} Gpoint;

typedef struct Gipoint
{
    Gint x;
    Gint y;
} Gipoint;

typedef struct Gdspsize
{
    Gdevunits units;
    Gpoint device;
    Gipoint raster;
} Gdspsize;

typedef struct Gintlist
{
    Gint number;
    Gint *integers;
} Gintlist;

typedef struct Grect
{
    Gpoint ul;
    Gpoint lr;
} Grect;

typedef struct Gstrlist
{
    Gint n_points;
    Gchar **strings;
} Gstrlist;

typedef struct Gtran
{
    Glimit w;
    Glimit v;
} Gtran;

typedef struct Gtxalign
{
    Gtxhor hor;
    Gtxver ver;
} Gtxalign;

typedef struct Gtxfp
{
    Gint font;
    Gtxprec prec;
} Gtxfp;

typedef struct Gwsmax
{
    Gint open;
    Gint active;
    Gint assoc;
} Gwsmax;

typedef struct Gwsti
{
    Gwstus wstus;
    Gtran request;
    Gtran current;
} Gwsti;

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);

void gescape(Gint function, Gescin *inData, Gint bufSize, Gescout *outData, Gint *escOutSize);

void gopengks(Gfile *errfile, Glong memory);
void gclosegks(void);

void ginqasf(Gasfs *value, Gint *errorStatus);
void ginqavailwstypes(Gint bufSize, Gint start, Gintlist *wsTypes, Gint *numTypes, Gint *errorStatus);
void ginqcharexpan(Gfloat *value, Gint *errorStatus);
void ginqcharbase(Gpoint *value, Gint *errorStatus);
void ginqcharheight(Gfloat *value, Gint *errorStatus);
void ginqcharspace(Gfloat *value, Gint *errorStatus);
void ginqcharwidth(Gfloat *value, Gint *errorStatus);
void ginqcharup(Gpoint *value, Gint *errorStatus);
void ginqclip(Gcliprect *value, Gint *errorStatus);
void ginqfillcolorind(Gint *value, Gint *errorStatus);
void ginqfillind(Gint *value, Gint *errorStatus);
void ginqfillstyle(Gflinter *value, Gint *errorStatus);
void ginqfillstyleind(Gint *value, Gint *errorStatus);
void ginqlevelgks(Glevel *value, Gint *errorStatus);
void ginqlinecolorind(Gint *value, Gint *errorStatus);
void ginqlineind(Gint *value, Gint *errorStatus);
void ginqlinetype(Gint *value, Gint *errorStatus);
void ginqmarkercolorind(Gint *value, Gint *errorStatus);
void ginqmarkerind(Gint *value, Gint *errorStatus);
void ginqmarkersize(Gfloat *value, Gint *errorStatus);
void ginqmarkertype(Gint *value, Gint *errorStatus);
void ginqmaxntrannum(Gint *value, Gint *errorStatus);
void ginqntran(Gint num, Gtran *value, Gint *errorStatus);
void ginqopenws(Gint maxIds, Gint start, Gintlist *wsids, Gint *actualIds, Gint *errorStatus);
void ginqopst(Gopst *value);
void ginqpatrefpt(Gpoint *value, Gint *errorStatus);
void ginqpatsize(Gpoint *value, Gint *errorStatus);
void ginqtextalign(Gtxalign *value, Gint *errorStatus);
void ginqtextcolorind(Gint *value, Gint *errorStatus);
void ginqtextfontprec(Gtxfp *value, Gint *errorStatus);
void ginqtextind(Gint *value, Gint *errorStatus);
void ginqwsmaxnum(Gwsmax *value, Gint *errorStatus);

void ginqcolorfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gcofac *facil, Gint *errorStatus);
void ginqdisplaysize(Gwstype wsType, Gdspsize *size, Gint *errorStatus);

void gsetasf(Gasfs *value);
void gsetcharexpan(Gfloat value);
void gsetcharheight(Gfloat value);
void gsetcharspace(Gfloat value);
void gsetcharup(Gpoint *value);
void gsetclip(Gclip value);
void gsetfillcolorind(Gint value);
void gsetfillind(Gint value);
void gsetfillstyle(Gflinter value);
void gsetfillstyleind(Gint value);
void gsetlinecolorind(Gint value);
void gsetlineind(Gint value);
void gsetlinetype(Gint value);
void gsetmarkercolorind(Gint value);
void gsetmarkerind(Gint value);
void gsetmarkersize(Gfloat value);
void gsetmarkertype(Gint value);
void gsettextalign(Gtxalign *value);
void gsettextcolorind(Gint value);
void gsettextfontprec(Gtxfp *fontPrec);
void gsettextind(Gint value);
void gsetviewport(Gint transform, Glimit *value);
void gsetwindow(Gint transform, Glimit *value);

void gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
void gclosews(Gint wsId);

void gactivatews(Gint wsId);
void gclearws(Gint wsId, Gclrflag flag);
void gdeactivatews(Gint wsId);
void gupdatews(Gint wsId, Gregen flag);

void ginqcolorrep(Gint wsId, Gint index, Gcobundl *value);
void ginqwstran(Gint wsId, Gwsti *value, Gint *errorStatus);

void gsetcolorrep(Gint wsId, Gint index, Gcobundl *value);
void gsetwsviewport(Gint wsId, Glimit *value);
void gsetwswindow(Gint wsId, Glimit *value);

void gcellarray(Grect *rect, Gidim *dims, Gint *colors);
void gfillarea(Gint numPoints, Gpoint *points);
void ggdp(Gint numPoints, Gpoint *points, Gint gdpId, Ggdprec *data);
void gpolyline(Gint numPoints, Gpoint *points);
void gpolymarker(Gint numPoints, Gpoint *points);
void gtext(Gpoint *start, const Gchar *text);

#if defined(__cplusplus)
}
#endif

#endif
