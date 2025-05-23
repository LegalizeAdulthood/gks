#include "error.h"

#include <gks/gks.h>

#include <catch.hpp>

#include <cstdio>
#include <string>

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
        ginqntran(1, &transform, &status);

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
    const Gwstype wsType{GWSTYPE_TEK4105};
    Gint status{-1};

    SECTION("color facilities")
    {
        Gcofac facil{};
        Gint buffSize{};
        Gint numColors{};
        ginqcolorfacil(wsType, buffSize, &numColors, &facil, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(numColors == 16);
        REQUIRE(facil.colors == 16);
        REQUIRE(facil.coavail == GCOLOR);
        REQUIRE(facil.predefined == 16);
    }
    SECTION("display space size")
    {
        Gdspsize size{};
        ginqdisplaysize(wsType, &size, &status);

        REQUIRE(status == GERROR_NONE);
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
        ginqfillfacil(wsType, numOf(hatchStyles), &numAvailHatches, &facil, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(facil.n_interiors == 1);
        REQUIRE(facil.interiors[0] == GHOLLOW);
        REQUIRE(facil.predefined == 1);
        REQUIRE(numAvailHatches == 0);
    }
    SECTION("line facilities")
    {
        Gint lineTypes[10]{};
        Gint numLineTypes{};
        Glnfac facil{};
        facil.types.number = numOf(lineTypes);
        facil.types.integers = lineTypes;
        ginqlinefacil(wsType, numOf(lineTypes), &numLineTypes, &facil, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(numLineTypes >= 1);
        REQUIRE(std::find(std::begin(lineTypes), std::end(lineTypes), GLN_SOLID) != std::end(lineTypes));
        REQUIRE(facil.widths == 1);
        REQUIRE(facil.nom_width == 1.0f);
        REQUIRE(facil.min_width == 1.0f);
        REQUIRE(facil.max_width == 1.0f);
    }
    SECTION("marker facilities")
    {
        Gint markerTypes[10]{};
        Gint numMarkerTypes{};
        Gmkfac facil{};
        facil.types.number = numOf(markerTypes);
        facil.types.integers = markerTypes;
        ginqmarkerfacil(wsType, numOf(markerTypes), &numMarkerTypes, &facil, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(numMarkerTypes >= 5);
        Gint expectedTypes[]{GMK_DOT, GMK_PLUS, GMK_STAR, GMK_CIRCLE, GMK_DIAGONAL_CROSS};
        std::sort(std::begin(expectedTypes), std::end(expectedTypes));
        std::sort(markerTypes, markerTypes + numMarkerTypes);
        REQUIRE(std::equal(markerTypes, markerTypes + numMarkerTypes, std::begin(expectedTypes)));
        REQUIRE(facil.sizes == 1);
        REQUIRE(facil.nom_size == 1.0f);
        REQUIRE(facil.min_size == 1.0f);
        REQUIRE(facil.max_size == 1.0f);
    }
    SECTION("text facilities")
    {
        Gtxfp fontPrec[10]{};
        Gint buffSize{numOf(fontPrec)};
        Gint numFontPrec{};
        Gtxfac facil{};
        facil.fp_list = fontPrec;
        ginqtextfacil(wsType, buffSize, &numFontPrec, &facil, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(numFontPrec == 1);
        REQUIRE(facil.fps == 1);
        REQUIRE(facil.fp_list[0].font == 1);
        REQUIRE(facil.fp_list[0].prec == GP_STRING);
    }
    SECTION("workstation state table lengths")
    {
        Gwstables tables{-1, -1, -1, -1, -1, -1};
        ginqmaxwssttables(wsType, &tables, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(tables.line == 0);
        REQUIRE(tables.mark == 0);
        REQUIRE(tables.text == 0);
        REQUIRE(tables.fill == 0);
        REQUIRE(tables.pat == 0);
        REQUIRE(tables.color == 16);
    }

    gclosegks();
}

TEST_CASE("workstation description table error handling", "[workstation]")
{
    gopengks(stderr, 0L);
    const Gwstype badWsType{GWSTYPE_TEK4105 + 1};
    Gint status{-1};

    SECTION("color facilities")
    {
        Gcofac facil{};
        Gint buffSize{};
        Gint numColors{};
        ginqcolorfacil(badWsType, buffSize, &numColors, &facil, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }
    SECTION("display space size")
    {
        Gdspsize size{};
        ginqdisplaysize(badWsType, &size, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }
    SECTION("fill area facilities")
    {
        Gflinter interiorStyles[4]{};
        Gint hatchStyles[10]{};
        Gflfac facil{};
        facil.interiors = interiorStyles;
        facil.hatches = hatchStyles;
        Gint numAvailHatches{};
        ginqfillfacil(badWsType, numOf(hatchStyles), &numAvailHatches, &facil, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }
    SECTION("line facilities")
    {
        Gint lineTypes[10]{};
        Gint numLineTypes{};
        Glnfac value{};
        ginqlinefacil(badWsType, numOf(lineTypes), &numLineTypes, &value, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }
    SECTION("marker facilities")
    {
        Gint markerTypes[10]{};
        Gint numMarkerTypes{};
        Gmkfac value{};
        ginqmarkerfacil(badWsType, numOf(markerTypes), &numMarkerTypes, &value, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }
    SECTION("text facilities")
    {
        Gtxfp fontPrec[10]{};
        Gint buffSize{numOf(fontPrec)};
        Gint numFontPrec{};
        Gtxfac facil{};
        facil.fp_list = fontPrec;
        ginqtextfacil(badWsType, buffSize, &numFontPrec, &facil, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }
    SECTION("workstation state table lengths")
    {
        Gwstables tables{-1, -1, -1, -1, -1, -1};
        ginqmaxwssttables(badWsType, &tables, &status);

        REQUIRE(status == GERROR_INVALID_WSTYPE);
    }

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
    std::string connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId.c_str(), wsType);

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
    SECTION("connection id and type")
    {
        Gint numCt{};
        Gwsct ct{};
        Gint status{};
        ginqwsconntype(wsId, 1, &numCt, &ct, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(numCt == 1);
        REQUIRE(ct.conn == connId);
        REQUIRE(ct.type == wsType);
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

        REQUIRE(status == GERROR_NONE);
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

        REQUIRE(status == GERROR_NONE);
        REQUIRE(transform.wstus == GNOTPENDING);
        REQUIRE(transform.current.w.xmin == xmin);
        REQUIRE(transform.current.w.xmax == xmax);
        REQUIRE(transform.current.w.ymin == ymin);
        REQUIRE(transform.current.w.ymax == ymax);
    }
    SECTION("text extent")
    {
        Gpoint pos{0.1f, 0.1f};
        Gextent value{};
        Gint status{};
        ginqtextextent(wsId, &pos, "Hello, World!", &value, &status);

        REQUIRE(status == GERROR_NONE);
    }

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

TEST_CASE("deactivate workstation error handling", "[workstation]")
{
    REQUIRE(getGksOpState() == GGKCL);
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    Gint wsId{};
    const Gchar *connId{"tek4105"};
    Gwstype wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);
    REQUIRE(getGksOpState() == GWSAC);
    REQUIRE(g_recordedErrors.empty());

    SECTION("workstation id negative")
    {
        Gint badWsId{-1};
        gdeactivatews(badWsId);

        requireError(GERROR_INVALID_WSID, GFN_DEACTIVATE_WORKSTATION);
        REQUIRE(getGksOpState() == GWSAC);
    }
    SECTION("workstation id too large")
    {
        const Gint maxOpenWorkstations{1};
        Gint badWsId{maxOpenWorkstations};
        gdeactivatews(badWsId);

        requireError(GERROR_INVALID_WSID, GFN_DEACTIVATE_WORKSTATION);
    }
    SECTION("workstation not activated")
    {
        // This test requires support for multiple open workstations
    }

    REQUIRE(getGksOpState() == GWSAC);
    gdeactivatews(wsId);
    gclosews(wsId);
    gclosegks();
    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("clear workstation error handling", "[workstation]")
{
    REQUIRE(getGksOpState() == GGKCL);
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
        gclearws(badWsId, GALWAYS);

        requireError(GERROR_INVALID_WSID, GFN_CLEAR_WORKSTATION);
    }
    SECTION("workstation id too large")
    {
        const Gint maxOpenWorkstations{1};
        Gint badWsId{maxOpenWorkstations};
        gclearws(badWsId, GALWAYS);

        requireError(GERROR_INVALID_WSID, GFN_CLEAR_WORKSTATION);
    }
    SECTION("workstation not activated")
    {
        // This test requires support for multiple open workstations
    }

    gclosews(wsId);
    gclosegks();
    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("update workstation error handling", "[workstation]")
{
    REQUIRE(getGksOpState() == GGKCL);
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
        gupdatews(badWsId, GPERFORM);

        requireError(GERROR_INVALID_WSID, GFN_UPDATE_WORKSTATION);
    }
    SECTION("workstation id too large")
    {
        const Gint maxOpenWorkstations{1};
        Gint badWsId{maxOpenWorkstations};
        gupdatews(badWsId, GPERFORM);

        requireError(GERROR_INVALID_WSID, GFN_UPDATE_WORKSTATION);
    }
    SECTION("workstation not open")
    {
        // This test requires multiple workstations to be open
    }

    gclosews(wsId);
    gclosegks();
    REQUIRE(getGksOpState() == GGKCL);
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

TEST_CASE("output primitive error handling", "[output]")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);
    Gpoint points[] = 
    {
        {0.25f, 0.25f},
        {0.50f, 0.25f},
        {0.50f, 0.50f},
        {0.25f, 0.50f}
    };

    SECTION("polyline with too few points")
    {
        gpolyline(1, points);

        requireError(GERROR_INVALID_NUM_POINTS, GFN_POLYLINE);
    }
    SECTION("polymarker with too few points")
    {
        gpolymarker(0, points);

        requireError(GERROR_INVALID_NUM_POINTS, GFN_POLYMARKER);
    }
    SECTION("fill area with too few points")
    {
        gfillarea(2, points);

        requireError(GERROR_INVALID_NUM_POINTS, GFN_FILL_AREA);
    }
    SECTION("cell array")
    {
        struct Grect rect =
        {
            {0.25f, 0.75f},
            {0.75f, 0.25f}
        };
        SECTION("invalid x dimension")
        {
            struct Gidim dim =
            {
                0, 1
            };
            Gint colors[] = { 0, 1 };
            gcellarray(&rect, &dim, colors);

            requireError(GERROR_INVALID_COLOR_DIMENSIONS, GFN_CELL_ARRAY);
        }
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

TEST_CASE("initial GKS state list values", "[output]")
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
    SECTION("pattern height is (0,1)")
    {
        Gpoint vec{};
        ginqpatheight(&vec, &status);

        REQUIRE(vec.x == 0.0f);
        REQUIRE(vec.y == 1.0f);
    }
    SECTION("pattern width is (1,0)")
    {
        Gpoint vec{};
        ginqpatwidth(&vec, &status);

        REQUIRE(vec.x == 1.0f);
        REQUIRE(vec.y == 0.0f);
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
    SECTION("char expansion factor")
    {
        const Gfloat expan{0.5f};
        gsetcharexpan(expan);

        Gfloat current{};
        ginqcharexpan(&current, &status);
        REQUIRE(current == expan);
    }
    SECTION("char height")
    {
        gsetcharheight(0.5f);

        Gfloat charHeight{-1.0f};
        ginqcharheight(&charHeight, &status);
        REQUIRE(charHeight == 0.5f);
    }
    SECTION("char spacing")
    {
        const Gfloat space{0.5f};
        gsetcharspace(space);

        Gfloat current{};
        ginqcharspace(&current, &status);
        REQUIRE(current == space);
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
    SECTION("fill area color index")
    {
        gsetfillcolorind(0);

        ginqfillcolorind(&value, &status);
        REQUIRE(value == 0);
    }
    SECTION("fill area index")
    {
        gsetfillind(1);

        ginqfillind(&value, &status);
        REQUIRE(value == 1);
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
    SECTION("line type")
    {
        gsetlinetype(GLN_DASHED);

        ginqlinetype(&value, &status);
        REQUIRE(value == GLN_DASHED);
    }
    SECTION("line color index")
    {
        gsetlinecolorind(1);

        ginqlinecolorind(&value, &status);
        REQUIRE(value == 1);
    }
    SECTION("line index")
    {
        gsetlineind(1);

        ginqlineind(&value, &status);
        REQUIRE(value == 1);
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
        Gint index{1};
        gsetmarkercolorind(index);

        ginqmarkercolorind(&value, &status);
        REQUIRE(value == index);
    }
    SECTION("marker index")
    {
        Gint index{1};
        gsetmarkerind(index);

        ginqmarkerind(&value, &status);
        REQUIRE(value == index);
    }
    SECTION("normalization transform")
    {
        gselntran(1);

        Gint transform{0};
        ginqcurrntrannum(&transform, &status);
        REQUIRE(transform == 1);
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
    SECTION("text color index is 1")
    {
        const Gint color{0};
        gsettextcolorind(color);

        ginqtextcolorind(&value, &status);
        REQUIRE(value == color);
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
    SECTION("text index")
    {
        const Gint index{1};
        gsettextind(index);

        ginqtextind(&value, &status);
        REQUIRE(value == index);
    }

    REQUIRE(status == 0);

    gclosegks();
}

TEST_CASE("global attribute error handling", "[gks]")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    Gint status{-1};

    SECTION("character expansion factor")
    {
        gsetcharexpan(0.0f);

        requireError(GERROR_CHAR_EXPANSION_NOT_POSITIVE, GFN_SET_CHARACTER_EXPANSION_FACTOR);
        Gfloat value{};
        ginqcharexpan(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1.0f);
    }
    SECTION("character height")
    {
        SECTION("zero")
        {
            gsetcharheight(0.0f);
        }
        SECTION("negative")
        {
            gsetcharheight(-1.0f);
        }

        requireError(GERROR_CHAR_HEIGHT_NOT_POSITIVE, GFN_SET_CHARACTER_HEIGHT);
        Gfloat value{-1.0f};
        ginqcharheight(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 0.01f);
    }
    SECTION("character up vector")
    {
        Gpoint up{};
        gsetcharup(&up);

        requireError(GERROR_CHAR_UP_LENGTH_ZERO, GFN_SET_CHARACTER_UP_VECTOR);
        Gpoint value{};
        ginqcharup(&value, &status);
        REQUIRE(value.x == 0.0f);
        REQUIRE(value.y == 1.0f);
    }
    SECTION("fill area color")
    {
        gsetfillcolorind(-1);

        requireError(GERROR_INVALID_COLOR_INDEX, GFN_SET_FILL_AREA_COLOR_INDEX);
        Gint value{};
        ginqfillcolorind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("fill area index")
    {
        gsetfillind(0);

        requireError(GERROR_INVALID_FILL_INDEX, GFN_SET_FILL_AREA_INDEX);
        Gint value{};
        ginqfillind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("fill area interior style index")
    {
        gsetfillstyleind(0);

        requireError(GERROR_FILL_STYLE_ZERO, GFN_SET_FILL_AREA_INTERIOR_STYLE);
        Gint value{};
        ginqfillstyleind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("line color")
    {
        SECTION("too small")
        {
            gsetlinecolorind(-1);
        }
        SECTION("too large")
        {
            gsetlinecolorind(2);
        }

        requireError(GERROR_INVALID_COLOR_INDEX, GFN_SET_POLYLINE_COLOR_INDEX);
        Gint value{};
        ginqlinecolorind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("line index")
    {
        gsetlineind(0);

        requireError(GERROR_INVALID_LINE_INDEX, GFN_SET_POLYLINE_INDEX);
        Gint value{};
        ginqlineind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("line type")
    {
        gsetlinetype(0);

        requireError(GERROR_LINE_TYPE_ZERO, GFN_SET_LINETYPE);
        Gint value{};
        ginqlinetype(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("marker color")
    {
        SECTION("too small")
        {
            gsetmarkercolorind(-1);
        }
        SECTION("too large")
        {
            gsetmarkercolorind(2);
        }

        requireError(GERROR_INVALID_COLOR_INDEX, GFN_SET_POLYMARKER_COLOR_INDEX);
        Gint value{};
        ginqmarkercolorind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("marker index")
    {
        gsetmarkerind(0);

        requireError(GERROR_INVALID_MARKER_INDEX, GFN_SET_POLYMARKER_INDEX);
        Gint value{};
        ginqmarkerind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("marker size")
    {
        gsetmarkersize(-1.0f);

        requireError(GERROR_NEGATIVE_MARKER_SIZE, GFN_SET_MARKER_SIZE_SCALE_FACTOR);
        Gfloat value{};
        ginqmarkersize(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1.0f);
    }
    SECTION("marker type")
    {
        gsetmarkertype(0);

        requireError(GERROR_MARKER_TYPE_ZERO, GFN_SET_MARKER_TYPE);
        Gint value{};
        ginqmarkertype(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == GMK_STAR);
    }
    SECTION("normalization transform")
    {
        SECTION("too small")
        {
            gselntran(-1);
        }
        SECTION("too large")
        {
            Gint maxTranNum{};
            ginqmaxntrannum(&maxTranNum, &status);
            REQUIRE(status == GERROR_NONE);

            gselntran(maxTranNum + 1);
        }

        requireError(GERROR_INVALID_TRAN_NUM, GFN_SELECT_NORMALIZATION_TRANSFORMATION);
        Gint value{};
        ginqcurrntrannum(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 0);
    }
    SECTION("text color")
    {
        gsettextcolorind(-1);

        requireError(GERROR_INVALID_COLOR_INDEX, GFN_SET_TEXT_COLOR_INDEX);
        Gint value{};
        ginqtextcolorind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("text font and precision")
    {
        Gtxfp fontPrec{0, GP_STROKE};
        gsettextfontprec(&fontPrec);

        requireError(GERROR_TEXT_FONT_ZERO, GFN_SET_TEXT_FONT_AND_PRECISION);
        Gtxfp value{};
        ginqtextfontprec(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value.font == 1);
    }
    SECTION("text index")
    {
        gsettextind(0);

        requireError(GERROR_INVALID_TEXT_INDEX, GFN_SET_TEXT_INDEX);
        Gint value{};
        ginqtextind(&value, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(value == 1);
    }
    SECTION("viewport")
    {
        SECTION("invalid index")
        {
            Glimit viewport{0.0f, 1.0f, 0.0f, 1.0f};
            gsetviewport(0, &viewport);

            requireError(GERROR_INVALID_TRAN_NUM, GFN_SET_VIEWPORT);
        }
        SECTION("invalid rectangle")
        {
            SECTION("x range")
            {
                Glimit viewport{1.0f, 0.0f, 0.0f, 1.0f};
                gsetviewport(1, &viewport);
            }
            SECTION("y range")
            {
                Glimit viewport{0.0f, 1.0f, 1.0f, 0.0f};
                gsetviewport(1, &viewport);
            }

            requireError(GERROR_INVALID_RECT, GFN_SET_VIEWPORT);
        }
        SECTION("not in NDC unit square")
        {
            Glimit viewport{0.0f, 1.0f, 0.0f, 1.0f};

            SECTION("x min out of range")
            {
                viewport.xmin = -1.0f;
            }
            SECTION("x max out of range")
            {
                viewport.xmax = 2.0f;
            }
            SECTION("y min out of range")
            {
                viewport.ymin = -1.0f;
            }
            SECTION("y max out of range")
            {
                viewport.ymax = 2.0f;
            }

            gsetviewport(1, &viewport);

            requireError(GERROR_VIEWPORT_NOT_IN_NDC, GFN_SET_VIEWPORT);
        }
    }
    SECTION("window")
    {
        SECTION("invalid index")
        {
            Glimit window{0.0f, 1.0f, 0.0f, 1.0f};
            gsetwindow(0, &window);

            requireError(GERROR_INVALID_TRAN_NUM, GFN_SET_WINDOW);
        }
        SECTION("invalid rectangle")
        {
            SECTION("x range")
            {
                Glimit window{1.0f, 0.0f, 0.0f, 1.0f};
                gsetwindow(1, &window);
            }
            SECTION("y range")
            {
                Glimit window{0.0f, 1.0f, 1.0f, 0.0f};
                gsetwindow(1, &window);
            }

            requireError(GERROR_INVALID_RECT, GFN_SET_WINDOW);
        }
    }

    gclosegks();
}

TEST_CASE("initial workstation dependent attribute values", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{0};
    std::string connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId.c_str(), wsType);

    SECTION("connection id and type")
    {
        Gwsct ct{};
        Gint numCt{};
        Gint status{};
        ginqwsconntype(wsId, 0, &numCt, &ct, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(ct.conn == connId);
        REQUIRE(ct.type == wsType);
    }
    SECTION("color indices")
    {
        Gint colors[16]{-1};
        Gint numIndices{};
        Gintlist indices{numOf(colors), colors};
        Gint status{-1};
        ginqcolorindices(wsId, numOf(colors), 0, &indices, &numIndices, &status);

        REQUIRE(status == GERROR_NONE);
        REQUIRE(numIndices == 2);
        REQUIRE(colors[0] == 0);
        REQUIRE(colors[1] == 1);
    }

    gclosews(wsId);
    gclosegks();
    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("set workstation dependent attribute values", "[output]")
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

TEST_CASE("workstation dependent attribute error handling", "[workstation]")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);

    SECTION("get color indices")
    {
        Gint colors[16]{};
        Gint numIndices{};
        Gintlist indices{};
        Gint status{-1};
        ginqcolorindices(wsId + 1, numOf(colors), 0, &indices, &numIndices, &status);

        REQUIRE(status == GERROR_WS_NOT_OPEN);
    }
    SECTION("get color representation")
    {
        Gint index{1};
        Gcobundl value{};
        Gint status{};

        SECTION("workstation not open")
        {
            ginqcolorrep(wsId + 1, index, &value, &status);

            REQUIRE(status == GERROR_WS_NOT_OPEN);
        }
        SECTION("bad color index")
        {
            ginqcolorrep(wsId, -1, &value, &status);

            REQUIRE(status == GERROR_INVALID_COLOR_INDEX);
        }
    }
    SECTION("get connection and type")
    {
        Gwsct ct{};
        Gint buffSize{};
        Gint numCts{};
        Gint status{};
        ginqwsconntype(wsId + 1, buffSize, &numCts, &ct, &status);

        REQUIRE(status == GERROR_WS_NOT_OPEN);
    }
    SECTION("get workstation transformation ws not open")
    {
        struct Gwsti transform{};
        Gint status{};
        ginqwstran(wsId + 1, &transform, &status);

        REQUIRE(status == GERROR_WS_NOT_OPEN);
    }
    SECTION("set color representation bad index")
    {
        struct Gcobundl rep{0.5f, 0.5f, 0.5f};
        gsetcolorrep(wsId, -1, &rep);

        requireError(GERROR_INVALID_COLOR_INDEX, GFN_SET_COLOR_REPRESENTATION);
        struct Gcobundl current{};
        Gint status{};
        ginqcolorrep(wsId, 0, &current, &status);
        REQUIRE(status == GERROR_NONE);
        REQUIRE(current.red == 0.0f);
        REQUIRE(current.green == 0.0f);
        REQUIRE(current.blue == 0.0f);
    }
    SECTION("set viewport")
    {
        Glimit viewport{0.0f, 1.0f, 0.0f, 1.0f};

        SECTION("invalid rectangle")
        {
            SECTION("x range")
            {
                viewport.xmin = 2.0f;
            }
            SECTION("y range")
            {
                viewport.ymin = 2.0f;
            }

            gsetwsviewport(wsId, &viewport);

            requireError(GERROR_INVALID_RECT, GFN_SET_WORKSTATION_VIEWPORT);
        }
    }
    SECTION("set window")
    {
        Glimit window{0.0f, 1.0f, 0.0f, 1.0f};

        SECTION("invalid rectangle")
        {
            SECTION("x range")
            {
                window.xmin = 2.0f;
            }
            SECTION("y range")
            {
                window.ymin = 2.0f;
            }

            gsetwswindow(wsId, &window);

            requireError(GERROR_INVALID_RECT, GFN_SET_WORKSTATION_WINDOW);
        }
    }

    gclosews(wsId);
    gclosegks();
}
