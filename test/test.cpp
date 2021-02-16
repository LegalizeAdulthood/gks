#include "error.h"

#include <gks/gks.h>

#include <catch2/catch.hpp>

#include <cstdio>

TEST_CASE("Initially closed", "[gks]")
{
    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("Opened", "[gks]")
{
    gopengks(stderr, 0L);

    REQUIRE(getGksOpState() == GGKOP);

    gclosegks();
}

TEST_CASE("Opened and closed", "[gks]")
{
    gopengks(stderr, 0L);

    gclosegks();

    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("Error file must be non-NULL", "[gks]")
{
    g_recordedErrors.clear();

    gopengks(nullptr, 0L);

    requireError(GERROR_INVALID_ERROR_FILE, GFN_OPEN_GKS);
    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("Error handling", "[gks]")
{
    gerrorhand(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS, stderr);
}

TEST_CASE("GKS state list", "[gks]")
{
    gopengks(stderr, 0L);
    Gint status{-1};

    SECTION("GKS level")
    {
        Glevel level{};
        ginqlevelgks(&level, &status);

        REQUIRE(level == GLMA);
    }
    SECTION("Availale workstation types")
    {
        const Gint numWsTypes{10};
        Gint start{};
        int wsTypeValues[numWsTypes];
        for (int &wsTypeValue : wsTypeValues)
        {
            wsTypeValue = -1;
        }
        Gintlist wsTypes{0, wsTypeValues};
        Gint numAvailTypes{};
        ginqavailwstypes(numWsTypes, start, &wsTypes, &numAvailTypes, &status);

        REQUIRE(numAvailTypes == 1);
        REQUIRE(wsTypes.number == 1);
        REQUIRE(wsTypes.integers[0] == GWSTYPE_TEK4105);
    }
    SECTION("Number of normalization transforms")
    {
        Gint numTransforms{-1};
        ginqmaxntrannum(&numTransforms, &status);

        REQUIRE(numTransforms == 2);
    }
    SECTION("Normalization transform zero is identity")
    {
        Gtran transform{};
        ginqntran(0, &transform, &status);

        REQUIRE(transform.w.xmin == 0.0f);
        REQUIRE(transform.w.xmax == 1.0f);
        REQUIRE(transform.w.ymin == 0.0f);
        REQUIRE(transform.w.ymax == 1.0f);
        REQUIRE(transform.v.xmin == 0.0f);
        REQUIRE(transform.v.xmax == 1.0f);
        REQUIRE(transform.v.ymin == 0.0f);
        REQUIRE(transform.v.ymax == 1.0f);
    }
    SECTION("Set viewport")
    {
        const Gint tranId = 1;
        const Gfloat xmin{0.2f};
        const Gfloat xmax{0.5f};
        const Gfloat ymin{0.4f};
        const Gfloat ymax{0.6f};
        Glimit viewport{xmin, xmax, ymin, ymax};
        gsetviewport(tranId, &viewport);

        Gtran transform{};
        ginqntran(tranId, &transform, &status);
        REQUIRE(transform.v.xmin == xmin);
        REQUIRE(transform.v.xmax == xmax);
        REQUIRE(transform.v.ymin == ymin);
        REQUIRE(transform.v.ymax == ymax);
    }
    SECTION("Set window")
    {
        const Gint tranId = 1;
        const Gfloat xmin{0.2f};
        const Gfloat xmax{0.5f};
        const Gfloat ymin{0.4f};
        const Gfloat ymax{0.6f};
        Glimit window{xmin, xmax, ymin, ymax};
        gsetwindow(tranId, &window);

        Gtran transform{};
        ginqntran(tranId, &transform, &status);
        REQUIRE(transform.w.xmin == xmin);
        REQUIRE(transform.w.xmax == xmax);
        REQUIRE(transform.w.ymin == ymin);
        REQUIRE(transform.w.ymax == ymax);
    }
    SECTION("Maximum workstation numbers")
    {
        Gwsmax value{};
        ginqwsmaxnum(&value, &status);

        REQUIRE(value.open == 1);
        REQUIRE(value.active == 1);
        REQUIRE(value.assoc == 0);
    }
    SECTION("Initial clipping indicator")
    {
        Gcliprect clipping{};
        ginqclip(&clipping, &status);

        REQUIRE(clipping.ind == GCLIP);
        REQUIRE(clipping.rec.xmin == 0.0f);
        REQUIRE(clipping.rec.xmax == 1.0f);
        REQUIRE(clipping.rec.ymin == 0.0f);
        REQUIRE(clipping.rec.ymax == 1.0f);
    }
    SECTION("Initial set of open workstations is empty")
    {
        const Gint numBuffIds = 10;
        Gint ids[numBuffIds]{};
        Gintlist idList{numBuffIds, ids};
        Gint numIds{-1};
        ginqopenws(numBuffIds, 0, &idList, &numIds, &status);

        REQUIRE(numIds == 0);
    }

    REQUIRE(status == 0);

    gclosegks();
}

TEST_CASE("workstation description table", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint status{-1};

    SECTION("color facilities")
    {
        Gcofac facil{};
        ginqcolorfacil(GWSTYPE_TEK4105, 0, NULL, &facil, &status);

        REQUIRE(facil.colors == 16);
        REQUIRE(facil.coavail == GCOLOR);
        REQUIRE(facil.predefined == 16);
    }
    SECTION("display space size")
    {
        Gdspsize size{};
        ginqdisplaysize(GWSTYPE_TEK4105, &size, &status);

        REQUIRE(size.units == GDC_OTHER);
        REQUIRE(size.raster.x == 640);
        REQUIRE(size.raster.y == 480);
    }
    SECTION("fill area facilities")
    {
        Gflinter interiorStyles[4]{};
        Gint hatchStyles[10]{};
        Gflfac facil{};
        facil.interiors = interiorStyles;
        facil.hatches = hatchStyles;
        Gint numAvailHatches{};
        ginqfillfacil(GWSTYPE_TEK4105, numOf(hatchStyles), &numAvailHatches, &facil, &status);

        REQUIRE(facil.n_interiors == 1);
        REQUIRE(facil.interiors[0] == GHOLLOW);
        REQUIRE(facil.predefined == 1);
        REQUIRE(numAvailHatches == 0);
    }

    REQUIRE(status == 0);

    gclosegks();
}

TEST_CASE("set clipping indicator", "[gks]")
{
    gopengks(stderr, 0L);

    gsetclip(GNOCLIP);

    Gcliprect clipping{};
    Gint status{-1};
    ginqclip(&clipping, &status);
    REQUIRE(clipping.ind == GNOCLIP);

    gclosegks();
}

TEST_CASE("open workstation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);

    SECTION("operating state is GSOP")
    {
        REQUIRE(getGksOpState() == GWSOP);
    }
    SECTION("after activate workstation state is GWSAC")
    {
        gactivatews(wsId);

        REQUIRE(getGksOpState() == GWSAC);

        gdeactivatews(wsId);
    }
    SECTION("deactivate last activated workstation state is GWSOP")
    {
        gactivatews(wsId);

        gdeactivatews(wsId);

        REQUIRE(getGksOpState() == GWSOP);
    }
    SECTION("clear workstation")
    {
        gactivatews(wsId);

        gclearws(wsId, GALWAYS);

        gdeactivatews(wsId);
    }
    SECTION("initial workstation transformation is identity")
    {
        struct Gwsti transform{};
        Gint status{-1};
        ginqwstran(wsId, &transform, &status);

        REQUIRE(transform.wstus == GNOTPENDING);
        REQUIRE(transform.request.v.xmin == 0.0f);
        REQUIRE(transform.request.v.xmax == 1.0f);
        REQUIRE(transform.request.v.ymin == 0.0f);
        REQUIRE(transform.request.v.ymax == 1.0f);
        REQUIRE(transform.request.w.xmin == 0.0f);
        REQUIRE(transform.request.w.xmax == 1.0f);
        REQUIRE(transform.request.w.ymin == 0.0f);
        REQUIRE(transform.request.w.ymax == 1.0f);
        REQUIRE(transform.current.v.xmin == 0.0f);
        REQUIRE(transform.current.v.xmax == 1.0f);
        REQUIRE(transform.current.v.ymin == 0.0f);
        REQUIRE(transform.current.v.ymax == 1.0f);
        REQUIRE(transform.current.w.xmin == 0.0f);
        REQUIRE(transform.current.w.xmax == 1.0f);
        REQUIRE(transform.current.w.ymin == 0.0f);
        REQUIRE(transform.current.w.ymax == 1.0f);
    }
    SECTION("update workstation")
    {
        gupdatews(wsId, GPERFORM);
    }
    SECTION("set workstation viewport")
    {
        const Gfloat xmin = 0.2f;
        const Gfloat xmax = 0.4f;
        const Gfloat ymin = 0.3f;
        const Gfloat ymax = 0.5f;
        struct Glimit viewport{xmin, xmax, ymin, ymax};
        gsetwsviewport(wsId, &viewport);

        struct Gwsti transform{};
        Gint status{-1};
        ginqwstran(wsId, &transform, &status);

        REQUIRE(transform.wstus == GNOTPENDING);
        REQUIRE(transform.current.v.xmin == xmin);
        REQUIRE(transform.current.v.xmax == xmax);
        REQUIRE(transform.current.v.ymin == ymin);
        REQUIRE(transform.current.v.ymax == ymax);
    }
    SECTION("set workstation window")
    {
        const Gfloat xmin = 0.2f;
        const Gfloat xmax = 0.4f;
        const Gfloat ymin = 0.3f;
        const Gfloat ymax = 0.5f;
        struct Glimit window{xmin, xmax, ymin, ymax};
        gsetwswindow(wsId, &window);

        struct Gwsti transform{};
        Gint status{-1};
        ginqwstran(wsId, &transform, &status);

        REQUIRE(transform.wstus == GNOTPENDING);
        REQUIRE(transform.current.w.xmin == xmin);
        REQUIRE(transform.current.w.xmax == xmax);
        REQUIRE(transform.current.w.ymin == ymin);
        REQUIRE(transform.current.w.ymax == ymax);
    }

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("open workstation error handling", "[workstation]")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    REQUIRE(getGksOpState() == GGKOP);
    Gint wsId{};
    const Gchar *connId{"tek4105"};
    Gwstype wsType{GWSTYPE_TEK4105};

    SECTION("workstation id negative")
    {
        wsId = -1;
        gopenws(wsId, connId, wsType);

        requireError(GERROR_INVALID_WSID, GFN_OPEN_WORKSTATION);
    }
    SECTION("workstation id too large")
    {
        const Gint maxOpenWorkstations{1};
        wsId = maxOpenWorkstations;
        gopenws(wsId, connId, wsType);

        requireError(GERROR_INVALID_WSID, GFN_OPEN_WORKSTATION);
    }
    SECTION("invalid workstation type")
    {
        ++wsType;
        gopenws(wsId, connId, wsType);

        requireError(GERROR_INVALID_WSTYPE, GFN_OPEN_WORKSTATION);
    } 
    SECTION("workstation already open")
    {
        gopenws(wsId, connId, wsType);
        REQUIRE(getGksOpState() == GWSOP);

        gopenws(wsId, connId, wsType);

        requireError(GERROR_WS_IS_OPEN, GFN_OPEN_WORKSTATION);

        gclosews(wsId);
    }

    REQUIRE(getGksOpState() == GGKOP);

    gclosegks();
}

TEST_CASE("close workstation error handling", "[workstation]")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    Gint wsId{};
    const Gchar *connId{"tek4105"};
    Gwstype wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);
    REQUIRE(getGksOpState() == GWSOP);
    REQUIRE(g_recordedErrors.empty());

    SECTION("workstation id negative")
    {
        Gint badWsId{-1};
        gclosews(badWsId);

        requireError(GERROR_INVALID_WSID, GFN_CLOSE_WORKSTATION);
    }
    SECTION("workstation id too large")
    {
        const Gint maxOpenWorkstations{1};
        Gint badWsId{maxOpenWorkstations};
        gclosews(badWsId);

        requireError(GERROR_INVALID_WSID, GFN_CLOSE_WORKSTATION);
    }
    SECTION("workstation is active")
    {
        gactivatews(wsId);

        gclosews(wsId);

        requireError(GERROR_WS_IS_ACTIVE, GFN_CLOSE_WORKSTATION);

        gdeactivatews(wsId);
    }

    REQUIRE(getGksOpState() == GWSOP);
    gclosews(wsId);
    gclosegks();
}

TEST_CASE("activate workstation error handling", "[workstation]")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    Gint wsId{};
    const Gchar *connId{"tek4105"};
    Gwstype wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);
    REQUIRE(getGksOpState() == GWSOP);
    REQUIRE(g_recordedErrors.empty());

    SECTION("workstation id negative")
    {
        Gint badWsId{-1};
        gactivatews(badWsId);

        requireError(GERROR_INVALID_WSID, GFN_ACTIVATE_WORKSTATION);
    }
    SECTION("workstation id too large")
    {
        const Gint maxOpenWorkstations{1};
        Gint badWsId{maxOpenWorkstations};
        gactivatews(badWsId);

        requireError(GERROR_INVALID_WSID, GFN_ACTIVATE_WORKSTATION);
    }
    SECTION("workstation is active")
    {
        gactivatews(wsId);

        gactivatews(wsId);

        requireError(GERROR_WS_IS_ACTIVE, GFN_ACTIVATE_WORKSTATION);

        gdeactivatews(wsId);
    }

    REQUIRE(getGksOpState() == GWSOP);
    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Close last workstation enters GGKOP state", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);

    Gopst before{};
    ginqopst(&before);
    gclosews(wsId);
    Gopst after{};
    ginqopst(&after);

    REQUIRE(before == GWSOP);
    REQUIRE(after == GGKOP);

    gclosegks();
}

