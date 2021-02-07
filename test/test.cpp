#include <gks/gks.h>

#include <catch2/catch.hpp>

#include <cstdio>

inline enum Gopst getGksOpState()
{
    Gopst result = static_cast<enum Gopst>(-1);
    ginqopst(&result);
    return result;
}

TEST_CASE("Initially closed", "[gks]")
{
    REQUIRE(getGksOpState() == GGKCL);
}

TEST_CASE("Opened", "[gks]")
{
    gopengks(stderr, 0L);

    REQUIRE( getGksOpState() == GGKOP );

    gclosegks();
}

TEST_CASE("Opened and closed", "[gks]")
{
    gopengks(stderr, 0L);
    const Gopst opened = getGksOpState();
    gclosegks();
    const Gopst closed = getGksOpState();

    REQUIRE( opened == GGKOP );
    REQUIRE( closed == GGKCL );
}

TEST_CASE("Operating level", "[gks]")
{
    gopengks(stderr, 0L);

    Glevel level{};
    Gint status{-1};
    ginqlevelgks(&level, &status);

    REQUIRE(status == 0);
    REQUIRE(level == GLMA);

    gclosegks();
}

TEST_CASE("Error handling", "[gks]")
{
    gerrorhand(1, 1, stderr);
}

TEST_CASE("Workstation types", "[gks]")
{
    gopengks(stderr, 0L);

    Gint bufSize{};
    Gint start{};
    Gstrlist wsTypes{};
    Gint numTypes{};
    Gint status{-1};
    ginqavailwstypes(bufSize, start, &wsTypes, &numTypes, &status);

    REQUIRE(status == 0);
    REQUIRE(numTypes >= 1);

    gclosegks();
}

TEST_CASE("Number of normalization transforms", "[gks]")
{
    gopengks(stderr, 0L);

    Gint numTransforms{-1};
    Gint status{0};
    ginqmaxntrannum(&numTransforms, &status);

    REQUIRE(status == 0);
    REQUIRE(numTransforms == 2);

    gclosegks();
}

TEST_CASE("Normalization transform zero is identity", "[gks]")
{
    gopengks(stderr, 0L);

    Gtran transform{};
    Gint status{-1};
    ginqntran(0, &transform, &status);

    REQUIRE(status == 0);
    REQUIRE(transform.w.xmin == 0.0f);
    REQUIRE(transform.w.xmax == 1.0f);
    REQUIRE(transform.w.ymin == 0.0f);
    REQUIRE(transform.w.ymax == 1.0f);
    REQUIRE(transform.v.xmin == 0.0f);
    REQUIRE(transform.v.xmax == 1.0f);
    REQUIRE(transform.v.ymin == 0.0f);
    REQUIRE(transform.v.ymax == 1.0f);

    gclosegks();
}

TEST_CASE("Set viewport", "[gks]")
{
    gopengks(stderr, 0L);

    const Gint tranId = 1;
    const Gfloat xmin{0.2f};
    const Gfloat xmax{0.5f};
    const Gfloat ymin{0.4f};
    const Gfloat ymax{0.6f};
    Glimit viewport{xmin, xmax, ymin, ymax};
    gsetviewport(tranId, &viewport);

    Gtran transform{};
    Gint status{-1};
    ginqntran(tranId, &transform, &status);
    REQUIRE(status == 0);
    REQUIRE(transform.v.xmin == xmin);
    REQUIRE(transform.v.xmax == xmax);
    REQUIRE(transform.v.ymin == ymin);
    REQUIRE(transform.v.ymax == ymax);

    gclosegks();
}

TEST_CASE("Set window", "[gks]")
{
    gopengks(stderr, 0L);

    const Gint tranId = 1;
    const Gfloat xmin{0.2f};
    const Gfloat xmax{0.5f};
    const Gfloat ymin{0.4f};
    const Gfloat ymax{0.6f};
    Glimit window{xmin, xmax, ymin, ymax};
    gsetwindow(tranId, &window);

    Gtran transform{};
    Gint status{-1};
    ginqntran(tranId, &transform, &status);
    REQUIRE(status == 0);
    REQUIRE(transform.w.xmin == xmin);
    REQUIRE(transform.w.xmax == xmax);
    REQUIRE(transform.w.ymin == ymin);
    REQUIRE(transform.w.ymax == ymax);

    gclosegks();
}

TEST_CASE("Maximum workstation numbers", "[gks]")
{
    gopengks(stderr, 0L);

    Gwsmax value{};
    Gint status{-1};
    ginqwsmaxnum(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value.open == 1);
    REQUIRE(value.active == 1);
    REQUIRE(value.assoc == 0);

    gclosegks();
}

TEST_CASE("Inquire workstation color facilities", "[gks]")
{
    gopengks(stderr, 0L);

    Gwstype wsType{};
    Gint buffSize{};
    Gint facilSize{};
    Gcofac facil{};
    Gint status{-1};
    ginqcolourfacil(wsType, buffSize, &facilSize, &facil, &status);

    REQUIRE(status == 0);

    gclosegks();
}

