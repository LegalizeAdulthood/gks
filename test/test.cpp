#include <gks/gks.h>

#include <catch2/catch.hpp>

#include <cstdio>

TEST_CASE("Initially closed", "[gks]")
{
    Gopst state{GGKOP};
    ginqopst(&state);
    REQUIRE( GGKCL == state );
}

TEST_CASE("Opened", "[gks]")
{
    gopengks(stderr, 0L);

    Gopst state{GGKCL};
    ginqopst(&state);

    REQUIRE( state == GGKOP );

    gclosegks();
}

TEST_CASE("Opened and closed", "[gks]")
{
    Gopst opened{GGKCL}, closed{GGKOP};

    gopengks(stderr, 0L);
    ginqopst(&opened);
    gclosegks();
    ginqopst(&closed);

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