TEST_CASE("Output primitives", "[output]")
{
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);

    SECTION("Polyline")
    {
        Gpoint points[] =
        {
        { 0.0f, 0.5f },
        { 0.5f, 0.5f }
        };
        gpolyline(sizeof(points)/sizeof(points[0]), points);
    }
    SECTION("Polymarker")
    {
        Gpoint points[] =
        {
        { 0.0f, 0.5f },
        { 0.5f, 0.5f }
        };
        gpolymarker(sizeof(points)/sizeof(points[0]), points);
    }
    SECTION("Text")
    {
        Gpoint start{0.0f, 0.5f};
        gtext(&start, "Hello, world!");
    }
    SECTION("Fill Area")
    {
        Gpoint points[] = 
        {
        {0.25f, 0.25f},
        {0.50f, 0.25f},
        {0.50f, 0.50f},
        {0.25f, 0.50f}
        };
        gfillarea(sizeof(points)/sizeof(points[0]), points);
    }
    SECTION("Cell Array")
    {
        struct Grect rect =
        {
        {0.25f, 0.75f},
        {0.75f, 0.25f}
        };
        struct Gidim dim =
        {
        128, 128
        };
        Gint colors[] = { 0, 1 };
        gcellarray(&rect, &dim, colors);
    }
    SECTION("Generalized Drawing Primitive")
    {
        struct Gpoint points[] =
        {
        {0.1f, 0.1f},
        {0.2f, 0.2f}
        };
        Gint gdpId = 1;
        Ggdprec data{};
        ggdp(sizeof(points)/sizeof(points[0]), points, gdpId, &data);
    }

    gdeactivatews(wsId);
    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Initial global attribute values", "[output]")
{
    gopengks(stderr, 0L);
    Gint value{-1};
    Gint status{-1};

    SECTION("line type is 1 (solid)")
    {
        ginqlinetype(&value, &status);

        REQUIRE(value == GLN_SOLID);
    }
    SECTION("line color index is 1")
    {
        ginqlinecolorind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("line index is 1")
    {
        ginqlineind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("marker type is 3 (star)")
    {
        ginqmarkertype(&value, &status);

        REQUIRE(value == GMK_STAR);
    }
    SECTION("marker size is 1", "[output]")
    {
        Gfloat size{};
        ginqmarkersize(&size, &status);

        REQUIRE(size == 1.0f);
    }
    SECTION("marker color index is 1")
    {
        ginqmarkercolorind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("marker index is 1")
    {
        ginqmarkerind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("text height is 0.01", "[output]")
    {
        Gfloat height{};
        ginqcharheight(&height, &status);

        REQUIRE(height == 0.01f);
    }
    SECTION("char base vector is (1.00")
    {
        Gpoint base{-1.0f, -1.0f};
        ginqcharbase(&base, &status);

        REQUIRE(base.x == 1.0f);
        REQUIRE(base.y == 0.0f);
    }
    SECTION("char up vector is (0,1)")
    {
        Gpoint up{-1.0f, -1.0f};
        ginqcharup(&up, &status);

        REQUIRE(up.x == 0.0f);
        REQUIRE(up.y == 1.0f);
    }
    SECTION("character width is 0.01")
    {
        Gfloat width{-1.0f};
        ginqcharwidth(&width, &status);

        REQUIRE(width == 0.01f);
    }
    SECTION("text color index is 1")
    {
        ginqtextcolorind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("text index is 1", "[output]")
    {
        ginqtextind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("text alignment is normal")
    {
        Gtxalign align{};
        ginqtextalign(&align, &status);

        REQUIRE(align.hor == GAH_NORMAL);
        REQUIRE(align.ver == GAV_NORMAL);
    }
    SECTION("text font and precision is (1, string)")
    {
        Gtxfp fontPrec{};
        ginqtextfontprec(&fontPrec, &status);

        REQUIRE(fontPrec.font == 1);
        REQUIRE(fontPrec.prec == GP_STRING);
    }
    SECTION("char expansion factor is 1.0")
    {
        Gfloat expandFactor{};
        ginqcharexpan(&expandFactor, &status);

        REQUIRE(expandFactor == 1.0f);
    }
    SECTION("char spacing is 0.0")
    {
        Gfloat spacing{-1.0f};
        ginqcharspace(&spacing, &status);

        REQUIRE(spacing == 0.0f);
    }
    SECTION("fill area interior style is HOLLOW")
    {
        Gflinter style{GSOLID};
        ginqfillstyle(&style, &status);

        REQUIRE(style == GHOLLOW);
    }
    SECTION("fill area interior style index is 1", "[output]")
    {
        ginqfillstyleind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("fill area color index is 1")
    {
        ginqfillcolorind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("fill area index is 1")
    {
        ginqfillind(&value, &status);

        REQUIRE(value == 1);
    }
    SECTION("pattern size is (1,1)")
    {
        Gpoint size{};
        ginqpatsize(&size, &status);

        REQUIRE(size.x == 1.0f);
        REQUIRE(size.y == 1.0f);
    }
    SECTION("pattern reference point is (0,0)")
    {
        Gpoint refPt{1.0f, 1.0f};
        ginqpatrefpt(&refPt, &status);

        REQUIRE(refPt.x == 0.0f);
        REQUIRE(refPt.y == 0.0f);
    }
    SECTION("attribute source flags")
    {
        Gasfs asfs{};
        ginqasf(&asfs, &status);

        REQUIRE(asfs.ln_type == GINDIVIDUAL);
        REQUIRE(asfs.ln_width == GINDIVIDUAL);
        REQUIRE(asfs.ln_color == GINDIVIDUAL);
        REQUIRE(asfs.mk_type == GINDIVIDUAL);
        REQUIRE(asfs.mk_size == GINDIVIDUAL);
        REQUIRE(asfs.mk_color == GINDIVIDUAL);
        REQUIRE(asfs.tx_fp == GINDIVIDUAL);
        REQUIRE(asfs.tx_exp == GINDIVIDUAL);
        REQUIRE(asfs.tx_space == GINDIVIDUAL);
        REQUIRE(asfs.tx_color == GINDIVIDUAL);
        REQUIRE(asfs.fl_inter == GINDIVIDUAL);
        REQUIRE(asfs.fl_style == GINDIVIDUAL);
        REQUIRE(asfs.fl_color == GINDIVIDUAL);
    }
    SECTION("current normalization transform is 0")
    {
        Gint transform{-1};
        ginqcurrntrannum(&transform, &status);

        REQUIRE(transform == 0);
    }

    REQUIRE(status == 0);

    gclosegks();
}

TEST_CASE("Set global attribute values", "[output]")
{
    gopengks(stderr, 0L);
    Gint value{-1};
    Gint status{-1};

    SECTION("line type")
    {
        gsetlinetype(GLN_DASHED);

        ginqlinetype(&value, &status);
        REQUIRE(value == GLN_DASHED);
    }
    SECTION("line color index")
    {
        gsetlinecolorind(GLN_DASHDOT);

        ginqlinecolorind(&value, &status);
        REQUIRE(value == GLN_DASHDOT);
    }
    SECTION("line index")
    {
        gsetlineind(4);

        ginqlineind(&value, &status);
        REQUIRE(value == 4);
    }
    SECTION("marker type")
    {
        gsetmarkertype(GMK_DIAGONAL_CROSS);

        ginqmarkertype(&value, &status);
        REQUIRE(value == GMK_DIAGONAL_CROSS);
    }
    SECTION("marker size")
    {
        gsetmarkersize(2.0f);

        Gfloat size{-1.0f};
        ginqmarkersize(&size, &status);
        REQUIRE(size == 2.0f);
    }
    SECTION("marker color index")
    {
        Gint color{4};
        gsetmarkercolorind(color);

        ginqmarkercolorind(&value, &status);
        REQUIRE(value == color);
    }
    SECTION("marker index")
    {
        Gint index{4};
        gsetmarkerind(index);

        ginqmarkerind(&value, &status);
        REQUIRE(value == index);
    }
    SECTION("char height")
    {
        gsetcharheight(0.5f);

        Gfloat charHeight{-1.0f};
        ginqcharheight(&charHeight, &status);
        REQUIRE(charHeight == 0.5f);
    }
    SECTION("char up vector")
    {
        Gpoint up{1.0f, 0.0f};
        gsetcharup(&up);

        Gpoint current{};
        ginqcharup(&current, &status);
        REQUIRE(current.x == up.x);
        REQUIRE(current.y == up.y);
    }
    SECTION("text color index is 1")
    {
        const Gint color{3};
        gsettextcolorind(color);

        ginqtextcolorind(&value, &status);
        REQUIRE(value == color);
    }
    SECTION("text index")
    {
        const Gint index{3};
        gsettextind(index);

        ginqtextind(&value, &status);
        REQUIRE(value == index);
    }
    SECTION("text alignment")
    {
        Gtxalign align{GAH_LEFT, GAV_BOTTOM};
        gsettextalign(&align);

        Gtxalign current{};
        ginqtextalign(&current, &status);
        REQUIRE(current.hor == align.hor);
        REQUIRE(current.ver == align.ver);
    }
    SECTION("text font and precision")
    {
        Gtxfp font{2, GP_STROKE};
        gsettextfontprec(&font);

        Gtxfp current{};
        ginqtextfontprec(&current, &status);
        REQUIRE(current.font == font.font);
        REQUIRE(current.prec == font.prec);
    }
    SECTION("char expansion factor")
    {
        const Gfloat expan{0.5f};
        gsetcharexpan(expan);

        Gfloat current{};
        ginqcharexpan(&current, &status);
        REQUIRE(current == expan);
    }
    SECTION("char spacing")
    {
        const Gfloat space{0.5f};
        gsetcharspace(space);

        Gfloat current{};
        ginqcharspace(&current, &status);
        REQUIRE(current == space);
    }
    SECTION("fill area interior style")
    {
        gsetfillstyle(GSOLID);

        enum Gflinter current{};
        ginqfillstyle(&current, &status);
        REQUIRE(current == GSOLID);
    }
    SECTION("fill area interior style index")
    {
        gsetfillstyleind(2);

        ginqfillstyleind(&value, &status);
        REQUIRE(value == 2);
    }
    SECTION("fill area color index")
    {
        gsetfillcolorind(2);

        ginqfillcolorind(&value, &status);
        REQUIRE(value == 2);
    }
    SECTION("fill area index")
    {
        gsetfillind(2);

        ginqfillind(&value, &status);
        REQUIRE(value == 2);
    }
    SECTION("aspect source flags")
    {
        struct Gasfs asfs =
        {
            GBUNDLED,
            GINDIVIDUAL,
            GBUNDLED,
            GINDIVIDUAL,
            GBUNDLED,
            GINDIVIDUAL,
            GBUNDLED,
            GINDIVIDUAL,
            GBUNDLED,
            GINDIVIDUAL,
            GBUNDLED,
            GINDIVIDUAL,
            GBUNDLED,
        };
        gsetasf(&asfs);

        struct Gasfs current{};
        ginqasf(&current, &status);
        REQUIRE(current.ln_type == asfs.ln_type);
        REQUIRE(current.ln_width == asfs.ln_width);
        REQUIRE(current.ln_color == asfs.ln_color);
        REQUIRE(current.mk_type == asfs.mk_type);
        REQUIRE(current.mk_size == asfs.mk_size);
        REQUIRE(current.mk_color == asfs.mk_color);
        REQUIRE(current.tx_fp == asfs.tx_fp);
        REQUIRE(current.tx_exp == asfs.tx_exp);
        REQUIRE(current.tx_space == asfs.tx_space);
        REQUIRE(current.tx_color == asfs.tx_color);
        REQUIRE(current.fl_inter == asfs.fl_inter);
        REQUIRE(current.fl_style == asfs.fl_style);
        REQUIRE(current.fl_color == asfs.fl_color);
    }
    SECTION("normalization transform")
    {
        gselntran(1);

        Gint transform{0};
        ginqcurrntrannum(&transform, &status);
        REQUIRE(transform == 1);
    }

    REQUIRE(status == 0);

    gclosegks();
}

TEST_CASE("Workstation dependent attribute values", "[output]")
{
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);

    SECTION("color representation")
    {
        struct Gcobundl rep{0.5f, 0.5f, 0.5f};
        gsetcolorrep(wsId, 0, &rep);

        struct Gcobundl current{};
        Gint status{};
        ginqcolorrep(wsId, 0, &current, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(current.red == rep.red);
        REQUIRE(current.green == rep.green);
        REQUIRE(current.blue == rep.blue);
    }

    gclosews(wsId);
    gclosegks();
}