TEST_CASE("Initial clipping indicator", "[gks]")
{
    gopengks(stderr, 0L);

    Gcliprect clipping{};
    Gint status{-1};
    ginqclip(&clipping, &status);

    REQUIRE(clipping.ind == GCLIP);
    REQUIRE(clipping.rec.xmin == 0.0f);
    REQUIRE(clipping.rec.xmax == 1.0f);
    REQUIRE(clipping.rec.ymin == 0.0f);
    REQUIRE(clipping.rec.ymax == 1.0f);

    gclosegks();
}

TEST_CASE("Set clipping indicator", "[gks]")
{
    gopengks(stderr, 0L);

    gsetclip(GNOCLIP);

    Gcliprect clipping{};
    Gint status{-1};
    ginqclip(&clipping, &status);
    REQUIRE(clipping.ind == GNOCLIP);

    gclosegks();
}

TEST_CASE("Open workstation", "[workstation]")
{
    gopengks(stderr, 0L);

    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

    Gopst state{};
    ginqopst(&state);
    REQUIRE( state == GWSOP );

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Close workstation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

    Gopst before{};
    ginqopst(&before);
    gclosews(wsId);
    Gopst after{};
    ginqopst(&after);

    REQUIRE( before == GWSOP );
    REQUIRE( after == GGKOP );

    gclosegks();
}

TEST_CASE("Activate workstation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

    Gopst before{};
    ginqopst(&before);
    gactivatews(wsId);
    Gopst after{};
    ginqopst(&after);

    REQUIRE( before == GWSOP );
    REQUIRE( after == GWSAC );

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Deactivate workstation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);

    Gopst before{};
    ginqopst(&before);
    gdeactivatews(wsId);
    Gopst after{};
    ginqopst(&after);

    REQUIRE( before == GWSAC );
    REQUIRE( after == GWSOP );

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Clear workstation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);

    gclearws(wsId, GALWAYS);

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Initial workstation transformation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

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

    gclosegks();
}

TEST_CASE("Update workstation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

    gupdatews(wsId, GPERFORM);

    gclosegks();
}

TEST_CASE("Set workstation viewport", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

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

    gclosegks();
}

TEST_CASE("Set workstation window", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

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

    gclosegks();
}

TEST_CASE("Polyline", "[output]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);

    Gpoint points[] =
    {
        { 0.0f, 0.5f },
        { 0.5f, 0.5f }
    };
    gpolyline(sizeof(points)/sizeof(points[0]), points);

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Initial current line type is solid", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqlinetype(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == GLN_SOLID);

    gclosegks();
}

TEST_CASE("Set current line type", "[output]")
{
    gopengks(stderr, 0L);

    gsetlinetype(GLN_DASHED);

    Gint value{};
    Gint status{-1};
    ginqlinetype(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == GLN_DASHED);

    gclosegks();
}

TEST_CASE("Initial line color index is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqlinecolorind(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1);

    gclosegks();
}

TEST_CASE("Set line color index", "[output]")
{
    gopengks(stderr, 0L);

    gsetlinecolorind(GLN_DASHDOT);

    Gint value{};
    Gint status{-1};
    ginqlinecolorind(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == GLN_DASHDOT);

    gclosegks();
}

TEST_CASE("Initial line index is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqlineind(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1);

    gclosegks();
}

TEST_CASE("Set line index", "[output]")
{
    gopengks(stderr, 0L);

    gsetlineind(4);

    Gint value{};
    Gint status{-1};
    ginqlineind(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == 4);

    gclosegks();
}

TEST_CASE("Polymarker", "[output]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);

    Gpoint points[] =
    {
        { 0.0f, 0.5f },
        { 0.5f, 0.5f }
    };
    gpolymarker(sizeof(points)/sizeof(points[0]), points);

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Initial current marker type is 3 (star)", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqmarkertype(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == GMK_STAR);

    gclosegks();
}

TEST_CASE("Set current marker type", "[output]")
{
    gopengks(stderr, 0L);

    gsetmarkertype(GMK_X);

    Gint value{};
    Gint status{-1};
    ginqmarkertype(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == GMK_X);

    gclosegks();
}

TEST_CASE("Initial current marker size is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gfloat value{};
    Gint status{-1};
    ginqmarkersize(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1.0f);

    gclosegks();
}

TEST_CASE("Set current marker size", "[output]")
{
    gopengks(stderr, 0L);

    const Gfloat size{2.0f};
    gsetmarkersize(size);

    Gfloat value{};
    Gint status{-1};
    ginqmarkersize(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == size);

    gclosegks();
}

TEST_CASE("Initial current marker color index is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqmarkercolorind(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1);

    gclosegks();
}

TEST_CASE("Set current marker color index", "[output]")
{
    gopengks(stderr, 0L);

    Gint color{4};
    gsetmarkercolorind(color);

    Gint value{};
    Gint status{-1};
    ginqmarkercolorind(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == color);

    gclosegks();
}

