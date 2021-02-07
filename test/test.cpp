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
    ginqlevelgks(&level);

    REQUIRE( level == GLMA );

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
    Gint errorStatus{};
    ginqavailwstypes(bufSize, start, &wsTypes, &numTypes, &errorStatus);

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
    Gint status{};
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
    Gint status{};
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
    Gint status{};
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

TEST_CASE("Initial transformation", "[workstation]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

    struct Gwsti transform{};
    Gint status{};
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
    Gint status{};
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
    Gint status{};
    ginqwstran(wsId, &transform, &status);

    REQUIRE(transform.wstus == GNOTPENDING);
    REQUIRE(transform.current.w.xmin == xmin);
    REQUIRE(transform.current.w.xmax == xmax);
    REQUIRE(transform.current.w.ymin == ymin);
    REQUIRE(transform.current.w.ymax == ymax);

    gclosegks();
}
