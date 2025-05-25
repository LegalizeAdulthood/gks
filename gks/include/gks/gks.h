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

typedef struct Gpoint
{
    Gfloat x;
    Gfloat y;
} Gpoint;

typedef struct Gextent
{
    Gpoint concat;
    Gpoint corner_1;
    Gpoint corner_2;
    Gpoint corner_3;
    Gpoint corner_4;
} Gextent;

typedef struct Gintlist
{
    Gint number;
    Gint *integers;
} Gintlist;

typedef struct Gflfac
{
    Gint n_interiors;
    Gflinter *interiors;
    Gint *hatches;
    Gint predefined;
} Gflfac;

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

typedef struct Glnfac
{
    Gintlist types;
    Gint widths;
    Gfloat nom_width;
    Gfloat min_width;
    Gfloat max_width;
    Gint predefined;
} Glnfac;

typedef struct Gmkfac
{
    Gintlist types;
    Gint sizes;
    Gfloat nom_size;
    Gfloat min_size;
    Gfloat max_size;
    Gint predefined;
} Gmkfac;

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

typedef struct Gtxfac
{
    Gint fps;
    Gtxfp *fp_list;
    Gint heights;
    Gfloat min_ht;
    Gfloat max_ht;
    Gint expansions;
    Gfloat min_exp;
    Gfloat max_exp;
    Gint predefined;
} Gtxfac;

typedef struct Gwsct
{
    const Gconn *conn;
    Gwstype type;
} Gwsct;

typedef struct Gwsmax
{
    Gint open;
    Gint active;
    Gint assoc;
} Gwsmax;

typedef struct Gwstables
{
    Gint line;
    Gint mark;
    Gint text;
    Gint fill;
    Gint pat;
    Gint color;
} Gwstables;

typedef struct Gwsti
{
    Gwstus wstus;
    Gtran request;
    Gtran current;
} Gwsti;

Gint gerrorhand(Gint errNum, Gint funcName, Gfile *errFile);
Gint gerrorlog(Gint errNum, Gint funcName, Gfile *errFile);

Gint gescape(Gint function, Gescin *inData, Gint bufSize, Gescout *outData, Gint *escOutSize);

Gint gopengks(Gfile *errfile, Glong memory);
Gint gclosegks(void);

Gint ginqasf(Gasfs *value);
Gint ginqavailwstypes(Gint bufSize, Gint start, Gintlist *wsTypes, Gint *numTypes);
Gint ginqcharexpan(Gfloat *value);
Gint ginqcharbase(Gpoint *value);
Gint ginqcharheight(Gfloat *value);
Gint ginqcharspace(Gfloat *value);
Gint ginqcharwidth(Gfloat *value);
Gint ginqcharup(Gpoint *value);
Gint ginqclip(Gcliprect *value);
Gint ginqcurrntrannum(Gint *value);
Gint ginqfillcolorind(Gint *value);
Gint ginqfillind(Gint *value);
Gint ginqfillstyle(Gflinter *value);
Gint ginqfillstyleind(Gint *value);
Gint ginqlevelgks(Glevel *value);
Gint ginqlinecolorind(Gint *value);
Gint ginqlineind(Gint *value);
Gint ginqlinetype(Gint *value);
Gint ginqmarkercolorind(Gint *value);
Gint ginqmarkerind(Gint *value);
Gint ginqmarkersize(Gfloat *value);
Gint ginqmarkertype(Gint *value);
Gint ginqmaxntrannum(Gint *value);
Gint ginqntran(Gint num, Gtran *value);
Gint ginqopenws(Gint maxIds, Gint start, Gintlist *wsids, Gint *actualIds);
Gint ginqopst(Gopst *value);
Gint ginqpatheight(Gpoint *value);
Gint ginqpatrefpt(Gpoint *value);
Gint ginqpatwidth(Gpoint *value);
Gint ginqtextalign(Gtxalign *value);
Gint ginqtextcolorind(Gint *value);
Gint ginqtextfontprec(Gtxfp *value);
Gint ginqtextind(Gint *value);
Gint ginqwsmaxnum(Gwsmax *value);

Gint ginqcolorfacil(Gwstype wsType, Gint buffSize, Gint *numColorIndices, Gcofac *value);
Gint ginqdisplaysize(Gwstype wsType, Gdspsize *value);
Gint ginqfillfacil(Gwstype wsType, Gint buffSize, Gint *facilSize, Gflfac *value);
Gint ginqlinefacil(Gwstype wsType, Gint buffSize, Gint *numLineTypes, Glnfac *value);
Gint ginqmarkerfacil(Gwstype wsType, Gint buffSIze, Gint *numMarkerTypes, Gmkfac *value);
Gint ginqtextfacil(Gwstype wsType, Gint buffSize, Gint *numFontPrecs, Gtxfac *value);
Gint ginqmaxwssttables(Gwstype wsType, Gwstables *value);

Gint gselntran(Gint value);
Gint gsetasf(Gasfs *value);
Gint gsetcharexpan(Gfloat value);
Gint gsetcharheight(Gfloat value);
Gint gsetcharspace(Gfloat value);
Gint gsetcharup(Gpoint *value);
Gint gsetclip(Gclip value);
Gint gsetfillcolorind(Gint value);
Gint gsetfillind(Gint value);
Gint gsetfillstyle(Gflinter value);
Gint gsetfillstyleind(Gint value);
Gint gsetlinecolorind(Gint value);
Gint gsetlineind(Gint value);
Gint gsetlinetype(Gint value);
Gint gsetmarkercolorind(Gint value);
Gint gsetmarkerind(Gint value);
Gint gsetmarkersize(Gfloat value);
Gint gsetmarkertype(Gint value);
Gint gsettextalign(Gtxalign *value);
Gint gsettextcolorind(Gint value);
Gint gsettextfontprec(Gtxfp *fontPrec);
Gint gsettextind(Gint value);
Gint gsetviewport(Gint transform, Glimit *value);
Gint gsetwindow(Gint tranId, Glimit *value);

Gint gopenws(Gint wsId, const Gconn *connId, Gwstype wsType);
Gint gclosews(Gint wsId);

Gint gactivatews(Gint wsId);
Gint gclearws(Gint wsId, Gclrflag flag);
Gint gdeactivatews(Gint wsId);
Gint gupdatews(Gint wsId, Gregen flag);

Gint ginqcolorindices(Gint wsId, Gint sizeIndices, Gint start, Gintlist *indices, Gint *numIndices);
Gint ginqcolorrep(Gint wsId, Gint index, Gcobundl *value);
Gint ginqtextextent(Gint wsId, Gpoint *pos, const Gchar *text, Gextent *value);
Gint ginqwsconntype(Gint wsId, Gint buffSize, Gint *ctSize, Gwsct *ct);
Gint ginqwstran(Gint wsId, Gwsti *value);

Gint gsetcolorrep(Gint wsId, Gint index, Gcobundl *value);
Gint gsetwsviewport(Gint wsId, Glimit *value);
Gint gsetwswindow(Gint wsId, Glimit *value);

Gint gcellarray(Grect *rect, Gidim *dims, Gint *colors);
Gint gfillarea(Gint numPoints, Gpoint *points);
Gint ggdp(Gint numPoints, Gpoint *points, Gint gdpId, Ggdprec *data);
Gint gpolyline(Gint numPoints, Gpoint *points);
Gint gpolymarker(Gint numPoints, Gpoint *points);
Gint gtext(Gpoint *start, const Gchar *text);

#if defined(__cplusplus)
}
#endif

#endif