TEST_CASE("Initial current marker index is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqmarkerind(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1);

    gclosegks();
}

TEST_CASE("Set current marker index", "[output]")
{
    gopengks(stderr, 0L);

    Gint index{4};
    gsetmarkerind(index);

    Gint value{};
    Gint status{-1};
    ginqmarkerind(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == index);

    gclosegks();
}

TEST_CASE("Text", "[output]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);

    Gpoint start{0.0f, 0.5f};
    gtext(&start, "Hello, world!");

    gclosews(wsId);
    gclosegks();
}

TEST_CASE("Initial current text height is 0.01", "[output]")
{
    gopengks(stderr, 0L);

    Gfloat value{};
    Gint status{-1};
    ginqcharheight(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 0.01f);

    gclosegks();
}

TEST_CASE("Set current char height", "[output]")
{
    gopengks(stderr, 0L);

    const Gfloat charHeight{0.5f};
    gsetcharheight(charHeight);

    Gfloat value{};
    Gint status{-1};
    ginqcharheight(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == charHeight);

    gclosegks();
}

TEST_CASE("Initial current char up vector is (0,1)", "[output]")
{
    gopengks(stderr, 0L);

    Gpoint value{};
    Gint status{-1};
    ginqcharup(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value.x == 0.0f);
    REQUIRE(value.y == 1.0f);

    gclosegks();
}

TEST_CASE("Set current char up vector", "[output]")
{
    gopengks(stderr, 0L);

    Gpoint up{1.0f, 0.0f};
    gsetcharup(&up);

    Gpoint value{};
    Gint status{-1};
    ginqcharup(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value.x == up.x);
    REQUIRE(value.y == up.y);

    gclosegks();
}

TEST_CASE("Initial text color index is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqtextcolorind(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1);

    gclosegks();
}

TEST_CASE("Set text color index is 1", "[output]")
{
    gopengks(stderr, 0L);

    const Gint color{3};
    gsettextcolorind(color);

    Gint value{};
    Gint status{-1};
    ginqtextcolorind(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == color);

    gclosegks();
}

TEST_CASE("Initial text index is 1", "[output]")
{
    gopengks(stderr, 0L);

    Gint value{};
    Gint status{-1};
    ginqtextind(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1);

    gclosegks();
}

TEST_CASE("Set text index", "[output]")
{
    gopengks(stderr, 0L);

    const Gint index{3};
    gsettextind(index);

    Gint value{};
    Gint status{-1};
    ginqtextind(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == index);

    gclosegks();
}

TEST_CASE("Initial text alignment is normal", "[output]")
{
    gopengks(stderr, 0L);

    Gtxalign value{};
    Gint status{-1};
    ginqtextalign(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value.hor == GAH_NORMAL);
    REQUIRE(value.ver == GAV_NORMAL);

    gclosegks();
}

TEST_CASE("Set text alignment", "[output]")
{
    gopengks(stderr, 0L);

    Gtxalign align{GAH_LEFT, GAV_BOTTOM};
    gsettextalign(&align);

    Gtxalign value{};
    Gint status{-1};
    ginqtextalign(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value.hor == align.hor);
    REQUIRE(value.ver == align.ver);

    gclosegks();
}

TEST_CASE("Initial text font and precision is (1, string)", "[output]")
{
    gopengks(stderr, 0L);

    Gtxfp value{};
    Gint status{-1};
    ginqtextfontprec(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value.font == 1);
    REQUIRE(value.prec == GP_STRING);

    gclosegks();
}

TEST_CASE("Set text font and precision", "[output]")
{
    gopengks(stderr, 0L);

    Gtxfp font{2, GP_STROKE};
    gsettextfontprec(&font);

    Gtxfp value{};
    Gint status{-1};
    ginqtextfontprec(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value.font == font.font);
    REQUIRE(value.prec == font.prec);

    gclosegks();
}

TEST_CASE("Initial char expansion factor is 1.0", "[output]")
{
    gopengks(stderr, 0L);

    Gfloat value{};
    Gint status{-1};
    ginqcharexpan(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 1.0f);

    gclosegks();
}

TEST_CASE("Set char expansion factor", "[output]")
{
    gopengks(stderr, 0L);

    const Gfloat expan{0.5f};
    gsetcharexpan(expan);

    Gfloat value{};
    Gint status{-1};
    ginqcharexpan(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == expan);

    gclosegks();
}

TEST_CASE("Initial char spacing is 0.0", "[output]")
{
    gopengks(stderr, 0L);

    Gfloat value{-1.0f};
    Gint status{-1};
    ginqcharspace(&value, &status);

    REQUIRE(status == 0);
    REQUIRE(value == 0.0f);

    gclosegks();
}

TEST_CASE("Set char spacing", "[output]")
{
    gopengks(stderr, 0L);

    const Gfloat space{0.5f};
    gsetcharspace(space);

    Gfloat value{};
    Gint status{-1};
    ginqcharspace(&value, &status);
    REQUIRE(status == 0);
    REQUIRE(value == space);

    gclosegks();
}
