#include <gks/gks.h>

#include <catch2/catch.hpp>

#include <cstdio>

inline Gopst getGksOpState()
{
    Gopst result = static_cast<Gopst>(-1);
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

    REQUIRE( level == GL0A );

    gclosegks();
}

TEST_CASE("Error handling", "[gks]")
{
    gerrorhand(1, 1, stderr);
}

TEST_CASE("Open workstation", "[gks]")
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

TEST_CASE("Close workstation", "[gks]")
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

TEST_CASE("Activate workstation", "[gks]")
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

TEST_CASE("Deactivate workstation", "[gks]")
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

TEST_CASE("Clear workstation", "[gks]")
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

TEST_CASE("Update", "[ws]")
{
    gopengks(stderr, 0L);
    Gint wsId{1};
    const Gchar *connId{"tek4105"};
    Gint wsType{};
    gopenws(wsId, connId, wsType);

    gupdatews(wsId, GPERFORM);

    gclosegks();
}
